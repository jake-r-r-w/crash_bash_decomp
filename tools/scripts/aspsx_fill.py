"""Make maspsx output look like Sony's ASPSX for gcc 2.7.2 units (stdin -> stdout).

1. Fill j/jr delay slots the way ASPSX does.

gcc 2.7.2 leaves `j` (and the `j $31` return) in reorder mode for the assembler to fill.
maspsx puts a nop there; ASPSX moves the instruction before the jump into the slot, and for
a store to a symbol (a two-instruction macro) it moves the second half:

    sw  $2,D_8006EA24          lui $at,%hi(D_8006EA24)
    j   $L1             ->     j   $L1
    nop                        sw  $2,%lo(D_8006EA24)($at)

2. Expand an indexed load from a symbol through the load's own register, not $at. ASPSX (like
GNU as) builds the address in the destination unless it is also the index register; maspsx
always uses $at:

    lbu $2,D_8006789E($3)      lui  $2,%hi(D_8006789E)       (maspsx: lui $at; addu $at,$at,$3;
                        ->     addu $2,$2,$3                  lbu $2,%lo(D_8006789E)($at))
                               lbu  $2,%lo(D_8006789E)($2)

Only units built with gcc 2.7.2 use this (Makefile: ASPSX_FILL)."""
import re
import sys

BRANCHES = {"j", "jal", "jr", "jalr", "b", "bal", "beq", "bne", "beqz", "bnez", "blez", "bgtz",
            "bltz", "bgez", "bltzal", "bgezal", "beql", "bnel"}
NO_MOVE = BRANCHES | {"nop", "mult", "multu", "div", "divu", "mflo", "mfhi", "mtlo", "mthi", "la",
                      "lw", "lh", "lhu", "lb", "lbu", "lwl", "lwr", "lwc2", "break", "syscall"}
STORES = {"sw", "sh", "sb"}
INSN = re.compile(r"^\s*([a-z][a-z0-9.]*)\s*(.*?)\s*(#.*)?$")


def insn(line):
    s = line.strip()
    if not s or s.startswith("#") or s.startswith(".") or s.endswith(":") or ";" in s:
        return None
    m = INSN.match(line)
    return (m.group(1), m.group(2)) if m else None


def regs_written(op, args):
    if op in STORES or op in BRANCHES:
        return set()
    return {args.split(",")[0].strip()}


def fits16(v):
    try:
        n = int(v, 0)
    except ValueError:
        return False
    return -0x8000 <= n <= 0xFFFF


LOADS = {"lw", "lh", "lhu", "lb", "lbu"}
LUI_AT = re.compile(r"^\s*lui\s+\$at,\s*%hi\(([^)]+)\)\s*$")
ADDU_AT = re.compile(r"^\s*addu\s+\$at,\s*\$at,\s*(\$\w+)\s*$")
LOAD_AT = re.compile(r"^\s*(\w+)\s+(\$\w+),\s*%lo\(([^)]+)\)\(\$at\)\s*$")


def load_via_dest(lines):
    """Rewrite `lui $at; addu $at,$at,rI; lX rD,%lo(sym)($at)` to use rD in place of $at."""
    out = list(lines)
    for i in range(len(out) - 2):
        a, b, c = LUI_AT.match(out[i]), ADDU_AT.match(out[i + 1]), LOAD_AT.match(out[i + 2])
        if not (a and b and c) or c.group(1) not in LOADS or c.group(3) != a.group(1):
            continue
        rd, ri = c.group(2), b.group(1)
        if rd in (ri, "$0", "$zero"):
            continue
        out[i] = f"lui\t{rd},%hi({a.group(1)})"
        out[i + 1] = f"addu\t{rd},{rd},{ri}"
        out[i + 2] = f"{c.group(1)}\t{rd},%lo({a.group(1)})({rd})"
    return out


def fill(lines):
    out = []
    i = 0
    while i < len(lines):
        line = lines[i]
        ins = insn(line)
        nxt = lines[i + 1].strip() if i + 1 < len(lines) else ""
        if ins and ins[0] in ("j", "jr") and nxt.startswith("nop") and "branch/jump" in nxt:
            # the previous line must be a plain instruction (no label or directive between)
            p = len(out) - 1
            while p >= 0 and (not out[p].strip() or out[p].strip().startswith("#")):
                p -= 1
            prev = insn(out[p]) if p >= 0 else None
            q = p - 1
            while q >= 0 and (not out[q].strip() or out[q].strip().startswith("#")):
                q -= 1
            before = insn(out[q]) if q >= 0 else None
            target = ins[1].strip()
            ok = (prev and prev[0] not in NO_MOVE and not (before and before[0] in BRANCHES)
                  and target not in regs_written(*prev)
                  and not (prev[0] == "li" and not fits16(prev[1].split(",")[1].strip())))
            if ok:
                op, args = prev
                r, mem = [x.strip() for x in args.split(",", 1)]
                if op in STORES and "(" not in mem:
                    out[p] = f".set\tnoat\nlui\t$at,%hi({mem})"
                    out.append(line)
                    out.append(f"{op}\t{r},%lo({mem})($at)\n.set\tat")
                else:
                    moved = out.pop(p)
                    out.append(line)
                    out.append(moved)
                i += 2
                continue
        out.append(line)
        i += 1
    return out


COND_BRANCHES = {"beq", "bne", "beqz", "bnez", "blez", "bgtz", "bltz", "bgez"}
LABEL = re.compile(r"^\s*(\$?[\w.]+):\s*$")


def steal_jump_slot(lines):
    """A conditional branch with an empty slot whose target is a `j X` with a filled slot: branch
    to X instead and copy that instruction into the empty slot. ASPSX does this (gcc 2.7.2 leaves
    the branch for the assembler); func_8002DE2C's `beq a0,v0` returns 1 straight to the exit:

        beq  $4,$2,$L26            beq  $4,$2,$L27
        nop                  ->    li   $2,1
        ...                        ...
    $L26:                      $L26:
        j    $L27                  j    $L27
        li   $2,1                  li   $2,1
    """
    labels = {}
    for i, line in enumerate(lines):
        m = LABEL.match(line)
        if m:
            labels[m.group(1)] = i
    out = list(lines)
    for i, line in enumerate(lines[:-1]):
        ins = insn(line)
        if not ins or ins[0] not in COND_BRANCHES or not lines[i + 1].strip().startswith("nop"):
            continue
        args = [a.strip() for a in ins[1].split(",")]
        t = labels.get(args[-1])
        if t is None:
            continue
        k = t + 1                                   # first instruction at the target
        while k < len(lines) and (not lines[k].strip() or LABEL.match(lines[k]) or lines[k].strip().startswith("#")):
            k += 1
        j = insn(lines[k]) if k < len(lines) else None
        slot = insn(lines[k + 1]) if k + 1 < len(lines) else None
        if not j or j[0] != "j" or j[1].strip().startswith("$31") or not slot or slot[0] in NO_MOVE | {"nop"}:
            continue
        out[i] = line.replace(args[-1], j[1].strip(), 1) if line.rstrip().endswith(args[-1]) else line
        out[i + 1] = lines[k + 1]
    return out


if __name__ == "__main__":
    text = sys.stdin.read().split("\n")
    sys.stdout.write("\n".join(steal_jump_slot(fill(load_via_dest(text)))))
