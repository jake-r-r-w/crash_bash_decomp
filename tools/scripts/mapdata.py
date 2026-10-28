#!/usr/bin/env python3
"""A model file's level data (docs/ASSETS.md, Model files): its maps, map directory and scripts,
to JSON and back into the file in place (mdl.py writes and reads it as map.json).

The layouts are the game's (ModelFile in include/bash/main.h; the maps through the Oct 9
prototype's map_list, map_map, trigger_list and Placement, read by mapscene_setup_tables,
mapenv_place_subs, InitTriggers and path_get). Offsets marked self are s32s counted from their own
position.

    header +0x18 self: map_list    u32 count, then a self offset each to a map_map
    map_map (placements at +0x34)  s32 cell size x, y, z; self +0x0C paths, +0x10 triggers;
                                   s32 +0x1C placements, self +0x20 to them; u8 r, g, b, clear
    paths                          s32 count, then a self offset each to a path: s32 keys, s32 frames
                                   (the last key's), s32, s32 id (path_get), 8 s32s, then 0x1C-byte
                                   keys: s32 x, y, z, s32 frame, s32, u32 flags (0x2000: the last),
                                   s32
    triggers                       u32 count, then 16 bytes each: s32 x, y, z, then a word: bits
                                   0-1 the cast, the rest a self offset to its info (u16 type, u16
                                   flags, s32 slot, s32 rot x, y, z)
    contours (+0x14)               s32 count, then a self offset each: s32 min x, y, z, max x, y, z,
                                   7 s32s, s32 (+0x34), s32 edges, then 0x38-byte edges: 3 s32s
                                   (+-0x3FFFFFFF, ...), s32 p0 x, y, z, p1 x, y, z, s32 length,
                                   s32 angle, 3 s32s. An arena's walls and zones (PointInZone,
                                   DinoTrackCollide); the cell list (+0x18) is empty in every map
    placements (0xA0 each)         u32 flags, s32 x, y, z, ..., +0x28 a MATRIX (s16 m[3][3], s16,
                                   s32 t[3]), ..., +0x74 4 bytes, ..., +0x88 u16 model id, ...,
                                   +0x9C u16, +0x9E u16
    header +0x1C self: entities    s32 count, then 12 bytes each: a self offset to a mesh header,
                                   s32, s32 group
    header +0x48, +0x4C            the scripts: count, then a self offset each to a script_header
                                   (s32 threads, subs, end frame, start frame, frames)

pack writes the maps' fields back (the counts and the layout stay: paths, keys, triggers and
placements can't be added or removed). The entities and scripts are listed for viewing; pack
leaves them as model.bin has them.
"""
import struct

PATH_HEAD = 0x30
KEY = 0x1C
TRIG = 0x10
PLACE = 0xA0
CONTOUR_HEAD = 0x3C
EDGE = 0x38


class Bad(Exception):
    pass


def _w(d, o, fmt="<i"):
    if not 0 <= o <= len(d) - struct.calcsize(fmt):
        raise Bad(f"level data: 0x{o:X} past the end")
    return struct.unpack_from(fmt, d, o)[0]


def _self(d, o):
    return o + _w(d, o)


def layout(d):
    """Where everything is: {"maps": [map], "entities": [(offset, mesh offset)], "scripts":
    [offset]}; a map is {"at", "paths": [(path, [key offsets])], "triggers": [(record, info)],
    "placements": [offset]}."""
    out = {"maps": [], "entities": [], "scripts": []}
    if len(d) < 0x58:
        return out
    ml = _self(d, 0x18)
    for m in range(_w(d, ml)):
        mp = _self(d, ml + 4 + 4 * m)
        pl, tl = _self(d, mp + 0x0C), _self(d, mp + 0x10)
        paths = []
        for i in range(_w(d, pl)):
            p = _self(d, pl + 4 + 4 * i)
            paths.append((p, [p + PATH_HEAD + k * KEY for k in range(_w(d, p))]))
        trigs = []
        for k in range(_w(d, tl)):
            r = tl + 4 + k * TRIG
            trigs.append((r, r + 0xC + (_w(d, r + 0xC) & ~3)))
        cg = _self(d, mp + 0x14)
        contours = []
        for i in range(_w(d, cg)):
            c = _self(d, cg + 4 + 4 * i)
            contours.append((c, [c + CONTOUR_HEAD + k * EDGE for k in range(_w(d, c + 0x38))]))
        pp = _self(d, mp + 0x20)
        out["maps"].append({"at": mp, "paths": paths, "triggers": trigs, "contours": contours,
                            "placements": [pp + i * PLACE for i in range(_w(d, mp + 0x1C))]})
    el = _self(d, 0x1C)
    for i in range(_w(d, el)):
        r = el + 4 + i * 12
        out["entities"].append((r, _self(d, r) if _w(d, r) else None))
    if _w(d, 0x48) > 0:
        st = _self(d, 0x4C)
        out["scripts"] = [_self(d, st + 4 * i) for i in range(_w(d, 0x48))]
    return out


# ---- scripts: their headers and threads, as (name, offset, struct format) specs

SCRIPT_HEAD = [("threads", 0, "i"), ("subs", 4, "i"), ("end_frame", 8, "i"), ("start_frame", 0xC, "i"),
               ("frames", 0x10, "i"), ("spare", 0x14, "2i")]
THREAD_HEAD = [("start_frame", 4, "i"), ("end_frame", 8, "i"), ("total_frames", 0xC, "i"),
               ("key_frame_count", 0x10, "i")]
OBJ_KEY = [("start", 0, "i"), ("len", 4, "i"), ("unk8", 8, "h"), ("unkA", 0xA, "h"), ("pos", 0xC, "3i"),
           ("unk18", 0x18, "3i"), ("rot", 0x24, "4i"), ("unk34", 0x34, "3i"), ("scale", 0x40, "3i"),
           ("fade", 0x4C, "i")]
CAM_KEY = [("start", 0, "i"), ("len", 4, "i"), ("at", 8, "3i"), ("eye", 0x14, "3i"), ("unk20", 0x20, "2i")]
ANIM_KEY = [("start", 0, "i"), ("len", 4, "i"), ("pos", 8, "3i"), ("unk14", 0x14, "3i"), ("rot", 0x20, "4i"),
            ("unk30", 0x30, "3i"), ("scale", 0x3C, "3i"), ("alpha", 0x48, "i")]
PARTDATA = ["flags", "pos", "maxparticlecount", "particlesperframe", "particlelifetime", "effecttime",
            "emitterspeed", "randomspeedflag", "frame", "emitterradius", "yrotmin", "yrotmax", "xrotmin",
            "xrotmax", "gxmin", "gxmax", "gymin", "gymax", "gzmin", "gzmax", "fadeinend", "fadeoutstart",
            "scaleinend", "scaleoutstart", "zrot", "regenparticle"]
# type: (name, fields, (keys at, key size, key fields) or None); the update functions in
# src/main/scripts.c read them (ScriptCommands), the names are the Oct 9 prototype's where it has them
THREADS = {
    0: ("model", [("unk14", 0x14, "i"), ("flags", 0x18, "i"), ("unk1C", 0x1C, "i"), ("unk20", 0x20, "i")],
        (0x24, 0x50, OBJ_KEY)),
    1: ("emitter", [(n, 0x14 + off, "3i" if n == "pos" else "i")     # partdata: flags, pos, then words
                    for n, off in zip(PARTDATA, [0, 4] + [0x10 + 4 * k for k in range(len(PARTDATA) - 2)])], None),
    2: ("camera", [("unk14", 0x14, "i"), ("dist", 0x18, "i")], (0x1C, 0x28, CAM_KEY)),
    3: ("entity", [("aframe", 0x14, "i"), ("start_aframe", 0x18, "i"), ("end_aframe", 0x1C, "i"),
                   ("delay", 0x20, "i"), ("prolong", 0x24, "i"), ("sort_priority", 0x28, "i"), ("spare", 0x2C, "i")],
        (0x30, 0x4C, ANIM_KEY)),
    4: ("fade", [("start", 0x14, "i"), ("end", 0x18, "i")], None),
    5: ("subscript", [("script_num", 0x14, "i"), ("sort_priority", 0x18, "i"), ("spare1", 0x1C, "i"),
                      ("spare2", 0x20, "i"), ("key", 0x24, "18i")], None),
}


def _get(d, o, fmt):
    if not 0 <= o <= len(d) - struct.calcsize("<" + fmt):
        raise Bad(f"scripts: 0x{o:X} past the end")
    v = struct.unpack_from("<" + fmt, d, o)
    return list(v) if len(v) > 1 else v[0]


def script_items(d):
    """[(path, offset, format)] of every script field, and the threads' (path, type) list. A path
    is a tuple of keys into read()'s "scripts"."""
    items, kinds = [], []
    for si, s in enumerate(layout(d)["scripts"]):
        for name, off, fmt in SCRIPT_HEAD:
            items.append(((si, name), s + off, fmt))
        for ti in range(_get(d, s, "i")):
            t = s + 0x1C + 4 * ti + _get(d, s + 0x1C + 4 * ti, "i")
            ty = _get(d, t, "i")
            if ty not in THREADS:
                raise Bad(f"scripts: thread at 0x{t:X} has type {ty}")
            name, fields, keys = THREADS[ty]
            kinds.append(((si, "thread_list", ti), name, t))
            for fname, off, fmt in THREAD_HEAD + fields:
                items.append(((si, "thread_list", ti, fname), t + off, fmt))
            if keys:
                at, size, kf = keys
                k, ki = t + at, 0
                while True:
                    for fname, off, fmt in kf:
                        items.append(((si, "thread_list", ti, "keys", ki, fname), k + off, fmt))
                    if _get(d, k + 4, "i") == 0:              # len 0 ends the track
                        break
                    k, ki = k + size, ki + 1
    return items, kinds


def _scripts(d):
    items, kinds = script_items(d)
    out = []
    for path, off, fmt in items:
        node = out
        for key in path[:-1]:                         # an int key indexes a list of dicts
            if isinstance(key, int):
                while len(node) <= key:
                    node.append({})
                node = node[key]
            else:
                node = node.setdefault(key, [])
        node[path[-1]] = _get(d, off, fmt)
    for s in out:
        s.setdefault("thread_list", [])
    for path, name, t in kinds:
        th = out[path[0]]["thread_list"][path[2]]
        th["type"] = name
        th["at"] = f"0x{t:X}"
    return out


def _write_scripts(d, scripts):
    """Write the fields that differ from the file's: threads may share bytes (a thread that starts
    inside another's keys), so only an edited field is written."""
    items, _ = script_items(bytes(d))
    for path, off, fmt in items:
        node = scripts
        try:
            for key in path:
                node = node[key]
        except (IndexError, KeyError, TypeError):
            raise Bad(f"scripts: {'/'.join(map(str, path))} is missing (threads and keys can't be added or removed)")
        if node != _get(d, off, fmt):
            v = node if isinstance(node, list) else [node]
            struct.pack_into("<" + fmt, d, off, *v)


def read(d):
    """The level data as JSON-ready dicts, or None if the file has none."""
    lay = layout(d)
    if not (lay["maps"] or lay["entities"] or lay["scripts"]):
        return None
    xyz = lambda o: list(struct.unpack_from("<3i", d, o))  # noqa: E731
    maps = []
    for mp in lay["maps"]:
        a = mp["at"]
        paths = []
        for p, keys in mp["paths"]:
            paths.append({"id": _w(d, p + 0xC), "unk8": _w(d, p + 8),
                          "unk10": list(struct.unpack_from("<8i", d, p + 0x10)),
                          "keys": [{"pos": xyz(k), "frame": _w(d, k + 0xC), "unk10": _w(d, k + 0x10),
                                    "flags": _w(d, k + 0x14, "<I"), "unk18": _w(d, k + 0x18)} for k in keys]})
        trigs = []
        for r, info in mp["triggers"]:
            ty, fl, slot = struct.unpack_from("<HHi", d, info)
            trigs.append({"pos": xyz(r), "cast": _w(d, r + 0xC) & 3, "type": ty, "flags": fl,
                          "slot": slot, "rot": xyz(info + 8)})
        places = []
        for o in mp["placements"]:
            m = struct.unpack_from("<10h3i", d, o + 0x28)
            places.append({"flags": _w(d, o, "<I"), "pos": xyz(o + 4),
                           "matrix": [list(m[0:3]), list(m[3:6]), list(m[6:9])], "t": list(m[10:13]),
                           "model": _w(d, o + 0x88, "<H"), "unk74": list(d[o + 0x74:o + 0x78]),
                           "unk9C": _w(d, o + 0x9C, "<H"), "unk9E": _w(d, o + 0x9E, "<H")})
        contours = []
        for c, edges in mp["contours"]:
            contours.append({"min": xyz(c), "max": xyz(c + 0xC), "unk18": list(struct.unpack_from("<7i", d, c + 0x18)),
                             "unk34": _w(d, c + 0x34),
                             "edges": [{"unk0": list(struct.unpack_from("<3i", d, k)), "p0": xyz(k + 0xC),
                                        "p1": xyz(k + 0x18), "len": _w(d, k + 0x24), "ang": _w(d, k + 0x28),
                                        "unk2C": list(struct.unpack_from("<3i", d, k + 0x2C))} for k in edges]})
        r, g, b, clear = d[a + 0x24:a + 0x28]
        maps.append({"cell_size": xyz(a), "clear_colour": [r, g, b], "clear": clear,
                     "paths": paths, "triggers": trigs, "contours": contours, "placements": places})
    ents = [{"mesh": None if m is None else f"0x{m:X}", "unk4": _w(d, r + 4), "group": _w(d, r + 8)}
            for r, m in lay["entities"]]
    return {"maps": maps, "entities": ents, "scripts": _scripts(d)}


def write(d, meta):
    """Write meta's map fields into d (a bytearray) in place."""
    lay = layout(bytes(d))
    if len(meta["maps"]) != len(lay["maps"]):
        raise Bad("map.json's maps don't match the file's")
    infos = {}
    for mi, (mp, mm) in enumerate(zip(lay["maps"], meta["maps"])):
        where = f"map {mi}"
        for name in ("paths", "triggers", "placements"):
            if len(mm[name]) != len(mp[name]):
                raise Bad(f"{where}: {name} can't be added or removed")
        a = mp["at"]
        struct.pack_into("<3i", d, a, *mm["cell_size"])
        d[a + 0x24:a + 0x28] = bytes(mm["clear_colour"] + [mm["clear"]])
        for (p, keys), jp in zip(mp["paths"], mm["paths"]):
            if len(jp["keys"]) != len(keys):
                raise Bad(f"{where}: path {jp['id']}'s keys can't be added or removed")
            struct.pack_into("<ii", d, p + 8, jp["unk8"], jp["id"])
            struct.pack_into("<8i", d, p + 0x10, *jp["unk10"])
            for k, jk in zip(keys, jp["keys"]):
                struct.pack_into("<3iiiIi", d, k, *jk["pos"], jk["frame"], jk["unk10"], jk["flags"], jk["unk18"])
            struct.pack_into("<i", d, p + 4, jp["keys"][-1]["frame"] if jp["keys"] else _w(d, p + 4))
        for (r, info), jt in zip(mp["triggers"], mm["triggers"]):
            struct.pack_into("<3i", d, r, *jt["pos"])
            if not 0 <= jt["cast"] <= 3:
                raise Bad(f"{where}: a trigger's cast is 0..3")
            struct.pack_into("<i", d, r + 0xC, (_w(d, r + 0xC) & ~3) | jt["cast"])
            val = (jt["type"], jt["flags"], jt["slot"], tuple(jt["rot"]))
            if infos.setdefault(info, val) != val:
                raise Bad(f"{where}: two triggers share one info at 0x{info:X}; give them the same values")
            struct.pack_into("<HHi3i", d, info, jt["type"], jt["flags"], jt["slot"], *jt["rot"])
        jcs = mm.get("contours")
        if jcs is not None:
            if len(jcs) != len(mp["contours"]):
                raise Bad(f"{where}: contours can't be added or removed")
            for (c, edges), jc in zip(mp["contours"], jcs):
                if len(jc["edges"]) != len(edges):
                    raise Bad(f"{where}: a contour's edges can't be added or removed")
                struct.pack_into("<3i3i7ii", d, c, *jc["min"], *jc["max"], *jc["unk18"], jc["unk34"])
                for k, je in zip(edges, jc["edges"]):
                    struct.pack_into("<3i3i3iii3i", d, k, *je["unk0"], *je["p0"], *je["p1"], je["len"], je["ang"],
                                     *je["unk2C"])
        for o, jp in zip(mp["placements"], mm["placements"]):
            struct.pack_into("<I3i", d, o, jp["flags"], *jp["pos"])
            m = [v for row in jp["matrix"] for v in row]
            pad = _w(d, o + 0x28 + 18, "<h")
            struct.pack_into("<10h3i", d, o + 0x28, *m, pad, *jp["t"])
            d[o + 0x74:o + 0x78] = bytes(jp["unk74"])
            struct.pack_into("<H", d, o + 0x88, jp["model"])
            struct.pack_into("<HH", d, o + 0x9C, jp["unk9C"], jp["unk9E"])
    if "scripts" in meta:
        _write_scripts(d, meta["scripts"])
    return d
