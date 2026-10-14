"""Region diff: score a function's C against the target block by block.

`decomp.py try` compares instruction i with instruction i, so one extra instruction early on turns
everything after it into a diff, and its STAGE line moves in big steps. This module aligns the two
instruction streams instead and scores the target's basic blocks one at a time:

- blocks are the target's basic blocks (split at branch targets, after a branch's delay slot, and
  at the asm's labels, which covers jump-table cases);
- the streams are aligned on opcode alone (difflib), so register differences don't break the
  alignment; aligned pairs are then compared exactly (relocated fields masked, branches must go to
  the aligned place);
- a block is `ok` when every instruction is paired, identical and contiguous; `operands` when every
  instruction is paired but some operands differ; `differ` when anything is unpaired.

Pure functions only (no build), so it can be tested on word lists.
"""
import difflib

LO16_MASK = 0xFFFF0000      # %hi / %lo / %gp_rel: the low half is the linker's
J26_MASK = 0xFC000000       # jal sym: the target is the linker's


def mnemonic(w):
    """Opcode key for alignment: the primary opcode, plus funct for SPECIAL, rt for REGIMM, rs for
    COPz (mfc2/mtc2/...)."""
    op = w >> 26
    if op == 0:
        return (0, w & 0x3F) if w else ("nop",)
    if op == 1:
        return (1, (w >> 16) & 0x1F)
    if op in (16, 17, 18, 19):
        return (op, (w >> 21) & 0x1F, w & 0x3F if (w >> 25) & 1 else 0)
    return (op,)


def is_branch(w):
    op = w >> 26
    return op in (1, 4, 5, 6, 7, 20, 21, 22, 23)


def is_jump(w):
    """j, jr (not jal/jalr: a call falls through)."""
    op = w >> 26
    return op == 2 or (op == 0 and (w & 0x3F) == 8)


def branch_dest(w, i):
    off = w & 0xFFFF
    if off & 0x8000:
        off -= 0x10000
    return i + 1 + off


def target_masks(texts):
    """Per-instruction mask from the splat text: relocated fields aren't compared."""
    out = []
    for t in texts:
        if "%hi(" in t or "%lo(" in t or "%gp_rel(" in t:
            out.append(LO16_MASK)
        elif t.startswith("jal "):
            out.append(J26_MASK)
        else:
            out.append(0xFFFFFFFF)
    return out


def compiled_masks(n, relocs):
    out = [0xFFFFFFFF] * n
    for i, (typ, _) in relocs.items():
        if i >= n:
            continue
        if typ in ("R_MIPS_HI16", "R_MIPS_LO16", "R_MIPS_GPREL16"):
            out[i] = LO16_MASK
        elif typ in ("R_MIPS_26", "R_MIPS_26_LOCAL"):
            out[i] = J26_MASK
    return out


def target_dests(tw, vram):
    """Index each target branch/j goes to (None if not a local jump)."""
    out = []
    for i, w in enumerate(tw):
        if is_branch(w):
            out.append(branch_dest(w, i))
        elif w >> 26 == 2 and vram is not None:
            out.append((((w & 0x3FFFFFF) * 4 | (vram & 0xF0000000)) - vram) // 4)
        else:
            out.append(None)
    return out


def compiled_dests(cw, relocs):
    out = []
    for i, w in enumerate(cw):
        typ, arg = relocs.get(i, ("", None))
        if is_branch(w):
            out.append(branch_dest(w, i))
        elif typ == "R_MIPS_26_LOCAL":
            out.append(arg // 4)
        else:
            out.append(None)
    return out


def blocks_of(tw, labels=()):
    """Target basic blocks as [start, end) index ranges."""
    n = len(tw)
    leaders = {0} | {i for i in labels if 0 <= i < n}
    for i, w in enumerate(tw):
        if is_branch(w) or is_jump(w):
            if i + 2 < n:
                leaders.add(i + 2)
        if is_branch(w):
            d = branch_dest(w, i)
            if 0 <= d < n:
                leaders.add(d)
    starts = sorted(leaders)
    return [(s, starts[k + 1] if k + 1 < len(starts) else n) for k, s in enumerate(starts)]


def _match(a, b):
    return difflib.SequenceMatcher(None, a, b, autojunk=False).get_matching_blocks()


def align(tw, cw, tkeys=None, ckeys=None):
    """Pair target and compiled instructions: {target index: compiled index}. Identical
    instructions (`tkeys`/`ckeys`, relocated fields and branch offsets masked) anchor the
    alignment; the gaps between anchors are then paired on opcode alone, so a register
    difference still pairs but a run of same-opcode instructions pairs with its exact partner."""
    tkeys = tkeys if tkeys is not None else tw
    ckeys = ckeys if ckeys is not None else cw
    pairs = {}
    ta = tb = 0
    for a, b, size in _match(tkeys, ckeys):
        if a > ta and b > tb:                      # the gap before this anchor, by opcode
            for ga, gb, gs in _match([mnemonic(w) for w in tw[ta:a]], [mnemonic(w) for w in cw[tb:b]]):
                for k in range(gs):
                    pairs[ta + ga + k] = tb + gb + k
        for k in range(size):
            pairs[a + k] = b + k
        ta, tb = a + size, b + size
    return pairs


def keys(ws, masks):
    """Alignment keys: the masked word, branch offsets masked too (checked separately)."""
    return [w & m & (0xFFFF0000 if is_branch(w) else 0xFFFFFFFF) for w, m in zip(ws, masks)]


def score(tw, cw, ttexts, relocs, vram=None, labels=()):
    """Region scores. Returns a dict:
    blocks: [{start, end, status, paired, exact, cstart, cend}], ok/total blocks, exact/len insns,
    extra: compiled indices no target instruction pairs with."""
    nt, nc = len(tw), len(cw)
    tmask, cmask = target_masks(ttexts), compiled_masks(nc, relocs)
    tdest, cdest = target_dests(tw, vram), compiled_dests(cw, relocs)
    pairs = align(tw, cw, keys(tw, tmask), keys(cw, cmask))

    exact = {}
    for t, c in pairs.items():
        m = tmask[t] & cmask[c]
        if is_branch(tw[t]) or (tw[t] >> 26 == 2 and tdest[t] is not None):
            m &= 0xFFFF0000 if is_branch(tw[t]) else J26_MASK
            same = (tw[t] & m) == (cw[c] & m) and pairs.get(tdest[t]) == cdest[c]
        else:
            same = (tw[t] & m) == (cw[c] & m)
        exact[t] = same

    blocks = []
    for s, e in blocks_of(tw, labels):
        idx = range(s, e)
        paired = [t for t in idx if t in pairs]
        contiguous = all(pairs[t + 1] == pairs[t] + 1 for t in idx[:-1] if t in pairs and t + 1 in pairs)
        nexact = sum(1 for t in paired if exact[t])
        if len(paired) == e - s and contiguous:
            status = "ok" if nexact == e - s else "operands"
        else:
            status = "differ"
        cs = [pairs[t] for t in paired]
        blocks.append({"start": s, "end": e, "status": status, "paired": len(paired), "exact": nexact,
                       "cstart": min(cs) if cs else None, "cend": max(cs) + 1 if cs else None})

    used = set(pairs.values())
    return {
        "blocks": blocks,
        "ok": sum(1 for b in blocks if b["status"] == "ok"),
        "total": len(blocks),
        "exact": sum(1 for v in exact.values() if v),
        "paired": len(pairs),
        "len": max(nt, nc),
        "extra": [c for c in range(nc) if c not in used],
        "pairs": pairs,
        "exact_map": exact,
    }
