"""Tests for tools/scripts/cc1_compare.py (tools/cc1/: does a cc1 variant change a build?).
Run: tools/dk make test-tools"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import cc1_compare  # noqa: E402


class Changed(unittest.TestCase):
    def test_same_builds(self):
        self.assertEqual(cc1_compare.changed([1, 2, 3], [1, 2, 3]), 0)

    def test_words_and_length_count(self):
        """A variant that drops one insn and moves another shows both."""
        self.assertEqual(cc1_compare.changed([1, 2, 3], [1, 3, 2]), 2)
        self.assertEqual(cc1_compare.changed([1, 2, 3], [1, 2]), 1)

    def test_a_failed_build_is_not_a_count(self):
        self.assertIsNone(cc1_compare.changed(None, [1]))
        self.assertIsNone(cc1_compare.changed([1], None))


if __name__ == "__main__":
    unittest.main()
