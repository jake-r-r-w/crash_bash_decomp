"""Symbol definitions for the engine's link from the slot overlays' own links.

    slot_syms.py OUT.ld ENGINE_OBJ... -- OVL_ELF...

The engine's AST table (src/overlays/engine/asttable.c) names the slot overlays' Init/Plot
functions. The overlays share one address range and link apart from the engine (against its
fixed addresses), so the engine can't link against them as objects: this takes every symbol
the engine objects leave undefined that an overlay ELF defines, and writes `NAME = 0xADDR;`
lines for the engine's link. A matching build gets the retail addresses; an overlay built
shifted gives its new ones, so the table follows it. The same name defined at two addresses
(two overlays' functions of one name) is an error.
"""
import subprocess
import sys


def nm(path, *flags):
    out = subprocess.run(["mipsel-linux-gnu-nm", *flags, path], capture_output=True, text=True, check=True).stdout
    return out.splitlines()


def undefined(objs):
    names = set()
    for o in objs:
        for l in nm(o, "-u"):
            parts = l.split()
            if parts and parts[0] == "U":
                names.add(parts[1])
    return names


def defined(elf):
    """name -> address of an ELF's global text and data symbols."""
    out = {}
    for l in nm(elf, "-g", "--defined-only"):
        parts = l.split()
        if len(parts) == 3 and parts[1] in "TtDdBbRr":
            out[parts[2]] = int(parts[0], 16)
    return out


def resolve(names, elfs):
    """[(name, address)] for the names some ELF defines; raises on a clash."""
    found = {}
    for e in elfs:
        for n, a in defined(e).items():
            if n in names:
                if n in found and found[n][1] != a:
                    raise SystemExit(f"slot_syms: {n} is {found[n][1]:#x} in {found[n][0]} and {a:#x} in {e}")
                found.setdefault(n, (e, a))
    return sorted((n, a) for n, (_, a) in found.items())


def main():
    args = sys.argv[1:]
    if len(args) < 3 or "--" not in args:
        raise SystemExit(__doc__)
    out, rest = args[0], args[1:]
    cut = rest.index("--")
    objs, elfs = rest[:cut], rest[cut + 1:]
    syms = resolve(undefined(objs), elfs)
    with open(out, "w", encoding="utf-8", newline="\n") as f:
        f.write("/* written by tools/scripts/slot_syms.py: the slot overlays' symbols the engine names */\n")
        for n, a in syms:
            f.write(f"{n} = 0x{a:08X};\n")
    print(f"slot_syms: {len(syms)} symbols -> {out}")


if __name__ == "__main__":
    main()
