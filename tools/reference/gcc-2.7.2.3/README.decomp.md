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

The two `main.exe` units built with gcc 2.7.2 (`src/ps1/modchip.c`, `src/ps1/modchip_msg.c`;
Makefile `CC_VER = 2.7.2-psx`) are matched by reading what this compiler does. The `-d` dumps
(docs/MATCHING.md) show what a pass did; the source says why. 2.7.2.1-2.7.2.3 are bug-fix
releases over 2.7.2 (`NEWS`), so for this project they are the same compiler.

It is also the reference for the gcc 2.8.1 units (everything else). For the passes in the
table, its code behaved like our 2.8.1 cc1 in every case checked: the sched1/sched2 traces,
local-alloc's choices and jump2's cross-jumps in `func_8007B330` and `func_8007F314`. When a
2.8.1 diff is a tie inside a pass, read the deciding function here and work it through by hand
before reaching for the permuter (docs/MATCHING.md, "Reading the compiler's rules", which also
lists the rules found so far).

The passes that decide most of a match, in the order cc1 runs them (`toplev.c`,
`rest_of_compilation`):

| File | Pass | Matching questions it answers |
|---|---|---|
| `convert.c`, `fold-const.c` | front end | 16-bit arithmetic for a result stored to a short or masked (`convert_to_integer`); constants moved outward through a sum (`fold`) |
| `stmt.c` | expand | loop rotation: whether a loop test stays at the top (`expand_end_loop`) |
| `calls.c`, `expr.c` | expand | whether a call's result gets a temp of its own or goes straight into the variable (`expand_call`, `store_expr`) |
| `integrate.c` | expand (inline) | an inlined function's argument copied into a new pseudo per parameter (`expand_inline_function`) |
| `cse.c` | cse, cse2 (`-frerun-cse-after-loop`) | folding of addresses and constants (`find_best_addr`, `fold_rtx`); which blocks one pass covers (`cse_end_of_basic_block`); which register of a copy later uses read (`make_regs_eqv`) |
| `loop.c` | loop | invariant hoisting (`scan_loop`, `combine_movables`, `move_movables`), which loops it skips ("phony"), strength reduction |
| `flow.c` | flow | basic blocks, liveness, `REG_DEAD`/`REG_UNUSED` notes, refs weighted by loop depth |
| `combine.c` | combine | merged instructions; `+` as `or` when the operands' bits can't overlap (`nonzero_bits`) |
| `sched.c` | sched1, sched2 | instruction order (`priority`, `rank_for_schedule`, `schedule_select`, `adjust_priority`, `birthing_insn_p`); which loads may move above which stores (`true_dependence`, `memrefs_conflict_p`) |
| `local-alloc.c`, `global.c` | register allocation | which register a value gets (`qty_compare`, `combine_regs`, `find_free_reg`, `update_equiv_regs`, `optimize_reg_copy_1`; global.c's `allocno_compare` and preferences) |
| `reload1.c` | reload | late register substitutions |
| `jump.c` | jump, jump2 | cross-jumping (`find_cross_jump`, `do_cross_jump`), threading (`follow_jumps`) |
| `reorg.c` | dbr | delay-slot filling (`fill_simple_delay_slots`, `fill_eager_delay_slots`, `steal_delay_list_from_fallthrough`, `redundant_insn`, `relax_delay_slots`) |
| `config/mips/mips.md`, `mips.h`, `mips.c` | target | patterns, costs, `REG_ALLOC_ORDER`, delay-slot rules |

## Known differences from our gcc 2.8.1

2.8.1 adds post-reload CSE (`reload_cse_regs`), which this tree doesn't have, so there is no
source here for it. It deletes a store of a value memory already holds, and turns a constant
load or operand into a register that already holds it. In `func_8007F314` (parked) it gives
`and v0,v0,a1` and `sllv ...,s5`, where a constant was set in the same block. It works only
within a block, from the last label on. The Diff patterns rows for `func_800926CC` and
`func_8008A924` use it.

`invoke.texi` documents the flags, `rtl.texi` the RTL in the dumps.

## Known difference from Sony's compiler

Sony's gcc 2.7.2 is not exactly this tree. Its `reorg.c` fills an unconditional `j` from its
target before it fills conditional branches; this one (and our `2.7.2-psx` cc1) does it later,
so a `beq` followed by such a `j` gets a different delay-slot instruction. `2.7.2-cdk` and
`2.8.1-psx` behave like Sony's. `tools/scripts/aspsx_fill.py` (`refill_from_jump`) redoes
Sony's order; see docs/AGENT_LOG.md (func_8002D4F4) and the Diff patterns table in
docs/MATCHING.md.
