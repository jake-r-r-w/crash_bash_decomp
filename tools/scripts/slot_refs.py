"""Which slot overlay's symbol each engine data word that points into the slot range names.

    tools/dk python3 tools/scripts/slot_refs.py [--report]

The slot overlays share one address range, so a word the engine's asm data holds there can be any of
them. Across all fourteen at once, the rules shift.py proved one overlay at a time (each shift test
passed with them): in the engine's per-mode table only a file_group_* is an overlay's; a word at an
address exactly one overlay names is that overlay's; a slot of a character's sequence table goes
with the game whose sequences fill the same id in the other characters' tables; and a word goes with
an overlay a neighbouring word already belongs to. The slot's first word (the loaded overlay's id)
is no overlay's own. data2c.py writes each word by the name found here; --report lists the words
left with no overlay or with several.
"""
import argparse
import glob
import os
import re
import struct
import sys

sys.path.insert(0, os.path.dirname(__file__))
import shift  # noqa: E402
import slot_syms  # noqa: E402



def overlay_symbols(elfs):
    """address -> [(overlay, name)] of the slot overlays' functions and data."""
    out = {}
    for e in elfs:
        ovl = os.path.basename(e).split(".")[0]
        for n, a in slot_syms.defined(e).items():
            if not re.match(re.escape(ovl) + r"_[A-Z]", n) and shift.SLOT_LO <= a < shift.SLOT_HI:
                out.setdefault(a, []).append((ovl, n))
    return out


def attribute(words, syms, tables=()):
    """words: [(engine address, value)]. Returns ({address: (overlay, name)}, left), left being
    [(address, value, candidates)] the rules could not settle."""
    done, maybe = {}, {}
    for a, v in words:
        if v == shift.SLOT_LO:   # the slot's id word: whichever overlay is loaded
            continue
        c = syms.get(v, [])
        if shift.MODE_TABLE <= a < shift.MODE_TABLE + 4 * shift.MODES:
            c = [x for x in c if x[1].startswith("file_group_")]
        if len(c) == 1:
            done[a] = c[0]
        else:
            maybe[a] = (v, c)

    def column(a):
        for t in tables:
            if t <= a < t + 4 * shift.SEQ_IDS:
                return (a - t) // 4
        return None
    while True:
        took = {}
        cols = {}
        for a, (o, _) in done.items():
            k = column(a)
            if k is not None:
                cols.setdefault(k, set()).add(o)
        for a, (v, c) in maybe.items():
            pick = None
            k = column(a)
            if k is not None and len(cols.get(k, ())) == 1:
                o = next(iter(cols[k]))
                pick = next((x for x in c if x[0] == o), None)
            if pick is None:
                near = {done[b][0] for b in (a - 4, a + 4) if b in done}
                hits = [x for x in c if x[0] in near]
                if len(hits) == 1:
                    pick = hits[0]
            if pick:
                took[a] = pick
        if not took:
            break
        done.update(took)
        for a in took:
            del maybe[a]
    return done, [(a, v, c) for a, (v, c) in sorted(maybe.items())]


def engine_words(data):
    out = []
    for lo, hi, delta in shift.data_ranges("engine"):
        for off in range(lo, hi - 3, 4):
            v = struct.unpack_from("<I", data, off)[0]
            if shift.SLOT_LO <= v < shift.SLOT_HI:
                out.append((off + delta, v))
    return out


def resolve():
    """{engine address: name} for the engine data's slot words, and the words left."""
    data = open("disc/engine.bin", "rb").read()
    elfs = sorted(e for e in glob.glob("build/*.ovl.elf") if not e.endswith("engine.ovl.elf"))
    done, left = attribute(engine_words(data), overlay_symbols(elfs), shift.hero_seq_tables(data))
    return {a: n for a, (o, n) in done.items()}, left


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--report", action="store_true")
    a = ap.parse_args()
    names, left = resolve()
    print(f"slot_refs: {len(names)} words named, {len(left)} left")
    if a.report:
        for addr, v, c in left:
            print(f"  {addr:08X} {v:08X} " + (", ".join(f"{o}:{n}" for o, n in c) if c else "no overlay names it"))


if __name__ == "__main__":
    main()
