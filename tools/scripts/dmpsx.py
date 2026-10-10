#!/usr/bin/env python3
"""DMPSX for the compile pipeline: turn the GTE placeholder words in Sony's
inline_c.h into real cop2 instructions.

PsyQ's GTE macros (ext/psyq47/include/inline_c.h, used through gtemac.h) can't
emit cop2 commands, since Sony's assembler didn't know them. Each command is a
placeholder `.word (index << 6) | 0x3f` (an undefined SPECIAL opcode), and
Sony's DMPSX tool rewrote those words in the object file. This filter does the
same on cc1's assembly output, between cc1 and maspsx:

    cc1 | dmpsx.py | maspsx | as

Only words inside inline asm (#APP ... #NO_APP) are touched. The index table
follows the macro order in inline_c.h; index 0x4E is gte_mvmva(sf,mx,v,cv,lm),
which packs its fields at bits 18-25 of the placeholder.
"""
import re
import sys

MVMVA = 0x4A400012  # mvmva: sf bit 19, mx 17-18, v 15-16, cv 13-14, lm bit 10


def mvmva(sf, mx, v, cv, lm=0):
    return MVMVA | sf << 19 | mx << 17 | v << 15 | cv << 13 | lm << 10


# mx: 0 = RT, 1 = LL, 2 = LC. v: 0..2 = V0..V2, 3 = IR. cv: 0 = TR, 1 = BK, 3 = none.
def _matrix_ops(mx, first):
    ops = {}
    for i, (v, cv) in enumerate([(0, 3), (1, 3), (2, 3), (3, 3),
                                  (0, 0), (1, 0), (2, 0), (3, 0),
                                  (0, 1), (1, 1), (2, 1), (3, 1)]):
        ops[first + i] = mvmva(1, mx, v, cv)
    return ops


TABLE = {
    0x01: 0x4A180001,  # rtps
    0x02: 0x4A280030,  # rtpt
    0x03: mvmva(1, 0, 0, 0),  # rt: RT*V0+TR
    0x08: mvmva(0, 0, 3, 3),  # rtir_sf0
    0x15: mvmva(1, 1, 0, 3, 1),  # ll: LL*V0, lm
    0x26: mvmva(1, 2, 3, 1, 1),  # lc: LC*IR+BK, lm
    0x37: 0x4A680029,  # dpcl
    0x38: 0x4A780010,  # dpcs
    0x39: 0x4AF8002A,  # dpct
    0x3A: 0x4A980011,  # intpl
    0x3B: 0x4AA80428,  # sqr12
    0x3C: 0x4AA00428,  # sqr0
    0x3D: 0x4AC8041E,  # ncs
    0x3E: 0x4AD80420,  # nct
    0x3F: 0x4AE80413,  # ncds
    0x40: 0x4AF80416,  # ncdt
    0x41: 0x4B08041B,  # nccs
    0x42: 0x4B18043F,  # ncct
    0x43: 0x4B280414,  # cdp
    0x44: 0x4B38041C,  # cc
    0x45: 0x4B400006,  # nclip
    0x46: 0x4B58002D,  # avsz3
    0x47: 0x4B68002E,  # avsz4
    0x48: 0x4B78000C,  # op12
    0x49: 0x4B70000C,  # op0
    0x4A: 0x4B98003D,  # gpf12
    0x4B: 0x4B90003D,  # gpf0
    0x4C: 0x4BA8003E,  # gpl12
    0x4D: 0x4BA0003E,  # gpl0
}
# rtv0..rtirbk (0x04..0x10, minus rtir_sf0 at 0x08), llv0..llirbk, lcv0..lcirbk
for _i, _op in _matrix_ops(0, 0x04).items():
    TABLE[_i if _i < 0x08 else _i + 1] = _op
TABLE.update(_matrix_ops(1, 0x16))
TABLE.update(_matrix_ops(2, 0x27))

WORD = re.compile(r"\.word\s+(0x[0-9a-fA-F]+|-?\d+)")


def convert(m):
    w = int(m.group(1), 0) & 0xFFFFFFFF
    if w & 0xFC00003F != 0x3F:
        return m.group(0)
    idx = (w >> 6) & 0xFFF
    if idx == 0x4E:  # gte_mvmva(sf, mx, v, cv, lm)
        op = mvmva(w >> 25 & 1, w >> 23 & 3, w >> 21 & 3, w >> 19 & 3, w >> 18 & 1)
    elif idx in TABLE:
        op = TABLE[idx]
    else:
        sys.exit(f"dmpsx.py: unknown GTE placeholder .word 0x{w:08x}")
    return f".word 0x{op:08X}"


def main():
    in_asm = False
    for line in sys.stdin:
        s = line.strip()
        if s == "#APP":
            in_asm = True
        elif s == "#NO_APP":
            in_asm = False
        elif in_asm and ".word" in line:
            line = WORD.sub(convert, line)
        sys.stdout.write(line)


if __name__ == "__main__":
    main()
