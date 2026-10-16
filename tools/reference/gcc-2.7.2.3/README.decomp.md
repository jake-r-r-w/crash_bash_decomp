# GCC 2.7.2.3 source (reference only)

Unmodified source of FSF GCC 2.7.2.3, the last 2.7.2 maintenance release, from
<https://ftp.gnu.org/gnu/gcc/gcc-2.7.2.3.tar.gz> (sha256
`16166e0f0f2064bb3114716650569065ef3855ae2331d7a29b85d4b9aa73773c`). It is here to read, not
to build: nothing in the Makefile uses it. GPL v2 (`COPYING`).

Only the C compiler and the MIPS target are kept: every top-level source, header and `.def`,
the `.texi` manuals, `ginclude/` and `config/mips/` (plus the generic `config/*.h` they include).
Left out: ChangeLogs, generated info files, the C++ and Objective-C front ends and the other
targets.

## Why

The two `main.exe` units built with gcc 2.7.2 (`src/main/8002d488.c`, `src/main/8002e0f0.c`;
Makefile `CC_VER = 2.7.2-psx`) are matched by reading what this compiler does. The `-d` dumps
(docs/MATCHING.md) show what a pass did; the source says why. 2.7.2.1-2.7.2.3 are bug-fix
releases over 2.7.2 (`NEWS`), so for this project they are the same compiler.

The passes that decide most of a match, in the order cc1 runs them (`toplev.c`,
`rest_of_compilation`):

| File | Pass | Matching questions it answers |
|---|---|---|
| `cse.c` | cse, cse2 (`-frerun-cse-after-loop`) | folding of addresses and constants (`find_best_addr`, `fold_rtx`) |
| `loop.c` | loop | invariant hoisting |
| `flow.c` | flow | basic blocks, liveness, `REG_DEAD` notes |
| `combine.c` | combine | merged instructions |
| `sched.c` | sched1, sched2 | instruction order (`rank_for_schedule`, `adjust_priority`, birthing insns) |
| `local-alloc.c`, `global.c` | register allocation | which register a value gets (priorities, preferences) |
| `reload1.c` | reload, `reload_cse_regs` | late register substitutions |
| `jump.c` | jump, jump2 | cross-jumping, threading |
| `reorg.c` | dbr | delay-slot filling (`fill_simple_delay_slots`, `fill_eager_delay_slots`) |
| `config/mips/mips.md`, `mips.h`, `mips.c` | target | patterns, costs, `REG_ALLOC_ORDER`, delay-slot rules |

`invoke.texi` documents the flags, `rtl.texi` the RTL in the dumps.

## Known difference from Sony's compiler

Sony's gcc 2.7.2 is not exactly this tree. Its `reorg.c` fills an unconditional `j` from its
target before it fills conditional branches; this one (and our `2.7.2-psx` cc1) does it later,
so a `beq` followed by such a `j` gets a different delay-slot instruction. `2.7.2-cdk` and
`2.8.1-psx` behave like Sony's. `tools/scripts/aspsx_fill.py` (`refill_from_jump`) redoes
Sony's order; see docs/AGENT_LOG.md (func_8002D4F4) and the Diff patterns table in
docs/MATCHING.md.
