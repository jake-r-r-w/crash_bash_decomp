"""Tests for decomp.py trace (ptrace.py): insns followed through pass dumps, and C variables
mapped to their pseudos by declaration order. Run: tools/dk make test-tools"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import ptrace  # noqa: E402

RTL = """;; Function f

(note 1 0 10 "" NOTE_INSN_DELETED)

(insn 10 1 11 (set (reg/v:SI 80)
        (reg:SI 4 a0)) -1 (nil)
    (nil))

(insn 11 10 12 (set (reg/v:SI 81)
        (reg/v:SI 80)) -1 (nil)
    (nil))

(insn 12 11 13 (set (reg:SI 90)
        (ashift:SI (reg/v:SI 81)
            (const_int 4))) -1 (nil)
    (nil))

(insn 13 12 0 (set (reg:SI 91)
        (minus:SI (reg:SI 90)
            (reg/v:SI 81))) -1 (nil)
    (nil))
"""

# loop: insn 12 is hoisted (a new uid 30 with the same pattern; the old one is gone)
LOOP = RTL.replace("(insn 12 11 13", "(insn 30 11 13")
# combine: the copy 11 is deleted and both its users now read r80
COMBINE = """;; Function f

(insn 10 1 11 (set (reg/v:SI 80)
        (reg:SI 4 a0)) 172 {movsi_internal2} (nil)
    (nil))

(note 11 10 30 "" NOTE_INSN_DELETED)

(insn 30 11 13 (set (reg:SI 90)
        (ashift:SI (reg/v:SI 80)
            (const_int 4))) 205 {ashlsi3} (nil)
    (nil))

(insn 13 30 0 (set (reg:SI 91)
        (minus:SI (reg:SI 90)
            (reg/v:SI 80))) 16 {subsi3_internal} (insn_list 30 (nil))
    (expr_list:REG_DEAD (reg:SI 90)
        (expr_list:REG_DEAD (reg/v:SI 80)
            (nil))))
"""
# greg: the same insns with hard registers
GREG = (COMBINE.replace("(reg/v:SI 80)", "(reg/v:SI 2 v0)").replace("(reg:SI 90)", "(reg:SI 3 v1)")
        .replace("(reg:SI 91)", "(reg:SI 4 a0)") + "\n;; Register dispositions:\n80 in 2  90 in 3  91 in 4\n")
DUMPS = [("rtl", RTL), ("loop", LOOP), ("combine", COMBINE), ("greg", GREG)]


class TestFollow(unittest.TestCase):
    def setUp(self):
        self.passes, self.user = ptrace.load(DUMPS, "f")

    def test_user_variables_come_from_the_rtl_dump(self):
        self.assertEqual(self.user, [80, 81])

    def test_deleted_insn_names_the_insns_it_went_into(self):
        rows = ptrace.follow(self.passes, 11)
        self.assertEqual([(p, ev) for p, _, _, ev in rows], [("rtl", "first"), ("loop", "same"), ("combine", "deleted")])
        self.assertEqual(rows[-1][1], [30, 13])

    def test_hoisted_insn_is_followed_to_its_new_uid(self):
        rows = ptrace.follow(self.passes, 12)
        self.assertEqual([(p, u, ev) for p, u, _, ev in rows],
                         [("rtl", 12, "first"), ("loop", 30, "moved"), ("combine", 30, "changed"),
                          ("greg", 30, "renamed")])

    def test_dies_notes(self):
        e = self.passes[2][1][13]
        self.assertEqual(e.dead, [90, 80])

    def test_report_insn(self):
        out = ptrace.report_insn(self.passes, "f", 12)
        self.assertIn("(moved: new uid 30)", out)
        self.assertIn("changes: loop moves it to uid 30, then combine rewrites it", out)

    def test_report_reg_lists_each_chain_once(self):
        out = ptrace.report_reg(self.passes, "f", 81, "r81")
        self.assertIn("combine deletes it; it went into 30, 13", out)
        self.assertIn("loop moves it to 30; combine: r90 = r80<<4", out)
        self.assertEqual(out.count("r90 = r81<<4"), 1)

    def test_insns_with_a_mode_are_insns(self):
        """reload and sched2 print some insns as insn:QI / insn:TI: still the same insn."""
        greg = GREG.replace("(insn 13 30 0", "(insn:QI 13 30 0")
        passes, _ = ptrace.load(DUMPS[:3] + [("greg", greg)], "f")
        self.assertEqual(ptrace.follow(passes, 13)[-1][3], "renamed")

    def test_a_deletion_first_seen_in_sched2_is_reload_cse_regs(self):
        sched2 = GREG[:GREG.index("(insn 13 ")] + GREG[GREG.index(";; Register dispositions"):]
        passes, _ = ptrace.load(DUMPS + [("sched2", sched2)], "f")
        out = ptrace.report_insn(passes, "f", 13)
        self.assertIn("sched2    deleted by reload_cse_regs", out)
        self.assertIn("reload_cse_regs deletes it", out)
        noop = ptrace.Entry("insn", "$v0 = $v0", set(), [], None)
        self.assertEqual(ptrace.actor("sched2", noop), "sched2")    # sched2 drops no-op moves

    def test_dispositions(self):
        self.assertEqual(ptrace.dispositions(GREG), {80: 2, 90: 3, 91: 4})


FUNC = """s32 f(s32 a, u8 *b) {
    s32 i, j;
    s16 arr[4];
    s32 unused;
    s32 taken;
    HeroInfo *h;
    HeroInfo copy;
    register s32 r;

    g(&taken);
    for (i = 0; i < 4; i++) {
        s32 inner = arr[i];
        j = inner + r;
        h = &Heroes[i];
        copy = *h;
    }
    return a + *b + j;
}
"""


class TestDecls(unittest.TestCase):
    def test_declaration_order_and_memory(self):
        ds = ptrace.decls(FUNC)
        self.assertEqual([d.name for d in ds], ["a", "b", "i", "j", "arr", "taken", "h", "copy", "r", "inner"])
        self.assertEqual([d.name for d in ds if d.memory], ["arr", "taken"])
        self.assertEqual([d.name for d in ds if d.maybe], ["copy"])

    def test_pointer_member_address_is_not_the_pointers(self):
        ds = ptrace.decls("void f(void) {\n    EXTengine *eng;\n    s32 *pos;\n    s32 *q;\n    s32 s;\n"
                          "    pos = (s32 *)&eng->unk10;\n    q = &pos[2];\n    g(&s);\n}\n")
        self.assertEqual([d.name for d in ds if d.memory], ["s"])

    def test_map_by_count(self):
        ds = ptrace.decls(FUNC)
        # copy in memory (BLKmode): 7 pseudos; copy in a pseudo: 8
        self.assertEqual(ptrace.map_vars(ds, list(range(80, 87)))["inner"], 86)
        self.assertEqual(ptrace.map_vars(ds, list(range(80, 88)))["copy"], 85)
        self.assertIsNone(ptrace.map_vars(ds, list(range(80, 83))))

    def test_decl_statements(self):
        self.assertEqual(ptrace.decl_stmt("register s32 x"), [("x", ["register", "s32"], False, False)])
        self.assertEqual(ptrace.decl_stmt("u8 *p, q[3]"), [("p", ["u8"], True, False), ("q", ["u8"], False, True)])
        self.assertEqual(ptrace.decl_stmt("void (*fp)(s32)")[0][:3], ("fp", ["void"], True))
        for stmt in ("x = y", "f(x)", "i++", "return x", "p->x = 1", "loop: x = 1"):
            self.assertIsNone(ptrace.decl_stmt(stmt), stmt)


if __name__ == "__main__":
    unittest.main()
