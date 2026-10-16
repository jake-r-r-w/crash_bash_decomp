"""Tests for mdl.py: model files' meshes to OBJ and back. Run: tools/dk make test-tools"""
import json
import os
import shutil
import struct
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import mdl  # noqa: E402

VERTS = [(1, 2, 3), (4, -5, 6), (-7, 8, 9), (10, 11, -12), (13, 14, 15)]


def sample():
    """A model file with two meshes: one strip of 3 faces (5 vertices), and an empty mesh."""
    head = bytearray(0x58)
    struct.pack_into("<I", head, mdl.COUNT_AT, 2)
    hdrs = 0x58
    strips = hdrs + 2 * mdl.HEAD                 # mesh 0: one strip, faces 3, flags 0
    verts = strips + 4                            # 0x14-byte block, then 8 bytes a vertex
    after = verts + 0x14 + 8 * len(VERTS)
    strips1 = after + 4                           # mesh 1: no strips, no vertices
    verts1 = strips1 + 2
    end = verts1 + 0x14
    d = bytearray(head) + bytes(end - 0x58)
    for m, (vp, sp, ap) in enumerate(((verts, strips, after), (verts1, strips1, verts1 + 0x14))):
        pos = hdrs + m * mdl.HEAD
        for k, target in enumerate((vp, sp, ap, ap, ap, ap)):
            p = pos + 0x10 + 4 * k
            struct.pack_into("<i", d, p, target - p)
    struct.pack_into("<HH", d, strips, 3 << 8, 0xFFFF)
    for i, v in enumerate(VERTS):
        struct.pack_into("<hhhH", d, verts + 0x14 + 8 * i, *v, i)
    struct.pack_into("<H", d, strips1, 0xFFFF)
    return bytes(d)


class Format(unittest.TestCase):
    def test_meshes(self):
        ms = mdl.meshes(sample())
        self.assertEqual([(nv, strips, np_) for _, nv, strips, np_ in ms], [(5, [0x300], None), (0, [], None)])

    def test_not_a_model(self):
        for d in (b"", bytes(0x58), b"\x08" + bytes(200)):
            with self.assertRaises(mdl.Bad):
                mdl.meshes(d)

    def test_faces_alternate_along_a_strip(self):
        self.assertEqual(mdl.faces([3 << 8]), [(1, 3, 2), (2, 3, 4), (3, 5, 4)])
        self.assertEqual(mdl.faces([1 << 8 | 8, 1 << 8]), [(1, 2, 3), (4, 6, 5)])


class Files(unittest.TestCase):
    def setUp(self):
        self.dir = tempfile.mkdtemp()
        self.bin = os.path.join(self.dir, "001.bin")
        self.out = os.path.join(self.dir, "001.mdl")
        with open(self.bin, "wb") as f:
            f.write(sample())
        mdl.unpack(self.bin, self.out)
        self.obj = os.path.join(self.out, mdl.OBJ)

    def tearDown(self):
        shutil.rmtree(self.dir)

    def test_unpack_pack_is_byte_identical(self):
        self.assertEqual(mdl.pack(self.out), sample())

    def test_obj_negates_y_and_z(self):
        self.assertIn("v 4 5 -6\n", open(self.obj).read())

    def test_a_moved_vertex_is_packed(self):
        t = open(self.obj).read().replace("v 4 5 -6\n", "v 40.4 5 -6\n")
        open(self.obj, "w").write(t)
        ms = mdl.meshes(sample())
        self.assertEqual(struct.unpack_from("<hhhH", mdl.pack(self.out), ms[0][0] + 8), (40, -5, 6, 1))

    def test_a_changed_vertex_count_is_refused(self):
        open(self.obj, "w").write(open(self.obj).read().replace("v 4 5 -6\n", ""))
        with self.assertRaises(mdl.Bad):
            mdl.pack(self.out)


def lit():
    """One mesh of one strip (2 faces, 4 vertices) with normals (+0x28 set), and an animation:
    2 frames, a keyframe and one halfway to a second keyframe, from its own vertex pool."""
    nv = 4
    hdr = 0x58
    strips = hdr + mdl.HEAD
    verts = strips + 4
    normals = verts + 0x14 + 8 * nv
    after = normals + 8 * nv
    uv_idx = after
    texs = uv_idx + 2 * 2
    table = texs + 2
    last = table + 2 * 2
    entry = last + 4
    anim = entry + 0x18
    frames = anim + 4
    list_a = frames + 32
    list_b = list_a + 0x14 + 2 * nv
    pool = list_b + 0x14 + 2 * nv
    end = pool + 6 * 8
    d = bytearray(end)
    struct.pack_into("<I", d, mdl.COUNT_AT, 1)
    struct.pack_into("<h", d, hdr + 8, 2)
    for k, target in enumerate((verts, strips, uv_idx, texs, table, last, normals)):
        p = hdr + 0x10 + 4 * k
        struct.pack_into("<i", d, p, target - p)
    struct.pack_into("<HH", d, strips, 2 << 8, 0xFFFF)
    for i in range(nv):
        struct.pack_into("<hhh", d, verts + 0x14 + 8 * i, 100 * i, 10, 20)
        struct.pack_into("<hhh", d, normals + 8 * i, 0, -4096, 0)
    struct.pack_into("<I", d, 0x40, 1)
    struct.pack_into("<i", d, 0x44, entry - 0x44)
    struct.pack_into("<iiii", d, entry, anim, end, 2, hdr - (entry + 0xC))
    struct.pack_into("<i", d, anim, pool - anim)
    struct.pack_into("<iiii", d, frames, list_a - 0x14 - frames, 0, 0, 0)
    struct.pack_into("<iiii", d, frames + 16, list_a - 0x14 - (frames + 16), list_b - 0x14 - (frames + 20), 0x800, 0)
    struct.pack_into(f"<{nv}H", d, list_a + 0x14, *[4 * i for i in range(nv)])
    struct.pack_into(f"<{nv}H", d, list_b + 0x14, *[4 * (i + 4) for i in range(nv)])
    for i in range(8):
        struct.pack_into("<hhh", d, pool + 6 * i, 100 * i, 10 * (i >= 4), 20)
    return bytes(d)


class Lit(unittest.TestCase):
    def setUp(self):
        self.dir = tempfile.mkdtemp()
        self.bin = os.path.join(self.dir, "001.bin")
        with open(self.bin, "wb") as f:
            f.write(lit())
        self.out = os.path.join(self.dir, "001.mdl")
        mdl.unpack(self.bin, self.out)

    def tearDown(self):
        shutil.rmtree(self.dir)

    def test_normals_round_trip_and_edit(self):
        ms = mdl.meshes(lit())
        self.assertIsNotNone(ms[0][3])
        self.assertEqual(mdl.pack(self.out), lit())
        obj = os.path.join(self.out, mdl.OBJ)
        t = open(obj).read()
        self.assertIn("vn 0 4096 0", t)
        self.assertIn("f 1//1 3//3 2//2", t)
        open(obj, "w").write(t.replace("vn 0 4096 0", "vn 4096 0 0", 1))
        self.assertEqual(struct.unpack_from("<hhh", mdl.pack(self.out), ms[0][3]), (4096, 0, 0))

    def test_glb_has_the_mesh_its_keyframes_and_animation(self):
        d = open(os.path.join(self.out, mdl.GLB), "rb").read()
        magic, version, length = struct.unpack_from("<III", d)
        self.assertEqual((magic, version, length), (0x46546C67, 2, len(d)))
        jl = struct.unpack_from("<I", d, 12)[0]
        doc = json.loads(d[20:20 + jl])
        prim = doc["meshes"][0]["primitives"][0]
        self.assertEqual(len(prim["targets"]), 2)                 # the two keyframes
        self.assertEqual(doc["accessors"][prim["indices"]]["type"], "SCALAR")
        sampler = doc["animations"][0]["samplers"][0]
        self.assertEqual(doc["accessors"][sampler["input"]]["count"], 2)


class NoMeshes(unittest.TestCase):
    def test_a_file_with_no_meshes_round_trips(self):
        d = bytearray(0x78)
        struct.pack_into("<I", d, 0x20, mdl.COUNT_AT - 0x1C)
        self.assertEqual(mdl.meshes(bytes(d)), [])
        d2 = bytearray(d)
        struct.pack_into("<I", d2, 0x20, 0x100)
        with self.assertRaises(mdl.Bad):
            mdl.meshes(bytes(d2))


if __name__ == "__main__":
    unittest.main()
