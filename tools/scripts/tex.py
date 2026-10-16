#!/usr/bin/env python3
"""Texture sets (CRASHBSH.DAT files of type 8): to PNGs and back, byte for byte.

Runs inside the toolchain container (Pillow is in its image):
    tools/dk python3 tools/scripts/tex.py unpack assets/CRASHBSH/000.bin   -> 000.tex/
    tools/dk python3 tools/scripts/tex.py pack assets/CRASHBSH/000.tex OUT
`dat.py extract` unpacks every texture set itself, and `make disc` packs them.

The format (docs/ASSETS.md):
    0x00 u32 type (8)        0x04 u32 size of the file      0x08 u16 images, u16 CLUTs
    0x0C u32 bytes from 0x0C to the first image (the CLUTs' end)
    0x10 16 bytes, kept as they are (CTR-tools: skip to palettes, unknown, the offset of the
         animated textures, 0)
    0x20 CLUTs: u32 n (16 or 256), then n 16-bit colours
    images: u16 w (16-bit VRAM units), u16 h, u16 a, u16 b, u32 c, u32 d, u32 e (kept as they
            are), then w * h * 2 bytes of pixels
    then the animated textures (u32 8, size, count, records) or zeros, kept as tail.bin

An image's s16 at +12 is its CLUT index << 1 | 1 for 8-bit (as CTR-tools reads it,
github.com/DCxDemo/CTR-tools, formats/bash_tex.ksy); an image with no CLUT of its own (negative,
or the wrong size) is shown in grey. img_NNN.png is indexed with that CLUT as its palette, for viewing: the pixel values are what
pack reads back, and the colours come from cluts.png (one row per CLUT), so edit colours there.
A colour is r, g, b (5 bits each, times 8) and its alpha is the semi-transparency bit: 255 off,
128 on, 0 for the colour 0x0000 (transparent).
"""
import json
import os
import struct
import sys

from PIL import Image

TYPE = 8
JSON = "tex.json"


class Bad(Exception):
    pass


def parse(d):
    """(header fields, raw 0x10..0x20, cluts [[u16]], images [(fields, pixel bytes)], tail)."""
    if len(d) < 0x20:
        raise Bad("short")
    typ, size, nimg, nclut, off = struct.unpack_from("<IIHHI", d)
    if typ != TYPE or size != len(d) or 0x0C + off > size:
        raise Bad("not a texture set")
    p, cluts = 0x20, []
    for _ in range(nclut):
        n = struct.unpack_from("<I", d, p)[0] if p + 4 <= size else 0
        if n not in (16, 256) or p + 4 + 2 * n > size:
            raise Bad(f"CLUT at 0x{p:X}")
        cluts.append(list(struct.unpack_from(f"<{n}H", d, p + 4)))
        p += 4 + 2 * n
    if p != 0x0C + off:
        raise Bad("CLUTs don't end at the image offset")
    images = []
    for _ in range(nimg):
        if p + 20 > size:
            raise Bad("image header past the end")
        f = struct.unpack_from("<HHHHIII", d, p)
        n = f[0] * f[1] * 2
        if p + 20 + n > size:
            raise Bad(f"image at 0x{p:X} past the end")
        images.append((list(f), d[p + 20:p + 20 + n]))
        p += 20 + n
    return d[0x10:0x20], cluts, images, d[p:]


def build(pre, cluts, images, tail):
    body = b"".join(struct.pack(f"<I{len(c)}H", len(c), *c) for c in cluts)
    imgs = b"".join(struct.pack("<HHHHIII", *f) + px for f, px in images)
    size = 0x20 + len(body) + len(imgs) + len(tail)
    head = struct.pack("<IIHHI", TYPE, size, len(images), len(cluts), 0x20 + len(body) - 0x0C)
    return head + pre + body + imgs + tail


# ---- colours: PSX 15-bit + STP <-> RGBA, exact both ways for what unpack writes

def to_rgba(c):
    if c == 0:
        return (0, 0, 0, 0)
    r, g, b = (c & 31) << 3, (c >> 5 & 31) << 3, (c >> 10 & 31) << 3
    return (r, g, b, 128 if c & 0x8000 else 255)


def from_rgba(r, g, b, a):
    if a == 0:
        return 0
    return (r >> 3) | (g >> 3) << 5 | (b >> 3) << 10 | (0x8000 if a < 192 else 0)


# ---- pixels: 4-bit (low nibble first) or 8-bit

LO = bytes(b & 15 for b in range(256))
HI = bytes(b >> 4 for b in range(256))


def to_indices(px, depth):
    if depth == 8:
        return bytes(px)
    out = bytearray(len(px) * 2)
    out[0::2] = px.translate(LO)
    out[1::2] = px.translate(HI)
    return bytes(out)


def from_indices(ix, depth):
    if depth == 8:
        return bytes(ix)
    return bytes(a | b << 4 for a, b in zip(ix[0::2], ix[1::2]))


GREY = [v for i in range(16) for v in (i * 17,) * 3]


def mode(f):
    """(depth, CLUT index) from an image's fields: the s16 at +12 is CLUT << 1 | 8-bit. Negative
    (the last image of some sets) means no CLUT of its own."""
    v = f[5] & 0xFFFF
    v -= 0x10000 if v & 0x8000 else 0
    return (8 if v & 1 else 4), v >> 1


def png_indices(path, w, h, depth):
    im = Image.open(path)
    if im.size != (w, h):
        raise Bad(f"{path} is {im.size[0]}x{im.size[1]}, the slot is {w}x{h}")
    if im.mode == "P":
        ix = im.tobytes()
    elif im.mode in ("L", "LA"):
        ix = im.convert("L").tobytes()
        if depth == 4:
            ix = bytes((v + 8) // 17 for v in ix)
    else:
        raise Bad(f"{path}: save it as an indexed or greyscale PNG, not {im.mode}")
    if max(ix, default=0) >= 1 << depth:
        raise Bad(f"{path} uses more than {1 << depth} colours")
    return ix


def unpack(src, dest):
    pre, cluts, images, tail = parse(open(src, "rb").read())
    os.makedirs(dest, exist_ok=True)
    meta = {"pre": pre.hex(), "cluts": [len(c) for c in cluts], "images": []}
    if cluts:
        width = max(len(c) for c in cluts)
        rows = b"".join(bytes(v for c in row + [0] * (width - len(row)) for v in to_rgba(c)) for row in cluts)
        Image.frombytes("RGBA", (width, len(cluts)), rows).save(os.path.join(dest, "cluts.png"))
    for i, (f, px) in enumerate(images):
        name = f"img_{i:03d}.png"
        depth, ci = mode(f)
        w, h = f[0] * (16 // depth), f[1]
        if w and h:
            im = Image.frombytes("P", (w, h), to_indices(px, depth))
            ok = 0 <= ci < len(cluts) and len(cluts[ci]) == 1 << depth
            im.putpalette([v for c in cluts[ci] for v in to_rgba(c)[:3]] if ok else GREY)
            im.save(os.path.join(dest, name))
        meta["images"].append({"file": name if w and h else None, "fields": f})
    if tail:
        with open(os.path.join(dest, "tail.bin"), "wb") as fh:
            fh.write(tail)
    with open(os.path.join(dest, JSON), "w") as fh:
        json.dump(meta, fh, indent=1)
        fh.write("\n")


def pack(src):
    meta = json.load(open(os.path.join(src, JSON)))
    cluts = []
    if meta["cluts"]:
        im = Image.open(os.path.join(src, "cluts.png")).convert("RGBA")
        data, width = im.tobytes(), im.width
        cluts = [[from_rgba(*data[(y * width + x) * 4:(y * width + x) * 4 + 4]) for x in range(n)]
                 for y, n in enumerate(meta["cluts"])]
    images = []
    for i, e in enumerate(meta["images"]):
        f = e["fields"]
        depth = mode(f)[0]
        px = (from_indices(png_indices(os.path.join(src, e["file"]), f[0] * (16 // depth), f[1], depth), depth)
              if e["file"] else b"")
        images.append((f, px))
    tp = os.path.join(src, "tail.bin")
    tail = open(tp, "rb").read() if os.path.exists(tp) else b""
    return build(bytes.fromhex(meta["pre"]), cluts, images, tail)


def main():
    if len(sys.argv) < 3 or sys.argv[1] not in ("unpack", "pack"):
        sys.exit(__doc__.split("\n\n")[1])
    if sys.argv[1] == "unpack":
        src = sys.argv[2]
        unpack(src, os.path.splitext(src)[0] + ".tex")
    else:
        with open(sys.argv[3], "wb") as fh:
            fh.write(pack(sys.argv[2]))


if __name__ == "__main__":
    main()
