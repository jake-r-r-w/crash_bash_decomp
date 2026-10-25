# GCC 2.8.1 source (reference only)

Unmodified source of FSF GCC 2.8.1, from <https://ftp.gnu.org/gnu/gcc/gcc-2.8.1.tar.gz> (sha256
`3b30fbfdf93e628373d90d174243f3267b0eec9ebe792bb64fd15b8828c2ea4c`). It is here to read, not to
build: nothing in the Makefile uses it. GPL v2 (`COPYING`).

Kept as for `../gcc-2.7.2.3/`: every top-level source, header and `.def`, the `.texi` manuals,
`ginclude/` and `config/mips/` (plus the generic `config/*.h` the 2.7.2.3 copy has). Left out:
ChangeLogs, `TAGS`, generated info files, the C++ and Objective-C front ends and the other
targets.

## Why

Every unit but the two gcc 2.7.2 ones in `main.exe` is built with `gcc 2.8.1-psx`
(decompals/old-gcc's build of this release for the PlayStation, `../../cc1/README.md`). This is
its FSF base, so it is the source to read when a 2.8.1 match depends
on what a pass does. `../gcc-2.7.2.3/README.decomp.md` has the table of which file holds which
pass; the files have the same names here.

Checked against 2.7.2.3 on 2026-10-20: `potential_hazard`, `schedule_select` and
`rank_for_schedule` (sched.c) are identical; `move_movables` and `scan_loop`'s movable rules
(loop.c), `expand_end_loop` (stmt.c) and `duplicate_loop_exit_test` (jump.c) are the same
rules. `birthing_insn_p` and `adjust_priority` changed only in how they read the live-register
set. Unlike 2.7.2.3, this tree has `reload_cse_regs` (reload1.c), the post-reload CSE that
2.7.2.3's README lists as a known difference, and `alias.c` (base-term alias analysis).
