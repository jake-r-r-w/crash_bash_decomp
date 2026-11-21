"""A hero model's strips in its first BREATHE frame, each with its corners' middle and box, those whose middle x is below X: for a template's
prop boxes (docs/HERO_PACKS.md, Templates), as a box takes a strip with most of its corners in it.
tools/dk python3 tools/scripts/hero_strips.py ENTRY_HEX MESH X"""
import struct, sys
sys.path.insert(0, "tools/scripts")
import mdl, gltf
e = int(sys.argv[1], 16); mesh = int(sys.argv[2])
d = open(f"assets/CRASHBSH/{e:03X}.mdl/model.bin", "rb").read()
ms = mdl.meshes(d); heads = {mdl.COUNT_AT + 4 + m * mdl.HEAD: m for m in range(len(ms))}
anims = gltf.animations(d, heads)
base = 0x44 + struct.unpack_from("<i", d, 0x44)[0]
h = lambda s: sum(ord(c) * (i + 1) for i, c in enumerate(s))
for k, (m, frs, pool) in enumerate(anims):
    if m == mesh and struct.unpack_from("<I", d, base + 0x18 * k + 0x10)[0] == h("BREATHE"):
        la, lb, bl = frs[0]
        nv = ms[mesh][1]
        pts = [struct.unpack_from("<hhh", d, pool + 6 * (struct.unpack_from("<H", d, la + 2 * i)[0] >> 2)) for i in range(nv)]
        cur = 0
        for si, s in enumerate(ms[mesh][2]):
            n = (s >> 8) + 2
            ps = pts[cur:cur + n]; cur += n
            c = [sum(p[j] for p in ps) / n for j in range(3)]
            if c[0] < float(sys.argv[3]):
                print(si, n, [round(v) for v in c], "x", min(p[0] for p in ps), max(p[0] for p in ps), "y", min(p[1] for p in ps), max(p[1] for p in ps), "z", min(p[2] for p in ps), max(p[2] for p in ps))
