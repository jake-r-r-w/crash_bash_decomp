"""Tests for mdl.py: model files' meshes to OBJ and back. Run: tools/dk make test-tools"""
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
        self.assertEqual([(nv, strips) for _, nv, strips in ms], [(5, [0x300]), (0, [])])

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


if __name__ == "__main__":
    unittest.main()
