"""Tests for the shift build: the padded linker script, and a grown overlay moving to the DAT's end.
Run: tools/dk make test-tools"""
import json
import os
import sys
import tempfile
import unittest
from unittest import mock

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import dat  # noqa: E402
import pack_disc  # noqa: E402
import shift  # noqa: E402

LD = """    .ovl5f 0x800B32B4 : AT(ovl5f_ROM_START) SUBALIGN(2)
    {
        build/asm/data/overlays/ovl5f/ovl5f_id.rodata.s.o(.rodata);
        build/src/overlays/ovl5f/ovl5f.c.o(.rodata);
        build/src/overlays/ovl5f/ovl5f.c.o(.text);
        build/asm/data/overlays/ovl5f/ovl5f_id.rodata.s.o(.data);
        build/src/overlays/ovl5f/ovl5f.c.o(.data);
        build/src/overlays/ovl5f/ovl5f.c.o(.bss);
    }
"""


class PadScript(unittest.TestCase):
    def test_pads_before_each_c_section_and_not_the_id(self):
        t = shift.pad_script(LD, "ovl5f", 0x40)
        lines = [l.strip() for l in t.splitlines()]
        for sect in (".rodata", ".text", ".data"):
            i = lines.index(f"build/src/overlays/ovl5f/ovl5f.c.o({sect});")
            self.assertTrue(lines[i - 1].startswith(". += 0x40;"), sect)
        i = lines.index("build/asm/data/overlays/ovl5f/ovl5f_id.rodata.s.o(.rodata);")
        self.assertFalse(lines[i - 1].startswith(". +="))
        self.assertEqual(t.count(". += 0x40;"), 3)

    def test_a_script_without_c_data_is_refused(self):
        with self.assertRaises(ValueError):
            shift.pad_script(LD.replace("ovl5f.c.o(.data)", "ovl5f_data.data.s.o(.data)"), "ovl5f", 0x40)


class Grow(unittest.TestCase):
    def test_the_overlay_goes_to_the_end_and_the_index_follows(self):
        with tempfile.TemporaryDirectory() as d:
            idx = {"entries": [{"id": "000", "sector": 0, "size": 10}, {"id": "001", "sector": 1, "size": 3000,
                                                                       "overlay": "ovl5f"}]}
            os.makedirs(f"{d}/assets")
            json.dump(idx, open(f"{d}/assets/{dat.INDEX}", "w"))
            datp, exep = f"{d}/CRASHBSH.DAT", f"{d}/exe"
            open(datp, "wb").write(bytes(3 * dat.SECTOR + 100))
            t_addr = 0x80010000
            open(exep, "wb").write(bytes(0x800 + dat.TABLE - t_addr + 16))
            proj = {"header": {"t_addr": f"{t_addr:08X}"}}
            with mock.patch.object(dat, "ASSETS", f"{d}/assets"):
                pack_disc.grow(datp, exep, proj, "ovl5f", b"\x11" * 5000)
            data = open(datp, "rb").read()
            self.assertEqual(data[4 * dat.SECTOR:4 * dat.SECTOR + 5000], b"\x11" * 5000)
            self.assertEqual(len(data) % dat.SECTOR, 0)
            e = open(exep, "rb").read()[0x800 + dat.TABLE - t_addr + 8:][:8]
            self.assertEqual((int.from_bytes(e[:4], "little"), int.from_bytes(e[4:], "little")), (4, 5000))


if __name__ == "__main__":
    unittest.main()
