"""Tests for proto_apply.py: C names from a disc's symbols and which matches become renames.
Run: tools/dk make test-tools"""
import os
import sys
import tempfile
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import proto_apply  # noqa: E402


def row(sym, retail, sim, module="main"):
    return (sym, "label", 0x80018000, module, retail, sim)


class CName(unittest.TestCase):
    def test_free_function_drops_its_parameters(self):
        self.assertEqual(proto_apply.c_name("mal_free__FPvP9_mal_area"), ("mal_free", None))
        self.assertEqual(proto_apply.c_name("init_file__Fv"), ("init_file", None))

    def test_c_name_stays(self):
        self.assertEqual(proto_apply.c_name("english_rules"), ("english_rules", None))

    def test_member_and_operator_have_no_c_name(self):
        self.assertIsNone(proto_apply.c_name("Update__6CameraFv")[0])
        self.assertIsNone(proto_apply.c_name("__ls__FR7ostreami")[0])


class Plan(unittest.TestCase):
    def test_rules(self):
        found = [
            row("good__Fv", "func_80010000", 1.0),
            row("weak__Fv", "func_80010100", 0.8),            # below --min: to check
            row("Sony", "sys.o+0x10", 1.0),                    # PsyQ: stays
            row("orig__Fv", "OurGuess", 1.0),                  # already named
            row("over__Fi", "func_80010200", 1.0),             # an overload pair
            row("over__Fv", "func_80010300", 1.0),
            row("taken__Fv", "func_80010400", 1.0),            # a name the build has
            row("a__Fv", "func_80010500", 1.0),                # two symbols, one function
            row("b__Fv", "func_80010500", 1.0),
            row("eng__Fv", "func_80078CA4", 1.0, "engine"),    # another module
        ]
        renames, check, left = proto_apply.plan(found, "main", 0.9, False, {"taken"})
        self.assertEqual(renames, [("func_80010000", "good", "good__Fv", 1.0)])
        self.assertEqual([r[0] for r in check], ["weak__Fv"])
        self.assertEqual(sorted(r[0] for r, _ in left),
                         ["Sony", "a__Fv", "b__Fv", "orig__Fv", "over__Fi", "over__Fv", "taken__Fv"])
        renames, _, _ = proto_apply.plan(found, "main", 0.9, True, {"taken"})
        self.assertIn(("OurGuess", "orig", "orig__Fv", 1.0), renames)


class Slot(unittest.TestCase):
    def test_overlay_functions_rename_by_key(self):
        found = [row("UpdatePAPU__Fv", "func_800B7DA4 (papu)", 1.0, "slot 17F"),
                 row("TA_HeroBot__FUi", "TA_HeroBot (tank)", 0.95, "slot 17E"),     # already this name
                 row("Other__Fv", "Named (tank)", 0.95, "slot 17E")]                # named otherwise
        renames, _, left = proto_apply.plan(found, "slot", 0.9, False, set())
        self.assertEqual(renames, [("papu:800B7DA4", "UpdatePAPU", "UpdatePAPU__Fv", 1.0)])
        self.assertEqual([r[0] for r, _ in left], ["Other__Fv"])


class Aliases(unittest.TestCase):
    def test_data_use_of_a_renamed_address(self):
        here = os.getcwd()
        with tempfile.TemporaryDirectory() as d:
            os.makedirs(os.path.join(d, "src"))
            with open(os.path.join(d, "src", "a.c"), "w") as f:
                f.write("x = func_80010000;\ny = D_80010000;\nz = D_80020000;\n")
            os.chdir(d)
            try:
                got = proto_apply.aliases([("func_80010000", "good", "good__Fv", 1.0)])
            finally:
                os.chdir(here)
        self.assertEqual(got, [("func_80010000", "src/a.c:2")])


if __name__ == "__main__":
    unittest.main()
