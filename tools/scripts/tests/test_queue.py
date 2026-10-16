"""Tests for decomp.py's target picking and function lookup (what `next`, `queue` and the swarm
lease). Run: tools/dk make test-tools"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import decomp  # noqa: E402

PROJ = {"lib_segments": ["^psyq/"]}


def fn(seg, name, size, fp=None):
    return {"seg": seg, "name": name, "orig": name, "size": size, "fp": fp}


class PickTargets(unittest.TestCase):
    def setUp(self):
        self.funcs = {
            "80010000": fn("main/main", "func_80010000", 400),
            "80020000": fn("main/main", "func_80020000", 40),
            "80030000": fn("main/main", "func_80030000", 200, fp="t"),
            "ovl5d:800B3A38": fn("overlays/ovl5d/ovl5d", "func_800B3A38", 200, fp="t"),
            "80040000": fn("main/main", "func_80040000", 8),
            "80050000": fn("psyq/libgte", "func_80050000", 12),
        }
        self.stat = {k: "todo" for k in self.funcs}
        self.stat["80040000"] = "matched"

    def keys(self, st=None, seg=None, count=10):
        return [k for k, _ in decomp.pick_targets(self.funcs, self.stat, st or {}, PROJ, seg, count)]

    def test_order_and_scope(self):
        # matched and library code are left out; cost = size / unmatched identical copies
        self.assertEqual(self.keys(), ["80020000", "80030000", "ovl5d:800B3A38", "80010000"])

    def test_seg_prefix(self):
        self.assertEqual(self.keys(seg="overlays/"), ["ovl5d:800B3A38"])
        self.assertEqual(self.keys(seg="psyq/"), ["80050000"])

    def test_in_progress_first_and_tried_last(self):
        st = {"80010000": {"status": "in_progress"}, "80020000": {"tries": 3}}
        self.stat["80010000"] = "in_progress"
        self.assertEqual(self.keys(st), ["80010000", "80030000", "ovl5d:800B3A38", "80020000"])

    def test_auto_diff_is_the_work(self):
        self.assertEqual(self.keys({"80010000": {"auto_diff": 2}})[0], "80010000")

    def test_count(self):
        self.assertEqual(len(self.keys(count=2)), 2)


if __name__ == "__main__":
    unittest.main()
