# cc1 2.8.1-psx from source

The build uses `/opt/gcc/2.8.1-psx/cc1`, which the toolchain image downloads prebuilt from
[decompals/old-gcc](https://github.com/decompals/old-gcc) release 0.17. This folder rebuilds
that cc1 from source, byte for byte, so we can make analysis variants of the very same compiler:
one whose pass prints its decisions, one with a decision forced the other way, one built with
other host flags. A variant is for finding out why the compiler does something. The build never
uses one, and a match is only a match with `/opt/gcc`.

## What 2.8.1-psx is

- **FSF GCC 2.8.1**, `gcc-2.8.1.tar.gz` from ftp.gnu.org (sha256 `3b30fbfd…`, the release
  `tools/reference/gcc-2.8.1/` is a trimmed copy of).
- **Three patches from old-gcc** (commit `b74211c9`, tag 0.17), plus `varargs.h` → `stdarg.h`:
  - `psx.patch` adds the `mips-sony-psx` target (`config/mips/psx.h`): R3000, ISA 1,
    soft-float, `-mgpopt`, split addresses, little-endian, unsigned plain `char`;
  - `mips.patch` adds `ASM_OUTPUT_SECTION_NAME` (section attributes);
  - `obstack-2.8.1.h.patch` replaces a cast used as an lvalue, which modern hosts reject.
- **It is not Sony's binary.** It is the FSF release configured for the PlayStation. Sony's own
  build isn't available; 2534 of the 2537 game functions match with this one.
- **The host build:** Ubuntu 20.04's gcc 9.4.0-1ubuntu1~20.04.2 with libc6-dev-i386
  2.31-0ubuntu9.18, `-std=gnu89 -m32 -static` and no `-O`. So cc1 is a 32-bit x86 program and
  its floating point runs on the x87 FPU at Linux's default extended precision. That matters in
  two places: local-alloc.c (`QTY_CMP_PRI`) and global.c (`allocno_priority`) rank registers
  with `double` arithmetic truncated to `int`. A cc1 built for Windows with MSVC would run at
  53-bit precision (MSVC's default x87 setting); `--cflags -mpc64` builds that variant.

## Use

```sh
tools/cc1/build.sh                    # the release build -> build/cc1/release/; checks it equals /opt
tools/cc1/build.sh pc64 --cflags -mpc64            # a variant with other host flags
tools/cc1/build.sh logqty --patch tools/cc1/patches/F.patch   # a variant with our source patch
tools/cc1/with.sh NAME COMMAND...     # COMMAND in the container, with that cc1 in place of /opt's
tools/cc1/check.sh NAME               # the whole game built with it, then make check
tools/cc1/with.sh NAME python3 tools/scripts/cc1_compare.py FUNC [FILE...]
                                      # which builds of FUNC it changes (default: FUNC's own C)
```

- `build.sh` runs on the host and needs Docker. It downloads the FSF tarball (8.4 MB) and
  old-gcc's patches and tests (8 KB) into the ignored `ext/cc1/`, checking every file's sha256,
  then builds in an `ubuntu:focal` container. The first build pulls that image and its
  `build-essential` and `gcc-multilib` packages and takes about 3 minutes; Docker caches them.
- `--patch FILE` (repeatable) applies our own patch with `patch -p1` in `gcc-2.8.1/`, after
  old-gcc's patches and before `configure`, in the order given. Keep such patches in
  `tools/cc1/patches/`. `--cflags` adds host compiler flags.
- Variants go to `build/cc1/NAME/` (cc1, cpp, gcc, cc1plus, g++ and a `BUILDINFO` with the host
  compiler, the flags and the patches). `make clean` deletes them; rebuild with `build.sh`.
- `with.sh` sets `GCC_ROOT`, which the Makefile and `decomp.py` read, to a container-local copy
  of `/opt/gcc` with the variant's cc1 in `2.8.1-psx/`, so `try --file`, `sched` and the pass
  dumps all run with it. The two gcc 2.7.2 units keep their compiler.
- `check.sh` copies the build inputs into the container and builds there, so `build/` keeps the
  real compiler's objects.
- `cc1_compare.py` builds each test copy twice, with `/opt/gcc` and with the variant, and lists
  the builds that change, with their distance from retail before and after. A variant that only
  reports (a logging patch) must change nothing, in `check.sh` and here.

## Checked

2026-10-25: `build.sh` rebuilds cc1, cpp, gcc, cc1plus and g++ byte-identical to the
toolchain's `/opt/gcc/2.8.1-psx/`, and `check.sh release` builds the whole game with the
rebuilt cc1 and passes `make check`.

2026-10-25, host precision (`pc64`, `--cflags -mpc64`): its startup code sets the x87
precision control to 53 bits (`set_precision`, in `.init_array`). The whole game built with it
passes `make check`, and the three parked functions' C and all 179 saved variants of them
(`func_800B4048` 159, `PlotTransmuterBeam` 15, `func_800BE914` 5) build to the same words as
with `/opt`. So the host's precision decides nothing here, which fits the arithmetic: `int`
truncation of `10000 * P / L` (refs and life length, small integers) can only move when that
value is an exact integer with a ratio that isn't a binary fraction, and the order only changes
if another quantity falls within 1 below it. The remaining parks are a source question, not a
compiler one.
