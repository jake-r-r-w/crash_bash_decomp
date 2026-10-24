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

    def test_blocks_and_control_flow_are_refused(self):
        for text in ("if (x) { y = 1; }", "while (i < 4) i++;", "x = 1; loop: y = 2;"):
            with self.assertRaises(ValueError):
                orders.statements(text)


if __name__ == "__main__":
    unittest.main()
