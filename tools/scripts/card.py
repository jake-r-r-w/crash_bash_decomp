#!/usr/bin/env python3
"""Crash Bash's memory-card save: from a card image to JSON (and icon PNGs) and back.

    tools/dk python3 tools/scripts/card.py unpack CARD [DIR]     -> DIR (default CARD's name + .card/)
    tools/dk python3 tools/scripts/card.py pack DIR CARD OUT     -> OUT: CARD with DIR's save written in

CARD is a 128 KB memory-card image (RetroArch's .srm, .mcr, .mcd: "MC", then 15 directory frames
of 0x80 bytes, then 15 blocks of 0x2000) or the save file alone (0x2000 bytes starting "SC"). The
game's file is BASCUS-94570, one block, laid out as card.c reads it (docs/ASSETS.md, The memory card):

    0x000  the PsyQ header: "SC", icon frames (2), blocks (1), the title (Shift-JIS, 64 bytes:
           "CRASH BASH" and each slot's completion), the icon's CLUT at 0x60, its frames from 0x80
    0x200  4 save slots (CardSlot, 0x194 each): 8 summary bytes (the heroes picked and out), then
           the slot's GameState (include/bash/nas.h)
    0x850  CardSaveFlags: a word (1), then 8 bytes;  0x85C  CardSaveSum: the sum of the words from
           0x200 to 0x850, which the game checks on loading;  0x860 to the end: unused

DIR holds save.bin (the save as it is), card.json (the header's title, the flags, whether the sum
is right, and each slot's summary and GameState fields, named from the C struct by
struct_layout.py) and icon_0.png, icon_1.png. pack writes the fields you change into save.bin,
works the sum out again and writes the save into a copy of CARD (or OUT is the save alone when
CARD is one). The icons and the title are for viewing.
"""
import json
import os
import struct
import sys
import unicodedata

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import struct_layout as sl  # noqa: E402

NAME = b"BASCUS-94570"
BLOCK = 0x2000
FRAME = 0x80
SLOTS, SLOT_AT, SLOT = 4, 0x200, 0x194
SUM_FROM, SUM_TO, SUM_AT = 0x200, 0x850, 0x85C
SAVE, JSON = "save.bin", "card.json"
FMT = {(1, True): "b", (1, False): "B", (2, True): "h", (2, False): "H", (4, True): "i", (4, False): "I"}


class Bad(Exception):
    pass


def find(card):
    """The save's offset in a card image (or 0 for a save file alone)."""
    if card[:2] == b"SC" and len(card) == BLOCK:
        return 0
    if card[:2] != b"MC" or len(card) != 16 * BLOCK:
        raise Bad("not a memory card image (128 KB, starting MC) or a save file (8 KB, starting SC)")
    for i in range(1, 16):
        f = card[i * FRAME:(i + 1) * FRAME]
        if struct.unpack_from("<I", f)[0] == 0x51 and f[0x0A:0x0A + len(NAME)] == NAME:
            return i * BLOCK
    raise Bad("no Crash Bash save (%s) on the card" % NAME.decode())


def checksum(save):
    return sum(struct.unpack_from("<%di" % ((SUM_TO - SUM_FROM) // 4), save, SUM_FROM)) & 0xFFFFFFFF


def state_fields():
    """[(name, offset, struct format)] of GameState's number fields, one per byte range."""
    structs, typedefs = sl.compile_layouts(["GameState"], ["bash/nas.h"])
    out, covered = [], set()
    for off, cands in sorted(sl.leaves("GameState", structs, typedefs).items()):
        for path, w, signed, kind in cands:
            span = set(range(off, off + w))
            if kind != "struct" and (w, signed if kind == "" else False) in FMT and not span & covered:
                out.append((path, off, FMT[(w, signed if kind == "" else False)]))
                covered |= span
                break
    return out


def read(save, fields):
    title = save[4:0x44].split(b"\0")[0].decode("shift_jis", "replace")
    slots = []
    for s in range(SLOTS):
        base = SLOT_AT + s * SLOT
        slots.append({"summary": list(save[base:base + 8]),
                      "state": {n: struct.unpack_from("<" + f, save, base + 8 + o)[0] for n, o, f in fields}})
    return {"title": title, "title_ascii": unicodedata.normalize("NFKC", title).strip(), "flags": [struct.unpack_from("<I", save, 0x850)[0], list(save[0x854:0x85C])],
            "sum_ok": struct.unpack_from("<I", save, SUM_AT)[0] == checksum(save), "slots": slots}


def write(save, meta, fields):
    """save (a bytearray) with meta's edited fields and its sum worked out again."""
    if len(meta["slots"]) != SLOTS:
        raise Bad(f"{JSON}: there are {SLOTS} slots")
    for s, js in enumerate(meta["slots"]):
        base = SLOT_AT + s * SLOT
        save[base:base + 8] = bytes(js["summary"])
        for n, o, f in fields:
            v = js["state"][n]
            if v != struct.unpack_from("<" + f, save, base + 8 + o)[0]:
                struct.pack_into("<" + f, save, base + 8 + o, v)
    struct.pack_into("<I", save, 0x850, meta["flags"][0])
    save[0x854:0x85C] = bytes(meta["flags"][1])
    struct.pack_into("<I", save, SUM_AT, checksum(save))
    return save


def icons(save, dest):
    from PIL import Image
    import tex
    clut = struct.unpack_from("<16H", save, 0x60)
    for k in range(save[2] & 0xF):
        px = save[0x80 + k * 0x80:0x100 + k * 0x80]
        im = Image.frombytes("P", (16, 16), tex.to_indices(px, 4))
        im.putpalette([v for c in clut for v in tex.to_rgba(c)[:3]])
        im.save(os.path.join(dest, f"icon_{k}.png"))


def unpack(src, dest):
    card = open(src, "rb").read()
    at = find(card)
    save = card[at:at + BLOCK]
    os.makedirs(dest, exist_ok=True)
    with open(os.path.join(dest, SAVE), "wb") as f:
        f.write(save)
    with open(os.path.join(dest, JSON), "w") as f:
        json.dump(read(save, state_fields()), f, indent=1)
        f.write("\n")
    icons(save, dest)


def pack(src, card_in):
    card = bytearray(open(card_in, "rb").read())
    at = find(bytes(card))
    save = bytearray(open(os.path.join(src, SAVE), "rb").read())
    write(save, json.load(open(os.path.join(src, JSON))), state_fields())
    card[at:at + BLOCK] = save
    return bytes(card)


def main():
    a = sys.argv[1:]
    if a[:1] == ["unpack"] and len(a) in (2, 3):
        unpack(a[1], a[2] if len(a) == 3 else os.path.splitext(a[1])[0] + ".card")
    elif a[:1] == ["pack"] and len(a) == 4:
        with open(a[3], "wb") as f:
            f.write(pack(a[1], a[2]))
    else:
        sys.exit(__doc__.split("\n\n")[1])


if __name__ == "__main__":
    main()
