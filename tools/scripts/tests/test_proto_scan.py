"""Tests for sym_scan.py's line records and proto_scan.py's disc analysis.
Run: tools/dk make test-tools"""
import os
import struct
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import proto_scan  # noqa: E402
import sym_scan  # noqa: E402

NOP, JR_RA = 0, 0x03E00008


def pstr(s):
    return bytes([len(s)]) + s.encode()


def r(val, tag, payload=b""):
    return struct.pack("<IB", val, tag) + payload


def func(addr, name, line, path="C:\\crash4\\A.CPP"):
    return r(addr, 0x8C, struct.pack("<HIHIiI", 29, 0x18, 31, 0x80000000, -4, line)
             + pstr(path) + pstr(name))


def code(bodies, header=5):
    """A module: header words, then each body (a list of words) ended by jr ra; nop.
    Returns (bytes, the word offset of each function)."""
    w, offs = [0x80071000] * header, []
    for b in bodies:
        offs.append(4 * len(w))
        w += b + [JR_RA, NOP]
    return struct.pack("<%dI" % len(w), *w), offs


def jal(t):
    return (3 << 26) | ((t >> 2) & 0x3FFFFFF)


class LineRecords(unittest.TestCase):
    A = 0x80072000

    def test_line_table(self):
        d = (b"\0" * 7 + func(self.A, "F__Fv", 100)
             + r(self.A + 4, 0x80)
             + r(self.A + 8, 0x82, b"\x02")
             + r(self.A + 12, 0x84, struct.pack("<H", 3))
             + r(self.A + 16, 0x86, struct.pack("<I", 50))
             + r(self.A + 20, 0x88, struct.pack("<I", 7) + pstr("C:\\crash4\\B.CPP"))
             + r(self.A + 24, 0x8A)
             + r(self.A + 32, 0x8E, struct.pack("<I", 110)) + b"\0" * 7)
        recs = sym_scan.records(d)
        self.assertEqual([x[0] for _, x in recs], ["func"] + ["line"] * 6 + ["fend"])
        self.assertEqual(sym_scan.lines(recs), [
            (self.A + 4, "C:\\crash4\\A.CPP", 101), (self.A + 8, "C:\\crash4\\A.CPP", 103),
            (self.A + 12, "C:\\crash4\\A.CPP", 106), (self.A + 16, "C:\\crash4\\A.CPP", 50),
            (self.A + 20, "C:\\crash4\\B.CPP", 7)])

    def test_value_must_be_code_address(self):
        d = b"".join(r(0x12345678 + 4 * i, 0x80) for i in range(5))
        self.assertEqual(sym_scan.records(d), [])


class Disc(unittest.TestCase):
    def test_find_index(self):
        table = b"".join(struct.pack("<II", 2 * i, 3000) for i in range(60))
        exe = b"PS-X EXE" + b"\0" * (0x800 - 8) + b"\xff" * 0x40 + table + b"\0" * 16
        off, entries = proto_scan.find_index(exe, 200 * 2048)
        self.assertEqual(off, 0x840)
        self.assertEqual(len(entries), 60)
        self.assertEqual(entries[3], (6, 3000))

    def test_find_engine(self):
        dat = bytearray(8 * 2048)
        struct.pack_into("<5I", dat, 4 * 2048, *[0x80071000 + 4 * i for i in range(5)])
        self.assertEqual(proto_scan.find_engine(bytes(dat), [(0, 0x1800), (4, 0x2000)]), (1, 4, 0x2000))

    def test_find_base_and_shift(self):
        base = 0x80071764
        sizes = [3, 7, 2, 5, 9, 4, 6, 3, 8, 2]
        _, offs = code([[NOP] * n for n in sizes])
        # each function calls the next one
        data, offs = code([[jal(base + offs[(i + 1) % len(offs)])] + [NOP] * (n - 1)
                           for i, n in enumerate(sizes)])
        self.assertEqual(proto_scan.find_base(data)[0], base)
        mod = proto_scan.Module("engine", data, base)
        # records from a link that put the module 4 bytes lower
        recs = [base + o - 4 for o in offs[1:]]
        self.assertEqual(proto_scan.fit_shift(mod, recs), (4, len(recs)))


class Matching(unittest.TestCase):
    def test_identical_functions_keep_their_order(self):
        f = [0x27BDFFE8, 0x8C820010, 0x24420001]       # addiu sp; lw v0; addiu v0
        g = [0x00851021, 0x00A41823]                   # addu; subu
        rdata, roffs = code([g, f, f, f, g])
        ddata, doffs = code([f, f, f])
        rbase, dbase = 0x80078C90, 0x80071764
        rmod = proto_scan.Module("engine", rdata, rbase)
        retail = []
        for i, o in enumerate(roffs):
            end = roffs[i + 1] if i + 1 < len(roffs) else len(rdata)
            retail.append((rbase + o, f"r{i}", rmod.body(rbase + o, rbase + end)))
        demo = {"engine": proto_scan.Module("engine", ddata, dbase)}
        funcs = [(dbase + o, None, ("func", dbase + o, f"d{i}", 0, 1, "A.CPP"))
                 for i, o in enumerate(doffs[1:], 1)]
        funcs.insert(0, (dbase + doffs[0], dbase + doffs[1], ("func", dbase + doffs[0], "d0", 0, 1, "A.CPP")))
        # the first function follows the header, not a jr ra: not a start
        placed, _ = proto_scan.place(funcs, list(demo.values()), [])
        got = proto_scan.match_funcs(placed, {"engine": retail})
        self.assertIsNone(got[0][1])
        self.assertEqual([p[2] for _, _, p in got[1:]], ["r1", "r2"])


class Placing(unittest.TestCase):
    SIZES = [3, 7, 2, 5, 9, 4, 6, 3, 8, 2, 5, 7]

    def chain(self, base):
        """A module whose functions each call the next."""
        _, offs = code([[NOP] * n for n in self.SIZES])
        return code([[jal(base + offs[(i + 1) % len(offs)])] + [NOP] * (n - 1)
                     for i, n in enumerate(self.SIZES)])

    def test_local_shift_after_a_size_change(self):
        base = 0x80071000
        data, offs = self.chain(base)
        mod = proto_scan.Module("engine", data, base)
        # the records' link had the code from function 6 on 8 bytes earlier
        sure = [base + o for o in offs[1:6]] + [base + o - 8 for o in offs[6:]]
        self.assertEqual(proto_scan.local_shifts(mod, sure), [(base + offs[6] - 8, base + offs[-1] - 8, 8)])

    def test_find_slots(self):
        base = 0x800B0C84
        a = self.chain(base)[0].ljust(0x1000, b"\0")   # smaller entries aren't code
        dat = a + a
        entries = [(0, len(a)), (len(a) // 2048, len(a))]
        got_base, slots = proto_scan.find_slots(dat, entries, skip=None)
        self.assertEqual(got_base, base)
        self.assertEqual([i for i, _ in slots], [0, 1])


if __name__ == "__main__":
    unittest.main()
