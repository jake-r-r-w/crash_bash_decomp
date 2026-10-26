"""The shiftable NON_MATCHING build: one slot overlay padded so every address in it moves.

    tools/dk make shift OVL=tank PAD=0x40     (after the matching build)

From the matching build's objects (nothing is recompiled), into build/shift/:
1. <ovl>.ovl.elf / <ovl>.bin: the overlay linked again with PAD bytes before its C rodata, its
   text and its data (its id word stays first, at the slot's start). Every pointer into it is
   a symbol, so the link moves them all.
2. engine_slot.bin: the engine's slot image as its id word (0x5C) and zeros, not the build
   machine's leftovers (ARCHITECTURE.md, The engine's slot image).
3. engine.ovl.elf / engine.bin: the engine linked with that image and with the slot symbols
   its AST table names taken from the moved overlay (slot_syms.py).
4. A stopgap until the engine's data is C (Phase 5): words of the engine's asm data that point
   at a place only this overlay has (a function, object or string no other overlay has at that
   address) are rewritten from the old link's address to the new one, by name. Each is listed.
5. CrashBash.bin/.cue: pack_disc.py --grow with the two binaries; the overlay, now bigger than
   its retail size, stays in its DAT place, which the DAT is laid out again around if it must
   (the exe's index and file group sizes follow).
Then the shift test: play the overlay's game on it (docs/PLAYING.md).
"""
import argparse
import glob
import os
import re
import subprocess
import sys

import yaml

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import slot_syms  # noqa: E402

OUT = "build/shift"
CFG = "configs/USA"
SLOT_ID = 0x5C


def run(cmd):
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode:
        sys.exit(f"shift: {' '.join(cmd[:3])} ... failed:\n{r.stdout}{r.stderr}")
    return r.stdout


def link(name, script, extra=()):
    elf = f"{OUT}/{name}.ovl.elf"
    run(["mipsel-linux-gnu-ld", "-nostdlib", "--no-check-sections", "-Map", f"{OUT}/{name}.map",
         # the Makefile's PROVIDE copy: an overlay's own halfword labels that splat lists there
         # must follow the C definitions, or they stay at their retail address
         "-T", script, "-T", f"build/{name}_undefined_syms.ld",
         "-T", f"linkers/{name}_undefined_funcs_auto.txt", "-T", f"{CFG}/undefined_syms_manual.txt",
         *extra, "-o", elf])
    run(["mipsel-linux-gnu-objcopy", "-O", "binary", elf, f"{OUT}/{name}.bin"])
    return elf


SECTIONS = (".rodata", ".text", ".data")


def pad_script(t, ovl, pad, sections=SECTIONS):
    """A linker script with pad bytes before the overlay's first C .rodata, .text and .data input
    (not its id unit, <ovl>_id, which has to stay the slot's first word); `sections` pads only
    those (to bisect a crash: which section's move breaks the game)."""
    n = 0
    for sect in sections:
        m = re.search(r"^(\s*)(build/src/overlays/" + ovl + r"/(?!" + ovl + r"_id\.)\w+\.c\.o\("
                      + re.escape(sect) + r"\);)", t, re.M)
        if m:
            t = t[:m.start()] + f"{m.group(1)}. += 0x{pad:X};   /* shift.py */\n" + t[m.start():]
            n += 1
    if n != len(sections):
        raise ValueError(f"found {n} of {ovl}'s {'/'.join(sections)} inputs ({len(sections)} needed)")
    return t


def padded_script(ovl, pad, sections=SECTIONS):
    """The overlay's linker script with pad bytes before its C rodata, text and data."""
    try:
        t = pad_script(open(f"linkers/{ovl}.ld", encoding="utf-8").read(), ovl, pad, sections)
    except ValueError as e:
        sys.exit(f"shift: linkers/{ovl}.ld: {e}")
    path = f"{OUT}/{ovl}.ld"
    open(path, "w", encoding="utf-8").write(t)
    return path


def slot_image():
    """engine_slot.bin as the id word and zeros, and the engine's linker script using it."""
    src = "asm/bin/overlays/engine/engine_slot.bin"
    size = os.path.getsize(src)
    path = f"{OUT}/engine_slot.bin"
    open(path, "wb").write(SLOT_ID.to_bytes(4, "little") + bytes(size - 4))
    run(["mipsel-linux-gnu-ld", "-r", "-b", "binary", "-o", f"{OUT}/engine_slot.bin.o", path])
    t = open("linkers/engine.ld", encoding="utf-8").read()
    old = f"build/{src}.o"
    if t.count(old) != 1:
        sys.exit(f"shift: {old} not found once in linkers/engine.ld")
    script = f"{OUT}/engine.ld"
    open(script, "w", encoding="utf-8").write(t.replace(old, f"{OUT}/engine_slot.bin.o"))
    return script


def data_ranges(name):
    """[(rom_lo, rom_hi, vram)] of a binary's asm data subsegments."""
    y = yaml.safe_load(open(f"{CFG}/overlays/{name}.yaml", encoding="utf-8"))
    seg = next(s for s in y["segments"] if isinstance(s, dict) and s.get("name") == name)
    subs = [s for s in seg["subsegments"] if isinstance(s, list)]
    out = []
    for i, s in enumerate(subs):
        if str(s[1]) in ("data", "sdata") and i + 1 < len(subs):
            out.append((s[0], subs[i + 1][0], seg["vram"] - seg["start"]))
    return out


SLOT_LO, SLOT_HI = 0x800B32B4, 0x800D7148


def section_words(dump):
    """The bytes of one section from `objdump -s -j SECT` output."""
    data = bytearray()
    for l in dump.splitlines():
        m = re.match(r" ([0-9a-f]{4,8}) ((?:[0-9a-f]{2,8} ){1,4})", l)
        if m:
            data += bytes.fromhex(m.group(2).replace(" ", ""))
    return data


def reloc_offsets(dump):
    """{section: {offset}} from `objdump -r` output."""
    out, cur = {}, None
    for l in dump.splitlines():
        m = re.match(r"RELOCATION RECORDS FOR \[(\S+)\]", l)
        if m:
            cur = m.group(1)
            continue
        m = re.match(r"([0-9a-f]{8}) \S+\s+\S+", l)
        if m and cur:
            out.setdefault(cur, set()).add(int(m.group(1), 16))
    return out


def unmoved(data, relocs):
    """Offsets of the words that hold a slot address as a plain number: a pointer that would not
    move (one written as bytes or halves, as the front end's CharCards were)."""
    return [o for o in range(0, len(data) - 3, 4)
            if SLOT_LO <= int.from_bytes(data[o:o + 4], "little") < SLOT_HI and o not in relocs]


def check_pointers(ovl):
    """Warn about the overlay's C data words that hold its addresses but have no relocation."""
    n = 0
    for obj in sorted(glob.glob(f"build/src/overlays/{ovl}/*.c.o")):
        rel = reloc_offsets(run(["mipsel-linux-gnu-objdump", "-r", obj]))
        for sect in (".data", ".rodata"):
            data = section_words(run(["mipsel-linux-gnu-objdump", "-s", "-j", sect, obj]))
            for o in unmoved(data, rel.get(sect, set())):
                v = int.from_bytes(data[o:o + 4], "little")
                print(f"shift: warning: {obj} {sect}+0x{o:X} holds 0x{v:08X} with no relocation: it won't move")
                n += 1
    return n


def take_neighbours(done, maybe):
    """Move the words of maybe (an address another overlay also has) that sit beside a word of
    done into done, and return the rest. An address is the overlay's when the words beside it
    are (TankHeroMove in Tank's hero mode table, which is also a data label of bonk), and a word
    taken that way vouches for the next one too (Bash's file groups in the engine's per-mode
    table: Space's is the overlay's, so Snow's beside it is, and so Drains' beside that).
    Entries are (offset, ...)."""
    fixed = {m[0] for m in done}
    left = list(maybe)
    while True:
        took = [m for m in left if m[0] - 4 in fixed or m[0] + 4 in fixed]
        if not took:
            return left
        done += took
        fixed |= {m[0] for m in took}
        left = [m for m in left if m not in took]


def fixup(ovl, old_elf, new_elf, others):
    """Rewrite the engine's asm-data words that point at a place only ovl has (stopgap)."""
    old = slot_syms.defined(old_elf)
    new = slot_syms.defined(new_elf)
    taken = set()
    for e in others:
        taken |= set(slot_syms.defined(e).values())
    by_addr, shared = {}, {}
    for n, a in old.items():
        if n in new:
            (shared if a in taken else by_addr).setdefault(a, n)
    path = f"{OUT}/engine.bin"
    data = bytearray(open(path, "rb").read())
    done, maybe = [], []
    for lo, hi, delta in data_ranges("engine"):
        for off in range(lo, hi - 3, 4):
            v = int.from_bytes(data[off:off + 4], "little")
            n = by_addr.get(v)
            if n:
                done.append((off, delta, n, v))
            elif v in shared:
                maybe.append((off, delta, shared[v], v))
    left = take_neighbours(done, maybe)
    for off, delta, n, v in done:
        data[off:off + 4] = new[n].to_bytes(4, "little")
    open(path, "wb").write(data)
    for off, delta, n, v in sorted(done):
        print(f"shift: engine data {off + delta:08X}: {n} {v:08X} -> {new[n]:08X}")
    for off, delta, n, v in left:
        print(f"shift: engine data {off + delta:08X}: {v:08X} is {n} here and a place in another overlay: left")
    return done


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("ovl")
    ap.add_argument("--pad", default="0x40", help="bytes before each of the rodata, text and data (multiple of 8)")
    ap.add_argument("--only", default=",".join(SECTIONS),
                    help="pad only these sections (comma-separated, e.g. .data), to bisect a crash")
    a = ap.parse_args()
    sections = tuple(s.strip() for s in a.only.split(",") if s.strip())
    if not sections or any(s not in SECTIONS for s in sections):
        sys.exit(f"shift: --only takes {', '.join(SECTIONS)}")
    pad = int(a.pad, 0)
    if pad % 8:
        sys.exit("shift: --pad must keep 8-byte alignment (jump tables)")
    os.makedirs(OUT, exist_ok=True)
    ovls = sorted(os.path.splitext(os.path.basename(p))[0] for p in os.listdir(f"{CFG}/overlays"))
    slots = [o for o in ovls if o != "engine"]
    check_pointers(a.ovl)
    new_elf = link(a.ovl, padded_script(a.ovl, pad, sections))
    # a name that links as an absolute address inside the overlay's own range stays put when the
    # overlay moves (a linker-script assignment overriding the C definition): refuse it
    fixed_names = [l.split()[2] for l in run(["mipsel-linux-gnu-nm", new_elf]).splitlines()
                   if len(l.split()) == 3 and l.split()[1] == "A"
                   and SLOT_LO <= int(l.split()[0], 16) < SLOT_LO + os.path.getsize(f"build/{a.ovl}.bin")]
    if fixed_names:
        sys.exit("shift: these link at fixed addresses inside the overlay and would not move: "
                 + ", ".join(sorted(fixed_names)))
    old_elf = f"build/{a.ovl}.ovl.elf"
    sym = f"{OUT}/engine_slot_syms.ld"
    elfs = [new_elf if o == a.ovl else f"build/{o}.ovl.elf" for o in slots]
    names = slot_syms.undefined(sorted(glob.glob("build/src/overlays/engine/*.c.o")))
    syms = slot_syms.resolve(names, elfs)
    with open(sym, "w", encoding="utf-8", newline="\n") as f:
        for n, v in syms:
            f.write(f"{n} = 0x{v:08X};\n")
    link("engine", slot_image(), ["-T", sym])
    fixed = fixup(a.ovl, old_elf, new_elf, [f"build/{o}.ovl.elf" for o in slots if o != a.ovl])
    moved = sum(1 for n, v in syms if v != dict(slot_syms.resolve(names, [old_elf] + [e for e in elfs if e != new_elf])).get(n))
    old_size, new_size = os.path.getsize(f"build/{a.ovl}.bin"), os.path.getsize(f"{OUT}/{a.ovl}.bin")
    print(f"shift: {a.ovl} {old_size} -> {new_size} bytes (pad 0x{pad:X} x{len(sections)}); {moved} AST entries moved; "
          f"{len(fixed)} engine data words rewritten")
    r = subprocess.run([sys.executable, "tools/scripts/pack_disc.py", "--out", OUT, "--grow",
                        "--use", f"{a.ovl}={OUT}/{a.ovl}.bin", "--use", f"engine={OUT}/engine.bin"])
    if r.returncode:
        sys.exit(r.returncode)


if __name__ == "__main__":
    main()
