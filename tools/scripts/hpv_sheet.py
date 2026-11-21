"""A contact sheet of what the game made of a hero (--dump-hero's ID_KIND.hpv files): each kind a row, a few frames of a move or two,
turned a little, each face its texture's colour at its middle times its corners' colours. tools/dk python3 tools/scripts/hpv_sheet.py DIR ID
[KIND ...] [--out FILE] [--turn DEG]"""
import math
import os
import struct
import sys

from PIL import Image, ImageDraw

args = sys.argv[1:]
out = None
turn = 30.0
if "--out" in args:
    i = args.index("--out")
    out = args[i + 1]
    del args[i:i + 2]
if "--turn" in args:
    i = args.index("--turn")
    turn = float(args[i + 1])
    del args[i:i + 2]
folder, hid = args[0], args[1]
kinds = args[2:] or ["foot", "polar", "pogo", "ball", "tank", "race", "splash", "mallet", "dino", "kegs", "warp"]


def read(path):
    d = open(path, "rb").read()
    p = 4
    n = struct.unpack_from("<I", d, p)[0]
    p += 4
    images = []
    for _ in range(n):
        w, h = struct.unpack_from("<HH", d, p)
        p += 4
        images.append((w, h, d[p:p + w * h * 4]))
        p += w * h * 4
    nf = struct.unpack_from("<I", d, p)[0]
    p += 4
    faces = []
    for _ in range(nf):
        v = struct.unpack_from("<iiii", d, p)
        b = d[p + 16:p + 32]
        p += 32
        faces.append((v[:3], v[3], [(b[k * 2], b[k * 2 + 1]) for k in range(3)], [tuple(b[6 + k * 3:9 + k * 3]) for k in range(3)], b[15]))
    nv, na = struct.unpack_from("<II", d, p)
    p += 8
    anims = []
    for _ in range(na):
        name = d[p:p + 32].split(b"\0")[0].decode()
        fr = struct.unpack_from("<I", d, p + 32)[0]
        p += 36
        frames = []
        for f in range(fr):
            frames.append(struct.unpack_from("<%dh" % (nv * 3), d, p))
            p += nv * 6
        anims.append((name, frames))
    return images, faces, nv, anims


CELL = 220
COLS = 6
rows = []
for kind in kinds:
    path = os.path.join(folder, f"{hid}_{kind}.hpv")
    if os.path.exists(path):
        rows.append((kind, read(path)))
img = Image.new("RGB", (CELL * COLS, CELL * max(1, len(rows))), (40, 40, 46))
dr = ImageDraw.Draw(img)
c, s = math.cos(math.radians(turn)), math.sin(math.radians(turn))
for r, (kind, (images, faces, nv, anims)) in enumerate(rows):
    picks = []
    for name, frames in anims[:2]:
        for q in range(3):
            picks.append((name, frames[min(len(frames) - 1, q * len(frames) // 3)]))
    first = anims[0][1][0]
    xs = [first[i * 3] * c - first[i * 3 + 2] * s for i in range(nv)]
    ys = [first[i * 3 + 1] for i in range(nv)]
    span = max(max(xs) - min(xs), max(ys) - min(ys)) * 1.25 + 1
    sc = CELL / span
    cx, cy = (max(xs) + min(xs)) / 2, (max(ys) + min(ys)) / 2
    for col, (name, v) in enumerate(picks[:COLS]):
        ox, oy = col * CELL, r * CELL

        def to(i):
            x = v[i * 3] * c - v[i * 3 + 2] * s
            z = v[i * 3] * s + v[i * 3 + 2] * c
            return ox + CELL / 2 + (x - cx) * sc, oy + CELL / 2 + (v[i * 3 + 1] - cy) * sc, z
        pts = [to(i) for i in range(nv)]
        order = sorted(range(len(faces)), key=lambda f: -sum(pts[k][2] for k in faces[f][0]))
        for f in order:
            vs, im, uv, rgb, semi = faces[f]
            col3 = [sum(rgb[k][j] for k in range(3)) / 3 for j in range(3)]
            if 0 <= im < len(images):
                w, h, px = images[im]
                u = int(sum(t[0] for t in uv) / 3) % w
                t = int(sum(t[1] for t in uv) / 3) % h
                o = (t * w + u) * 4
                if px[o + 3] == 0:
                    continue
                col3 = [col3[j] * px[o + j] / 128 for j in range(3)]
            dr.polygon([pts[k][:2] for k in vs], fill=tuple(min(255, int(x)) for x in col3))
        dr.text((ox + 4, oy + 4), f"{kind} {name}", fill=(255, 255, 140))
img.save(out or os.path.join(folder, f"{hid}-sheet.png"))
print(f"{len(rows)} kinds")
