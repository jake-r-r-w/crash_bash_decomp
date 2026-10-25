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
  build isn't available; every matched function builds byte for byte with this one.
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

## Patches

`patches/` holds our analysis patches. Each one is off unless its environment variables are set,
and with them unset the variant must build exactly like `/opt` (check with `check.sh` and
`cc1_compare.py`). `tools/dk` passes `DK_DOCKER_ARGS="-e VAR=value"` into the container, which
is how `check.sh` gets them.

- **`local-tie.patch`** (local-alloc.c). `CC1_LOCAL_TIE=reverse` breaks `qty_compare_1`'s and
  `qty_sugg_compare_1`'s priority ties by the higher qty number instead of the lower.
  `CC1_LOCAL_LOG=1` prints each block's qtys to stderr in allocation order: priority, refs,
  life in half-insns, suggestions, the register each got and the pseudos in it.
  `CC1_LOCAL_FUNC=NAME` and `CC1_LOCAL_BLOCK=N` limit both to one function and block. To see a
  function's log, run its unit's preprocessed source through the variant:

  ```sh
  tools/dk python3 tools/scripts/decomp.py sched NAME >/dev/null      # writes build/sched/NAME.i
  tools/cc1/with.sh localtie sh -c 'CC1_LOCAL_LOG=1 CC1_LOCAL_FUNC=NAME \
    $GCC_ROOT/2.8.1-psx/cc1 -quiet -O2 -G0 -fno-builtin -o /dev/null build/sched/NAME.i'
  ```

- **`keep-live.patch`** (toplev.c). Only in the function named by `CC1_KEEP_FUNC`:
  `CC1_KEEP_USE=REG@UID[,REG@UID...]` puts `(use (reg REG))` before insn UID ahead of flow
  analysis. The pseudo then stays live up to there for flow, combine and both allocators, and
  no instruction is output for the USE. Take REG and UID from the `-df` dump of the same file.
  `CC1_KEEP_LOG=SYMBOL` prints to stderr, after combine, each pseudo set to `(lo_sum X SYMBOL)`
  (refs, sets, block or -2 for global, calls crossed, live length, the insns that mention it)
  and each insn that still folds SYMBOL into its address; after global alloc it prints the hard
  register each of those pseudos got. To test whether a hidden use is all a function lacks,
  diff the build with one:

  ```sh
  DK_DOCKER_ARGS="-e CC1_KEEP_FUNC=NAME -e CC1_KEEP_USE=807@1915" \
    tools/cc1/with.sh keeplive python3 tools/scripts/decomp.py try NAME --file F.c
  ```

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

2026-10-25, local-alloc's tie (`localtie`, `--patch patches/local-tie.patch`): unset, it builds
the whole game to `make check`'s checksums, and `func_800B4048`'s parked C and 22 test shapes
to the same words as `/opt`. Its log shows the tie the park note works out by hand: in
`func_800B4048`'s tail (block 20) the x chain's qty and the `*60` chain's both score 7500, and
the x chain, born first, takes v1. With `CC1_LOCAL_TIE=reverse` for that block only, the 12
shapes with the 60% at the loop top and `q` set after it byte-match retail: that tie is the only
decision between them and the target. Reversed for every function, all 16 binaries fail
`make check`, so the original compiler broke ties the FSF way, and the original source gives the
`*60` qty the higher priority with the same final code.

2026-10-25, the C that wins that tie: with `CC1_LOCAL_LOG=1` as the check, one of those shapes
plus an always-zero `(t * 60) & 3` added to the 60% (outside the divide) gives the `*60` qty 10
refs instead of 9 (8333 against 7500), and the shipped cc1 byte-matches: `func_800B4048` is
matched. Flow counts the use before combine drops it. Inside the divide the count is lost:
combine folds the pseudo that carries it into a new one (the log still shows 9 refs).

2026-10-25, a hidden use (`keeplive`, `--patch patches/keep-live.patch`): unset, it builds the
whole game to `make check`'s checksums and `PlotTransmuterBeam`'s parked C to the same words as
`/opt`. Its log shows the park's one diff: the spawn's base (pseudo 807, set by insn 1890) folds
into the mode read. With `CC1_KEEP_USE=807@1915` (the join after the mode test) the parked C
byte-matches retail, and so it does with the use at any insn from the test's two arms to the
`InitDelete` call (1901-1949). Inside the test's own block (1896, 1897) the base stays local
and local-alloc ties it to the `lui`'s register. After the `if (bx)` block (1956, 1973) it is 3
registers off. So a later use that leaves no instruction was all the park lacked. The C that
gives one is a test in an arm whose body flow finds dead, and whose load jump2 deletes
(MATCHING.md, Diff patterns, the unfolded-base row). `PlotTransmuterBeam` is matched with it.
