"""Tests for leak_order.py: the climb/fall fit of a file against a leaked header's order.
Run: tools/dk make test-tools"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import leak_order  # noqa: E402


class Fit(unittest.TestCase):
    def test_one_file_is_a_climb_then_its_inline_tail(self):
        (e1, k), _ = leak_order.best_splits([0, 2, 4, 6, 5, 3, 1])
        # the peak fits either side; ties go to the earliest split
        self.assertEqual((e1, k), (7, 3))

    def test_two_files_show_two_climbs_and_falls(self):
        (e1, _), (e2, b) = leak_order.best_splits([4, 6, 5, 0, 2, 1])
        self.assertEqual(e2, 6)
        self.assertIn(b, (3, 4))
        self.assertLess(e1, e2)

    def test_a_few_out_of_place_dont_hide_the_shape(self):
        (e1, k), _ = leak_order.best_splits([0, 1, 9, 2, 3, 8, 7, 6])
        self.assertEqual(k, 5)
        self.assertEqual(e1, 7)


class Header(unittest.TestCase):
    def test_declarations_in_first_order(self):
        text = "/* void Hidden(void); */\nvoid A(u32 player);\ns32 *B(void);\nvoid A(u32 player);\n" \
               "static inline void C(void) {\n}\n"
        self.assertEqual(leak_order.declared_order(text), ["A", "B", "C"])


if __name__ == "__main__":
    unittest.main()
