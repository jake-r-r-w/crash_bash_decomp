"""Tests for decomp.py orders (orders.py): statement dependencies and random orders that keep
them. Run: tools/dk make test-tools"""
import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))
import orders  # noqa: E402

LOCAL = {"rx", "lx", "dx", "c", "ang", "mv", "i", "p", "id", "arg0"}


class TestEffects(unittest.TestCase):
    def test_assignment_to_a_variable(self):
        wr, rd, mw, mr, call = orders.effects("lx = rx + dx;", LOCAL)
        self.assertEqual((wr, rd, mw, mr, call), ({"lx"}, {"rx", "dx"}, False, False, False))

    def test_compound_assignment_reads_its_target(self):
        self.assertEqual(orders.effects("lx += dx;", LOCAL)[:2], ({"lx"}, {"lx", "dx"}))

    def test_hex_constants_and_products_are_not_memory(self):
        wr, rd, mw, mr, _ = orders.effects("dx = c * 11 >> 7 & 0xFFF;", LOCAL)
        self.assertEqual((rd, mw, mr), ({"c"}, False, False))

    def test_loads_and_globals_read_memory(self):
        self.assertTrue(orders.effects("ang = mv->forward_angle;", LOCAL)[3])
        self.assertTrue(orders.effects("c = D_80068BD4[ang].unk2;", LOCAL)[3])
        self.assertTrue(orders.effects("dx = *p;", LOCAL)[3])

    def test_stores_and_global_writes_write_memory(self):
        self.assertTrue(orders.effects("p->x = 1;", LOCAL)[2])
        self.assertTrue(orders.effects("p->x += 1;", LOCAL)[2])
        self.assertTrue(orders.effects("D_800BC444 = 0;", LOCAL)[2])
        self.assertEqual(orders.effects("i++;", LOCAL)[:2], ({"i"}, {"i"}))

    def test_calls(self):
        self.assertTrue(orders.effects("i = Random(4);", LOCAL)[4])


class TestOrders(unittest.TestCase):
    STMTS = orders.statements("""dx = c * 15 >> 9;
        ang = mv->forward_angle;
        lx = rx + dx;
        rx = rx - dx;
        p->x = lx;
        id = (arg0 << 5) | 0x80;""")

    def test_dependencies(self):
        dep = orders.edges(self.STMTS, LOCAL)
        self.assertEqual(dep[2], {0})               # lx = rx + dx needs dx
        self.assertEqual(dep[3], {0, 2})            # rx = rx - dx: dx, and lx read the old rx
        self.assertEqual(dep[4], {1, 2})            # the store after lx, and after the load
        self.assertEqual(dep[5], set())

    def test_orders_keep_dependencies(self):
        dep = orders.edges(self.STMTS, LOCAL)
        found = orders.orders(self.STMTS, LOCAL, 30, seed=1)
        self.assertEqual(found[0], list(range(6)))
        self.assertEqual(len({tuple(o) for o in found}), len(found))
        for o in found:
            pos = {j: k for k, j in enumerate(o)}
            self.assertTrue(all(pos[i] < pos[j] for j in dep for i in dep[j]))

    def test_free_memory_lets_stores_to_distinct_places_swap(self):
        loc = LOCAL | {"e", "nas", "x"}
        stmts = orders.statements("""D_800C5BD8 = -1;
            e->unk10.x = -0x100;
            e->unk10.z = 0x900;
            D_800C5BE0 = 0;
            nas->UserInfo.user1 = 0xA0;
            x = e->unk10.x;
            GotoLinkAnim(anim, 0);""")
        dep = orders.edges(stmts, loc, free=True)
        self.assertEqual([dep[j] for j in range(5)], [set()] * 5)
        self.assertEqual(dep[5], {1})               # the load stays after the store to its place
        self.assertEqual(dep[6], {0, 1, 2, 3, 4, 5})    # a call keeps its place
        self.assertEqual(orders.edges(stmts, loc)[2], {0, 1})   # by default all memory meets

    def test_a_comment_stays_with_its_statement(self):
        stmts = orders.statements("x = 1; /* a; {b} */\n    D_1 = 0;\n    /* c */ y = 2;")
        self.assertEqual(stmts, ["x = 1; /* a; {b} */", "D_1 = 0;", "/* c */ y = 2;"])
        self.assertEqual(orders.places(stmts[2], {"y"}), (set(), set()))
        self.assertEqual(orders.effects(stmts[2], {"y"})[0], {"y"})

    def test_blocks_and_control_flow_are_refused(self):
        for text in ("if (x) { y = 1; }", "while (i < 4) i++;", "x = 1; loop: y = 2;"):
            with self.assertRaises(ValueError):
                orders.statements(text)


class TestPlaces(unittest.TestCase):
    L = LOCAL | {"e", "nas", "cx", "g", "pos", "x"}
    UNK10_X = ("e", (("f", "unk10"), ("f", "x")))

    def test_stores_place_their_lvalue(self):
        self.assertEqual(orders.places("e->unk10.x = -0x100;", self.L), ({self.UNK10_X}, set()))
        self.assertEqual(orders.places("D_800C5BE0 = 0;", self.L), ({("D_800C5BE0", ())}, set()))
        self.assertEqual(orders.places("lx = rx;", self.L), (set(), set()))
        self.assertEqual(orders.places("*p = 1;", self.L)[0], {orders.STAR})
        self.assertEqual(orders.places("((s32 *)p)[2] = 1;", self.L)[0], {orders.STAR})

    def test_reads(self):
        self.assertEqual(orders.places("c = D_80068BD4[ang].unk2;", self.L)[1],
                         {("D_80068BD4", (("i", "ang"), ("f", "unk2")))})
        self.assertEqual(orders.places("x = *p;", self.L)[1], {orders.STAR})
        self.assertEqual(orders.places("pos = &e->unk10;", self.L), (set(), set()))
        self.assertEqual(orders.places("x = *(s32 *)(g + 0x30);", self.L)[1], {orders.STAR})
        self.assertEqual(orders.places("x = (u32)D_800C5BF0 + (D_1);", self.L)[1],
                         {("D_800C5BF0", ()), ("D_1", ())})

    def test_index_expressions_are_read(self):
        self.assertEqual(orders.places("D_800C53DC[cx][D_800C5BB8].unkA = 5;", self.L)[1],
                         {("D_800C5BB8", ())})

    def test_steps_and_compound_stores_read_their_place(self):
        p = ("nas", (("f", "UserInfo"), ("f", "user1")))
        self.assertEqual(orders.places("nas->UserInfo.user1--;", self.L), ({p}, {p}))
        self.assertEqual(orders.places("D_800C5BF0 += 2;", self.L),
                         ({("D_800C5BF0", ())}, {("D_800C5BF0", ())}))
        self.assertEqual(orders.places("x = i++;", self.L)[0], {orders.STAR})

    def test_overlap(self):
        a = ("a", (("i", "i"), ("f", "x")))
        self.assertTrue(orders.overlap(a, ("a", (("i", "j"), ("f", "x")))))    # i may be j
        self.assertFalse(orders.overlap(a, ("a", (("i", "i"), ("f", "y")))))
        self.assertFalse(orders.overlap(("p", (("f", "x"),)), ("q", (("f", "x"),))))
        self.assertTrue(orders.overlap(("e", (("f", "unk10"),)), self.UNK10_X))  # a prefix
        self.assertTrue(orders.overlap(orders.STAR, ("D_1", ())))


if __name__ == "__main__":
    unittest.main()
