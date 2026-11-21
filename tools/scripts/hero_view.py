"""A hero model's frame drawn front (x, y) and side (z, y) on a grid of the game's units (y down, as the game has it), each strip its
own colour, for placing a template's markers (docs/HERO_PACKS.md, Templates). With a map (JSON: {"joints": {NAME: [x, y, z], ...}}),
its joints drawn on it. tools/dk python3 tools/scripts/hero_view.py HERO KIND [--anim N] [--frame F] [--mesh M] [--map FILE] [--out FILE]"""
import colorsys
import json
import struct
import sys

from PIL import Image, ImageDraw

sys.path.insert(0, "tools/scripts")
import mdl   # noqa: E402
import gltf  # noqa: E402

args = sys.argv[1:]


def opt(name, default):
    if name in args:
        i = args.index(name)
        v = args[i + 1]
        del args[i:i + 2]
        return v
    return default


anim_i = int(opt("--anim", "0"))
frame_i = int(opt("--frame", "0"))
mesh_i = int(opt("--mesh", "0"))
map_file = opt("--map", None)
out = opt("--out", None)
hero, kind = args[0], args[1]
names = {}
for line in open("configs/USA/dat_names.txt", encoding="utf-8"):
    if "=" in line and line[0].isdigit():
        n, v = line.strip().split("=", 1)
        names[v.strip().lower()] = int(n)
entry = names[f"models\\chars\\{kind}\\{hero}.mdl"]
d = open(f"assets/CRASHBSH/{entry:03X}.mdl/model.bin", "rb").read()
ms = mdl.meshes(d)
heads = {mdl.COUNT_AT + 4 + m * mdl.HEAD: m for m in range(len(ms))}
anims = [a for a in gltf.animations(d, heads)]
vp, nv, strips, _ = ms[mesh_i]


def frame_points(a, f):
    m, frs, pool = anims[a]
    fr = frs[f]
    la, lb, blend = fr
    pts = []
    for i in range(nv):
        ia = struct.unpack_from("<H", d, la + 2 * i)[0] >> 2
        p = struct.unpack_from("<hhh", d, pool + 6 * ia)
        if lb is not None and blend:
            ib = struct.unpack_from("<H", d, lb + 2 * i)[0] >> 2
            q = struct.unpack_from("<hhh", d, pool + 6 * ib)
            p = tuple(p[k] + (q[k] - p[k]) * blend for k in range(3))
        pts.append(p)
    return pts


mine = [i for i, a in enumerate(anims) if a[0] == mesh_i]
pts = frame_points(mine[anim_i], frame_i) if mine else [struct.unpack_from("<hhh", d, vp + 8 * i) for i in range(nv)]
faces, cur = [], 0
for si, s in enumerate(strips):
    for i in range(s >> 8):
        faces.append((cur + i, cur + i + 1, cur + i + 2, si))
    cur += (s >> 8) + 2
joints = json.load(open(map_file))["joints"] if map_file else {}
lo = [min(p[k] for p in pts) for k in range(3)]
hi = [max(p[k] for p in pts) for k in range(3)]
for j in joints.values():
    for k in range(3):
        lo[k], hi[k] = min(lo[k], j[k]), max(hi[k], j[k])
W = 560
span = max(hi[k] - lo[k] for k in range(3)) * 1.1 + 1
sc = (W - 60) / span
img = Image.new("RGB", (W * 2, W), (40, 40, 46))
dr = ImageDraw.Draw(img)


def panel(ox, ax, title):
    cx = (lo[ax] + hi[ax]) / 2
    cy = (lo[1] + hi[1]) / 2

    def to(p):
        return ox + W / 2 + (p[ax] - cx) * sc, W / 2 + (p[1] - cy) * sc
    step = 50 if span < 1200 else 100
    g = int((cx - span / 2) // step) * step
    while g < cx + span / 2:
        x = ox + W / 2 + (g - cx) * sc
        dr.line([(x, 0), (x, W)], fill=(70, 70, 80) if g % (step * 2) else (95, 95, 110))
        if g % (step * 2) == 0:
            dr.text((x + 2, W - 14), str(g), fill=(170, 170, 190))
        g += step
    g = int((cy - span / 2) // step) * step
    while g < cy + span / 2:
        y = W / 2 + (g - cy) * sc
        dr.line([(ox, y), (ox + W, y)], fill=(70, 70, 80) if g % (step * 2) else (95, 95, 110))
        if g % (step * 2) == 0:
            dr.text((ox + 2, y + 1), str(g), fill=(170, 170, 190))
        g += step
    # far faces first: depth along the other axis (front: z, larger z is behind? the game's z grows away from the viewer's front)
    other = 2 if ax == 0 else 0
    order = sorted(faces, key=lambda f: -sum(pts[i][other] for i in f[:3]) if ax == 0 else sum(pts[i][other] for i in f[:3]))
    for f in order:
        a, b, c, si = f
        poly = [to(pts[a]), to(pts[b]), to(pts[c])]
        r, gg, bb = colorsys.hsv_to_rgb((si * 0.137) % 1, 0.45, 0.85)
        dr.polygon(poly, fill=(int(r * 255), int(gg * 255), int(bb * 255)), outline=(20, 20, 20))
    for name, j in joints.items():
        x, y = to(j)
        dr.ellipse([x - 4, y - 4, x + 4, y + 4], fill=(255, 40, 40))
        dr.text((x + 5, y - 6), name, fill=(255, 255, 120))
    dr.text((ox + 6, 4), title, fill=(255, 255, 255))


panel(0, 0, f"{hero} {kind} front: x across, y down")
panel(W, 2, "side: z across, y down")
img.save(out or f"scratch/hero-view-{hero}-{kind}.png")
print(f"{hero} {kind} {entry:03X}: {nv} vertices, {len(strips)} strips, box x {lo[0]}..{hi[0]} y {lo[1]}..{hi[1]} z {lo[2]}..{hi[2]}")
