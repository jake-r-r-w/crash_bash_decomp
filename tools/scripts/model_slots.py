"""Each exe model slot the games' file lists load into, with the archive files ({mdl, tex}) each game
puts there (names from configs/USA/dat_names.txt), most used first: the evidence for naming a slot.

    tools/dk python3 tools/scripts/model_slots.py
"""
import glob
import re

DAT_INDEX = 0x8004E110   # the exe's {sector, size} entries, 8 bytes each


def dat_names(path="configs/USA/dat_names.txt"):
    names = {}
    for l in open(path, encoding="utf-8"):
        m = re.match(r"(\d+)\s*=\s*(.*)", l.strip())
        if m:
            names[int(m.group(1))] = m.group(2).strip()
    return names


def entry(sym):
    """The archive entry a D_8004xxxx index symbol names, or None."""
    m = re.match(r"D_(8004[DEF][0-9A-F]{3})$", sym)
    if not m or int(m.group(1), 16) < DAT_INDEX:
        return None
    return (int(m.group(1), 16) - DAT_INDEX) // 8


def triples(body):
    """(slot, mdl entry, tex entry) from a file list's initialiser body, up to its zeros."""
    syms = [re.sub(r"^\(s32\)&", "", w.strip()) for w in body.split(",") if w.strip()]
    out = []
    for i in range(0, len(syms) - 2, 3):
        s, a, b = syms[i:i + 3]
        if s == "0":
            break
        if entry(a) is not None and entry(b) is not None:
            out.append((s, entry(a), entry(b)))
    return out


def main():
    names = dat_names()
    slots = {}
    for p in sorted(glob.glob("src/overlays/*/*.c")):
        ovl = p.replace("\\", "/").split("/")[2]
        for m in re.finditer(r"^s32 \w+\[\d+\] = \{(.*?)\};", open(p, encoding="utf-8").read(), re.M | re.S):
            for s, a, b in triples(m.group(1)):
                slots.setdefault(s, []).append(f"{ovl}:{names.get(a, a)}|{names.get(b, b)}")
    for s in sorted(slots, key=lambda s: -len(slots[s])):
        print(s, len(slots[s]))
        for v in sorted(set(slots[s])):
            print("   ", v)


if __name__ == "__main__":
    main()
