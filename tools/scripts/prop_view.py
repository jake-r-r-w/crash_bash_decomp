"""A template's prop as packfollow.c takes it: a hero's model of a kind drawn front (x, y) and side (z, y) in its own colours (the texel at
each face's middle times its corners' colours), the strips the map's prop boxes take (most of their corners in one) bright, the rest
dim, the boxes outlined, and each strip's number at its middle: for fitting a map's boxes (docs/HERO_PACKS.md, Templates).
tools/dk python3 tools/scripts/prop_view.py HERO KIND [--mesh M] [--move NAME (BREATHE)] [--out FILE] [--only] [--box x0,y0,z0,x1,y1,z1[,sym] ...]
HERO by name (coco ... rilla), KIND as the models' folders (pogo, ball, tank, race, splash, mallet, dino, kegs); the boxes are the
map's in port/game/main/packtemplates.c unless --box gives others; --only draws the kept strips alone."""
import json
import re
import struct
import sys

from PIL import Image, ImageDraw

sys.path.insert(0, "tools/scripts")
import mdl   # noqa: E402
import gltf  # noqa: E402

HEROES = {"crash": 1, "coco": 2, "cortex": 3, "brio": 4, "tiny": 5, "kong": 6, "dingo": 7, "rilla": 8}
KINDS = {"pogo": "KIND_POGO", "ball": "KIND_BALL", "tank": "KIND_TANK", "race": "KIND_RACE", "splash": "KIND_SPLASH",
         "mallet": "KIND_MALLET", "dino": "KIND_DINO", "kegs": "KIND_KEGS"}


def opt(args, name, default, many=False):
    out = [] if many else default
    while name in args:
        i = args.index(name)
        if many:
            out.append(args[i + 1])
        else:
            out = args[i + 1]
        del args[i:i + 2]
    return out if not many or out else default


def map_entry(hero, kind):
    """the map's entry in packtemplates.c, its strips left out (.skip) group 3"""
    src = open("port/game/main/packtemplates.c", encoding="utf-8").read()
    return re.search(r"\{\s*%d,\s*%s,.*?\}(\s*,\s*-?\d+){2,3}\s*(,\s*\.skip\s*=\s*\(const s16\[\]\)\s*\{([^}]*)\})?\s*\}"
                     % (HEROES[hero], KINDS[kind]), src, re.S)


def map_boxes(hero, kind):
    """the map's prop boxes, from packtemplates.c: [(lo, hi, sym, faces)]"""
    m = map_entry(hero, kind)
    if m is None:
        return []
    out = []
    for kind_, nums in re.findall(r"\b(F?BOX|F?BOTH|NOTBOX|NOTBOTH|SKIPBOX|SKIPBOTH)\(([^)]*)\)", m.group(0)):
        v = [float(x) for x in nums.split(",")]
        out.append((v[:3], v[3:], kind_.endswith("BOTH"), 3 if kind_.startswith("SKIP") else 2 if kind_.startswith("NOT") else 1 if kind_.startswith("F") else 0))
    return out


def map_skip(hero, kind):
    """the map's strips left out (.skip), from packtemplates.c"""
    m = map_entry(hero, kind)
    if m is None or m.group(3) is None:
        return set()
    return {int(x) for x in m.group(3).split(",") if int(x) >= 0}


def load(hero, kind, mesh, move):
    names = {}
    for line in open("configs/USA/dat_names.txt", encoding="utf-8"):
        if "=" in line and line[0].isdigit():
            n, v = line.strip().split("=", 1)
            names[v.strip().lower()] = int(n)
    entry = names[f"models\\chars\\{kind}\\{hero}.mdl"]
    d = open(f"assets/CRASHBSH/{entry:03X}.mdl/model.bin", "rb").read()
    tdir = f"assets/CRASHBSH/{entry - 1:03X}.tex"
    meta = json.load(open(f"{tdir}/tex.json"))
    cl = Image.open(f"{tdir}/cluts.png").convert("RGBA")
    ms = mdl.meshes(d)
    heads = {mdl.COUNT_AT + 4 + m * mdl.HEAD: m for m in range(len(ms))}
    anims = gltf.animations(d, heads)
    base = 0x44 + struct.unpack_from("<i", d, 0x44)[0]
    h = lambda s: sum(ord(c) * (i + 1) for i, c in enumerate(s))
    pts = first = None
    for k, (m, frs, pool) in enumerate(anims):
        if m != mesh:
            continue
        la, lb, bl = frs[0]
        p = [struct.unpack_from("<hhh", d, pool + 6 * (struct.unpack_from("<H", d, la + 2 * i)[0] >> 2)) for i in range(ms[mesh][1])]
        first = first or p
        if struct.unpack_from("<I", d, base + 0x18 * k + 0x10)[0] == h(move):
            pts = p
    pts = pts or first
    pos = mdl.COUNT_AT + 4 + mesh * mdl.HEAD

    def ptr(k):
        q = pos + 0x10 + 4 * k
        return q + struct.unpack_from("<i", d, q)[0]
    uv_idx, tex_at, col_idx = ptr(2), ptr(3), ptr(4)
    uvs_at = 0x24 + struct.unpack_from("<i", d, 0x24)[0]
    cols_at = 0x20 + struct.unpack_from("<i", d, 0x20)[0]
    ims = {}

    def texel(v, c, u, w):
        if (v, c) not in ims:
            im = Image.open(f"{tdir}/{meta['images'][v]['file']}")
            if c is not None:
                im = im.copy()
                im.putpalette([x for i in range(meta["cluts"][c]) for x in cl.getpixel((i, c))[:3]])
            ims[(v, c)] = im.convert("RGB")
        im = ims[(v, c)]
        return im.getpixel((min(max(int(u), 0), im.width - 1), min(max(int(w), 0), im.height - 1)))
    strips = ms[mesh][2]
    faces = []
    left, q, t, f, cur = 0, tex_at, 0, 0, 0
    for si, s in enumerate(strips):
        n = s >> 8
        for j in range(n):
            left -= 1
            if left < 0:
                t = struct.unpack_from("<H", d, q)[0]
                q += 2
                left = (t >> 9) & 0x3F
            a, b, c = cur + j, cur + j + 1, cur + j + 2
            tri = (a, c, b) if j % 2 == (1 if s & 8 else 0) else (a, b, c)
            k = struct.unpack_from("<H", d, col_idx + 2 * f)[0] & 0x1FFF
            vc = [sum(struct.unpack_from("<3B", d, cols_at + 4 * (k + i))[ch] for i in range(3)) / 3 for ch in range(3)]
            if s & 1:
                rgb = tuple(int(x) for x in vc)
            else:
                v, cc = (len(meta["images"]) - 1, t & 0x1FF) if t & 0x8000 else (t & 0x1FF, None)
                cu = struct.unpack_from("<3H", d, uvs_at + 2 * struct.unpack_from("<H", d, uv_idx + 2 * f)[0])
                tx = texel(v, cc, sum(x & 0xFF for x in cu) / 3, sum(x >> 8 for x in cu) / 3)
                rgb = tuple(min(255, int(tx[i] * vc[i] / 128)) for i in range(3))
            faces.append((si, tri, rgb))
            f += 1
        cur += n + 2
    return pts, strips, faces


def main():
    args = sys.argv[1:]
    mesh = int(opt(args, "--mesh", "0"))
    move = opt(args, "--move", "BREATHE")
    out = opt(args, "--out", None)
    only = "--only" in args
    faces_mode = "--faces" in args
    if faces_mode:
        args.remove("--faces")
    if only:
        args.remove("--only")
    given = opt(args, "--box", [], many=True)
    skips = opt(args, "--skip", None)
    hero, kind = args[0], args[1]
    boxes = [([float(x) for x in b.split(",")[:3]], [float(x) for x in b.split(",")[3:6]], "sym" in b.split(",")[6:],
              3 if "skip" in b.split(",")[6:] else 2 if "not" in b.split(",")[6:] else 1 if "faces" in b.split(",")[6:] else 0) for b in given] or map_boxes(hero, kind)
    pts, strips, faces = load(hero, kind, mesh, move)
    skip = {int(x) for x in skips.split(",")} if skips else set() if given else map_skip(hero, kind)

    def inside(p, faces_=None):
        for lo, hi, sym, fb in boxes:
            x = abs(p[0]) if sym else p[0]
            if (faces_ is None or fb == faces_) and lo[0] <= x <= hi[0] and lo[1] <= p[1] <= hi[1] and lo[2] <= p[2] <= hi[2]:
                return True
        return False
    kept, facey, cur, mids = set(), set(), 0, {}
    for si, s in enumerate(strips):
        n = (s >> 8) + 2
        ps = pts[cur:cur + n]
        cur += n
        if sum(inside(p, 3) for p in ps) * 2 > n or si in skip:
            pass
        elif sum(inside(p, False) for p in ps) * 2 > n:
            kept.add(si)
        elif any(inside(p, True) for p in ps):
            facey.add(si)
        mids[si] = [sum(p[j] for p in ps) / n for j in range(3)]

    def taken(si, tri):
        return (si in kept or (si in facey and all(inside(pts[i], 1) for i in tri))) and not all(inside(pts[i], 2) for i in tri)
    xs = [p[0] for p in pts] + [p[2] for p in pts]
    ys = [p[1] for p in pts]
    span = max(max(xs) - min(xs), max(ys) - min(ys)) * 1.1 + 1
    W = 560
    sc = (W - 40) / span
    img = Image.new("RGB", (W * 2, W), (40, 40, 48))
    dr = ImageDraw.Draw(img)
    cy = (max(ys) + min(ys)) / 2
    for view in range(2):
        ox = W * view + W / 2

        def P(p):
            return (ox + (p[0] if view == 0 else p[2]) * sc, W / 2 + (p[1] - cy) * sc)
        depth = lambda fc: -sum(pts[i][2] if view == 0 else -pts[i][0] for i in fc[1]) / 3
        for si, tri, rgb in sorted(faces, key=depth):
            t = taken(si, tri)
            if not t and only:
                continue
            col = rgb if t else tuple(c // 4 + 20 for c in rgb)
            if faces_mode and t and not all(inside(pts[i]) for i in tri):
                col = (255, 0, 255)
            dr.polygon([P(pts[i]) for i in tri], fill=col, outline=(0, 0, 0) if t else None)
        for lo, hi, sym, fb in boxes:
            for sgn in ((1, -1) if sym and view == 0 else (1,)):
                a, b = (lo[0] * sgn, hi[0] * sgn) if view == 0 else (lo[2], hi[2])
                x0, y0 = P((a, lo[1], a))
                x1, y1 = P((b, hi[1], b))
                dr.rectangle([min(x0, x1), min(y0, y1), max(x0, x1), max(y0, y1)], outline=(255, 160, 0) if fb == 3 else (255, 80, 80) if fb == 2 else (0, 255, 255) if fb else (255, 255, 0))
        for si in mids:
            if not only or si in kept or si in facey:
                dr.text(P(mids[si]), str(si), fill=(120, 255, 120) if si in kept else (150, 150, 150))
    dr.text((4, 4), f"{hero} {kind} mesh {mesh}: {len(kept)} of {len(strips)} strips kept; front x/y, side z/y", fill=(255, 255, 0))
    print("kept", sorted(kept), "by faces", sorted(facey), "faces", sum(taken(si, tri) for si, tri, rgb in faces), "yellow", [rgb for si, tri, rgb in faces if taken(si, tri) and rgb[0] > 150 and rgb[1] > 120 and rgb[2] < 90][:6])
    img.save(out or f"scratch/prop-{hero}-{kind}.png")


main()
