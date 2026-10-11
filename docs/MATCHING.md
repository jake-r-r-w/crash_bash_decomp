# Matching workflow

Functions are matched by hand, by a person or an AI agent, **one function per
commit**, and every commit goes through one validation gate ([diagram](../README.md#how-functions-are-matched)). The bulk matcher
(`decomp.py auto`) is retired. Its earlier drafts still show up in `next` as a
starting point.

## The loop

All commands run through `tools/dk` (Docker).

1. **Pick:** `tools/dk python3 tools/scripts/decomp.py next`
   prints the target, its callers, the asm and a draft (m2c, or an earlier
   compiling draft). `next --func NAME` picks a specific one, `--seg REGEX`
   restricts to a segment (`overlays/ovl62`, ...).
2. **Evidence first, then write C.** Before writing anything, gather
   evidence (see [Evidence first](#evidence-first)). Then replace the
   function's `INCLUDE_ASM` line in its source file with C. Use real types
   from `include/` (add structs there, see [CONVENTIONS.md](CONVENTIONS.md)).
   Don't leave m2c leftovers.
3. **Diff:** `tools/dk make diff FUNC=NAME` builds that file and shows
   only the differing instructions. Look the symptom up in
   [Diff patterns](#diff-patterns), then
   [classify the diff and work through its list](#classify-the-diff-then-work-through-the-list).
   For register differences, also read the [cc1 dump](#why-does-a-register-differ).
   Note in your reasoning which items you tried.
4. **Stuck?** Run the permuter only after the evidence step, the checklist
   for that kind of diff and, for register differences, the cc1 dump:
   `tools/dk make permute FUNC=NAME MINUTES=10`. When it finds something,
   port it by hand and add the general rule to [Diff patterns](#diff-patterns).
   Still stuck: restore the `INCLUDE_ASM` line, keep the best attempt under
   `#ifndef NON_MATCHING`, and run `decomp.py block NAME --note "what differs"`.
   Then move on.
5. **Validate and commit:**
   `tools/dk python3 tools/scripts/decomp.py done NAME --commit --note "what made it match"`

## Evidence first

Most matches come from what's already known, not from guessing. Before the
first diff:

- **Near-identical functions:** `tools/dk python3 tools/scripts/decomp.py dups`
  lists groups of identical functions and of near-identical ones ("constants
  differ"). If a member is matched, start from its C.
- **Matched C:** grep `src/` for the same globals, callees or shape (the same
  loop, the same struct offsets). A sibling's C often shows the idiom.
  Example: `Random` and `func_800155FC` share the seed step.
- **Names and types:** check `include/bash/*.h` and the leaked minigame
  headers (`bash/tank.h`, `pogo.h`, `arkenoid.h`, `polar.h`; see
  [ARCHITECTURE.md](ARCHITECTURE.md), Leaked source) for the function's
  prototype, its structs and the globals' types.

## Diff patterns

Look up what the diff shows. **Every session that learns something from a
match or a park adds a row here** (not only an AGENT_LOG paragraph).

| Symptom in the diff | Likely source change | Example |
|---|---|---|
| Count-down loop ending `bne v1, a1` with `a1 = -1`, and a `beqz` guard before it | `for (i = n - 1; i != -1; i--)`; `while (n--)` and `while (n-- != 0)` don't match. Fixed count: `for (i = 62; i != -1; i--)` | `func_800154C0`, `func_800154EC`; `func_80012E54`, `func_8001325C` |
| Loop exit test at the top, `j` back at the bottom | Leave the loop by `return` or `goto` past it ([below](#loops-gcc-rotates-but-the-target-doesnt)) | `func_80015780` |
| `lhu` + `sll 16` + `sra 16` for a field | Nothing to fix: the field is `s16` (combine reuses the shifted value, e.g. for the `srl 31` of a `/ 2`) | `func_8001C7FC` |
| gcc leaves a branch delay slot unfilled that the target fills (its `.set noreorder` block is missing) | A return register is live on the taken path, e.g. a `return f(...)` that makes v0 live at the shared exit | `func_8001C7FC` (parked) |
| `mfhi` result lands in a different register | Register-allocation preference ("pref HI_REG"); read the [cc1 dump](#why-does-a-register-differ) | `func_8001C7FC`, `func_8001C974` |
| Argument still in `a0`, never written, at a call | Pass the caller's argument straight through to the callee; m2c drops it | `func_8001CFE8`, `func_800C6828` |
| A constant stored to two globals, `li` scheduled before the first `lui` | Hold the constant in a local: `on = TRUE; A = on; B = on;` | `func_8002D0C8` |
| A pointer init placed after a hoisted loop constant (`li a0,-1` then `move s0,s1`) | It's a strength-reduced giv: write `&base[i]` with an `i` counter | `func_80015780` |
| Two pointer `addiu`s in the loop in the wrong order | Move `i++` before or after the other pointer's `++` | `func_80015780` |
| Loop constant not hoisted (stays in the loop) | The loop is too big for loop.c's threshold: compute a repeated address once into a local (`p = &pts[i]`) | `func_80015780` |
| Two pointers stepping in lockstep (one loaded through, one returned) | Two variables in the source, not one | `func_80015780` |
| `addiu a0,a0,0x6C` then fields at small offsets | A pointer to a sub-struct (`&ps->set`) | `func_80015C1C` |
| Vector 0 loaded with `lw` + `mtc2` | `gte_ldv0_w(xy, z)` (`bash/main.h`); load the arguments into locals first if the `lw`s come first | `func_8001D048` |
| `lw t4/t5/t6` + `ctc2` | Sony's `gte_SetRotMatrix` and siblings; pass one `MATRIX *` local to both `gte_SetRotMatrix` and `gte_SetTransMatrix` | `func_80015C1C`, `func_8002AEE0` |
| Other instructions right before a GTE command (`intpl`, `rtps`, `avsz3`, `nclip`, ...) | The `_b` macro (no nops): `gte_intpl_b()`, `gte_rtps_b()`, `gte_avsz3_b()`, `gte_nclip_b()` | `func_8001C0F0`, `func_800193A8` |
| `lw`/`or`/`sw` of a whole POLY word | Work on the word whole: `*(u32 *)&poly->r0 \| 0x02000000`, not `setSemiTrans` | `func_800177F4` (parked) |
| A commutative op's result lands in the other operand's register (`addu v1,v0,v1` vs `addu v0,v0,v1`) | local-alloc ties the result to the first operand that dies there; swap the operand order. If that moves the other registers, it's priority order: still open | `func_800155FC` (parked) |
| Callee-saved registers from s2 up (s0/s1 unused, t-registers free) in a leaf | The original pinned those locals: `register s32 xy asm("$20");`. Normal allocation never skips s0. Needs `done --hack` | `func_800193A8` |
| SXY stored at +0x10/+0x18/+0x20 straight off the primitive pointer | `gte_stsxy3_10(p)` (`bash/main.h`); no Sony macro has these offsets | `func_800193A8` |
| A tag word written whole, never loaded: `0x09000000 \| (ot[otz] & 0xFFFFFF)` | `poly->tag = 0x09000000 \| (ot[otz] & 0xFFFFFF); setaddr(ot + otz, poly);`, not `setlen` + `addPrim` (those give an `sb` and a read-modify-write) | `func_800193A8` |
| Several branches go to the later of two identical `j top; addiu` blocks; ours all go to the first | Each skip path `goto`s one label before the shared `p++` at the loop's end; repeated `p++; continue;` blocks get cross-jumped | `func_800193A8` |
| `base + 1` kept in its own register (`addiu t3,a2,1`), initialised after the hoisted constants | Read `base[1]` twice in the loop (test and use) so loop.c makes it a giv; a separate `count` pointer is initialised too early | `func_800193A8` |
| A late register copy of a value loaded earlier (`addu t0,v1,zero` before `rtps`) | Read the same value again at that point in the source (`n = runs[1]` after the test); CSE turns the reread into the copy | `func_800193A8` |
| Two identical `return x` blocks merged, but the target keeps the block before the second test (`bnez a; jr ra; ...; beq b,c → back`) where ours keeps it after (`beqz a → forward`) | Cross-jumping keeps the later copy, so write only the first one: put a label inside that block (`if (sub == 0) { found: return ext; }`) and `goto found` from the second test | `FindExtension` |
| An early exit returns a callee-saved register (`j exit; move v0,s0`) that holds an initialised `NULL` | One `return voice;` at the end, reached from that path; `return NULL` there gives `move v0,zero` | `PlaySfx` |
| Another function's body repeated inside this one | gcc 2.8 `-O2` doesn't inline, and the helper is often defined after the caller: write the code out in place (or a macro) | `PlaySfx` (`func_80022924`) |
| A pointer copied into a saved register in a branch delay slot, while nearby stores still use the original register | Assign the copy after those stores (`if (a) a->prev = v; voice = v;`); reorg moves it into the slot | `PlaySfx` |
| A loop's `return p` stub (`j exit; move v0,v1`) sits up after the first early return, and that first test is inverted (`beqz` → rest) | Join the later NULL checks into one condition: `if (a != NULL && (p = f(a)) != NULL) { loop }`, then one `return NULL`. Separate early returns, nested `if`s and `while`/`for`/`do` all give the other layout | `func_80025A48` |
| A global's `lui` hoisted to the entry (often into a delay slot) and shared by several later loads and stores of it | Use the global through a pointer local: `NAS **head = (NAS **)&D_8005AA34;`, then `*head` | `func_80026980` |
| `beqz b → far; j near` right after a first test (gcc threaded a jump through a repeated test) | Test both first and keep the rest inside: `if (a || b) { if (a) ... else ...; } else { ... }` | `func_80026980` |
| A loop tests a register that is always 0 (`beqz a2` after `move a2,zero`) and an id kept in a register | A helper written out in place with its parameters as locals (`id = 0x400; want = 0;` then `FindExtension`'s loop, `goto found`). Give each job its own local: reusing one for the switch key moved 8 operands | `func_800250B0` |
| A list loop's "append at the end" block sits before the loop (a backward `beqz` to it) | `while (p != NULL) { if (found) { ...; return; } if (p->next == NULL) { append; return; } p = p->next; }`, both exits inside; `for (;;)` with `break` puts it after the loop | `func_800110A8` |
| `x + 6 + y` computed as `x + (y + 6)`, or a nearby zero store scheduled before the loads | `x->size += 6 + y->size;`, and put the other store (`x->unk0 = 0`) after it in the source | `func_800110A8` |
| Stack frame bigger than the calls need (`addiu sp,-0x30` with `ra` at 0x28 and no locals used) | An unused local array of the missing size: `char unused[24];` | `func_800101E0` |
| `a * K + C + b * L` computed with the constant on the last term (`addiu` after both products), target adds it to the first | Two statements: `size = a * K + C; size += b * L;` (one expression is always reassociated) | `func_8002A650` |
| A fill loop sets up its pointer before the counter and the `-1` constant | Walk an explicit pointer (`p = Table[0]; ... p += 2;`), not `Table[i]` | `func_8002BD68`, `func_8002BBB4`, `func_8002B9A4` |
| A literal `0` argument passed from a register that holds a `0` result (`move a2,v0`), target uses `addu a2,zero,zero` | Assign the zero result in an `else` after the calls, not before them | `func_800280C0` |
| Signed `/ 0x1000` rounding temp in another register than the target's | Do the division as its own statement on a reused variable: `b = (b - a) * t; b /= 0x1000;` | `func_80015284` |
| Unlink writes `prev->next` from a saved `next` copy, target reloads `node->next` | Write `prev->next = node->next` (permuter find); a global's address held in a pointer local can need setting inside the outer loop (`pool = &D_80056844;`) | `func_8001828C` |
| `base + (off + K)` then `+ i * size` in two `addu`s into a saved register | Two statements: `rec = t + (t->off + K); rec += i * size;` | `func_8001682C` |
| A loop over records keeps one base pointer (fields at their offsets), ours splits it into two induction pointers with odd offsets (`-36(v1)`, `0(a0)`) | Build the loop with a label and `goto` (loop.c only optimises `for`/`while`/`do`), and hold a stored constant in a local | `func_80027F00` |
| A mask kept as `andi 0xF0` before a byte or halfword store (ours uses a hoisted `-16`), and the value loaded twice | Read the other half into a narrow local first and pass the result through an `s32` before the store: `lo = *p >> 4; v = ((*p << 4) & 0xF0) | lo; *p = v;` (permuter find) | `func_80015518`, `func_80015554` |
| A loop accumulator and counter in the wrong registers (sum `a1`, target `a2`), every loop rewrite stuck at the same diff | Route the term through a reused `s32` temp: `c = *s++; c = c * i++; sum += c;` (permuter find) | `func_8001534C` |
| A `beqz` on a pointer jumps past the following `if`/`else` too, not just its own block | The second block is nested inside the first: `if (p->prev) { ...; if (p->next) ... else ...; }` | `KillNAS` |
| A loaded global hoisted above the argument copies in the prologue (the target keeps source order) | Copy a narrow (`u16`) parameter into an `int` local before using it (`want = id;`); permuter find | `SpawnNASById` (parked) |
| A short-lived temp gets the wrong register, and a fresh temp doesn't help | Reuse an existing variable for it (`nas = free->next; NASFreeList = nas; ...; nas = free;`): reuse changes the allocation priorities; permuter find | `SpawnNASById` (parked) |

## Classify the diff, then work through the list

Classify the diff by what differs, then try the items for that kind in
order. Note which ones you tried.

**Register-only** (same opcodes, operands differ):
1. Add or remove a temp.
2. Split or merge an expression.
3. `x += y` vs `x = x + y`.
4. Signedness and width of locals.
5. Declaration order of locals.
6. A cast on an intermediate value.
7. Operand order of a commutative operator.
8. Then read the [cc1 dump](#why-does-a-register-differ).

**Instruction order** (same instructions, different order):
1. Statement order.
2. Chained (`a = b = 1`) vs separate assignments.
3. Reading a global into a local once vs reading it twice.
4. `if (a && b)` vs nested ifs.
5. The declared type of the globals involved.
6. `TRUE` vs `1`: no codegen effect, so rule it out quickly.

**Opcode or length** (different instructions or count):
1. Loop form (see [Diff patterns](#diff-patterns)).
2. `/` vs `>>`.
3. `(u32)` casts.
4. Unsigned vs signed compares.
5. Early return vs if/else.
6. Macros (Sony's, or a repo macro in `include/`).

## Why does a register differ?

Dump the compiler's register allocation for the file:

```sh
tools/dk sh -c 'mipsel-linux-gnu-cpp -P -nostdinc -undef -Iinclude -Iext/psyq47/include -D__GNUC__=2 -D_LANGUAGE_C -DPSX -DGVAL=0 src/main/main.c > x.i && /opt/gcc/2.8.1-psx/cc1 -quiet -O2 -G0 -dl -dg -o x.s x.i'
```

Run it with the function's C in place of its `INCLUDE_ASM` line (swap in the
file of the function's segment for `src/main/main.c`). It writes `x.i`,
`x.s`, `x.i.lreg` and `x.i.greg` in the repo root; they aren't gitignored,
so delete them afterwards.

- `x.i.lreg` (local-alloc) lists each pseudo's uses and lifetime, then
  "Register N in M" for the pseudos it placed, then the RTL with `REG_DEAD`
  notes. It shows which pseudo was tied to which (an insn's result shares
  the register of a source operand that dies there).
- `x.i.greg` lists "Register dispositions" for pseudos global.c placed.
- "pref HI_REG" marks `mfhi` results: local-alloc skips them and global.c
  picks the register, avoiding registers other pseudos prefer (a return
  value prefers v0).

For branch layout differences, dump the jump passes instead (`-dj -dJ -dd`
write `x.i.jump`, `x.i.jump2` and `x.i.dbr`) to see which pass fixes the
layout. Add `-fno-schedule-insns2` to tell register choice apart from sched2's
reordering.

## The validation gate (`done`)

`done` records nothing, and commits nothing, unless every check passes:

| Check | Fails when |
|---|---|
| Scope | the working tree replaces any other function's `INCLUDE_ASM` (one function per commit) |
| Lint | the function still contains `M2C_*` macros, m2c's `?` type, or inline asm with instructions |
| Hacks | a register pin or an `asm("")` barrier is used without `--hack "why"` |
| Function | the function's compiled words differ from the target |
| Full build | a clean rebuild of every target (exe and overlays) is not byte-identical |

Then it records the match, copies the same C to identical functions elsewhere
(they're listed in the commit body), refreshes the dashboard, and commits
`src/ include/ configs/ .decomp/ docs/` as `Match NAME (seg, N bytes)` with a
`Validated-by:` trailer.

**Enforcement:** the pre-commit hook (`tools/hooks/pre-commit`, enable it with
`git config core.hooksPath tools/hooks`) refuses any commit that replaces an
`INCLUDE_ASM` line unless the commit came from `done`.

## Commits that aren't matches

Renames (`decomp.py rename`), header moves, typing an already matched function,
and file splits are ordinary small commits made after `tools/dk make verify`
prints `OK` for every target. Keep each one to a single purpose.

## GTE code

Write GTE code with Sony's macros from `inline_c.h` and `gtemac.h` (PsyQ 4.7,
included by `include/psyq.h`): `gte_ldv0(v); gte_rt(); gte_stlvl(out);` is
`lwc2 x2, nop, nop, mvmva, swc2 x3`. Read the target's cop2 instruction to pick
the macro: `rtps`, `nclip`, `avsz3`, and `mvmva sf, mx, v, cv, lm`, where mx
0/1/2 = RT/LL/LC, v 0..2 = V0..V2 (3 = IR), cv 0/1/3 = TR/BK/none
(`mvmva 1,0,0,0,0` is `gte_rt`/`gte_rtv0tr`). The macros emit placeholder
words; `tools/scripts/dmpsx.py` turns them into the real instructions after
cc1, as Sony's DMPSX did. The gate's lint doesn't count these macros as
inline asm. m2c can't draft GTE functions, so write them from the asm.

## Loops gcc rotates but the target doesn't

gcc 2.8 moves a `while`/`for` loop's exit test to the bottom when that test
jumps to the loop's own end. If the target tests at the top and jumps back
with `j` at the bottom, the source left the loop by `return` or by `goto` to
a label after it:

```c
for (;;) {
    if (--n == -1) {
        goto none; /* or return */
    }
    ...
}
```

A loop built from labels and `goto` alone doesn't match: loop.c then skips it,
so loop constants (`li $a0, -1`) aren't hoisted.

## For AI agents

- Work on one function at a time and commit it before picking the next.
- Start each session by reading [AGENT_LOG.md](AGENT_LOG.md). End it by
  updating its current state, next steps and open leads (what got parked and
  why), and adding a short session-log entry. Idioms you learned go in
  [Diff patterns](#diff-patterns).
- Never fake a match: no whole-function `asm()`, no editing `asm/`, `build/`
  or the checksums, and no committing around the gate.
- The project skill `.claude/skills/match-function` runs this loop: `/match-function`
  matches the next function, `/match-function NAME` a specific one.
