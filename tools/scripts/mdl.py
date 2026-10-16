#!/usr/bin/env python3
"""Model files (CRASHBSH.DAT .mdl entries): their meshes to OBJ and back, byte for byte.

Runs inside the toolchain container:
    tools/dk python3 tools/scripts/mdl.py unpack assets/CRASHBSH/001.bin   -> 001.mdl/
    tools/dk python3 tools/scripts/mdl.py pack assets/CRASHBSH/001.mdl OUT
`dat.py extract` unpacks every model file it can read, and `make disc` packs them.

Only the meshes' geometry is decoded so far (docs/ASSETS.md, as CTR-tools reads it:
github.com/DCxDemo/CTR-tools, CTRFramework/Code/bash/BashMesh.cs): a u32 mesh count at 0x54, then
a 0x34-byte header per mesh with offsets relative to their own position, to the vertices (after a
0x14-byte block: s16 x, y, z and a u16 not decoded, 8 bytes each) and to the triangle strips (u16
each, faces << 8 | flags, until 0xFFFF; a strip of n faces takes n + 2 vertices in order). The
rest (texture coordinates, per-face data, animation: most of each file) isn't decoded.

<id>.mdl/ holds model.bin (the whole file, as it is) and model.obj (one object per mesh, its
vertices as the file's integers with y and z negated, CTR-tools' orientation, and the strips'
faces). pack writes the OBJ's vertex positions into model.bin: move vertices, but keep each
mesh's count and order (Blender: import with Split > Keep Vert Order).
"""
import os
import struct
import sys

COUNT_AT = 0x54
HEAD = 0x34
OBJ = "model.obj"
RAW = "model.bin"


class Bad(Exception):
    pass


def meshes(d):
    """[(offset of the first vertex, vertex count, strips)] of a model file; Bad if it isn't one."""
    if len(d) < COUNT_AT + 4:
        raise Bad("short")
    n = struct.unpack_from("<I", d, COUNT_AT)[0]
    if not 0 < n <= 256 or COUNT_AT + 4 + n * HEAD > len(d):
        raise Bad("no mesh count")
    out = []
    for m in range(n):
        pos = COUNT_AT + 4 + m * HEAD
        z = struct.unpack_from("<IIhhI", d, pos)
        if z[0] or z[1]:
            raise Bad(f"mesh {m}: header doesn't start with zeros")

        def ptr(k):
            p = pos + 0x10 + 4 * k
            return p + struct.unpack_from("<i", d, p)[0]
        vp, sp, ap = ptr(0), ptr(1), ptr(2)
        nv = (ap - vp - 0x14) // 8
        if not (0 <= vp and vp + 0x14 + 8 * nv <= len(d) and 0 <= sp < len(d)) or nv < 0:
            raise Bad(f"mesh {m}: offsets out of the file")
        strips, q, used = [], sp, 0
        while True:
            if q + 2 > len(d):
                raise Bad(f"mesh {m}: strips run off the end")
            s = struct.unpack_from("<H", d, q)[0]
            q += 2
            if s == 0xFFFF:
                break
            strips.append(s)
            used += (s >> 8) + 2
        if used != nv:
            raise Bad(f"mesh {m}: strips use {used} vertices, it has {nv}")
        out.append((vp + 0x14, nv, strips))
    return out


def parse(d):
    meshes(d)


def faces(strips):
    """1-based (a, b, c) per face, winding alternating along a strip as CTR-tools draws it."""
    out, cur = [], 1
    for s in strips:
        flip0 = 1 if s & 0x08 else 0
        for i in range(s >> 8):
            a, b, c = cur + i, cur + i + 1, cur + i + 2
            out.append((a, c, b) if i % 2 == flip0 else (a, b, c))
        cur += (s >> 8) + 2
    return out


def unpack(src, dest):
    d = open(src, "rb").read()
    ms = meshes(d)
    os.makedirs(dest, exist_ok=True)
    with open(os.path.join(dest, RAW), "wb") as f:
        f.write(d)
    lines = [f"# {os.path.basename(src)}: {len(ms)} meshes; keep each one's vertex count and order"]
    base = 0
    for m, (vp, nv, strips) in enumerate(ms):
        lines.append(f"o mesh_{m:02d}")
        for i in range(nv):
            x, y, z = struct.unpack_from("<hhh", d, vp + 8 * i)
            lines.append(f"v {x} {-y} {-z}")
        lines += [f"f {a + base} {b + base} {c + base}" for a, b, c in faces(strips)]
        base += nv
    with open(os.path.join(dest, OBJ), "w") as f:
        f.write("\n".join(lines) + "\n")


def obj_vertices(path):
    """[[(x, y, z)] per object] of an OBJ, in order."""
    objs = []
    for line in open(path):
        w = line.split()
        if not w:
            continue
        if w[0] == "o":
            objs.append([])            # every object, empty ones too (meshes with no vertices)
        elif w[0] == "v":
            if not objs:
                objs.append([])
            objs[-1].append(tuple(round(float(v)) for v in w[1:4]))
    return objs


def pack(src):
    d = bytearray(open(os.path.join(src, RAW), "rb").read())
    ms = meshes(bytes(d))
    objs = obj_vertices(os.path.join(src, OBJ))
    if len(objs) != len(ms):
        raise Bad(f"{OBJ} has {len(objs)} objects, the file {len(ms)} meshes")
    for m, ((vp, nv, _), vs) in enumerate(zip(ms, objs)):
        if len(vs) != nv:
            raise Bad(f"{OBJ} mesh_{m:02d} has {len(vs)} vertices, the file {nv} (keep the count and order)")
        for i, (x, y, z) in enumerate(vs):
            if not all(-32768 <= v <= 32767 for v in (x, -y, -z)):
                raise Bad(f"{OBJ} mesh_{m:02d} vertex {i} is out of range (16-bit)")
            struct.pack_into("<hhh", d, vp + 8 * i, x, -y, -z)
    return bytes(d)


def main():
    if len(sys.argv) < 3 or sys.argv[1] not in ("unpack", "pack"):
        sys.exit(__doc__.split("\n\n")[1])
    if sys.argv[1] == "unpack":
        src = sys.argv[2]
        unpack(src, os.path.splitext(src)[0] + ".mdl")
    else:
        with open(sys.argv[3], "wb") as fh:
            fh.write(pack(sys.argv[2]))


if __name__ == "__main__":
    main()
