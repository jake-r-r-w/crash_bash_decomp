"""Each exe model slot the games' file lists load into, with the archive files ({mdl, tex}) each game
puts there (names from configs/USA/dat_names.txt), most used first: the evidence for naming a slot.

    tools/dk python3 tools/scripts/model_slots.py
"""
import re
import glob

names = {}
for l in open("configs/USA/dat_names.txt", encoding="utf-8"):
    l = l.strip()
    if l and not l.startswith("#"):
        m = re.match(r"(\d+)\s*=\s*(.*)", l)
        if m:
            names[int(m.group(1))] = m.group(2).strip()


def ent(sym):
    m = re.match(r"D_(8004[DEF][0-9A-F]{3})$", sym)
    if not m:
        return None
    a = int(m.group(1), 16)
    return (a - 0x8004E110) // 8 if a >= 0x8004E110 else None


slots = {}
for p in sorted(glob.glob("src/overlays/*/*.c")):
    ovl = p.split("/")[2]
    t = open(p, encoding="utf-8").read()
    for m in re.finditer(r"^s32 (\w+)\[\d+\] = \{(.*?)\};", t, re.M | re.S):
        words = [w.strip() for w in m.group(2).split(",") if w.strip()]
        syms = [re.sub(r"^\(s32\)&", "", w) for w in words]
        if len(syms) < 3 or not any(ent(s) is not None for s in syms[1:3]):
            continue
        for i in range(0, len(syms) - 2, 3):
            s, a, b = syms[i:i + 3]
            if s == "0":
                break
            ea, eb = ent(a), ent(b)
            if ea is None or eb is None:
                continue
            slots.setdefault(s, []).append(f"{ovl}:{names.get(ea, ea)}|{names.get(eb, eb)}")
for s in sorted(slots, key=lambda s: -len(slots[s])):
    print(s, len(slots[s]))
    for v in sorted(set(slots[s]))[:14]:
        print("   ", v)
