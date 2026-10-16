#!/usr/bin/env python3
"""Title screens (CRASHBSH.DAT .tga entries): TGA to PNG and back, byte for byte.

Runs inside the toolchain container:
    tools/dk python3 tools/scripts/tga.py unpack assets/CRASHBSH/088.bin   -> 088.tga/
    tools/dk python3 tools/scripts/tga.py pack assets/CRASHBSH/088.tga OUT
`dat.py extract` unpacks them, and `make disc` packs them.

They are plain TGA 2.0 files (docs/ASSETS.md): an 18-byte header (uncompressed true colour, 16
bits a pixel, 1 attribute bit, rows bottom-up), the pixels (x1r5g5b5, little-endian), then the
26-byte TGA footer. <id>.tga/ holds screen.png (RGB, top row first; 5 bits a channel, so pack
keeps the top 5 bits of each) and tga.json (the header and footer as they are). None of the five
sets the attribute bit; a file that does, or that isn't this kind of TGA, stays a .bin.
"""
import json
import os
import struct
import sys

from PIL import Image

PNG = "screen.png"
JSON = "tga.json"


class Bad(Exception):
    pass


def parse(d):
    """(header, width, height, pixels [u16, bottom row first], footer)."""
    if len(d) < 18:
        raise Bad("short")
    idlen, cmap, kind = d[0], d[1], d[2]
    w, h, bpp, desc = struct.unpack_from("<HHBB", d, 12)
    if idlen or cmap or kind != 2 or bpp != 16 or desc & 0x30 or not w or not h:
        raise Bad("not an uncompressed 16-bit bottom-up TGA")
    end = 18 + w * h * 2
    if end > len(d):
        raise Bad("pixels run past the end")
    px = struct.unpack_from(f"<{w * h}H", d, 18)
    if any(v & 0x8000 for v in px):
        raise Bad("uses the attribute bit")
    return d[:18], w, h, px, d[end:]


def unpack(src, dest):
    head, w, h, px, foot = parse(open(src, "rb").read())
    rgb = bytearray(w * h * 3)
    for y in range(h):
        row = px[(h - 1 - y) * w:(h - y) * w]
        for x, v in enumerate(row):
            o = (y * w + x) * 3
            rgb[o:o + 3] = bytes(((v >> 10 & 31) << 3, (v >> 5 & 31) << 3, (v & 31) << 3))
    os.makedirs(dest, exist_ok=True)
    Image.frombytes("RGB", (w, h), bytes(rgb)).save(os.path.join(dest, PNG))
    with open(os.path.join(dest, JSON), "w") as f:
        json.dump({"header": head.hex(), "footer": foot.hex()}, f, indent=1)
        f.write("\n")


def pack(src):
    meta = json.load(open(os.path.join(src, JSON)))
    head = bytes.fromhex(meta["header"])
    w, h = struct.unpack_from("<HH", head, 12)
    im = Image.open(os.path.join(src, PNG)).convert("RGB")
    if im.size != (w, h):
        raise Bad(f"{PNG} is {im.size[0]}x{im.size[1]}, the screen is {w}x{h}")
    rgb = im.tobytes()
    px = []
    for y in range(h - 1, -1, -1):
        for x in range(w):
            r, g, b = rgb[(y * w + x) * 3:(y * w + x) * 3 + 3]
            px.append((r >> 3) << 10 | (g >> 3) << 5 | b >> 3)
    return head + struct.pack(f"<{w * h}H", *px) + bytes.fromhex(meta["footer"])


def main():
    if len(sys.argv) < 3 or sys.argv[1] not in ("unpack", "pack"):
        sys.exit(__doc__.split("\n\n")[1])
    if sys.argv[1] == "unpack":
        src = sys.argv[2]
        unpack(src, os.path.splitext(src)[0] + ".tga")
    else:
        with open(sys.argv[3], "wb") as fh:
            fh.write(pack(sys.argv[2]))


if __name__ == "__main__":
    main()
