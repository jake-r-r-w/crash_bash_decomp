"""Tests for mapdata.py: a model file's maps, map directory and scripts. Run: tools/dk make test-tools"""
import json
import os
import struct
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import mapdata  # noqa: E402


def model():
    """A model file with one map (one path of two keys, two triggers sharing an info, one
    placement), one entity and one script."""
    d = bytearray(0x58)
    out = []

    def put(at, blob):
        out.append((at, blob))

    base = 0x58
    ml = base                                  # map_list: count, offset
    mp = ml + 8                                # map_map, placements at +0x34
    pp = mp + 0x34
    pl = pp + 0xA0                             # paths: count, offset
    path = pl + 8
    tl = path + 0x30 + 2 * 0x1C                # triggers: count, two records
    info = tl + 4 + 2 * 0x10
    el = info + 0x14                           # entities: count, one record
    mesh = el + 4 + 12
    sc = mesh + 0x34                           # scripts table
    sh = sc + 4
    end = sh + 0x20
    d = bytearray(end)
    struct.pack_into("<i", d, 0x18, ml - 0x18)
    struct.pack_into("<i", d, 0x1C, el - 0x1C)
    struct.pack_into("<ii", d, 0x48, 1, sc - 0x4C)
    struct.pack_into("<Ii", d, ml, 1, mp - (ml + 4))
    struct.pack_into("<3iii", d, mp, 8192, 8192, 8192, pl - (mp + 0xC), tl - (mp + 0x10))
    struct.pack_into("<ii", d, mp + 0x1C, 1, pp - (mp + 0x20))
    d[mp + 0x24:mp + 0x28] = bytes([1, 2, 3, 1])
    struct.pack_into("<I3i", d, pp, 0x2000, 10, 20, 30)
    struct.pack_into("<10h3i", d, pp + 0x28, 4096, 0, 0, 0, 4096, 0, 0, 0, 4096, 0, 10, 20, 30)
    struct.pack_into("<H", d, pp + 0x88, 0x5001)
    struct.pack_into("<ii", d, pl, 1, path - (pl + 4))
    struct.pack_into("<iiii", d, path, 2, 60, 0, 7)
    struct.pack_into("<3iiiIi", d, path + 0x30, 1, 2, 3, 0, 0, 0, 0)
    struct.pack_into("<3iiiIi", d, path + 0x30 + 0x1C, 4, 5, 6, 60, 0, 0x2000, 0)
    struct.pack_into("<I", d, tl, 2)
    for k in range(2):
        r = tl + 4 + k * 0x10
        struct.pack_into("<3ii", d, r, k, k, k, ((info - (r + 0xC)) & ~3) | k)
    struct.pack_into("<HHi3i", d, info, 3, 0, 1, 0, 0, 0)
    struct.pack_into("<i", d, el, 1)
    struct.pack_into("<iii", d, el + 4, mesh - (el + 4), 0, 5)
    struct.pack_into("<i", d, sc, sh - sc)
    struct.pack_into("<5i", d, sh, 2, 0, 100, 0, 100)
    return bytes(d), mesh


class MapData(unittest.TestCase):
    def setUp(self):
        self.d, self.mesh = model()
        self.m = mapdata.read(self.d)

    def test_read(self):
        mm = self.m["maps"][0]
        self.assertEqual((mm["cell_size"], mm["clear_colour"], mm["clear"]), ([8192] * 3, [1, 2, 3], 1))
        self.assertEqual([k["pos"] for k in mm["paths"][0]["keys"]], [[1, 2, 3], [4, 5, 6]])
        self.assertEqual(mm["paths"][0]["id"], 7)
        self.assertEqual([t["cast"] for t in mm["triggers"]], [0, 1])
        self.assertEqual(mm["triggers"][1]["type"], 3)
        p = mm["placements"][0]
        self.assertEqual((p["pos"], p["t"], p["model"], p["matrix"][1]), ([10, 20, 30], [10, 20, 30], 0x5001, [0, 4096, 0]))
        self.assertEqual(self.m["entities"], [{"mesh": f"0x{self.mesh:X}", "unk4": 0, "group": 5}])
        self.assertEqual(self.m["scripts"][0]["threads"], 2)

    def test_round_trip_and_edit(self):
        m = json.loads(json.dumps(self.m))
        self.assertEqual(bytes(mapdata.write(bytearray(self.d), m)), self.d)
        m["maps"][0]["placements"][0]["pos"] = [-1, -2, -3]
        m["maps"][0]["paths"][0]["keys"][1]["frame"] = 90
        out = bytes(mapdata.write(bytearray(self.d), m))
        mm = mapdata.read(out)["maps"][0]
        self.assertEqual(mm["placements"][0]["pos"], [-1, -2, -3])
        lay = mapdata.layout(out)["maps"][0]["paths"][0][0]
        self.assertEqual(struct.unpack_from("<i", out, lay + 4)[0], 90)   # frames follows the last key

    def test_counts_and_shared_infos_are_checked(self):
        m = json.loads(json.dumps(self.m))
        m["maps"][0]["triggers"][1]["slot"] = 9                          # same info, different value
        with self.assertRaises(mapdata.Bad):
            mapdata.write(bytearray(self.d), m)
        m = json.loads(json.dumps(self.m))
        m["maps"][0]["placements"].append(m["maps"][0]["placements"][0])
        with self.assertRaises(mapdata.Bad):
            mapdata.write(bytearray(self.d), m)

    def test_no_level_data(self):
        self.assertIsNone(mapdata.read(bytes(0x58)))


if __name__ == "__main__":
    unittest.main()
