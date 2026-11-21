"""A pack's polar bear alone, from what the game made of it (--dump-hero's ID_polar.hpv): the model's faces after the hero's (as many as
its model on foot has), drawn from four sides in BREATHE's first frame and PUSH's, untextured faces (the fill, and the bear's own flat
ones) outlined blue: for seeing a bear is whole and has none of its rider left on it (docs/HERO_PACKS.md, On foot, the podium and the
polar bear). tools/dk python3 tools/scripts/hpv_bear.py DIR ID [--out FILE] [--label TEXT]"""
import math
import os
import struct
import sys

from PIL import Image, ImageDraw


def read(path):
    """an .hpv file (port/game/main/bonusheroes.c, made_dump): its images as RGBA, its faces, its vertex count and its moves' frames"""
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
        faces.append((v[:3], v[3], [(b[k * 2], b[k * 2 + 1]) for k in range(3)], [tuple(b[6 + k * 3:9 + k * 3]) for k in range(3)]))
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


def main():
    args = sys.argv[1:]
    out, label = None, ""
    if "--out" in args:
        i = args.index("--out")
        out = args[i + 1]
        del args[i:i + 2]
    if "--label" in args:
        i = args.index("--label")
        label = args[i + 1]
        del args[i:i + 2]
    global CULL
    CULL = 0
    if "--cull" in args:
        i = args.index("--cull")
        CULL = int(args[i + 1])
        del args[i:i + 2]
    global PC, PS
    pitch = 0.0
    if "--pitch" in args:
        i = args.index("--pitch")
        pitch = float(args[i + 1])
        del args[i:i + 2]
    PC, PS = math.cos(math.radians(pitch)), math.sin(math.radians(pitch))
    folder, hid = args
    _, foot, _, _ = read(os.path.join(folder, f"{hid}_foot.hpv"))
    images, faces, nv, anims = read(os.path.join(folder, f"{hid}_polar.hpv"))
    bear = faces[len(foot):]
    used = sorted({k for f in bear for k in f[0]})
    cell, views = 240, [0, 90, 180, 270]
    picks = [(n, fr[0]) for n, fr in anims if n in ("BREATHE", "PUSH")]
    img = Image.new("RGB", (cell * len(views), cell * max(1, len(picks))), (40, 40, 46))
    dr = ImageDraw.Draw(img)
    for r, (name, v) in enumerate(picks):
        for col, turn in enumerate(views):
            c, s = math.cos(math.radians(turn)), math.sin(math.radians(turn))
            ox, oy = col * cell, r * cell

            def to(i):
                x = v[i * 3] * c - v[i * 3 + 2] * s
                z = v[i * 3] * s + v[i * 3 + 2] * c
                y = v[i * 3 + 1] * PC - z * PS
                z = v[i * 3 + 1] * PS + z * PC
                return ox + cell / 2 + x * 0.55, oy + cell - 60 + y * 0.55, z
            pts = {i: to(i) for i in used}
            for vs, im, uv, rgb in sorted(bear, key=lambda f: -sum(pts[k][2] for k in f[0])):
                (ax, ay, _), (bx, by, _), (cx, cy, _) = (pts[k] for k in vs)
                if CULL and ((bx - ax) * (cy - ay) - (by - ay) * (cx - ax)) * CULL < 0:
                    continue
                col3 = [sum(rgb[k][j] for k in range(3)) / 3 for j in range(3)]
                if 0 <= im < len(images):
                    w, h, px = images[im]
                    o = ((int(sum(t[1] for t in uv) / 3) % h) * w + int(sum(t[0] for t in uv) / 3) % w) * 4
                    col3 = [col3[j] * px[o + j] / 128 for j in range(3)]
                dr.polygon([pts[k][:2] for k in vs], fill=tuple(min(255, int(x)) for x in col3),
                           outline=(90, 160, 255) if im < 0 else None)
            dr.text((ox + 4, oy + 4), f"{label} {name} {turn}", fill=(255, 255, 140))
    img.save(out or os.path.join(folder, f"{hid}-bear.png"))


main()
