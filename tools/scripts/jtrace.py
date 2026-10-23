"""Trace jump2's cross-jumping for one function (decomp.py jtrace).

jump2 (the jump_optimize run after reload, with cross_jump set) decides which copy of a
repeated tail survives, and the -dJ dump shows only the result. The container's cc1 keeps its
symbols, so this runs it under ptrace with breakpoints on jump_optimize, find_cross_jump (entry
and return), do_cross_jump and redirect_jump, and logs every step by insn uid:

- `fcj E1 vs E2 min N -> F1 F2` (find_cross_jump): the jump E1 compares the insns before it
  with those before E2 (a label for the first try, then each jump in the label's chain), and
  matches from F1 (its own copy) and F2 (the copy it would jump to), or `no`;
- `CROSS J: delete from F1, keep from F2` (do_cross_jump): J gave its copy up;
- `redirect J: OLD -> NEW` (redirect_jump), which also puts J at the head of NEW's chain.

Each uid is annotated with its insn from the .greg dump (the RTL jump2 starts from); labels
jump2 makes have no dump line. The rules behind the order are in MATCHING.md (Cross-jumping)
and tools/reference/gcc-2.8.1/jump.c."""
import ctypes
import os
import re
import signal
import subprocess

INSN, JUMP_INSN, CALL_INSN, BARRIER, CODE_LABEL, NOTE = 27, 28, 29, 30, 31, 32
KIND = {INSN: "i", JUMP_INSN: "J", CALL_INSN: "C", BARRIER: "B", CODE_LABEL: "L", NOTE: "n"}
SYMS = ("jump_optimize", "find_cross_jump", "do_cross_jump", "redirect_jump", "current_function_name")
JUMP_LABEL_OFS = 32         # JUMP_INSN "iuueiee0": field 7 is JUMP_LABEL, after the 4-byte header


class Regs(ctypes.Structure):
    _fields_ = [(n, ctypes.c_ulonglong) for n in (
        "r15 r14 r13 r12 rbp rbx r11 r10 r9 r8 rax rcx rdx rsi rdi orig_rax "
        "rip cs eflags rsp ss fs_base gs_base ds es fs gs").split()]


def symbols(cc1):
    """Addresses of the traced functions and globals, from nm (cc1 and cc1plus differ)."""
    out = subprocess.run(["nm", cc1], capture_output=True, text=True).stdout
    found = {}
    for line in out.splitlines():
        p = line.split()
        if len(p) == 3 and p[2] in SYMS:
            found[p[2]] = int(p[0], 16)
    missing = [s for s in SYMS if s not in found]
    if missing:
        raise SystemExit(f"{cc1}: no symbol for {', '.join(missing)} (a stripped cc1?)")
    return found


class Tracer:
    PEEK, POKE, CONT, STEP, GETREGS, SETREGS = 1, 4, 7, 9, 12, 13

    def __init__(self, argv):
        self.libc = ctypes.CDLL("libc.so.6", use_errno=True)
        self.libc.ptrace.restype = ctypes.c_long
        self.libc.ptrace.argtypes = [ctypes.c_long, ctypes.c_long, ctypes.c_void_p, ctypes.c_void_p]
        self.pid = os.fork()
        if self.pid == 0:
            self.libc.ptrace(0, 0, None, None)                  # PTRACE_TRACEME
            fd = os.open(os.devnull, os.O_WRONLY)
            os.dup2(fd, 2)                                      # cc1's warnings
            os.execv(argv[0], argv)
        self.wait()
        self.orig = {}

    def ptrace(self, req, addr=None, data=None):
        return self.libc.ptrace(req, self.pid, ctypes.c_void_p(addr) if addr is not None else None,
                                ctypes.c_void_p(data) if data is not None else None)

    def peek(self, addr):
        return self.ptrace(self.PEEK, addr) & 0xFFFFFFFFFFFFFFFF

    def word(self, addr):
        return self.peek(addr) & 0xFFFFFFFF

    def string(self, addr, limit=200):
        out = bytearray()
        while addr and len(out) < limit:
            b = self.peek(addr) & 0xFF
            if not b:
                break
            out.append(b)
            addr += 1
        return out.decode("latin-1")

    def regs(self):
        r = Regs()
        self.ptrace(self.GETREGS, None, ctypes.addressof(r))
        return r

    def wait(self):
        return os.waitpid(self.pid, 0)[1]

    def set_bp(self, addr):
        if addr not in self.orig:
            w = self.peek(addr)
            self.orig[addr] = w
            self.ptrace(self.POKE, addr, (w & ~0xFF) | 0xCC)

    def step_over(self, addr, regs, keep):
        """Run the original instruction at ADDR once, then re-arm the breakpoint if KEEP."""
        w = self.orig[addr]
        self.ptrace(self.POKE, addr, w)
        regs.rip = addr
        self.ptrace(self.SETREGS, None, ctypes.addressof(regs))
        self.ptrace(self.STEP)
        self.wait()
        if keep:
            self.ptrace(self.POKE, addr, (w & ~0xFF) | 0xCC)
        else:
            del self.orig[addr]


def trace(argv, func):
    """Run cc1 ARGV and return the jump2 events for FUNC: one list per jump2 run of it."""
    sym = symbols(argv[0])
    t = Tracer(argv)
    entry = {sym[s]: s for s in ("jump_optimize", "find_cross_jump", "do_cross_jump", "redirect_jump")}
    for a in entry:
        t.set_bp(a)

    def uid(x):
        return f"{KIND.get(t.word(x) & 0xFFFF, '?')}{t.word(x + 4)}" if x else "0"

    runs, active, pending = [], False, {}
    while True:
        t.ptrace(t.CONT)
        st = t.wait()
        if os.WIFEXITED(st) or os.WIFSIGNALED(st):
            break
        if not os.WIFSTOPPED(st) or os.WSTOPSIG(st) != signal.SIGTRAP:
            continue
        r = t.regs()
        at = (r.rip - 1) & 0xFFFFFFFF
        if at not in t.orig:
            continue
        esp = r.rsp & 0xFFFFFFFF
        arg = [t.word(esp + 4 * k) for k in range(6)]
        name = entry.get(at)
        if name == "jump_optimize":
            active = arg[2] != 0 and t.string(t.word(sym["current_function_name"])) == func
            if active:
                runs.append([])
        elif at in pending:
            f1, f2, e1, e2, mn = pending.pop(at)
            got = (uid(t.word(f1)), uid(t.word(f2))) if t.word(f1) else None
            runs[-1].append(("fcj", e1, e2, mn, got))
        elif active and name == "find_cross_jump":
            mn = arg[3] - (1 << 32) if arg[3] & 0x80000000 else arg[3]
            pending[arg[0]] = (arg[4], arg[5], uid(arg[1]), uid(arg[2]), mn)
            t.set_bp(arg[0])
        elif active and name == "do_cross_jump":
            runs[-1].append(("cross", uid(arg[1]), uid(arg[2]), uid(arg[3])))
        elif active and name == "redirect_jump":
            runs[-1].append(("redirect", uid(arg[1]), uid(t.word(arg[1] + JUMP_LABEL_OFS)), uid(arg[2])))
        t.step_over(at, r, at in entry or at in pending)
    return runs


INSN_RE = re.compile(r"^\((insn|jump_insn|call_insn|code_label|note|barrier) (\d+) ")


def insns(dump, func):
    """{uid: one-line summary} for FUNC's insns in a .greg (or any RTL) dump."""
    m = re.search(rf"^;; Function {re.escape(func)}\n", dump, re.M)
    if not m:
        return {}
    end = dump.find("\n;; Function ", m.end())
    text = dump[m.end():end if end >= 0 else len(dump)]
    out, cur, depth = {}, [], 0
    for line in text.splitlines():
        if not cur and not INSN_RE.match(line):
            continue
        cur.append(line.strip())
        depth += line.count("(") - line.count(")")
        if depth <= 0:
            whole = " ".join(cur)
            kind, u = INSN_RE.match(whole).groups()
            out[int(u)] = summary(kind, whole)
            cur, depth = [], 0
    return out


def first_sexp(s):
    """The first balanced (...) in S."""
    start = s.find("(")
    depth = 0
    for k in range(start, len(s)):
        depth += {"(": 1, ")": -1}.get(s[k], 0)
        if depth == 0:
            return s[start:k + 1]
    return s[start:]


def summary(kind, whole):
    if kind == "code_label":
        m = re.match(r"\(code_label \d+ \d+ \d+ (\d+)", whole)
        return f"label $L{m.group(1)}" if m else "label"
    if kind in ("note", "barrier"):
        return kind
    body = first_sexp(re.sub(r"^\(\w+ \d+ \d+ \d+ ", "", whole))   # the pattern, not the notes
    body = re.sub(r"\(reg(?:/\w+)?:\w+ \d+ (\w+)\)", r"\1", body)
    body = re.sub(r"\(reg(?:/\w+)?:\w+ (\d+)\)", r"r\1", body)
    body = re.sub(r"\(const_int (-?\d+)( \[\w+\])?\)", r"\1", body)
    body = re.sub(r"\(label_ref (\d+)\)", r"L\1", body)
    body = re.sub(r":SI|:HI|:QI|/[a-z]+", "", body)
    return " ".join(body.split())[:90]


def report(runs, names, cross_only=False):
    def show(u):
        n = names.get(int(u[1:])) if u[1:].isdigit() else None
        return f"{u} [{n}]" if n else u

    out = []
    for k, run in enumerate(runs, 1):
        out.append(f"jump2 run {k}: {sum(e[0] == 'cross' for e in run)} cross-jumps")
        for e in run:
            if e[0] == "fcj" and not cross_only:
                res = f"{e[4][0]} {e[4][1]}" if e[4] else "no"
                out.append(f"  fcj {show(e[1])} vs {e[2]} min {e[3]} -> {res}")
            elif e[0] == "cross":
                out.append(f"CROSS {show(e[1])}: delete from {show(e[2])}, keep from {show(e[3])}")
            elif e[0] == "redirect" and not cross_only:
                out.append(f"  redirect {e[1]}: {e[2]} -> {e[3]}")
    return "\n".join(out)
