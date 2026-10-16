#!/usr/bin/env python3
"""Model files (CRASHBSH.DAT .mdl entries): their meshes to OBJ (and glTF) and back, byte for byte.

Runs inside the toolchain container:
    tools/dk python3 tools/scripts/mdl.py unpack assets/CRASHBSH/001.bin   -> 001.mdl/
    tools/dk python3 tools/scripts/mdl.py pack assets/CRASHBSH/001.mdl OUT
`dat.py extract` unpacks every model file, and `make disc` packs them.

The format is in docs/ASSETS.md (from CTR-tools' BashMesh.cs and the game's own code: main.c's
func_80017EE8, func_80017D90, func_8001C0F0). A u32 mesh count at 0x54, then a 0x34-byte header
per mesh with self-relative offsets to its vertices (after a 0x14-byte bounding box: s16 x, y,
z, u16), triangle strips (u16 faces << 8 | flags until 0xFFFF; n faces take n + 2 vertices),
per-face UV indices, texture runs and colour indices, and (lit meshes) normals.

<id>.mdl/ holds model.bin (the whole file, as it is), model.obj and model.mtl (one object per
mesh: its vertices as the file's integers with y and z negated, CTR-tools' orientation, its
normals, and the strips' faces with texture coordinates and materials from the texture set
before the model in the DAT), and model.glb (gltf.py: the same with the animations). pack writes
the OBJ's vertex positions and normals into model.bin: move vertices, but keep each mesh's count
and order (Blender: import with Split > Keep Vert Order). UVs, materials and animations are for
viewing; pack ignores them.
"""
import os
import struct
import sys

COUNT_AT = 0x54
HEAD = 0x34
OBJ = "model.obj"
MTL = "model.mtl"
GLB = "model.glb"                      # gltf.py: meshes, textures, animations (view only)
RAW = "model.bin"


class Bad(Exception):
    pass


def meshes(d):
    """[(first vertex offset, vertex count, strips, first normal offset or None)] of a model file;
    Bad if it isn't one. A file may have no meshes (the fonts, some arenas): its header's offset
    to the end of the meshes (+0x20, counted from 0x1C) then points at the empty count."""
    if len(d) < COUNT_AT + 4:
        raise Bad("short")
    n = struct.unpack_from("<I", d, COUNT_AT)[0]
    if n == 0:
        if 0x1C + struct.unpack_from("<I", d, 0x20)[0] != COUNT_AT:
            raise Bad("no mesh count")
        return []
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
        if not (0 <= vp < len(d) and 0 <= sp < len(d)):
            raise Bad(f"mesh {m}: offsets out of the file")
        strips, q, nv = [], sp, 0
        while True:
            if q + 2 > len(d):
                raise Bad(f"mesh {m}: strips run off the end")
            s = struct.unpack_from("<H", d, q)[0]
            q += 2
            if s == 0xFFFF:
                break
            strips.append(s)
            nv += (s >> 8) + 2
        end = vp + 0x14 + 8 * nv
        normals = None
        if struct.unpack_from("<i", d, pos + 0x28)[0]:      # lit: a normals block after the positions
            if ptr(6) != end:
                raise Bad(f"mesh {m}: normals don't follow the positions")
            normals, end = end, end + 8 * nv
        if end != ap or end > len(d):
            raise Bad(f"mesh {m}: strips use {nv} vertices, the block holds {(ap - vp - 0x14) // 8}")
        out.append((vp + 0x14, nv, strips, normals))
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


def textures(dest):
    """The texture set before this model in the DAT (dat.py decodes it first), as a dict: its
    folder, json and cluts, or None. A model's texture v is that set's image v; a face with bit 15
    uses its last image with CLUT N (the engine's func_80017EE8)."""
    folder = os.path.dirname(os.path.abspath(dest))
    try:
        prev = f"{int(os.path.basename(dest).split('.')[0], 16) - 1:03X}.tex"
    except ValueError:
        return None
    tdir = os.path.join(folder, prev)
    if not os.path.exists(os.path.join(tdir, "tex.json")):
        return None
    import json
    import tex
    meta = json.load(open(os.path.join(tdir, "tex.json")))
    return {"dir": tdir, "name": prev, "meta": meta, "tex": tex}


def image_size(tx, v):
    f = tx["meta"]["images"][v]["fields"]
    depth = tx["tex"].mode(f)[0]
    return f[0] * (16 // depth), f[1]


def recolour(tx, v, clut, path):
    """A copy of image v with CLUT `clut`, for viewing (pack never reads it)."""
    from PIL import Image
    e = tx["meta"]["images"][v]
    im = Image.open(os.path.join(tx["dir"], e["file"]))
    rows = Image.open(os.path.join(tx["dir"], "cluts.png")).convert("RGBA")
    n = tx["meta"]["cluts"][clut]
    im.putpalette([c for x in range(n) for c in rows.getpixel((x, clut))[:3]])
    im.save(path)


def face_uvs(d, pos, nf, strips, tx, dest, made):
    """Per face (material, [(u, v)] in strip order) or None when untextured."""
    def ptr(k):
        p = pos + 0x10 + 4 * k
        return p + struct.unpack_from("<i", d, p)[0]
    uv_idx, tex_at = ptr(2), ptr(3)
    uvs_at = 0x24 + struct.unpack_from("<i", d, 0x24)[0]
    cols_at = 0x20 + struct.unpack_from("<i", d, 0x20)[0]
    col_idx = ptr(4)
    flat = [s & 1 for s in strips for _ in range(s >> 8)]
    out, left, q, t = [], 0, tex_at, 0
    for f in range(nf):
        left -= 1
        if left < 0:
            t = struct.unpack_from("<H", d, q)[0]
            q += 2
            left = (t >> 9) & 0x3F
        if f >= len(flat) or flat[f]:
            # untextured: Gouraud colours, 3 u32 from the colour table (func_80017D90); the
            # material is their average
            k = struct.unpack_from("<H", d, col_idx + 2 * f)[0] & 0x1FFF
            cs = [struct.unpack_from("<4B", d, cols_at + 4 * (k + j)) for j in range(3)]
            rgb = tuple(sum(c[j] for c in cs) // 3 for j in range(3))
            mat = "col_%02x%02x%02x" % rgb
            made.setdefault(mat, rgb)
            out.append((mat, None))
            continue
        if t & 0x8000:
            v, clut = len(tx["meta"]["images"]) - 1, t & 0x1FF
            mat = f"tex_{v:03d}_clut_{clut:03d}"
            if mat not in made:
                made[mat] = f"{mat}.png"
                recolour(tx, v, clut, os.path.join(dest, made[mat]))
        else:
            v = t & 0x1FF
            mat = f"tex_{v:03d}"
            made.setdefault(mat, f"../{tx['name']}/{tx['meta']['images'][v]['file']}")
        w, h = image_size(tx, v)
        k = struct.unpack_from("<H", d, uv_idx + 2 * f)[0]
        corners = struct.unpack_from("<3H", d, uvs_at + 2 * k)
        out.append((mat, [((c & 0xFF) / w, 1 - (c >> 8) / h) for c in corners]))
    return out


def unpack(src, dest):
    d = open(src, "rb").read()
    ms = meshes(d)
    os.makedirs(dest, exist_ok=True)
    with open(os.path.join(dest, RAW), "wb") as f:
        f.write(d)
    tx = textures(dest) if ms else None
    lines = [f"# {os.path.basename(src)}: {len(ms)} meshes; keep each one's vertex count and order"]
    if tx:
        lines.append(f"mtllib {MTL}")
    made, base, vt, glb_faces = {}, 0, 0, []
    for m, (vp, nv, strips, np_) in enumerate(ms):
        glb_faces.append([])
        lines.append(f"o mesh_{m:02d}")
        for i in range(nv):
            x, y, z = struct.unpack_from("<hhh", d, vp + 8 * i)
            lines.append(f"v {x} {-y} {-z}")
        if np_ is not None:
            for i in range(nv):
                x, y, z = struct.unpack_from("<hhh", d, np_ + 8 * i)
                lines.append(f"vn {x} {-y} {-z}")
        pos = COUNT_AT + 4 + m * HEAD
        fl = faces(strips)
        uv = face_uvs(d, pos, len(fl), strips, tx, dest, made) if tx else [None] * len(fl)
        mat = None
        for (a, b, c), (strip_order), u in zip(fl, strip_corners(strips), uv):
            want = u[0] if u else ("untextured" if tx else None)
            if want != mat and want:
                lines.append(f"usemtl {want}")
                mat = want
            corner, at = {}, {}
            if u and u[1]:
                for vi, (uu, vv) in zip(strip_order, u[1]):
                    lines.append(f"vt {uu:.6f} {vv:.6f}")
                    vt += 1
                    corner[vi] = vt
                    at[vi] = (uu, vv)
            glb_faces[-1].append((want, (a - 1, b - 1, c - 1), [at[vi] for vi in (a, b, c)] if at else None))
            refs = []
            for vi in (a, b, c):
                ref = f"{vi + base}"
                if corner or np_ is not None:
                    ref += f"/{corner.get(vi, '')}"
                if np_ is not None:
                    ref += f"/{vi + base}"
                refs.append(ref)
            lines.append("f " + " ".join(refs))
        base += nv
    with open(os.path.join(dest, OBJ), "w") as f:
        f.write("\n".join(lines) + "\n")
    if tx:
        mtl = ["# materials: the paired texture set's images (view only: pack reads vertices only)",
               "newmtl untextured", "Kd 0.8 0.8 0.8"]
        for name, what in sorted(made.items()):
            if isinstance(what, tuple):
                mtl += [f"newmtl {name}", "Kd %.3f %.3f %.3f" % tuple(c / 255 for c in what)]
            else:
                mtl += [f"newmtl {name}", "Kd 1 1 1", f"map_Kd {what}"]
        with open(os.path.join(dest, MTL), "w") as f:
            f.write("\n".join(mtl) + "\n")
    if ms:
        import gltf
        mats = {k: (v if isinstance(v, tuple) else os.path.normpath(os.path.join(dest, v))) for k, v in made.items()}
        mats["untextured"] = (204, 204, 204)
        gltf.export(d, ms, glb_faces, mats, os.path.join(dest, GLB))


def strip_corners(strips):
    """Per face, its vertices (1-based, mesh-local) in strip order: the order of its UV corners."""
    out, cur = [], 1
    for s in strips:
        for i in range(s >> 8):
            out.append((cur + i, cur + i + 1, cur + i + 2))
        cur += (s >> 8) + 2
    return out


def obj_vertices(path):
    """[([(x, y, z)], [normal (x, y, z)]) per object] of an OBJ, in order."""
    objs = []
    for line in open(path):
        w = line.split()
        if not w:
            continue
        if w[0] == "o":
            objs.append(([], []))      # every object, empty ones too (meshes with no vertices)
        elif w[0] in ("v", "vn"):
            if not objs:
                objs.append(([], []))
            objs[-1][w[0] == "vn"].append(tuple(round(float(v)) for v in w[1:4]))
    return objs


def pack(src):
    d = bytearray(open(os.path.join(src, RAW), "rb").read())
    ms = meshes(bytes(d))
    objs = obj_vertices(os.path.join(src, OBJ))
    if len(objs) != len(ms):
        raise Bad(f"{OBJ} has {len(objs)} objects, the file {len(ms)} meshes")
    for m, ((vp, nv, _, np_), (vs, ns)) in enumerate(zip(ms, objs)):
        if len(vs) != nv:
            raise Bad(f"{OBJ} mesh_{m:02d} has {len(vs)} vertices, the file {nv} (keep the count and order)")
        if np_ is not None and len(ns) != nv:
            raise Bad(f"{OBJ} mesh_{m:02d} has {len(ns)} normals, the file {nv} (keep them)")
        for at, xs in ((vp, vs), (np_, ns if np_ is not None else [])):
            for i, (x, y, z) in enumerate(xs):
                if not all(-32768 <= v <= 32767 for v in (x, -y, -z)):
                    raise Bad(f"{OBJ} mesh_{m:02d} vertex {i} is out of range (16-bit)")
                struct.pack_into("<hhh", d, at + 8 * i, x, -y, -z)
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
