#!/usr/bin/env python3
"""A model file's meshes, textures and animations as one glTF binary (model.glb), for viewing.

mdl.py calls export() after writing model.obj; pack never reads the .glb (the animation stays
as it is in model.bin). Blender: File > Import > glTF; each animation is an action, each
keyframe a shape key.

The animation layout (docs/ASSETS.md; the engine's func_80014xxx draw switch and func_8001C0F0):
the file's header word 0x40 counts animation entries, at 0x44 + [0x44], 0x18 bytes each: u32
start and end (file offsets of the animation's data), u32 frames, s32 offset from +0xC to the
mesh header it moves. The data: s32 offset to its own vertex pool (s16 x, y, z; 0: the file's
pool at 0x28 + [0x28]), then 16 bytes a frame: s32 offset (from the frame) to an index list,
s32 offset (from +4) to a second one, s32 blend (0..0x1000) and a u32. An index list is a 0x14-
byte bounding box, then a u16 per mesh vertex (pool index << 2 | flags). A frame shows the
first list's positions blended towards the second's by blend / 0x1000 (frames between two
keyframes); a frame whose blend is 0 is a keyframe. Timing isn't decoded: 30 frames a second.
"""
import json
import os
import struct
from array import array

SCALE = 1 / 256
FPS = 30


class Glb:
    def __init__(self):
        self.bin = bytearray()
        self.doc = {"asset": {"version": "2.0", "generator": "crash_bash_decomp tools/scripts/gltf.py"},
                    "buffers": [{}], "bufferViews": [], "accessors": [], "meshes": [], "nodes": [],
                    "scenes": [{"nodes": []}], "scene": 0}

    def view(self, data, target=None):
        while len(self.bin) % 4:
            self.bin.append(0)
        v = {"buffer": 0, "byteOffset": len(self.bin), "byteLength": len(data)}
        if target:
            v["target"] = target
        self.bin += data
        self.doc["bufferViews"].append(v)
        return len(self.doc["bufferViews"]) - 1

    def floats(self, values, kind, minmax=False):
        """An accessor for flat float values grouped as VEC2/VEC3/SCALAR/..."""
        n = {"SCALAR": 1, "VEC2": 2, "VEC3": 3}[kind]
        a = array("f", values)
        acc = {"bufferView": self.view(a.tobytes(), 34962 if kind != "SCALAR" else None),
               "componentType": 5126, "count": len(a) // n, "type": kind}
        if minmax and len(a):
            acc["min"] = [min(a[i::n]) for i in range(n)]
            acc["max"] = [max(a[i::n]) for i in range(n)]
        self.doc["accessors"].append(acc)
        return len(self.doc["accessors"]) - 1

    def shorts(self, values):
        """A VEC3 accessor of 16-bit integers (KHR_mesh_quantization): the game's own values,
        exact; the node's scale makes them units."""
        n = len(values) // 3
        a = array("h", bytes(8 * n))             # each element padded to 8 bytes: glTF wants vertex
        for i in range(n):                       # elements on 4-byte boundaries (stride 8)
            for k in range(3):
                a[4 * i + k] = max(-32768, min(32767, int(values[3 * i + k])))
        view = self.view(a.tobytes(), 34962)
        self.doc["bufferViews"][view]["byteStride"] = 8
        acc = {"bufferView": view, "componentType": 5122, "count": n,
               "type": "VEC3", "min": [min(values[i::3]) if n else 0 for i in range(3)],
               "max": [max(values[i::3]) if n else 0 for i in range(3)]}
        self.doc["accessors"].append(acc)
        ext = self.doc.setdefault("extensionsUsed", [])
        if "KHR_mesh_quantization" not in ext:
            ext.append("KHR_mesh_quantization")
            self.doc.setdefault("extensionsRequired", []).append("KHR_mesh_quantization")
        return len(self.doc["accessors"]) - 1

    def unorm16(self, values):
        """Animation weights as normalized u16 (core glTF allows it for weights)."""
        a = array("H", [max(0, min(65535, round(v * 65535))) for v in values])
        self.doc["accessors"].append({"bufferView": self.view(a.tobytes()), "componentType": 5123,
                                      "normalized": True, "count": len(a), "type": "SCALAR"})
        return len(self.doc["accessors"]) - 1

    def save(self, path):
        self.doc["buffers"][0]["byteLength"] = len(self.bin)
        js = json.dumps(self.doc, separators=(",", ":")).encode()
        js += b" " * (-len(js) % 4)
        while len(self.bin) % 4:
            self.bin.append(0)
        with open(path, "wb") as f:
            f.write(struct.pack("<III", 0x46546C67, 2, 12 + 8 + len(js) + 8 + len(self.bin)))
            f.write(struct.pack("<II", len(js), 0x4E4F534A) + js)
            f.write(struct.pack("<II", len(self.bin), 0x004E4942) + bytes(self.bin))


def animations(d, heads):
    """[(mesh index, [(list offset A, list offset B or None, blend 0..1)] per frame, pool offset)]."""
    out = []
    n = struct.unpack_from("<I", d, 0x40)[0]
    if not n:
        return out
    base = 0x44 + struct.unpack_from("<i", d, 0x44)[0]
    default_pool = 0x28 + struct.unpack_from("<i", d, 0x28)[0]
    for k in range(n):
        e = base + 0x18 * k
        if e + 0x18 > len(d):
            break
        start, end, nfr, moff = struct.unpack_from("<iiii", d, e)
        mesh = heads.get(e + 0xC + moff)
        if mesh is None or not 0 < start < end <= len(d) or not 0 < nfr <= 4096:
            continue
        own = struct.unpack_from("<i", d, start)[0]
        pool = start + own if own else default_pool
        frames = []
        for f in range(nfr):
            fr = start + 4 + 16 * f
            if fr + 16 > len(d):
                break
            a, b, p, _ = struct.unpack_from("<iiiI", d, fr)
            if a == 0:
                frames.append(None)
                continue
            la = fr + a + 0x14
            lb = fr + 4 + b + 0x14 if p else None
            frames.append((la, lb, p / 4096))
        out.append((mesh, frames, pool))
    return out


def export(d, ms, mesh_faces, materials, path):
    """d: the model file; ms: mdl.meshes(d); mesh_faces[m]: [(material, (v0, v1, v2) mesh-local
    0-based, [(u, v)] or None)] in OBJ winding; materials: name -> PNG path or (r, g, b)."""
    g = Glb()
    mat_index, images = {}, {}

    def material(name):
        if name in mat_index:
            return mat_index[name]
        what = materials.get(name)
        m = {"name": name, "pbrMetallicRoughness": {"metallicFactor": 0, "roughnessFactor": 1}}
        if isinstance(what, str) and os.path.exists(what):
            if what not in images:
                g.doc.setdefault("images", []).append(
                    {"bufferView": g.view(open(what, "rb").read()), "mimeType": "image/png"})
                g.doc.setdefault("textures", []).append({"source": len(g.doc["images"]) - 1, "sampler": 0})
                g.doc["samplers"] = [{"magFilter": 9728, "minFilter": 9728}]
                images[what] = len(g.doc["textures"]) - 1
            m["pbrMetallicRoughness"]["baseColorTexture"] = {"index": images[what]}
        elif isinstance(what, tuple):
            m["pbrMetallicRoughness"]["baseColorFactor"] = [c / 255 for c in what] + [1]
        g.doc.setdefault("materials", []).append(m)
        mat_index[name] = len(g.doc["materials"]) - 1
        return mat_index[name]

    heads = {0x58 + 0x34 * k: k for k in range(len(ms))}
    anims = animations(d, heads)
    by_mesh = {}
    for a in anims:
        by_mesh.setdefault(a[0], []).append(a)
    weights_for = {}
    for m, (vp, nv, _, _) in enumerate(ms):
        base = [struct.unpack_from("<hhh", d, vp + 8 * i) for i in range(nv)]
        P = lambda v: (v[0], max(-32768, min(32767, -v[1])), max(-32768, min(32767, -v[2])))
        # the keyframes of this mesh's animations: one morph target each
        keys, key_pos = {}, []
        for _, frames, pool in by_mesh.get(m, []):
            for fr in frames:
                for la in (fr[:2] if fr else ()):
                    if la is None or (la, pool) in keys or la + 2 * nv > len(d):
                        continue
                    idx = struct.unpack_from(f"<{nv}H", d, la)
                    pts = []
                    for i, ix in enumerate(idx):
                        q = pool + 6 * (ix >> 2)
                        pts.append(struct.unpack_from("<hhh", d, q) if q + 6 <= len(d) else base[i])
                    keys[(la, pool)] = len(key_pos)
                    key_pos.append(pts)
        prims = {}
        for mat, vs, uv in mesh_faces[m]:
            prims.setdefault(mat, []).append((vs, uv))
        gm = {"name": f"mesh_{m:02d}", "primitives": []}
        for mat, fl in prims.items():
            # indexed: a corner with the same vertex and uv as an earlier one reuses it
            pos, tex, corners, index, seen = [], [], [], [], {}
            for vs, uv in fl:
                for j, v in enumerate(vs):
                    key = (v, uv[j] if uv else None)
                    if key not in seen:
                        seen[key] = len(corners)
                        pos += P(base[v])
                        corners.append(v)
                        tex += uv[j] if uv else (0.0, 0.0)
                    index.append(seen[key])
            wide = len(corners) > 0xFFFF
            ia = array("I" if wide else "H", index)
            g.doc["accessors"].append({"bufferView": g.view(ia.tobytes(), 34963), "count": len(ia),
                                       "componentType": 5125 if wide else 5123, "type": "SCALAR"})
            indices = len(g.doc["accessors"]) - 1
            prim = {"attributes": {"POSITION": g.shorts(pos)}, "mode": 4, "indices": indices}
            if any(u for _, u in fl):
                prim["attributes"]["TEXCOORD_0"] = g.floats([t if i % 2 == 0 else 1 - t for i, t in enumerate(tex)], "VEC2")
            if mat is not None:
                prim["material"] = material(mat)
            if key_pos:
                prim["targets"] = []
                for pts in key_pos:
                    delta = []
                    for v in corners:
                        a, b = P(pts[v]), P(base[v])
                        delta += (a[0] - b[0], a[1] - b[1], a[2] - b[2])
                    prim["targets"].append({"POSITION": g.shorts(delta)})
            gm["primitives"].append(prim)
        if key_pos:
            gm["weights"] = [0.0] * len(key_pos)
            weights_for[m] = keys
        g.doc["meshes"].append(gm)
        g.doc["nodes"].append({"name": gm["name"], "mesh": len(g.doc["meshes"]) - 1, "scale": [SCALE] * 3})
        g.doc["scenes"][0]["nodes"].append(len(g.doc["nodes"]) - 1)
    for k, (m, frames, pool) in enumerate(anims):
        keys = weights_for.get(m)
        if not keys:
            continue
        times, w, prev = [], [], None
        for f, fr in enumerate(frames):
            # the game's in-betweens blend two keyframes linearly, as glTF's LINEAR does between
            # samples: keep keyframes, the ends, and a frame where the blended pair changes
            pair = fr[:2] if fr else None
            inbetween = fr and fr[1] is not None and fr[2]
            if inbetween and 0 < f < len(frames) - 1 and pair == prev:
                continue
            prev = pair
            row = [0.0] * len(keys)
            if fr:
                la, lb, p = fr
                if (la, pool) in keys:
                    row[keys[(la, pool)]] += 1 - p
                if lb is not None and (lb, pool) in keys:
                    row[keys[(lb, pool)]] += p
            times.append(f / FPS)
            w += row
        if not times:
            continue
        g.doc.setdefault("animations", []).append({
            "name": f"anim_{k:02d}",
            "samplers": [{"input": g.floats(times, "SCALAR", True), "output": g.unorm16(w),
                          "interpolation": "LINEAR"}],
            "channels": [{"sampler": 0, "target": {"node": m, "path": "weights"}}]})
    g.save(path)
