"""Tests for dat.py: splitting CRASHBSH.DAT into its entries and filler and joining them back.
Run: tools/dk make test-tools"""
import os
import struct
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import dat  # noqa: E402

S = dat.SECTOR
# three entries: one ending mid-sector (its padding holds junk), an unreferenced sector, then two more
ENTRIES = [(0, S + 100), (3, 50), (4, S)]
TOTAL = 5 * S


def sample():
    data = bytearray(os.urandom(TOTAL))   # junk everywhere, as the retail padding is
    return bytes(data)


class Index(unittest.TestCase):
    def test_reads_pairs_after_the_exe_header(self):
        t_addr, table = 0x80010000, 0x80010010
        exe = bytes(0x800 + 0x10) + struct.pack("<4I", 7, 100, 8, 200)
        self.assertEqual(dat.read_index(exe, t_addr, count=2, table=table), [(7, 100), (8, 200)])


class SplitJoin(unittest.TestCase):
    def test_gaps_are_padding_and_unreferenced_sectors(self):
        self.assertEqual(dat.gaps(ENTRIES, TOTAL), [(S + 100, 3 * S), (3 * S + 50, 4 * S)])

    def test_round_trip_is_byte_identical(self):
        d = sample()
        files, filler = dat.split(d, ENTRIES)
        self.assertEqual([len(f) for f in files], [S + 100, 50, S])
        self.assertEqual(len(filler), (2 * S - 100) + (S - 50))
        self.assertEqual(dat.join(files, filler, ENTRIES, TOTAL), d)

    def test_a_shorter_file_is_zero_padded_in_place(self):
        d = sample()
        files, filler = dat.split(d, ENTRIES)
        files[1] = b"abc"
        out = dat.join(files, filler, ENTRIES, TOTAL)
        self.assertEqual(out[3 * S:3 * S + 50], b"abc" + bytes(47))
        self.assertEqual(out[4 * S:], d[4 * S:])          # nothing after it moved
        self.assertEqual(out[3 * S + 50:4 * S], d[3 * S + 50:4 * S])

    def test_a_bigger_file_is_refused(self):
        files, filler = dat.split(sample(), ENTRIES)
        files[1] = bytes(51)
        with self.assertRaises(SystemExit):
            dat.join(files, filler, ENTRIES, TOTAL)


class Names(unittest.TestCase):
    def test_list_lines_become_slash_paths(self):
        with tempfile.NamedTemporaryFile("w", suffix=".txt", delete=False) as f:
            f.write("# a comment\n\n000=models\\cutscene\\uka\\data.tex\n991=overlays\\gameeng.bin\n")
        try:
            self.assertEqual(dat.names(f.name), {0: "models/cutscene/uka/data.tex", 991: "overlays/gameeng.bin"})
        finally:
            os.remove(f.name)

    def test_the_list_names_every_entry(self):
        self.assertEqual(sorted(dat.names(os.path.join(os.path.dirname(__file__), "..", "..", "..", dat.NAMES))),
                         list(range(dat.COUNT)))


if __name__ == "__main__":
    unittest.main()
