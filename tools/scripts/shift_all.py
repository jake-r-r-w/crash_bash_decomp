#!/usr/bin/env python3
"""The fully shifted build: main.exe, the engine and every slot overlay moved at once.

    tools/dk make shift-all [PAD=0x40]       (after the matching build)

From the matching build's objects (nothing is recompiled), into build/shiftall/:
1. Every binary is linked again with PAD bytes before its first C .rodata, .text and .data input
   (the exe after its header, the engine after its entry table, a slot overlay after its id
   word), so every address after those moves.
2. Where a binary sits follows the one before it: the engine loads at the exe's end (HeapStart,
   which the retail link fixes at 0x80078C90), the slot at the engine's end, the slot heap past
   it (SlotHeapStart), and the exe's PsyQ .bss sections, pinned at their retail addresses, move
   with the exe.
3. splat's lists link names at their retail addresses; here every such line is a PROVIDE (a C
   definition wins) and a name another binary defines takes its address in that binary's new
   link. The links are repeated until no address changes.
4. Then the checks: a name a binary uses that still links by its retail address inside a range
   that moved (a pointer that would not move), and data words holding such an address with no
   relocation.
5. The mode text files hold absolute pointers into the engine's ModeTextBuffer: they are moved
   with it. Then pack_disc.py packs build/shiftall/CrashBash.bin with the moved exe, engine and
   overlays (the DAT laid out again around the bigger ones, the exe's index found by name).
"""
import argparse
import os
import re
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import shift  # noqa: E402
import slot_syms  # noqa: E402

OUT = "build/shiftall"
CFG = "configs/USA"
LD, NM, OBJCOPY = "mipsel-linux-gnu-ld", "mipsel-linux-gnu-nm", "mipsel-linux-gnu-objcopy"
EXE_BASE, ENGINE_BASE, SLOT_BASE = 0x80010000, 0x80078C90, 0x800B32B4
SLOT_HEAP = 0x800D7144
EXE_FILE_SIZE = 0x69800
MODE_TEXT = 0x800AEC60          # ModeTextBuffer: where the engine loads a mode's text file
SECTIONS = (".rodata", ".text", ".data")
ASSIGN = re.compile(r"^(\s*)(?:PROVIDE\s*\(\s*)?([A-Za-z_][\w.]*)\s*=\s*(0x[0-9A-Fa-f]+)\s*\)?\s*;(.*)$", re.M)


def run(cmd):
    r = subprocess.run(cmd, capture_output=True, text=True)
    if r.returncode:
        sys.exit(f"shift_all: {' '.join(cmd[:3])} ... failed:\n{r.stdout[-3000:]}{r.stderr[-3000:]}")
    return r.stdout


def provide(text, override):
    """A linker script whose `name = 0xADDR;` lines are PROVIDEs (a definition in the objects wins),
    except names in override, which are set to their new address."""
    def line(m):
        ind, name, val, rest = m.groups()
        if name in override:
            return f"{ind}{name} = 0x{override[name]:08X};{rest}"
        if name in (".", "_gp") or name.startswith("__"):
            return m.group(0)
        return f"{ind}PROVIDE({name} = {val});{rest}"
    return ASSIGN.sub(line, text)


def pad_first(t, pattern, pad, sections=SECTIONS):
    """pad bytes before the first input line matching pattern (a regex for the object) for each
    section."""
    for sect in sections:
        m = re.search(r"^(\s*)(" + pattern + r"\(" + re.escape(sect) + r"\);)", t, re.M)
        if not m:
            raise ValueError(f"no {pattern}({sect}) input")
        t = t[:m.start()] + f"{m.group(1)}. += 0x{pad:X};   /* shift_all.py */\n" + t[m.start():]
    return t


def defined(elf):
    """{name: address} of what an ELF's objects define (not linker-script absolutes)."""
    out = {}
    for l in run([NM, elf]).splitlines():
        p = l.split()
        if len(p) == 3 and p[1] not in "AaUwWv":
            out[p[2]] = int(p[0], 16)
    return out


def absolutes(elf):
    """{name: address} of the linker-script (absolute) names an ELF ends up with."""
    return {l.split()[2]: int(l.split()[0], 16) for l in run([NM, elf]).splitlines()
            if len(l.split()) == 3 and l.split()[1] in "Aa"}


def referenced(objs):
    """Names the objects use but don't define."""
    names = set()
    for o in objs:
        for l in run([NM, "-u", o]).splitlines():
            names.add(l.split()[-1])
    return names


def retail_names(paths):
    """{name: retail address} of every `name = ADDR;` line (PROVIDE or not) in the scripts."""
    out = {}
    for p in paths:
        if os.path.exists(p):
            for m in ASSIGN.finditer(open(p, encoding="utf-8").read()):
                if m.group(2) not in (".", "_gp") and not m.group(2).startswith("__"):
                    out[m.group(2)] = int(m.group(3), 16)
    return out


def lists(paths, override):
    """The concatenated name lists, PROVIDE'd with overrides, as one script."""
    return "\n".join(provide(open(p, encoding="utf-8").read(), override) for p in paths if os.path.exists(p))


def write(path, text):
    with open(path, "w", encoding="utf-8", newline="\n") as f:
        f.write(text)
    return path


def link(name, script, override, list_paths, extra=None):
    """Link one binary from its script and lists (overridden), plus extra {name: address}
    assignments no list holds; returns its ELF."""
    elf = f"{OUT}/{name}.elf"
    text = lists(list_paths, override)
    if extra:
        text += "\n" + "".join(f"{n} = 0x{v:08X};\n" for n, v in sorted(extra.items()))
    syms = write(f"{OUT}/{name}_syms.ld", text)
    run([LD, "-nostdlib", "--no-check-sections", "-Map", f"{OUT}/{name}.map", "-T", script, "-T", syms, "-o", elf])
    return elf


# Sony's startup object, 2MBYTE.OBJ (__main, __SN_ENTRY_POINT): splat keeps it as words
# (asm/main/psyq/2mbyte.s; it isn't in the converted libraries), so its addresses are numbers
# that don't move. The same instructions, naming what they use: the bytes of the retail link, and
# relocations that follow a moved exe. It clears the .bss (StartupRa up to HeapStart), sets $sp to
# the top of RAM (the table's 2 MB) less nothing, gives InitHeap the heap past the stack, and calls
# machine_init with $gp at ExportsInit.
STARTUP = """.set noat
.set noreorder
.section .text, "ax"
.globl __main
__main:
    jr      $ra
    nop
.globl __SN_ENTRY_POINT
__SN_ENTRY_POINT:
    lui     $v0, %hi(StartupRa)
    addiu   $v0, $v0, %lo(StartupRa)
    lui     $v1, %hi(HeapStart)
    addiu   $v1, $v1, %lo(HeapStart)
1:  sw      $zero, 0($v0)
    addiu   $v0, $v0, 4
    sltu    $at, $v0, $v1
    bnez    $at, 1b
    nop
    addiu   $v0, $zero, 4
    nop
    nop
    nop
    nop
    lui     $a0, %hi(2f)
    addiu   $a0, $a0, %lo(2f)
    addu    $a0, $a0, $v0
    lw      $v0, 0($a0)
    lui     $t0, 0x8000
    or      $sp, $v0, $t0
    lui     $a0, %hi(HeapStart)
    addiu   $a0, $a0, %lo(HeapStart)
    sll     $a0, $a0, 3
    srl     $a0, $a0, 3
    lui     $v1, %hi(_stacksize)
    lw      $v1, %lo(_stacksize)($v1)
    nop
    subu    $a1, $v0, $v1
    subu    $a1, $a1, $a0
    or      $a0, $a0, $t0
    lui     $at, %hi(StartupRa)
    sw      $ra, %lo(StartupRa)($at)
    lui     $gp, %hi(ExportsInit)
    addiu   $gp, $gp, %lo(ExportsInit)
    addu    $fp, $sp, $zero
    jal     InitHeap
    addi    $a0, $a0, 4
    lui     $ra, %hi(StartupRa)
    lw      $ra, %lo(StartupRa)($ra)
    nop
    jal     machine_init
    nop
    .word   0x0000004D          /* break: the original encoding (as writes break 1 differently) */
2:  .word 0x00200000, 0x00200000, 0x00200000, 0x00200000
"""


class Build:
    def __init__(self, pad_main, pad_engine, pad_slot):
        self.pads = {"main": pad_main, "engine": pad_engine}
        self.pad_slot = pad_slot
        self.engine_base = ENGINE_BASE + 3 * pad_main               # the exe's end moved by its growth
        self.slot_base = SLOT_BASE + 3 * pad_main + 3 * pad_engine  # the engine's end, both moved
        self.slot_heap = SLOT_HEAP + self.slot_base - SLOT_BASE + 3 * pad_slot
        self.slots = sorted(os.path.splitext(f)[0] for f in os.listdir(f"{CFG}/overlays") if f != "engine.yaml")

    def pad(self, b):
        return self.pads.get(b, self.pad_slot)

    # ---- the three kinds of binary

    def main_script(self):
        t = open("build/main.full.ld", encoding="utf-8").read()
        t = pad_first(t, r"build/src/main/\w+\.c\.o", self.pad("main"))
        t = self.move_sections(t, "main", ".main")
        startup = f"{OUT}/2mbyte.s.o"
        run(["mipsel-linux-gnu-as", "-march=r3000", "-mtune=r3000", "-no-pad-sections", "-O0", "-G0", "-EL",
             "-o", startup, write(f"{OUT}/2mbyte.s", STARTUP)])
        old = "build/asm/main/psyq/2mbyte.s.o"
        if t.count(old) < 1:
            sys.exit(f"shift_all: {old} not in the exe's link script")
        t = t.replace(old, startup)
        return write(f"{OUT}/main.ld", provide(t, getattr(self, "main_over", {"HeapStart": self.engine_base})))

    def engine_script(self):
        t = open("linkers/engine.ld", encoding="utf-8").read()
        t = t.replace(f".engine 0x{ENGINE_BASE:08X} :", f".engine 0x{self.engine_base:08X} :", 1)
        t = t.replace(f".engine_slot 0x{SLOT_BASE:08X} :", f".engine_slot 0x{self.slot_base:08X} :", 1)
        t = self.move_sections(t, "engine", ".engine", ".engine_slot")
        t = pad_first(t, r"build/src/overlays/engine/(?!entry\.)\w+\.c\.o", self.pad("engine"))
        # the slot image: its id word and zeros, not the build machine's leftovers
        src = "asm/bin/overlays/engine/engine_slot.bin"
        img = f"{OUT}/engine_slot.bin"
        open(img, "wb").write(shift.SLOT_ID.to_bytes(4, "little") + bytes(os.path.getsize(src) - 4))
        run([LD, "-r", "-b", "binary", "-o", img + ".o", img])
        t = t.replace(f"build/{src}.o", img + ".o")
        return write(f"{OUT}/engine.ld", t)

    def slot_script(self, ovl):
        t = open(f"linkers/{ovl}.ld", encoding="utf-8").read()
        t = t.replace(f".{ovl} 0x{SLOT_BASE:08X} :", f".{ovl} 0x{self.slot_base:08X} :", 1)
        t = self.move_sections(t, ovl, f".{ovl}")
        t = pad_first(t, rf"build/src/overlays/{ovl}/(?!{ovl}_id\.)\w+\.c\.o", self.pad(ovl))
        return write(f"{OUT}/{ovl}.ld", t)

    # ---- linking until the addresses settle

    PADDED = {"main": r"build/src/main/\w+\.c\.o", "engine": r"build/src/overlays/engine/(?!entry\.)\w+\.c\.o"}

    def points(self, b):
        """The retail addresses where binary b's pads go: its first padded C input of each section
        (from the matching build's link map, in link order)."""
        if not hasattr(self, "_points"):
            self._points = {}
        if b not in self._points:
            pat = self.PADDED.get(b, rf"build/src/overlays/{b}/(?!{b}_id\.)\w+\.c\.o")
            mp = open("build/main.map" if b == "main" else f"build/{b}.map", encoding="utf-8").read()
            pts = []
            for sect in SECTIONS:
                m = re.search(r"^ " + re.escape(sect) + r"\s+0x([0-9a-f]+)\s+0x([0-9a-f]+) (" + pat + r")$", mp, re.M)
                while m and (int(m.group(2), 16) == 0 or int(m.group(1), 16) == 0):
                    m = re.compile(r"^ " + re.escape(sect) + r"\s+0x([0-9a-f]+)\s+0x([0-9a-f]+) (" + pat + r")$",
                                   re.M).search(mp, m.end())
                if not m:
                    sys.exit(f"shift_all: no padded {sect} input in {b}'s map")
                pts.append(int(m.group(1), 16))
            self._points[b] = sorted(pts)
        return self._points[b]

    def moved(self, b, a):
        """Where retail address a of binary b is in the shifted build: its base's shift plus one pad
        for each pad point at or below it."""
        base = {"main": 0, "engine": self.engine_base - ENGINE_BASE}.get(b, self.slot_base - SLOT_BASE)
        return a + base + self.pad(b) * sum(p <= a for p in self.points(b))

    def move_sections(self, t, b, *skip):
        """The script with every section placed at an explicit retail address (the exe's PsyQ .bss
        and bios_1 .data, the engine's .bss image) moved by position, except the skipped ones."""
        def sect(m):
            if m.group(2) in skip:
                return m.group(0)
            return f"{m.group(1)}{m.group(2)} 0x{self.moved(b, int(m.group(3), 16)):08X}{m.group(4)}"
        return re.sub(r"^(\s*)([.A-Za-z_][\w.]*) (0x[0-9A-Fa-f]{8})(\s*(?:\(NOLOAD\))?\s*:)", sect, t, flags=re.M)

    @staticmethod
    def region(a):
        return "main" if EXE_BASE <= a < ENGINE_BASE else "engine" if ENGINE_BASE <= a < SLOT_BASE else \
            "slot" if SLOT_BASE <= a <= SLOT_HEAP else None

    def own(self, b):
        """The names binary b's own objects define (from the matching build's link): an override
        never replaces one of them."""
        if not hasattr(self, "_own"):
            self._own = {"main": set(defined("build/main.elf")), "engine": set(defined("build/engine.ovl.elf"))}
            for o in self.slots:
                self._own[o] = set(defined(f"build/{o}.ovl.elf"))
        return self._own[b]

    def overrides(self, b, list_paths, *tables):
        """The new address of each name binary b's lists give, that b doesn't define: another
        binary's definition if one has it, else its retail address moved by position (a .bss
        buffer, a label inside a data object). A slot address the exe or the engine names by
        number, outside any overlay's definitions, can't be placed: left (and reported)."""
        defs = {}
        for t in tables:
            defs.update(t)
        # the exe's end, the slot heap's start and the slot's id word (its first word)
        out = {"HeapStart": self.engine_base, "SlotHeapStart": self.slot_heap, "SlotOverlayId": self.slot_base}
        for n, v in retail_names(list_paths).items():
            if n in out or (n in self.own(b) and self.region(v) is None):
                continue
            if n in self.own(b):
                continue
            if n in defs:
                out[n] = defs[n]
                continue
            r = self.region(v)
            if r in ("main", "engine"):
                out[n] = self.moved(r, v)
            elif r == "slot" and b in self.slots:
                out[n] = self.moved(b, v)
        return out

    def lists_of(self, b):
        if b == "main":
            return ["linkers/undefined_syms_auto.txt", "linkers/undefined_funcs_auto.txt",
                    f"{CFG}/undefined_syms_manual.txt", "build/main.full.ld"]
        return [f"build/{b}_undefined_syms.ld", f"linkers/{b}_undefined_funcs_auto.txt", f"{CFG}/undefined_syms_manual.txt"]

    def link_all(self, new):
        """Link everything once with new {binary: {name: address}}; returns the new tables."""
        main_over = self.overrides("main", self.lists_of("main"), new.get("engine", {}), *(new.get(o, {}) for o in self.slots))
        self.used_over = {"main": main_over}
        # main.full.ld's own name lines (the PsyQ symbols) are rewritten in main_script's provide()
        self.main_over = main_over
        main = link("main", self.main_script(), main_over, self.lists_of("main")[:3])
        out = {"main": defined(main)}
        # the slot names the engine's C uses, from the moved overlays (as the matching build's link)
        slot_elfs = [f"{OUT}/{o}.elf" if os.path.exists(f"{OUT}/{o}.elf") else f"build/{o}.ovl.elf" for o in self.slots]
        names = slot_syms.undefined(sorted(f"build/src/overlays/engine/{f}" for f in os.listdir("build/src/overlays/engine")
                                           if f.endswith(".c.o")))
        slot_over = dict(slot_syms.resolve(names, slot_elfs))
        eng_over = self.overrides("engine", self.lists_of("engine"), out["main"], slot_over)
        self.used_over["engine"] = eng_over
        eng = link("engine", self.engine_script(), eng_over, self.lists_of("engine"), extra=slot_over)
        out["engine"] = defined(eng)
        for o in self.slots:
            over = self.overrides(o, self.lists_of(o), out["main"], out["engine"])
            self.used_over[o] = over
            elf = link(o, self.slot_script(o), over, self.lists_of(o))
            out[o] = defined(elf)
        return out

    def settle(self, passes=4):
        new = {}
        for i in range(passes):
            got = self.link_all(new)
            # the engine's and main's names are what other links take: settled when they stop changing
            if new and all(got[b] == new.get(b) for b in ("main", "engine")):
                print(f"shift_all: addresses settled after {i + 1} links")
                return got
            new = got
        sys.exit("shift_all: the addresses didn't settle")

    # ---- checks

    def stale(self):
        """Names a binary uses that link by a retail address inside a range that moved."""
        moved = [(EXE_BASE, ENGINE_BASE), (ENGINE_BASE, SLOT_BASE), (SLOT_BASE, SLOT_HEAP + 4)]
        objs = {"main": [l.strip().rsplit("(", 1)[0] for l in open("build/main.full.ld") if re.match(r"\s+build/src/main/\w+\.c\.o\(\.text\)", l)],
                "engine": [f"build/src/overlays/engine/{f}" for f in os.listdir("build/src/overlays/engine") if f.endswith(".c.o")]}
        for o in self.slots:
            objs[o] = [f"build/src/overlays/{o}/{f}" for f in os.listdir(f"build/src/overlays/{o}") if f.endswith(".c.o")]
        bad = []
        for b, os_ in objs.items():
            used = referenced(os_)
            retail = retail_names(self.lists_of(b))
            for n, v in absolutes(f"{OUT}/{b}.elf").items():
                if (n in used and n not in self.used_over.get(b, {}) and retail.get(n) == v
                        and any(lo <= v < hi for lo, hi in moved)):
                    bad.append((b, n, v))
        return bad

    # ---- outputs

    def binaries(self):
        exe = f"{OUT}/main.exe"
        run([OBJCOPY, "-O", "binary", f"{OUT}/main.elf", exe])
        size = os.path.getsize(exe)
        if size > EXE_FILE_SIZE:
            sys.exit(f"shift_all: the exe is {size:#x} bytes, more than its {EXE_FILE_SIZE:#x} on the disc")
        with open(exe, "ab") as f:
            f.write(bytes(EXE_FILE_SIZE - size))
        # the header's initial PC (asm/header.s, a constant) is the entry point, which moved
        with open(exe, "r+b") as f:
            f.seek(0x10)
            f.write(defined(f"{OUT}/main.elf")["__SN_ENTRY_POINT"].to_bytes(4, "little"))
        for b in ["engine"] + self.slots:
            run([OBJCOPY, "-O", "binary", f"{OUT}/{b}.elf", f"{OUT}/{b}.bin"])
        return exe

    def mode_texts(self):
        """The mode text files with their pointers moved with ModeTextBuffer: [(DAT id, path)]."""
        import json
        import txt
        eng = {**absolutes(f"{OUT}/engine.elf"), **defined(f"{OUT}/engine.elf")}
        delta = eng["ModeTextBuffer"] - MODE_TEXT
        out = []
        for e in json.load(open("assets/CRASHBSH/index.json"))["entries"]:
            if e.get("format") != "txt":
                continue
            d = bytearray(txt.pack(f"assets/CRASHBSH/{e['id']}.txt"))
            s = txt.strings(bytes(d))
            send = s[-1][0] + s[-1][2] if s else 4
            n = 0
            for p in range(send, len(d) - 3, 4):
                w = int.from_bytes(d[p:p + 4], "little")
                if MODE_TEXT <= w < MODE_TEXT + len(d):
                    d[p:p + 4] = (w + delta).to_bytes(4, "little")
                    n += 1
            path = write_bin(f"{OUT}/text_{e['id']}.bin", d)
            out.append((e["id"], path, n))
        return delta, out


def write_bin(path, data):
    with open(path, "wb") as f:
        f.write(data)
    return path


def main():
    global OUT
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--pad", default="0x40", help="bytes before each binary's rodata, text and data (multiple of 8)")
    ap.add_argument("--pad-main", help="the exe's pad (default --pad; 0 leaves it where it is: to bisect a crash)")
    ap.add_argument("--pad-engine", help="the engine's pad (default --pad)")
    ap.add_argument("--pad-slot", help="each slot overlay's pad (default --pad)")
    ap.add_argument("--no-disc", action="store_true", help="link and check only")
    ap.add_argument("--out", default=OUT, help="the output directory (default build/shiftall)")
    a = ap.parse_args()
    OUT = a.out
    pads = [int(x if x is not None else a.pad, 0) for x in (a.pad_main, a.pad_engine, a.pad_slot)]
    if any(p % 8 for p in pads):
        sys.exit("shift_all: the pads must keep 8-byte alignment (jump tables)")
    pad = max(pads)
    os.makedirs(OUT, exist_ok=True)
    b = Build(*pads)
    b.settle()
    bad = b.stale()
    for bin_, n, v in bad:
        print(f"shift_all: warning: {bin_} uses {n} at its retail address 0x{v:08X}, which moved: it won't follow")
    exe = b.binaries()
    delta, texts = b.mode_texts()
    print(f"shift_all: pads exe 0x{pads[0]:X}, engine 0x{pads[1]:X}, slot 0x{pads[2]:X} (x3 each; engine at "
          f"0x{b.engine_base:08X}, slot at 0x{b.slot_base:08X}); {len(bad)} stale names; "
          f"{sum(n for _, _, n in texts)} mode text pointers moved by 0x{delta:X}")
    if a.no_disc:
        return
    cmd = [sys.executable, "tools/scripts/pack_disc.py", "--out", OUT, "--grow", "--exe", exe, "--elf", f"{OUT}/main.elf",
           "--use", f"engine={OUT}/engine.bin"]
    cmd += [x for o in b.slots for x in ("--use", f"{o}={OUT}/{o}.bin")]
    cmd += [x for i, p, _ in texts for x in ("--dat", f"{i}={p}")]
    r = subprocess.run(cmd)
    sys.exit(r.returncode)


if __name__ == "__main__":
    main()
