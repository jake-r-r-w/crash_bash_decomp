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
PLACE_AT = 0x34


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
        pp = _self(d, mp + 0x20)
        out["maps"].append({"at": mp, "paths": paths, "triggers": trigs,
                            "placements": [pp + i * PLACE for i in range(_w(d, mp + 0x1C))]})
    el = _self(d, 0x1C)
    for i in range(_w(d, el)):
        r = el + 4 + i * 12
        out["entities"].append((r, _self(d, r) if _w(d, r) else None))
    if _w(d, 0x48) > 0:
        st = _self(d, 0x4C)
        out["scripts"] = [_self(d, st + 4 * i) for i in range(_w(d, 0x48))]
    return out


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
        r, g, b, clear = d[a + 0x24:a + 0x28]
        maps.append({"cell_size": xyz(a), "clear_colour": [r, g, b], "clear": clear,
                     "paths": paths, "triggers": trigs, "placements": places})
    ents = [{"mesh": None if m is None else f"0x{m:X}", "unk4": _w(d, r + 4), "group": _w(d, r + 8)}
            for r, m in lay["entities"]]
    scripts = [dict(zip(("threads", "subs", "end_frame", "start_frame", "frames"),
                        struct.unpack_from("<5i", d, s))) for s in lay["scripts"]]
    return {"maps": maps, "entities": ents, "scripts": scripts}


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
        for o, jp in zip(mp["placements"], mm["placements"]):
            struct.pack_into("<I3i", d, o, jp["flags"], *jp["pos"])
            m = [v for row in jp["matrix"] for v in row]
            pad = _w(d, o + 0x28 + 18, "<h")
            struct.pack_into("<10h3i", d, o + 0x28, *m, pad, *jp["t"])
            d[o + 0x74:o + 0x78] = bytes(jp["unk74"])
            struct.pack_into("<H", d, o + 0x88, jp["model"])
            struct.pack_into("<HH", d, o + 0x9C, jp["unk9C"], jp["unk9E"])
    return d
