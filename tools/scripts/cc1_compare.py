"""Which builds of a function change under a cc1 variant (tools/cc1/README.md).

Builds each test copy of FUNC twice with its unit's settings, once with the shipped compilers
(/opt/gcc) and once with the variant's (GCC_ROOT), and prints which builds change and how far
each one is from retail. Run it through tools/cc1/with.sh, which points GCC_ROOT at the variant:

  tools/cc1/with.sh NAME python3 tools/scripts/cc1_compare.py FUNC [FILE...]

With no FILE it builds FUNC's own C in its unit (a parked function's C under NON_MATCHING). A
variant that only reports, such as a logging patch, must change no build; one that forces a
decision shows here which shapes it moves.
"""
import argparse
import os
import sys
from concurrent.futures import ThreadPoolExecutor

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import decomp  # noqa: E402

SHIPPED = "/opt/gcc"


def changed(a, b):
    """How many words differ between two builds of a function (lengths count too), or None when
    either one failed to build."""
    if a is None or b is None:
        return None
    return sum(1 for x, y in zip(a, b) if x != y) + abs(len(a) - len(b))


def build_all(key, e, name, src, paths, defs, root, tag):
    """[(words or None, relocs)] for PATHS (None: the unit itself), all built with the compilers
    under ROOT. pipeline() reads decomp.GCC_ROOT when it runs, so one root per batch."""
    decomp.GCC_ROOT = root
    cfg = decomp.mk_config()
    os.makedirs("build/cc1/compare", exist_ok=True)

    def one(job):
        k, path = job
        o = f"build/cc1/compare/{name}_{tag}_{k}.o"
        if os.path.exists(o):
            os.remove(o)
        cmd = decomp.unit_pipeline(src, o, cfg, defs=defs if path is None else (), c_path=path)
        if decomp.run("set -o pipefail; " + cmd).returncode:
            return None, None
        cw, _, relocs = decomp.obj_func(o, name)
        return (cw or None), relocs

    with ThreadPoolExecutor(8) as ex:
        return list(ex.map(one, enumerate(paths)))


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("func")
    ap.add_argument("files", nargs="*", help="test copies of the function (default: its own C)")
    a = ap.parse_args()
    variant = os.environ.get("GCC_ROOT", SHIPPED)
    if variant == SHIPPED:
        decomp.die("GCC_ROOT is the shipped /opt/gcc: run this through tools/cc1/with.sh NAME")
    proj, funcs = decomp.need_proj(), decomp.need_funcs()
    key, e = decomp.find_func(funcs, a.func)
    name = decomp.cur_name(key, e, decomp.sym_names())
    src = e.get("src") or decomp.locate_def(name)
    if not src:
        decomp.die(f"can't find the source file defining {name}")
    paths = a.files or [None]
    defs = tuple(decomp.c_defines(src, name, e)) if not a.files else ()
    decomp.make_flags("CPPFLAGS"), decomp.make_flags("ASFLAGS")      # cached before the threads
    old = build_all(key, e, name, src, paths, defs, SHIPPED, "shipped")
    new = build_all(key, e, name, src, paths, defs, variant, "variant")
    tw, vram = decomp.target_words(e, proj), decomp.key_vram(key)

    def to_retail(r):
        return len(decomp.compare(tw, r[0], r[1], vram)[0]) if r[0] else None

    n_changed = 0
    for path, o, v in zip(paths, old, new):
        d = changed(o[0], v[0])
        label = path or f"{src} ({' '.join(defs) or 'its C'})"
        if d is None:
            print(f"  FAILED   {label}: {'shipped' if o[0] is None else 'variant'} build failed")
            n_changed += 1
        elif d == 0:
            print(f"  same     {label} (retail: {to_retail(o)} differ)")
        else:
            n_changed += 1
            print(f"  CHANGED  {label}: {d} words; retail: {to_retail(o)} -> {to_retail(v)} differ")
    print(f"{name}: {n_changed} of {len(paths)} builds changed or failed under GCC_ROOT={variant}")


if __name__ == "__main__":
    main()
