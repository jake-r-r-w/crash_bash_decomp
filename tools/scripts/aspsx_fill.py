"""Fill j/jr delay slots the way Sony's ASPSX does, on maspsx output (stdin -> stdout).

gcc 2.7.2 leaves `j` (and the `j $31` return) in reorder mode for the assembler to fill.
maspsx puts a nop there; ASPSX moves the instruction before the jump into the slot, and for
a store to a symbol (a two-instruction macro) it moves the second half:

    sw  $2,D_8006EA24          lui $at,%hi(D_8006EA24)
    j   $L1             ->     j   $L1
    nop                        sw  $2,%lo(D_8006EA24)($at)

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


if __name__ == "__main__":
    text = sys.stdin.read().split("\n")
    sys.stdout.write("\n".join(fill(text)))
