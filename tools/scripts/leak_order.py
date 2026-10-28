"""File evidence from a leaked header's declaration order (ARCHITECTURE.md, Files inside the slot
overlays). cc1plus writes a file's functions in source order and then its out-of-line inline
functions in reverse, so against the header's order (definition order) one file reads as a climb
and then a fall. This finds the climb/fall split that explains the most functions, and how many
more a second file (climb, fall, climb, fall) would explain.

    tools/dk python3 tools/scripts/leak_order.py pogo include/bash/pogo.h [--seq]

Only functions the header declares count; "explained" is the longest increasing run of header
positions before the split plus the longest decreasing run after it (not necessarily
consecutive), so a few out of place don't hide the shape."""
import argparse
import bisect
import re
import subprocess


def declared_order(text):
    """Function names in the order the header first declares or defines them."""
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.S)
    out = []
    for m in re.finditer(r"^[A-Za-z_][\w \*]*?\b(\w+)\s*\([^;{]*\)\s*[;{]", text, re.M):
        if m.group(1) not in out:
            out.append(m.group(1))
    return out


def text_functions(ovl):
    """[(address, name)] of the functions in build/OVL.ovl.elf's text, in address order."""
    nm = subprocess.run(["mipsel-linux-gnu-nm", "-n", f"build/{ovl}.ovl.elf"],
                        capture_output=True, text=True, check=True).stdout
    rows = [l.split() for l in nm.splitlines() if len(l.split()) == 3]
    lo = next(int(a, 16) for a, _, n in rows if n == f"{ovl}_TEXT_START")
    hi = next(int(a, 16) for a, _, n in rows if n == f"{ovl}_TEXT_END")
    out = []
    for a, t, n in rows:
        a = int(a, 16)
        if t in "tT" and lo <= a < hi and not n.startswith((ovl + "_", "jtbl", ".", "gcc2", "__gnu")):
            if not out or out[-1][0] != a:
                out.append((a, n))
    return out


def lis_table(seq):
    """t[i][j]: longest strictly increasing subsequence of seq[i:j]."""
    n = len(seq)
    t = [[0] * (n + 1) for _ in range(n + 1)]
    for i in range(n):
        tails = []
        for j in range(i, n):
            k = bisect.bisect_left(tails, seq[j])
            tails[k:k + 1] = [seq[j]]
            t[i][j + 1] = len(tails)
    return t


def best_splits(pos):
    """(one, two): one = (explained, tail start) for climb+fall, two = (explained, (a, b, c))
    for climb+fall+climb+fall with the second file starting at b; indices into pos."""
    n = len(pos)
    up = lis_table(pos)
    down = lis_table([-p for p in pos])
    # ties go to the earliest split
    one = max((up[0][k] + down[k][n], -k) for k in range(n + 1))
    one = (one[0], -one[1])
    # two files: best climb+fall for each prefix and each suffix
    pre = [max(up[0][k] + down[k][e] for k in range(e + 1)) for e in range(n + 1)]
    two = (0, None)
    for b in range(n + 1):
        suf = max(up[b][k] + down[k][n] for k in range(b, n + 1))
        if pre[b] + suf > two[0]:
            two = (pre[b] + suf, b)
    return one, two


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("ovl")
    ap.add_argument("header")
    ap.add_argument("--seq", action="store_true", help="print each function's header position")
    a = ap.parse_args()
    order = declared_order(open(a.header, encoding="utf-8").read())
    where = {n: i for i, n in enumerate(order)}
    funcs = text_functions(a.ovl)
    rows = [(i, addr, n, where[n]) for i, (addr, n) in enumerate(funcs, 1) if n in where]
    print(f"{a.ovl}: {len(funcs)} functions, {len(rows)} declared in {a.header}")
    if a.seq:
        for i, addr, n, p in rows:
            print(f"  {i:4} {addr:08X} {p:4} {n}")
    pos = [p for *_, p in rows]
    (e1, k), (e2, b) = best_splits(pos)
    tail = rows[k] if k < len(rows) else None
    print(f"one file (climb, then the inline tail from function "
          f"{tail[0] if tail else '-'} {tail[2] if tail else ''}): explains {e1} of {len(pos)}")
    sec = rows[b] if b is not None and b < len(rows) else None
    print(f"two files (the second from function {sec[0] if sec else '-'} {sec[2] if sec else ''}): "
          f"explains {e2} of {len(pos)} (+{e2 - e1})")
    last = rows[-1][0] if rows else 0
    if last < len(funcs):
        print(f"functions {last + 1}..{len(funcs)} come after the last declared one: "
              "the header can't place them")


if __name__ == "__main__":
    main()
