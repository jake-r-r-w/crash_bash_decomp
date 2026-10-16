#!/usr/bin/env python3
"""Mode text (CRASHBSH.DAT overlays/text/*.bin): its strings to a text file and back.

Runs inside the toolchain container:
    tools/dk python3 tools/scripts/txt.py unpack assets/CRASHBSH/08D.bin   -> 08D.txt/
    tools/dk python3 tools/scripts/txt.py pack assets/CRASHBSH/08D.txt OUT
`dat.py extract` unpacks them, and `make disc` packs them.

Each file (docs/ASSETS.md) is a u32 (the file's number: 7 to 18), then the mode's strings (zero-
terminated, each starting on a 4-byte boundary), then data that points at them by absolute
address. So a string can change but not move or grow: it has its slot, from its start to the
4-byte boundary after its terminator. <id>.txt/ holds data.bin (the file as it is) and text.txt
(one line per string: its offset, then the text; \\n and \\r are the bytes 0A and 0D, the game's
line breaks (0D is followed by two digits), \\xNN any other byte (FC-FF look like button icons),
\\\\ a backslash). pack writes each changed line into its slot, zero-padded, and refuses one too
long; an unchanged one keeps its slot as it is (four files hold, after a terminator, the tail of
a string cut short in development: "...SCORE TO ZERO", then "9IN THE ALLOTTED TIME").
"""
import os
import re
import struct
import sys

DATA = "data.bin"
TEXT = "text.txt"
CHARS = set(range(0x20, 0x7F)) | {0x0A, 0x0D} | set(range(0xF0, 0x100))
LINE = re.compile(r"^@([0-9A-F]{4}): (.*)$")


class Bad(Exception):
    pass


def strings(d):
    """[(offset, bytes, slot size)] of the strings after the first u32."""
    out, p = [], 4
    while p < len(d):
        e = d.find(b"\0", p)
        if e <= p or any(b not in CHARS for b in d[p:e]):
            break
        nxt = (e + 4) & ~3
        out.append((p, d[p:e], min(nxt, len(d)) - p))
        while d[nxt:nxt + 4] == bytes(4):        # zero words between strings stay as they are
            nxt += 4
        p = nxt
    return out


def parse(d):
    if len(d) < 8 or struct.unpack_from("<I", d)[0] > 0xFFFF:
        raise Bad("no header")
    s = strings(d)
    if len(s) < 3 or sum(len(t) for _, t, _ in s) < 32:
        raise Bad("not a text file")
    return s


def escape(b):
    out = []
    for c in b:
        out.append("\\n" if c == 0x0A else "\\r" if c == 0x0D else "\\\\" if c == 0x5C
                   else chr(c) if 0x20 <= c < 0x7F else f"\\x{c:02X}")
    return "".join(out)


def unescape(s, where):
    out, i = bytearray(), 0
    while i < len(s):
        c = s[i]
        if c != "\\":
            if not 0x20 <= ord(c) < 0x7F:
                raise Bad(f"{where}: {c!r} isn't in the game's character set (ASCII; \\xNN for others)")
            out.append(ord(c))
            i += 1
            continue
        nxt = s[i + 1:i + 2]
        if nxt in ("n", "r", "\\"):
            out.append({"n": 0x0A, "r": 0x0D, "\\": 0x5C}[nxt])
            i += 2
        elif nxt == "x" and re.fullmatch(r"[0-9A-Fa-f]{2}", s[i + 2:i + 4]):
            out.append(int(s[i + 2:i + 4], 16))
            i += 4
        else:
            raise Bad(f"{where}: bad escape {s[i:i + 4]!r}")
    return bytes(out)


def unpack(src, dest):
    d = open(src, "rb").read()
    s = parse(d)
    os.makedirs(dest, exist_ok=True)
    with open(os.path.join(dest, DATA), "wb") as f:
        f.write(d)
    with open(os.path.join(dest, TEXT), "w", newline="\n") as f:
        f.write(f"# {os.path.basename(src)}: one string a line. Keep the @offset; a string may not\n"
                "# be longer than it was by more than its padding (pack says).\n")
        for off, t, _ in s:
            f.write(f"@{off:04X}: {escape(t)}\n")


def pack(src):
    d = bytearray(open(os.path.join(src, DATA), "rb").read())
    slots = {off: size for off, _, size in strings(bytes(d))}
    seen = set()
    for n, line in enumerate(open(os.path.join(src, TEXT), newline="\n"), 1):
        line = line.rstrip("\n")
        if not line or line.startswith("#"):
            continue
        m = LINE.match(line)
        if not m:
            raise Bad(f"{TEXT}:{n}: expected '@OFFSET: text'")
        off = int(m.group(1), 16)
        if off not in slots:
            raise Bad(f"{TEXT}:{n}: no string starts at @{off:04X}")
        b = unescape(m.group(2), f"{TEXT}:{n}")
        if len(b) + 1 > slots[off]:
            raise Bad(f"{TEXT}:{n}: {len(b)} bytes, the slot holds {slots[off] - 1}")
        if d[off:off + len(b)] != b or d[off + len(b)]:
            # changed: zero the rest of the slot. An unchanged one keeps what follows its
            # terminator: some hold the tail of a string cut short in development
            d[off:off + slots[off]] = b + bytes(slots[off] - len(b))
        seen.add(off)
    if seen != set(slots):
        raise Bad(f"{TEXT}: missing strings at " + ", ".join(f"@{o:04X}" for o in sorted(set(slots) - seen)))
    return bytes(d)


def main():
    if len(sys.argv) < 3 or sys.argv[1] not in ("unpack", "pack"):
        sys.exit(__doc__.split("\n\n")[1])
    if sys.argv[1] == "unpack":
        src = sys.argv[2]
        unpack(src, os.path.splitext(src)[0] + ".txt")
    else:
        with open(sys.argv[3], "wb") as fh:
            fh.write(pack(sys.argv[2]))


if __name__ == "__main__":
    main()
