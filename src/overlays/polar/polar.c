#include "common.h"
#include "bash/main.h"
#include "bash/polar_ovl.h"
#include "bash/polar.h"
#include "bash/hero.h"
#include "bash/shell.h"

/* the exe's data the tables point at, not declared in the headers yet */
/* engine.h declares it, but polar.c does not include that */
extern s32 VramNodePool[];
extern u8 VramAreas[];

/* rodata: the strings (const arrays keep their retail order; see data2c.py) */
const char PolarAnimLight[] = "LIGHT";
const char PolarAnimRun[] = "RUN";
const char PolarAnimBreathe[] = "BREATHE";
const char PolarAnimFall[] = "FALL";
const char PolarAnimPush[] = "PUSH";
const char PolarAnimTaunt[] = "TAUNT";
const char PolarAnimSkate[] = "SKATE";
const char PolarAnimHit[] = "HIT";
const char PolarAnimBarge[] = "BARGE";

/* data, in address order */
u16 PolarDefaultAiSkill[10] = { 5, 2, 1, 1, 0x14, 0x1C, 0xB, 0x10, 0x10, 0x1C };
u16 PolarDefaultAiSkillAlt[10] = { 7, 4, 1, 1, 0x18, 0x1C, 0x12, 0xC, 0xC, 0x1C };
u16 PolarDefaultAiTeamSkill[6] = { 1, 8, 0x18, 1, 8, 0x18 };
u16 PolarDefaultAiHumanRoundTweak[12] = { 8, 0x10, 0x18, 7, 0xE, 0x15, 6, 0xC, 0x13, 5, 0xA, 0x10 };
u16 PolarDefaultAiBotTweak[10] = { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 2 };
u16 PolarDefaultAiHumanTweak[42] = { 0x10, 0x10, 0x10, 0x10, 0x20, 0x10, 0x20, 0x10, 0x10, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 2, 0x10, 0x10, 0x10, 0x10, 0x18, 0x10, 0x18, 0x10, 0x10, 0x20, 8, 0x10, 0x18, 7, 0xE, 0x15, 6, 0xC, 0x13, 5, 0xA, 0x10 };
u16 PolarDayAiSkill[10] = { 0x13, 0xF, 0xB, 7, 0x1A, 0x1C, 0x1C, 0x16, 0x16, 0x1C };
u16 PolarDayAiSkillAlt[10] = { 0x14, 0x10, 0xC, 8, 0x1A, 0x1C, 0x1C, 0x15, 0x15, 0x1C };
u16 PolarDayAiTeamSkill[6] = { 4, 0x12, 0x1C, 4, 0x12, 0x1C };
u16 PolarDayAiHumanRoundTweak[12] = { 0xE, 0x15, 0x1C, 0xC, 0x13, 0x19, 0xA, 0xF, 0x15, 8, 0xD, 0x12 };
u16 PolarDayAiBotTweak[10] = { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 2 };
u16 PolarDayAiHumanTweak[42] = { 0x10, 0x10, 0x10, 0x10, 0x20, 0x10, 0x20, 0x10, 0x10, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 2, 0x10, 0x10, 0x10, 0x10, 0x18, 0x10, 0x18, 0x10, 0x10, 0x20, 0xE, 0x15, 0x1C, 0xC, 0x13, 0x19, 0xA, 0xF, 0x15, 8, 0xD, 0x12 };
u16 PolarCutAiSkill[10] = { 0x18, 0x15, 0x12, 0xF, 0x1A, 0x1C, 0x1C, 0x18, 0x18, 0x1C };
u16 PolarCutAiSkillAlt[10] = { 0x19, 0x16, 0x13, 0x10, 0x1A, 0x1C, 0x1C, 0x15, 0x15, 0x1C };
u16 PolarCutAiTeamSkill[6] = { 6, 0x12, 0x1C, 6, 0x12, 0x1C };
u16 PolarCutAiHumanRoundTweak[12] = { 0x14, 0x1A, 0x20, 0x11, 0x17, 0x1C, 0xE, 0x13, 0x18, 0xC, 0x10, 0x14 };
u16 PolarCutAiBotTweak[10] = { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 2 };
u16 PolarCutAiHumanTweak[42] = { 0x10, 0x10, 0x10, 0x10, 0x20, 0x10, 0x20, 0x10, 0x10, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20, 0x10, 0x10, 2, 0x10, 0x10, 0x10, 0x10, 0x18, 0x10, 0x18, 0x10, 0x10, 0x20, 0x14, 0x1A, 0x20, 0x11, 0x17, 0x1C, 0xE, 0x13, 0x18, 0xC, 0x10, 0x14 };
u16 PolarEndAiSkill[10] = { 0x18, 0x16, 0x14, 0x12, 0x20, 0x1C, 0x1C, 0x20, 0x20, 0x1C };
u16 PolarEndAiSkillAlt[10] = { 0x18, 0x16, 0x14, 0x12, 0x20, 0x1C, 0x1C, 0x10, 0x10, 0x1C };
u16 PolarEndAiTeamSkill[6] = { 6, 0x14, 0x20, 6, 0x14, 0x20 };
u16 PolarEndAiHumanRoundTweak[12] = { 0x14, 0x1A, 0x20, 0x11, 0x17, 0x1C, 0xE, 0x13, 0x18, 0xC, 0x10, 0x14 };
u16 PolarEndAiBotTweak[10] = { 0x10, 0x10, 0x10, 0x10, 2, 0x10, 2, 0x10, 0x10, 2 };
u16 PolarEndAiHumanTweak[42] = { 0x10, 0x10, 0x10, 0x10, 0x20, 0x10, 0x20, 0x10, 0x10, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 2, 0x10, 0x10, 0x10, 0x10, 0x18, 0x10, 0x18, 0x10, 0x10, 0x20, 0x14, 0x1A, 0x20, 0x11, 0x17, 0x1C, 0xE, 0x13, 0x18, 0xC, 0x10, 0x14 };
u16 PolarBossAiSkill[40] = { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 };
u16 PolarBossAiBotTweak[34] = { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 };
u16 PolarBossAiHumanTweak[12] = { 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0 };
u16 PolarBossAiHumanRoundTweak[12] = { 0x10, 0x18, 0x20, 0x10, 0x16, 0x1C, 0x10, 0x14, 0x18, 0x10, 0x12, 0x14 };
u16 PolarBossAiSkillAlt[70] = {
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0, 0x10, 0x10,
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20, 0x20, 0, 0x10, 0x18, 0x20, 0x10, 0x16, 0x1C,
    0x10, 0x14, 0x18, 0x10, 0x12, 0x14,
};
hero_seq CRA_seq_breathe_ft[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq CRA_seq_win_ft[6] = {
    0, 0x8007, 0x22, 0x4200, 0x8000, 0,
};
hero_seq CRA_seq_polarwin[48] = {
    0, 0x4300, 0x4301, 0x4302, 0x4303, 0x4304, 0x4305, 0x4306, 0x4307, 0x8008, 0x19, 0x4308, 0x4309,
    0x430A, 0x430B, 0x430C, 0x430D, 0x430E, 0x430F, 0x4310, 0x4311, 0x4312, 0x4313, 0x4314, 0x4315,
    0x4316, 0x4317, 0x8008, 0x18, 0x4318, 0x4319, 0x431A, 0x431B, 0x431C, 0x431D, 0x431E, 0x431F,
    0x4320, 0x4321, 0x4322, 0x4323, 0x4324, 0x4325, 0x4326, 0x4327, 0x8000, 0, 0,
};
hero_seq CRA_seq_skate_ft[14] = {
    0, 0x8008, 0x15, 0x8007, 0xC, 0x4100, 0x8008, 0x14, 0x8007, 0xC, 0x410D, 0x8000, 0, 0,
};
hero_seq CRA_seq_skate_attack[24] = {
    0, 0x4180, 0x4181, 0x4182, 0x4183, 0x4184, 0x4185, 0x4186, 0x4187, 0x4187, 0x4187, 0x4187,
    0x4187, 0x4187, 0x4187, 0x4187, 0x4187, 0x4187, 0x4187, 0x4187, 0x4187, 0x8004, 0x54, 0,
};
hero_seq CRA_seq_polaridle[24] = {
    0, 0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A,
    0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113, 0x8000, 0, 0,
};
hero_seq CRA_seq_lightbear[8] = {
    0, 0x4280, 0x4281, 0x4282, 0x4283, 0x8000, 0, 0,
};
hero_seq CRA_seq_polarhit[22] = {
    0, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x4090, 0x4091, 0x4092, 0x8004, 0x54,
};
hero_seq CRA_seq_polardead[54] = {
    0, 0x4180, 0x4181, 0x4182, 0x4183, 0x4184, 0x4185, 0x4186, 0x4187, 0x4188, 0x4189, 0x418A,
    0x418B, 0x418C, 0x418D, 0x418E, 0x418F, 0x4190, 0x4191, 0x4192, 0x4400, 0x4401, 0x4402, 0x4403,
    0x4404, 0x4405, 0x4406, 0x4407, 0x4408, 0x4409, 0x440A, 0x440B, 0x440C, 0x440D, 0x440E, 0x440F,
    0x4410, 0x4411, 0x4412, 0x4413, 0x4414, 0x4415, 0x4416, 0x4417, 0x4418, 0x4419, 0x441A, 0x441B,
    0x441C, 0x441D, 0x441E, 0x441F, 0x8004, 0x50,
};
hero_seq CRA_seq_polardazed[24] = {
    0, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x4090, 0x8001, 0x4091, 0x4092, 0x8000, 0, 0,
};
hero_seq CRA_seq_polarattack[26] = {
    0, 0x4180, 0x4181, 0x4182, 0x4183, 0x4184, 0x4185, 0x4186, 0x4187, 0x4188, 0x4189, 0x418A,
    0x418B, 0x418C, 0x418D, 0x418E, 0x8008, 0x18, 0x418F, 0x4190, 0x4191, 0x4192, 0x4193, 0x8004,
    0x49, 0,
};
hero_seq CRA_seq_polarrun[20] = {
    0, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x8000, 0, 0,
};
hero_seq CRA_seq_polarfall[46] = {
    0, 0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005, 0x4006, 0x4007, 0x4008, 0x4009, 0x400A,
    0x400B, 0x400C, 0x400D, 0x400E, 0x400F, 0x4010, 0x4011, 0x4012, 0x4013, 0x4014, 0x4015, 0x4016,
    0x4017, 0x4018, 0x4019, 0x401A, 0x401B, 0x401C, 0x401D, 0x401E, 0x401F, 0x4020, 0x4021, 0x4022,
    0x4023, 0x4024, 0x4025, 0x4026, 0x4027, 0x4028, 0x4029, 0x8004, 0x50, 0,
};
hero_seq CRA_seq_polartaunt[24] = {
    0, 0x4200, 0x4201, 0x4202, 0x4203, 0x4204, 0x4205, 0x4206, 0x4207, 0x4208, 0x4209, 0x420A,
    0x420B, 0x420C, 0x420D, 0x420E, 0x420F, 0x4210, 0x4211, 0x4212, 0x4213, 0x8000, 0, 0,
};
hero_seq COC_seq_lightbear[8] = {
    0, 0x4200, 0x4201, 0x4202, 0x4203, 0x8000, 0, 0,
};
hero_seq COC_seq_breathe_ft[6] = {
    0, 0x8007, 0x14, 0x4000, 0x8000, 0,
};
hero_seq COC_seq_skate_ft[14] = {
    0, 0x8008, 0x47, 0x8007, 0xC, 0x4100, 0x8008, 0x46, 0x8007, 0xC, 0x410B, 0x8000, 0, 0,
};
hero_seq COC_seq_polarwin[14] = {
    0, 0x8007, 0x13, 0x4300, 0x8008, 0x4B, 0x8001, 0x4313, 0x8007, 9, 0x4314, 0x8000, 0, 0,
};
hero_seq COC_seq_win_ft[6] = {
    0, 0x8007, 0x64, 0x4200, 0x8000, 0,
};
hero_seq COC_seq_skate_attack[24] = {
    0, 0x4180, 0x4181, 0x4182, 0x4183, 0x4184, 0x4185, 0x4186, 0x4187, 0x4187, 0x4187, 0x4187,
    0x4187, 0x4187, 0x4187, 0x4187, 0x4187, 0x4187, 0x4187, 0x4187, 0x4187, 0x8004, 0x54, 0,
};
hero_seq COC_seq_polartaunt[26] = {
    0, 0x4280, 0x4281, 0x4282, 0x4283, 0x4284, 0x4285, 0x4286, 0x4287, 0x4288, 0x8001, 0x4289,
    0x428A, 0x428B, 0x428C, 0x428D, 0x428E, 0x428F, 0x4290, 0x4291, 0x4292, 0x4293, 0x4294, 0x8000,
    0, 0,
};
hero_seq COC_seq_polarhit[22] = {
    0, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x4090, 0x4091, 0x4092, 0x8004, 0x54,
};
hero_seq COC_seq_polardead[4] = {
    0, 0x8004, 0x50, 0,
};
hero_seq COC_seq_polardazed[24] = {
    0, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x4090, 0x8001, 0x4091, 0x4092, 0x8000, 0, 0,
};
hero_seq COC_seq_polarattack[26] = {
    0, 0x4180, 0x4181, 0x4182, 0x4183, 0x4184, 0x4185, 0x4186, 0x4187, 0x4188, 0x4189, 0x418A,
    0x418B, 0x418C, 0x418D, 0x8008, 0x4A, 0x418E, 0x418F, 0x4190, 0x4191, 0x4192, 0x4193, 0x8004,
    0x49, 0,
};
hero_seq COC_seq_polaridle[24] = {
    0, 0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005, 0x4006, 0x4007, 0x4008, 0x4009, 0x400A,
    0x400B, 0x400C, 0x400D, 0x400E, 0x400F, 0x4010, 0x4011, 0x4012, 0x4013, 0x8000, 0, 0,
};
hero_seq COC_seq_polarrun[20] = {
    0, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x8000, 0, 0,
};
hero_seq COC_seq_polarfall[46] = {
    0, 0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A,
    0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113, 0x4114, 0x4115, 0x4116,
    0x4117, 0x4118, 0x4119, 0x411A, 0x411B, 0x411C, 0x411D, 0x411E, 0x411F, 0x4120, 0x4121, 0x4122,
    0x4123, 0x4124, 0x4125, 0x4126, 0x4127, 0x4128, 0x4129, 0x8004, 0x50, 0,
};
hero_seq CTX_seq_lightbear[8] = {
    0, 0x4000, 0x4001, 0x4002, 0x4003, 0x8000, 0, 0,
};
hero_seq CTX_seq_breathe_ft[6] = {
    0, 0x8007, 0x20, 0x4100, 0x8000, 0,
};
hero_seq CTX_seq_skate_ft[14] = {
    0, 0x8008, 0x79, 0x8007, 0xC, 0x4000, 0x8008, 0x78, 0x8007, 0xC, 0x400D, 0x8000, 0, 0,
};
hero_seq CTX_seq_win_ft[12] = {
    0, 0x8007, 9, 0x4200, 0x8001, 0x4209, 0x8007, 0x15, 0x420A, 0x8000, 0, 0,
};
hero_seq CTX_seq_polarwin[52] = {
    0, 0x4300, 0x4301, 0x8008, 0x7D, 0x4302, 0x4303, 0x4304, 0x4305, 0x4306, 0x4307, 0x4308, 0x4309,
    0x430A, 0x430B, 0x430C, 0x430D, 0x430E, 0x430F, 0x4310, 0x4311, 0x4312, 0x4313, 0x8008, 0x7E,
    0x4314, 0x4315, 0x4316, 0x4317, 0x4318, 0x4319, 0x431A, 0x431B, 0x431C, 0x431E, 0x431F, 0x4320,
    0x4321, 0x4322, 0x4323, 0x4324, 0x4325, 0x4326, 0x4327, 0x8008, 0x7F, 0x4328, 0x8007, 0x27,
    0x4329, 0x8000, 0,
};
hero_seq CTX_seq_skate_attack[24] = {
    0, 0x4180, 0x4181, 0x4182, 0x4183, 0x4184, 0x4185, 0x4186, 0x4187, 0x4188, 0x4188, 0x4188,
    0x4188, 0x4188, 0x4188, 0x4188, 0x4188, 0x4188, 0x4188, 0x4188, 0x4188, 0x8004, 0x54, 0,
};
hero_seq CTX_seq_polartaunt[36] = {
    0, 0x4280, 0x4281, 0x4282, 0x4283, 0x4284, 0x4285, 0x4286, 0x4287, 0x4288, 0x4288, 0x4288,
    0x4288, 0x4288, 0x4288, 0x4288, 0x4288, 0x4288, 0x4288, 0x4288, 0x4288, 0x4288, 0x4289, 0x428A,
    0x428B, 0x428C, 0x428D, 0x428E, 0x428F, 0x4290, 0x4291, 0x4292, 0x4293, 0x8000, 0, 0,
};
hero_seq CTX_seq_polarhit[30] = {
    0, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x4090, 0x4091, 0x4092, 0x4093, 0x4094, 0x4095, 0x4096,
    0x4097, 0x4098, 0x4099, 0x8004, 0x54, 0,
};
hero_seq CTX_seq_polardead[4] = {
    0, 0x8004, 0x50, 0,
};
hero_seq CTX_seq_polaridle[24] = {
    0, 0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A,
    0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113, 0x8000, 0, 0,
};
hero_seq CTX_seq_polardazed[30] = {
    0, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x4090, 0x4091, 0x4092, 0x4093, 0x4094, 0x4095, 0x4096,
    0x4097, 0x8001, 0x4098, 0x4099, 0x8000, 0,
};
hero_seq CTX_seq_polarattack[42] = {
    0, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x4088, 0x4089, 0x408A, 0x408B, 0x408C, 0x408D, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x4090, 0x4091, 0x8008, 0x7C, 0x4092, 0x4093, 0x4094,
    0x4095, 0x4096, 0x4097, 0x4098, 0x8004, 0x49,
};
hero_seq CTX_seq_polarrun[20] = {
    0, 0x4200, 0x4201, 0x4202, 0x4203, 0x4204, 0x4205, 0x4206, 0x4207, 0x4208, 0x4209, 0x420A,
    0x420B, 0x420C, 0x420D, 0x420E, 0x420F, 0x8000, 0, 0,
};
hero_seq CTX_seq_polarfall[44] = {
    0, 0x4180, 0x4181, 0x4182, 0x4183, 0x4184, 0x4185, 0x4186, 0x4187, 0x4188, 0x4189, 0x418A,
    0x418B, 0x418C, 0x418D, 0x418E, 0x418F, 0x4190, 0x4191, 0x4192, 0x4193, 0x4194, 0x4195, 0x4196,
    0x4197, 0x4198, 0x4199, 0x419A, 0x419B, 0x419C, 0x419D, 0x419E, 0x419F, 0x41A0, 0x41A1, 0x41A2,
    0x41A3, 0x41A4, 0x41A5, 0x41A6, 0x41A7, 0x8004, 0x50, 0,
};
hero_seq DNG_seq_lightbear[8] = {
    0, 0x4200, 0x4201, 0x4202, 0x4203, 0x8000, 0, 0,
};
hero_seq DNG_seq_breathe_ft[6] = {
    0, 0x8007, 0x16, 0x4000, 0x8000, 0,
};
hero_seq DNG_seq_win_ft[6] = {
    0, 0x8007, 0x60, 0x4200, 0x8000, 0,
};
hero_seq DNG_seq_polarwin[44] = {
    0, 0x4280, 0x8008, 0x113, 0x4281, 0x4282, 0x4283, 0x4284, 0x4285, 0x4286, 0x4287, 0x4288,
    0x4289, 0x428A, 0x428B, 0x428C, 0x428D, 0x428E, 0x428F, 0x4290, 0x4291, 0x4292, 0x4293, 0x4294,
    0x4295, 0x8008, 0x112, 0x4296, 0x4297, 0x4298, 0x4299, 0x429A, 0x429B, 0x429C, 0x429D, 0x429E,
    0x429F, 0x42A0, 0x42A1, 0x42A2, 0x42A3, 0x8000, 0, 0,
};
hero_seq DNG_seq_skate_ft[14] = {
    0, 0x8008, 0x10F, 0x8007, 0xC, 0x4100, 0x8008, 0x10E, 0x8007, 0xC, 0x410D, 0x8000, 0, 0,
};
hero_seq DNG_seq_skate_attack[24] = {
    0, 0x4180, 0x4181, 0x4182, 0x4183, 0x4184, 0x4185, 0x4186, 0x4187, 0x4187, 0x4187, 0x4187,
    0x4187, 0x4187, 0x4187, 0x4187, 0x4187, 0x4187, 0x4187, 0x4187, 0x4187, 0x8004, 0x54, 0,
};
hero_seq DNG_seq_polartaunt[24] = {
    0, 0x4180, 0x4181, 0x4182, 0x4183, 0x4184, 0x4185, 0x4186, 0x4187, 0x4188, 0x4189, 0x418A,
    0x418B, 0x418C, 0x418D, 0x418E, 0x418F, 0x4190, 0x4191, 0x4192, 0x4193, 0x8000, 0, 0,
};
hero_seq DNG_seq_polarhit[34] = {
    0, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x4090, 0x4091, 0x4092, 0x4093, 0x4094, 0x4095, 0x4096,
    0x4097, 0x4097, 0x4099, 0x409A, 0x409B, 0x409C, 0x409D, 0x8004, 0x54, 0,
};
hero_seq DNG_seq_polardead[4] = {
    0, 0x8004, 0x50, 0,
};
hero_seq DNG_seq_polaridle[24] = {
    0, 0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005, 0x4006, 0x4007, 0x4008, 0x4009, 0x400A,
    0x400B, 0x400C, 0x400D, 0x400E, 0x400F, 0x4010, 0x4011, 0x4012, 0x4013, 0x8000, 0, 0,
};
hero_seq DNG_seq_polardazed[34] = {
    0, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x4090, 0x4091, 0x4092, 0x4093, 0x4094, 0x4095, 0x4096,
    0x4097, 0x4098, 0x4099, 0x409A, 0x409B, 0x8001, 0x409C, 0x409D, 0x8000, 0,
};
hero_seq DNG_seq_polarattack[28] = {
    0, 0x4300, 0x4301, 0x4302, 0x4303, 0x4304, 0x4305, 0x4306, 0x4307, 0x4308, 0x4309, 0x430A,
    0x430B, 0x430C, 0x430D, 0x430E, 0x430F, 0x8008, 0x112, 0x4310, 0x4311, 0x4312, 0x4313, 0x4314,
    0x4315, 0x4316, 0x8004, 0x49,
};
hero_seq DNG_seq_polarrun[20] = {
    0, 0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A,
    0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x8000, 0, 0,
};
hero_seq DNG_seq_polarfall[46] = {
    0, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x4090, 0x4091, 0x4092, 0x4093, 0x4094, 0x4095, 0x4096,
    0x4097, 0x4098, 0x4099, 0x409A, 0x409B, 0x409C, 0x409D, 0x409E, 0x409F, 0x40A0, 0x40A1, 0x40A2,
    0x40A3, 0x40A4, 0x40A5, 0x40A6, 0x40A7, 0x40A8, 0x40A9, 0x8004, 0x50, 0,
};
hero_seq TIN_seq_lightbear[8] = {
    0, 0x4200, 0x4201, 0x4202, 0x4203, 0x8000, 0, 0,
};
hero_seq TIN_seq_polarwin[56] = {
    0, 0x8007, 0x10, 0x4300, 0x8008, 0x145, 0x4311, 0x4312, 0x4313, 0x4314, 0x4315, 0x4316, 0x4317,
    0x4318, 0x4319, 0x431A, 0x430F, 0x4310, 0x4311, 0x4312, 0x4313, 0x4314, 0x4315, 0x4316, 0x4317,
    0x4318, 0x4319, 0x431A, 0x431B, 0x431C, 0x431D, 0x431E, 0x431F, 0x4320, 0x4321, 0x4322, 0x4323,
    0x4324, 0x8008, 0x144, 0x4325, 0x4326, 0x4327, 0x4328, 0x4329, 0x432A, 0x432B, 0x432C, 0x432D,
    0x432E, 0x432F, 0x4330, 0x4331, 0x8000, 0, 0,
};
hero_seq TIN_seq_breathe_ft[6] = {
    0, 0x8007, 0x20, 0x4000, 0x8000, 0,
};
hero_seq TIN_seq_skate_ft[14] = {
    0, 0x8008, 0x141, 0x8007, 0x10, 0x4100, 0x8008, 0x140, 0x8007, 0x10, 0x410F, 0x8000, 0, 0,
};
hero_seq TIN_seq_win_ft[12] = {
    0, 0x8007, 0x22, 0x4200, 0x8001, 0, 0x4222, 0x8007, 0x2B, 0x4223, 0x8000, 0,
};
hero_seq TIN_seq_skate_attack[24] = {
    0, 0x4180, 0x4181, 0x4182, 0x4183, 0x4184, 0x4185, 0x4186, 0x4187, 0x4187, 0x4187, 0x4187,
    0x4187, 0x4187, 0x4187, 0x4187, 0x4187, 0x4187, 0x4187, 0x4187, 0x4187, 0x8004, 0x54, 0,
};
hero_seq TIN_seq_polartaunt[34] = {
    0, 0x4280, 0x4281, 0x4282, 0x4283, 0x4284, 0x4285, 0x4286, 0x4287, 0x4288, 0x4289, 0x428A,
    0x428B, 0x428C, 0x428D, 0x428E, 0x428F, 0x4290, 0x4291, 0x4292, 0x4293, 0x4294, 0x4295, 0x4296,
    0x4297, 0x4298, 0x4299, 0x429A, 0x429B, 0x429C, 0x429D, 0x429E, 0x8000, 0,
};
hero_seq TIN_seq_polarhit[34] = {
    0, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x4090, 0x4091, 0x4092, 0x4093, 0x4094, 0x4095, 0x4096,
    0x4097, 0x4098, 0x4099, 0x409A, 0x409B, 0x409C, 0x409D, 0x8004, 0x54, 0,
};
hero_seq TIN_seq_polardead[34] = {
    0, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x4090, 0x4091, 0x4092, 0x4093, 0x4094, 0x4095, 0x4096,
    0x4097, 0x4098, 0x4099, 0x409A, 0x409B, 0x409C, 0x409D, 0x8004, 0x50, 0,
};
hero_seq TIN_seq_polaridle[24] = {
    0, 0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005, 0x4006, 0x4007, 0x4008, 0x4009, 0x400A,
    0x400B, 0x400C, 0x400D, 0x400E, 0x400F, 0x4010, 0x4011, 0x4012, 0x4013, 0x8000, 0, 0,
};
hero_seq TIN_seq_polarattack[32] = {
    0, 0x4180, 0x4189, 0x418A, 0x418B, 0x418C, 0x418D, 0x418E, 0x418F, 0x4190, 0x4191, 0x4192,
    0x4193, 0x4194, 0x4195, 0x4196, 0x4197, 0x4198, 0x4199, 0x419A, 0x419B, 0x419C, 0x419D, 0x419E,
    0x8008, 0x144, 0x419F, 0x41A0, 0x41A1, 0x8004, 0x49, 0,
};
hero_seq TIN_seq_polardazed[34] = {
    0, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x4090, 0x4091, 0x4092, 0x4093, 0x4094, 0x4095, 0x4096,
    0x4097, 0x4098, 0x4099, 0x409A, 0x409B, 0x8001, 0x409C, 0x409D, 0x8000, 0,
};
hero_seq TIN_seq_polarrun[20] = {
    0, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x8000, 0, 0,
};
hero_seq TIN_seq_polarfall[46] = {
    0, 0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A,
    0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113, 0x4114, 0x4115, 0x4116,
    0x4117, 0x4118, 0x4119, 0x411A, 0x411B, 0x411C, 0x411D, 0x411E, 0x411F, 0x4120, 0x4121, 0x4122,
    0x4123, 0x4124, 0x4125, 0x4126, 0x4127, 0x4128, 0x4129, 0x8004, 0x50, 0,
};
hero_seq BRO_seq_lightbear[8] = {
    0, 0x4200, 0x4201, 0x4202, 0x4203, 0x8000, 0, 0,
};
hero_seq BRO_seq_polarwin[22] = {
    0, 0x4300, 0x4301, 0x4302, 0x4303, 0x4304, 0x4305, 0x4306, 0x4307, 0x8008, 0xAF, 0x4308, 0x8007,
    0xD, 0x4309, 0x8001, 0x4317, 0x8007, 0x12, 0x4318, 0x8000, 0,
};
hero_seq BRO_seq_breathe_ft[6] = {
    0, 0x8007, 0x20, 0x4000, 0x8000, 0,
};
hero_seq BRO_seq_skate_ft[14] = {
    0, 0x8008, 0xAB, 0x8007, 0xC, 0x4100, 0x8008, 0xAA, 0x8007, 0xC, 0x410D, 0x8000, 0, 0,
};
hero_seq BRO_seq_win_ft[6] = {
    0, 0x8007, 0x18, 0x4200, 0x8000, 0,
};
hero_seq BRO_seq_skate_attack[24] = {
    0, 0x4180, 0x4181, 0x4182, 0x4183, 0x4184, 0x4185, 0x4186, 0x4187, 0x4188, 0x4188, 0x4188,
    0x4188, 0x4188, 0x4188, 0x4188, 0x4188, 0x4188, 0x4188, 0x4188, 0x4188, 0x8004, 0x54, 0,
};
hero_seq BRO_seq_polarhit[24] = {
    0, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x4090, 0x4091, 0x4092, 0x4093, 0x8004, 0x54, 0,
};
hero_seq BRO_seq_polardead[4] = {
    0, 0x4000, 0x8004, 0x50,
};
hero_seq BRO_seq_polaridle[24] = {
    0, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x4090, 0x4091, 0x4092, 0x4093, 0x8000, 0, 0,
};
hero_seq BRO_seq_polardazed[24] = {
    0, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x4090, 0x4091, 0x8001, 0x4092, 0x4093, 0x8000, 0,
};
hero_seq BRO_seq_polarattack[42] = {
    0, 0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005, 0x4006, 0x4007, 0x4008, 0x4009, 0x400A,
    0x400B, 0x400C, 0x400D, 0x4008, 0x4009, 0x400A, 0x400B, 0x400C, 0x400D, 0x4008, 0x4009, 0x400A,
    0x400B, 0x400C, 0x400D, 0x400E, 0x400F, 0x4010, 0x4011, 0x8008, 0xAE, 0x4012, 0x4013, 0x4014,
    0x4015, 0x4016, 0x4017, 0x4018, 0x8004, 0x49,
};
hero_seq BRO_seq_polarrun[20] = {
    0, 0x4180, 0x4181, 0x4182, 0x4183, 0x4184, 0x4185, 0x4186, 0x4187, 0x4188, 0x4189, 0x418A,
    0x418B, 0x418C, 0x418D, 0x418E, 0x418F, 0x8000, 0, 0,
};
hero_seq BRO_seq_polarfall[44] = {
    0, 0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A,
    0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113, 0x4114, 0x4115, 0x4116,
    0x4117, 0x4118, 0x4119, 0x411A, 0x411B, 0x411C, 0x411D, 0x411E, 0x411F, 0x4120, 0x4121, 0x4122,
    0x4123, 0x4124, 0x4125, 0x4126, 0x4127, 0x8004, 0x50, 0,
};
hero_seq BRO_seq_polartaunt[20] = {
    0, 0x4280, 0x4281, 0x4282, 0x8001, 0x4283, 0x4284, 0x4285, 0x4286, 0x4287, 0x4288, 0x4289,
    0x428A, 0x428B, 0x428C, 0x428D, 0x428E, 0x8000, 0, 0,
};
hero_seq KON_seq_breathe_ft[6] = {
    0, 0x8007, 0x20, 0x4180, 0x8000, 0,
};
hero_seq KON_seq_win_ft[6] = {
    0, 0x8007, 0x50, 0x4200, 0x8000, 0,
};
hero_seq KON_seq_polarwin[14] = {
    0, 0x4280, 0x4281, 0x4282, 0x4283, 0x4284, 0x8008, 0x177, 0x4285, 0x8007, 0x28, 0x4286, 0x8000,
    0,
};
hero_seq KON_seq_skate_ft[14] = {
    0, 0x8008, 0x173, 0x8007, 0x10, 0x4000, 0x8008, 0x172, 0x8007, 0xE, 0x4011, 0x8000, 0, 0,
};
hero_seq KON_seq_skate_attack[24] = {
    0, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4087, 0x4087, 0x4087,
    0x4087, 0x4087, 0x4087, 0x4087, 0x4087, 0x4087, 0x4087, 0x4087, 0x4087, 0x8004, 0x54, 0,
};
hero_seq KON_seq_polaridle[24] = {
    0, 0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005, 0x4006, 0x4007, 0x4008, 0x4009, 0x400A,
    0x400B, 0x400C, 0x400D, 0x400E, 0x400F, 0x4010, 0x4011, 0x4012, 0x4013, 0x8000, 0, 0,
};
hero_seq KON_seq_lightbear[8] = {
    0, 0x4200, 0x4201, 0x4202, 0x4203, 0x8000, 0, 0,
};
hero_seq KON_seq_polarhit[32] = {
    0, 0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A,
    0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113, 0x4114, 0x4115, 0x4116,
    0x4117, 0x4118, 0x4119, 0x411A, 0x411B, 0x411C, 0x8004, 0x54,
};
hero_seq KON_seq_polardead[4] = {
    0, 0x8004, 0x50, 0,
};
hero_seq KON_seq_polardazed[34] = {
    0, 0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A,
    0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113, 0x4114, 0x4115, 0x4116,
    0x4117, 0x4118, 0x4119, 0x411A, 0x8001, 0x411B, 0x411C, 0x8000, 0, 0,
};
hero_seq KON_seq_polarattack[28] = {
    0, 0x4180, 0x4189, 0x418A, 0x418B, 0x418C, 0x418D, 0x418E, 0x418F, 0x4190, 0x4191, 0x4192,
    0x4193, 0x8008, 0x176, 0x4194, 0x4195, 0x4196, 0x4197, 0x4198, 0x4199, 0x419A, 0x419B, 0x419C,
    0x419D, 0x8004, 0x49, 0,
};
hero_seq KON_seq_polarrun[20] = {
    0, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x8000, 0, 0,
};
hero_seq KON_seq_polarfall[46] = {
    0, 0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A,
    0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113, 0x4114, 0x4115, 0x4116,
    0x4117, 0x4118, 0x4119, 0x411A, 0x411B, 0x411C, 0x411D, 0x411E, 0x411F, 0x4120, 0x4121, 0x4122,
    0x4123, 0x4124, 0x4125, 0x4126, 0x4127, 0x4128, 0x4129, 0x8004, 0x50, 0,
};
hero_seq KON_seq_polartaunt[36] = {
    0, 0x4300, 0x4301, 0x4302, 0x4303, 0x4304, 0x4305, 0x4306, 0x4307, 0x4308, 0x4309, 0x8001,
    0x430A, 0x430B, 0x430C, 0x430D, 0x430E, 0x430F, 0x4310, 0x4311, 0x4312, 0x4313, 0x4314, 0x4315,
    0x4316, 0x4317, 0x4318, 0x4319, 0x431A, 0x431B, 0x431C, 0x431D, 0x431E, 0x8000, 0, 0,
};
hero_seq KAN_seq_lightbear[8] = {
    0, 0x4180, 0x4181, 0x4182, 0x4183, 0x8000, 0, 0,
};
hero_seq KAN_seq_breathe_ft[6] = {
    0, 0x8007, 0x14, 0x4080, 0x8000, 0,
};
hero_seq KAN_seq_win_ft[6] = {
    0, 0x8007, 0x76, 0x4200, 0x8000, 0,
};
hero_seq KAN_seq_polarwin[44] = {
    0, 0x4280, 0x4281, 0x4282, 0x4283, 0x8008, 0xE1, 0x4284, 0x4285, 0x4286, 0x4287, 0x4288, 0x4289,
    0x428A, 0x428B, 0x428C, 0x428D, 0x428E, 0x428F, 0x4290, 0x4291, 0x4292, 0x4293, 0x4294, 0x4295,
    0x4296, 0x4297, 0x4298, 0x4299, 0x429A, 0x429B, 0x429C, 0x429D, 0x429E, 0x429F, 0x42A0, 0x42A1,
    0x42A2, 0x42A3, 0x42A4, 0x42A5, 0x8000, 0, 0,
};
hero_seq KAN_seq_skate_ft[14] = {
    0, 0x8008, 0xDD, 0x8007, 0xC, 0x4180, 0x8008, 0xDC, 0x8007, 0xC, 0x418D, 0x8000, 0, 0,
};
hero_seq KAN_seq_skate_attack[24] = {
    0, 0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4004, 0x4004, 0x4004, 0x4004, 0x4004, 0x4004,
    0x4004, 0x4004, 0x4004, 0x4004, 0x4004, 0x4004, 0x4004, 0x4004, 0x4004, 0x8004, 0x54, 0,
};
hero_seq KAN_seq_polartaunt[22] = {
    0, 0x4300, 0x4301, 0x4302, 0x4303, 0x4304, 0x4305, 0x4306, 0x4307, 0x4308, 0x4309, 0x430A,
    0x430B, 0x430C, 0x430D, 0x430E, 0x430F, 0x4310, 0x4311, 0x8000, 0, 0,
};
hero_seq KAN_seq_polarhit[30] = {
    0, 0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A,
    0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113, 0x4114, 0x4115, 0x4116,
    0x4117, 0x4117, 0x4119, 0x411A, 0x8004, 0x54,
};
hero_seq KAN_seq_polardead[4] = {
    0, 0x8004, 0x50, 0,
};
hero_seq KAN_seq_polaridle[24] = {
    0, 0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005, 0x4006, 0x4007, 0x4008, 0x4009, 0x400A,
    0x400B, 0x400C, 0x400D, 0x400E, 0x400F, 0x4010, 0x4011, 0x4012, 0x4013, 0x8000, 0, 0,
};
hero_seq KAN_seq_polardazed[32] = {
    0, 0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A,
    0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113, 0x4114, 0x4115, 0x4116,
    0x4117, 0x4118, 0x8001, 0x4119, 0x411A, 0x8000, 0, 0,
};
hero_seq KAN_seq_polarattack[32] = {
    0, 0x4200, 0x8008, 0xDE, 0x4201, 0x4202, 0x4203, 0x4204, 0x4205, 0x4206, 0x4207, 0x4208, 0x4209,
    0x420A, 0x420B, 0x420C, 0x420D, 0x420E, 0x420F, 0x8008, 0xE0, 0x4210, 0x4211, 0x4212, 0x4213,
    0x4214, 0x4215, 0x4216, 0x4217, 0x4218, 0x8004, 0x49,
};
hero_seq KAN_seq_polarrun[20] = {
    0, 0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x8000, 0, 0,
};
hero_seq KAN_seq_polarfall[46] = {
    0, 0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A,
    0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113, 0x4114, 0x4115, 0x4116,
    0x4117, 0x4118, 0x4119, 0x411A, 0x411B, 0x411C, 0x411D, 0x411E, 0x411F, 0x4120, 0x4121, 0x4122,
    0x4123, 0x4124, 0x4125, 0x4126, 0x4127, 0x4128, 0x4129, 0x8004, 0x50, 0,
};
s32 PolarRoundState = 0;
s32 PolarFirstFlag = 0;
shell_setup polar_shell_info = {
    0x1000, 0x10, 0x2000, 0xA0, 0x104, 0x92, VramAreas, VramNodePool, 0x20, 0x4B0,
};
/* the other worlds' (0x0012, 0x0014..0x0016; the engine's records at 0x80099020..68) */
shell_setup polar_shell_info_b = {
    0xC18, 0x10, 0x2000, 0x78, 0x104, 0x6A, VramAreas, VramNodePool, 0x20, 0x4B0,
};
s32 PolarAIParams[28] = {
    0x1000, 0x1800, 0x2000, 0x1000, 0x1600, 0x1C00, 0x1000, 0x1400, 0x1800, 0x1000, 0x1200, 0x1400, 0x19A, 0x19A, 0x666, 0x999, 0xC8, 0x19A, 0x666, 0x999, 0x20, 0x24, 0x28, 0x2C, 0, 4, 8, 0x10,
};
s32 PolarArenaRadius2 = 0x400000;
VECTOR PolarCentre = {0};
s32 PolarUnused0[2] = { 0, 0x800 };
s32 PolarClock = 0;
s32 (*PolarAITests[5])(s32 player) = { TestAttackChance, TestHuntChance, TestWanderChance, TestChaseChance, TestBombChance };
s32 PolarIceRadius = 0x658;
s32 PolarAnimFlag = 0;
s32 PolarAIChances[72] = {
    0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0x800, 0xC00, 0x800, 0x800, 0x800,
    0x800, 0x800, 0x800, 0, 0, 0, 0, 0, 0, 0, 0, 0x800, 0x1000, 0x1000, 0x800, 0, 0, 0x200,
    0x200, 0x800, 0, 0, 0, 0, 0, 0x800, 0x800, 0x800, 0x800, 0x400, 0x400, 0x400, 0x400, 0x400,
    0x400, 0x800, 0x800, 0x800, 0x800, 0x800, 0x800, 0x800, 0x800, 0x1000, 0x1000, 0x1000,
    0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x800, 0x1000, 0x1000, 0x1000, 0x200, 0x200, 0x200,
    0x200,
};
s16 PolarPreLoadDB[8] = { 0x1C, 0x82, -1, 0, 0, 0, 0, 0 };
char *PolarAnims[7] = {
    (char *)PolarAnimLight, (char *)PolarAnimRun, (char *)PolarAnimBreathe, (char *)PolarAnimFall, (char *)PolarAnimPush, (char *)PolarAnimTaunt, NULL,
};
char *PolarAnimsB[6] = {
    (char *)PolarAnimRun, (char *)PolarAnimBreathe, (char *)PolarAnimFall, (char *)PolarAnimPush, (char *)PolarAnimTaunt, NULL,
};
char *PolarAnimsC[10] = {
    (char *)PolarAnimSkate, (char *)PolarAnimHit, (char *)PolarAnimBarge, (char *)PolarAnimLight, (char *)PolarAnimRun, (char *)PolarAnimBreathe, (char *)PolarAnimFall, (char *)PolarAnimPush, (char *)PolarAnimTaunt, NULL,
};
s16 PolarLevelDB0[4] = { 0x80, 0x85, -1, 0 };
s16 PolarLevelDB1[2] = { 0x80, -1 };
s16 PolarLevelDB2[2] = { 0x80, -1 };
s16 PolarLevelDB3[2] = { -1, 0 };
s16 PolarLevelDB4[4] = { 0x80, 0x91, 0x17, -1 };
WWInf PolarWorldInfo[10] = {
    { 0x7F, PolarAnims, PolarLevelDB0, 1 },
    { 0x83, PolarAnims, PolarLevelDB1, 1 },
    { 0x84, PolarAnimsB, PolarLevelDB2, 1 },
    { 0x86, PolarAnimsC, PolarLevelDB3, 1 },
    { 0x8D, PolarAnims, PolarLevelDB4, 1 },
    { 0x8F, PolarAnims, PolarLevelDB2, 1 },
    { 0x93, PolarAnims, PolarLevelDB1, 1 },
    { 0x96, PolarAnimsC, PolarLevelDB3, 1 },
    { 0x97, PolarAnims, PolarLevelDB2, 1 },
    { 0x98, PolarAnims, PolarLevelDB1, 1 },
};
s32 PolarEndFiles[15] = {
    (s32)&HeroModelBank[DB_Font], (s32)&DatIndex[2 * 445], (s32)&DatIndex[2 * 444], (s32)&HeroModelBank[DB_Loading], (s32)&DatIndex[2 * 441],
    (s32)&DatIndex[2 * 440], (s32)&HeroModelBank[DB_Interface], (s32)&DatIndex[2 * 443], (s32)&DatIndex[2 * 442], (s32)&HeroModelBank[DB_PolarObjects],
    (s32)&DatIndex[2 * 439], (s32)&DatIndex[2 * 438], 0, 0, 0,
};
s32 PolarFiles[18] = {
    (s32)&HeroModelBank[DB_Font], (s32)&DatIndex[2 * 437], (s32)&DatIndex[2 * 436], (s32)&HeroModelBank[DB_Loading], (s32)&DatIndex[2 * 433],
    (s32)&DatIndex[2 * 432], (s32)&HeroModelBank[DB_Interface], (s32)&DatIndex[2 * 435], (s32)&DatIndex[2 * 434], (s32)&HeroModelBank[DB_PolarObjects],
    (s32)&DatIndex[2 * 431], (s32)&DatIndex[2 * 430], (s32)&HeroModelBank[DB_Polar], (s32)&DatIndex[2 * 429], (s32)&DatIndex[2 * 428],
    0, 0, 0,
};
s32 PolarBossFiles[24] = {
    (s32)&HeroModelBank[DB_Font], (s32)&DatIndex[2 * 427], (s32)&DatIndex[2 * 426], (s32)&HeroModelBank[DB_Loading], (s32)&DatIndex[2 * 423],
    (s32)&DatIndex[2 * 422], (s32)&HeroModelBank[DB_Interface], (s32)&DatIndex[2 * 425], (s32)&DatIndex[2 * 424], (s32)&HeroModelBank[DB_PolarObjects],
    (s32)&DatIndex[2 * 421], (s32)&DatIndex[2 * 420], (s32)&HeroModelBank[DB_Polar], (s32)&DatIndex[2 * 419], (s32)&DatIndex[2 * 418],
    (s32)&HeroModelBank[DB_RoboPolar], (s32)&DatIndex[2 * 417], (s32)&DatIndex[2 * 416], (s32)&HeroModelBank[DB_Powerups], (s32)&DatIndex[2 * 415],
    (s32)&DatIndex[2 * 414], 0, 0,
};
/* the levels' files, in the leak's order (Day, Night, Cut, their challenges, End and its challenge,
 * the boss): the exe's file group and the model list; the engine's table at 0x800A0030 points at
 * modes 0x12..0x16's */
file_group_load_info file_group_POLARDAY = { &FileGroups[17], PolarFiles };
file_group_load_info file_group_POLARNGT = { &FileGroups[17], PolarFiles };
file_group_load_info file_group_POLARCUT = { &FileGroups[17], PolarFiles };
file_group_load_info file_group_POLARCHA = { &FileGroups[17], PolarFiles };
file_group_load_info file_group_POLARCUC = { &FileGroups[17], PolarFiles };
file_group_load_info file_group_POLARNGC = { &FileGroups[17], PolarFiles };
file_group_load_info file_group_POLAREND = { &FileGroups[18], PolarEndFiles };
file_group_load_info file_group_POLARENC = { &FileGroups[18], PolarEndFiles };
file_group_load_info file_group_POLARBSS = { &FileGroups[16], PolarBossFiles };
u16 PolarLUPtab[8] = { 3, 0x20D, 0x1601, 0x1603, 0x1605, 0x1607, 0x160A, 0x1619 };
TrigLookUp PolarLUP = { PolarLUPtab, 8 };
PreMap POLAR_PREMAP = { PolarPreLoadDB, PolarWorldInfo, &PolarLUP };
PolarOwner *PolarBeamOwner = 0;
u16 PolarHeroModels[10] = { 0, 0x87, 0x88, 0x89, 0x8C, 0x8A, 0x95, 0x8B, 0x94, 0 };
u16 PolarHeroModelsB[10] = { 0, 0x11, 0x12, 0x13, 0x16, 0x14, 0x90, 0x15, 0x92, 0 };
u16 PolarHeroModelsCut[10] = { 0, 0x99, 0x9A, 0x9B, 0x9E, 0x9C, 0xA0, 0x9D, 0x9F, 0 };
NAS *PolarBomb = 0;
subengine *bossmapsub = 0;
SceneObj *PolarSceneA = 0;
SceneObj *PolarSceneB = 0;
s32 PolarSceneWord0 = 0;
s32 PolarSceneWord1 = 0;
s32 PolarSceneWord2 = 0x100;
s32 PolarSceneWord3 = 0x10;
NAS *BombMaster = 0;
s32 PolarVictorySeqs[10] = {
    (s32)&PolarAnimWin, 0x46, (s32)&PolarAnimWin, 0x47,
    (s32)&PolarAnimWin, 0xD, (s32)&PolarAnimWin, 0xD, 1, 2,
};
s32 PolarAttackOdds[33] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 8, 0xC, 0x10, 0x14, 0x18, 0x1C, 0x20, 0x24, 0x28, 0x2C, 0x30,
    0x34, 0x38, 0x3C, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
};
s32 PolarRollOdds[33] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 6, 8, 0xA, 0xC, 0xE, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1A,
    0x1C, 0x1E, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
};
s32 PolarMeltOffsets[8] = { 0, 0x400, 0x800, 0xC00, 0x600, 0xA00, 0xE00, 0x200 };
u16 PolarHeroSfx[32] = {
    0x37, 0x28, 0x38, 0x37, 0x28, 0x38, 0x37, 0x28, 0x38, 0x37, 0x28, 0x38, 0x37, 0x20, 0x2C,
    0x28, 0x20, 0x2C, 0x28, 0x20, 0x2C, 0x28, 0x20, 0x2C, 0x28, 0x20, 0x2C, 0, 0, 0, 0, 0,
};
NAS *polarbossnas = 0;
s32 PolarBeamTimer = 0;
s32 PolarUpdateWord = 0;
u8 *PolarBears = 0;
u32 BeamFrames[4] = { 0, 1, 6, 5 };
s32 PolarUnused1[3] = {0};
s32 BeamState = 0;
s32 BeamTimer = 0;
s32 BeamTarget = 0;
s32 UkaMaskFrames[4] = { 0, 8, 2, 5 };
s32 UkaMaskTimer = 0;
PolarProx *PolarProximity = 0;
s32 WalrusFrames[26] = {
    0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4001, 0x4002, 0x4003, 0x4004,
    0x4005, 0x4006, 0x4007, 0x4008, 0x4009, 0x400A, 0x400B, 0x400C, 0x400D, 0x400E, 0x400F,
    0x4010, 0x4011, 0x4012, 0x8000,
};
s32 WalrusSteps0[2] = { 0x3E74013, 0x8000 };
s32 WalrusSteps1[14] = {
    0x4014, 0x4015, 0x4016, 0x4017, 0x4018, 0x4019, 0x401A, 0x401B, 0x401C, 0x401D, 0x401E,
    0x401F, 0x18005, 0x4020,
};
s32 WalrusSteps2[17] = {
    0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x8000,
};
s32 WalrusSteps3[40] = {
    0x4100, 0x4101, 0x4102, 0x4103, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109, 0x410A,
    0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113, 0x4114, 0x4115,
    0x4116, 0x4117, 0x4118, 0x4119, 0x411A, 0x411B, 0x411C, 0x411D, 0x411E, 0x411F, 0x4120,
    0x4121, 0x4122, 0x4123, 0x4124, 0x4125, 0x18005, 0x4126,
};
s32 WalrusAnimList[3] = { (s32)&WalrusFrames, 0, 0 };
ScriptInit WalrusScriptInit = { 0, (void *)&WalrusAnimList, 0x402 };
s32 WalrusState = 0x240;
s32 WalrusFishSteps[17] = {
    0x4180, 0x4181, 0x4182, 0x4183, 0x4184, 0x4185, 0x4186, 0x4187, 0x4188, 0x4189, 0x418A,
    0x418B, 0x418C, 0x418D, 0x418E, 0x418F, 0x8001,
};
s32 WalrusFishAnimList[3] = { (s32)&WalrusFishSteps, 0, 0 };
ScriptInit WalrusFishScriptInit = { 0, (void *)&WalrusFishAnimList, 0x402 };
s32 BearDeathSteps0[8] = { 0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005, 0x4006, 0x8001 };
s32 BearDeathSteps1[2] = { 0x4006, 0x8000 };
s32 BearDeathAnimList[4] = { (s32)&BearDeathSteps0, (s32)&BearDeathSteps1, 0, 0 };
ScriptInit BearDeathScriptInit = { 0, (void *)&BearDeathAnimList, 0x402 };
s32 PolarBossSteps0[7] = { 0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005, 0x8000 };
s32 PolarBossSteps1[43] = {
    0x4080, 0x4081, 0x4082, 0x4083, 0x18005, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089,
    0x408A, 0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x4090, 0x4091, 0x4092, 0x4093, 0x4094,
    0x4095, 0x4096, 0x4097, 0x4098, 0x4099, 0x409A, 0x409B, 0x409C, 0x409D, 0x409E, 0x409F,
    0x40A0, 0x40A1, 0x40A2, 0x40A3, 0x40A4, 0x40A5, 0x40A6, 0x40A7, 0x40A8, 0x8001,
};
s32 PolarBossSteps2[22] = {
    0x4400, 0x4401, 0x4402, 0x4403, 0x4404, 0x4405, 0x4406, 0x4407, 0x4408, 0x4409, 0x440A,
    0x440B, 0x440C, 0x440D, 0x440E, 0x440F, 0x4410, 0x4411, 0x4412, 0x4413, 0x4414, 0x8001,
};
s32 PolarBossSteps3[43] = {
    0x4100, 0x4101, 0x4102, 0x4103, 0x18005, 0x4104, 0x4105, 0x4106, 0x4107, 0x4108, 0x4109,
    0x410A, 0x410B, 0x410C, 0x410D, 0x410E, 0x410F, 0x4110, 0x4111, 0x4112, 0x4113, 0x4114,
    0x4115, 0x4116, 0x4117, 0x4118, 0x4119, 0x411A, 0x411B, 0x411C, 0x411D, 0x411E, 0x411F,
    0x4120, 0x4121, 0x4122, 0x4123, 0x4124, 0x4125, 0x4126, 0x4127, 0x4128, 0x8001,
};
s32 PolarBossSteps4[43] = {
    0x4180, 0x4181, 0x4182, 0x4183, 0x18005, 0x4184, 0x4185, 0x4186, 0x4187, 0x4188, 0x4189,
    0x418A, 0x418B, 0x418C, 0x418D, 0x418E, 0x418F, 0x4190, 0x4191, 0x4192, 0x4193, 0x4194,
    0x4195, 0x4196, 0x4197, 0x4198, 0x4199, 0x419A, 0x419B, 0x419C, 0x419D, 0x419E, 0x419F,
    0x41A0, 0x41A1, 0x41A2, 0x41A3, 0x41A4, 0x41A5, 0x41A6, 0x41A7, 0x41A8, 0x8001,
};
s32 PolarBossSteps5[42] = {
    0x4200, 0x4201, 0x4202, 0x4203, 0x4204, 0x4205, 0x4206, 0x4207, 0x4208, 0x4209, 0x420A,
    0x420B, 0x420C, 0x420D, 0x420E, 0x420F, 0x4210, 0x4211, 0x4212, 0x4213, 0x4214, 0x4215,
    0x4216, 0x4217, 0x4218, 0x4219, 0x421A, 0x421B, 0x421C, 0x421D, 0x421E, 0x421F, 0x4220,
    0x4221, 0x4222, 0x4223, 0x4224, 0x4225, 0x4226, 0x4227, 0x4228, 0x8001,
};
s32 PolarBossAnimList[13] = {
    (s32)&PolarBossSteps0, (s32)&PolarBossSteps1, (s32)&PolarBossSteps0, (s32)&PolarBossSteps3, (s32)&PolarBossSteps0,
    (s32)&PolarBossSteps4, (s32)&PolarBossSteps0, (s32)&PolarBossSteps5, (s32)&PolarBossSteps0, (s32)&PolarBossSteps2,
    (s32)&PolarBossSteps0, 0, 0,
};
ScriptInit PolarBossScriptInit = { 0, (void *)&PolarBossAnimList, 0x402 };
s32 BearBossSteps0[21] = {
    0x4280, 0x4281, 0x4282, 0x4283, 0x4284, 0x4285, 0x4286, 0x4287, 0x4288, 0x4289, 0x428A,
    0x428B, 0x428C, 0x428D, 0x428E, 0x428F, 0x4290, 0x4291, 0x4292, 0x4293, 0x8000,
};
s32 BearBossSteps1[42] = {
    0x4300, 0x4301, 0x4302, 0x4303, 0x4304, 0x4305, 0x4306, 0x4307, 0x4308, 0x4309, 0x430A,
    0x430B, 0x430C, 0x430D, 0x430E, 0x430F, 0x4310, 0x4311, 0x4312, 0x4313, 0x4314, 0x4315,
    0x4316, 0x4317, 0x4318, 0x4319, 0x431A, 0x431B, 0x431C, 0x431D, 0x431E, 0x431F, 0x4320,
    0x4321, 0x4322, 0x4323, 0x4324, 0x4325, 0x4326, 0x4327, 0x4328, 0x8001,
};
s32 BearBossSteps2[22] = {
    0x4380, 0x4381, 0x4382, 0x4383, 0x4384, 0x4385, 0x4386, 0x4387, 0x4388, 0x4389, 0x438A,
    0x438B, 0x438C, 0x438D, 0x438E, 0x438F, 0x4390, 0x4391, 0x4392, 0x4393, 0x4394, 0x8001,
};
s32 BearBossSteps3[31] = {
    0x4480, 0x4481, 0x4482, 0x4483, 0x4484, 0x4485, 0x4486, 0x4487, 0x4488, 0x4489, 0x448A,
    0x448B, 0x448C, 0x448D, 0x448E, 0x448F, 0x4490, 0x4491, 0x4492, 0x4493, 0x4494, 0x4495,
    0x4496, 0x4497, 0x4498, 0x4499, 0x449A, 0x449B, 0x449C, 0x449D, 0x8001,
};
s32 BearBossSteps4[2] = { 0x449D, 0x8000 };
s32 BearBossSteps5[46] = {
    0x4500, 0x4501, 0x4502, 0x4503, 0x4504, 0x4505, 0x4506, 0x4507, 0x4508, 0x4509, 0x450A,
    0x450B, 0x450C, 0x450D, 0x450E, 0x450F, 0x4510, 0x4511, 0x4512, 0x4513, 0x4514, 0x4515,
    0x4516, 0x4517, 0x4518, 0x4519, 0x451A, 0x451B, 0x451C, 0x451D, 0x451E, 0x451F, 0x4520,
    0x4521, 0x4522, 0x4523, 0x4524, 0x4525, 0x4526, 0x4527, 0x4528, 0x4529, 0x452A, 0x452B,
    0x452C, 0x8000,
};
s32 BearBossSteps6[46] = {
    0x4500, 0x4501, 0x4502, 0x4503, 0x4504, 0x4505, 0x4506, 0x4507, 0x4508, 0x4509, 0x450A,
    0x450B, 0x450C, 0x450D, 0x450E, 0x450F, 0x4510, 0x4511, 0x4512, 0x4513, 0x4514, 0x4515,
    0x4516, 0x4517, 0x4518, 0x4519, 0x451A, 0x451B, 0x451C, 0x451D, 0x451E, 0x451F, 0x4520,
    0x4521, 0x4522, 0x4523, 0x4524, 0x4525, 0x4526, 0x4527, 0x4528, 0x4529, 0x452A, 0x452B,
    0x452C, 0x8001,
};
s32 BearBossAnimList[12] = {
    (s32)&BearBossSteps0, (s32)&BearBossSteps1, (s32)&BearBossSteps0, (s32)&BearBossSteps2, (s32)&BearBossSteps0,
    (s32)&BearBossSteps3, (s32)&BearBossSteps4, (s32)&BearBossSteps5, (s32)&BearBossSteps6, (s32)&BearBossSteps0,
    0, 0,
};
ScriptInit BearBossScriptInit = { 0, (void *)&BearBossAnimList, 0x402 };
NAS *BearminatorNas = 0;
s32 BearminatorSteps0[17] = {
    0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005, 0x4006, 0x4007, 0x4008, 0x4009, 0x400A,
    0x400B, 0x400C, 0x400D, 0x400E, 0x400F, 0x8000,
};
s32 BearminatorSteps1[26] = {
    0x4080, 0x4081, 0x4082, 0x4083, 0x4084, 0x4085, 0x4086, 0x4087, 0x4088, 0x4089, 0x408A,
    0x408B, 0x408C, 0x408D, 0x408E, 0x408F, 0x4090, 0x4091, 0x4092, 0x4093, 0x4094, 0x4095,
    0x4096, 0x18005, 0x4097, 0x8000,
};
s32 BearminatorSteps2[35] = {
    0x4200, 0x4201, 0x4202, 0x4203, 0x4204, 0x4205, 0x4206, 0x4207, 0x4208, 0x4209, 0x420A,
    0x420B, 0x420C, 0x420D, 0x420E, 0x420F, 0x4210, 0x4211, 0x4212, 0x4213, 0x4214, 0x4215,
    0x4216, 0x4217, 0x4218, 0x4219, 0x421A, 0x421B, 0x421C, 0x421D, 0x421E, 0x421F, 0x18005,
    0x4220, 0x8000,
};
s32 BearminatorSteps3[17] = {
    0x4180, 0x4181, 0x4182, 0x4183, 0x4183, 0x4185, 0x4186, 0x4187, 0x4188, 0x4189, 0x418A,
    0x418B, 0x418C, 0x418D, 0x418E, 0x418F, 0x8000,
};
s32 BearminatorSteps4[17] = {
    0x4180, 0x4181, 0x4182, 0x4183, 0x4184, 0x4185, 0x4186, 0x4187, 0x4188, 0x4189, 0x418A,
    0x418B, 0x418C, 0x418D, 0x418E, 0x418F, 0x8001,
};
s32 BearminatorAnimList[7] = {
    (s32)&BearminatorSteps0, (s32)&BearminatorSteps1, (s32)&BearminatorSteps4, (s32)&BearminatorSteps3, (s32)&BearminatorSteps2,
    0, 0,
};
ScriptInit BearminatorScriptInit = { 0, (void *)&BearminatorAnimList, 0x402 };
s32 PolarAIPace[4] = {0};
s32 PolarInitWord = 0;
s32 PolarSceneCount = 0;
s32 PolarSceneTimer = 0;
MATRIX PolarSceneMatrix = {0};
s32 PolarAIFlags[4] = {0};
s32 PolarBeamMode = 0;
s32 PolarUnused2[2] = {0};
s32 BeamCount = 0;
s32 BeamDelay = 0;
SceneObj *PolarIceObjects[80] = {0};
PolarRiserObj *UkaMaskRisers[16] = {0};
SceneObj *UkaMaskObjects[16] = {0};
u16 PolarShadowFrames0[31][2] = {
    { 0x4180, 0 }, { 0x4181, 0 }, { 0x4182, 0 }, { 0x4183, 0 }, { 0x4184, 0 }, { 0x4185, 0 },
    { 0x4186, 0 }, { 0x4187, 0 }, { 0x4188, 0 }, { 0x4189, 0 }, { 0x418A, 0 }, { 0x418B, 0 },
    { 0x418C, 0 }, { 0x418D, 0 }, { 0x4188, 0 }, { 0x4189, 0 }, { 0x418A, 0 }, { 0x418B, 0 },
    { 0x418C, 0 }, { 0x418D, 0 }, { 0x418E, 0 }, { 0x418F, 0 }, { 0x4190, 0 }, { 0x4191, 0 },
    { 0x4192, 0 }, { 0x4193, 0 }, { 0x4194, 0 }, { 0x4195, 0 }, { 0x4196, 0 }, { 0x4197, 0 },
    { 0x4198, 0 },
};
u16 PolarShadowFrames1[26][2] = {
    { 0x4080, 0 }, { 0x4081, 0 }, { 0x4082, 0 }, { 0x4083, 0 }, { 0x4084, 0 }, { 0x4085, 0 },
    { 0x4086, 0 }, { 0x4087, 0 }, { 0x4088, 0 }, { 0x4088, 0 }, { 0x4088, 0 }, { 0x4088, 0 },
    { 0x4088, 0 }, { 0x4088, 0 }, { 0x4088, 0 }, { 0x4089, 0 }, { 0x408A, 0 }, { 0x408B, 0 },
    { 0x408C, 0 }, { 0x408D, 0 }, { 0x408E, 0 }, { 0x408F, 0 }, { 0x4090, 0 }, { 0x4091, 0 },
    { 0x4092, 0 }, { 0x4093, 0 },
};
u16 PolarShadowFrames2[63][2] = {
    { 0x4080, 0 }, { 0x4081, 0 }, { 0x4082, 0 }, { 0x4083, 0 }, { 0x4084, 0 }, { 0x4085, 0 },
    { 0x4086, 0 }, { 0x4087, 0 }, { 0x4088, 0 }, { 0x4088, 0 }, { 0x4088, 0 }, { 0x4088, 0 },
    { 0x4088, 0 }, { 0x4088, 0 }, { 0x4088, 0 }, { 0x4088, 0 }, { 0x4088, 0 }, { 0x4088, 0 },
    { 0x4089, 0 }, { 0x408A, 0 }, { 0x408B, 0 }, { 0x408C, 0 }, { 0x408D, 0 }, { 0x408E, 0 },
    { 0x408F, 0 }, { 0x4090, 0 }, { 0x4091, 0 }, { 0x4092, 0 }, { 0x4093, 0 }, { 0x4080, 0 },
    { 0x4080, 0 }, { 0x4080, 0 }, { 0x4080, 0 }, { 0x4080, 0 }, { 0x4080, 0 }, { 0x4080, 0 },
    { 0x4080, 0 }, { 0x4080, 0 }, { 0x4081, 0 }, { 0x4082, 0 }, { 0x4083, 0 }, { 0x4084, 0 },
    { 0x4085, 0 }, { 0x4086, 0 }, { 0x4087, 0 }, { 0x4088, 0 }, { 0x4088, 0 }, { 0x4088, 0 },
    { 0x4088, 0 }, { 0x4088, 0 }, { 0x4088, 0 }, { 0x4088, 0 }, { 0x4089, 0 }, { 0x408A, 0 },
    { 0x408B, 0 }, { 0x408C, 0 }, { 0x408D, 0 }, { 0x408E, 0 }, { 0x408F, 0 }, { 0x4090, 0 },
    { 0x4091, 0 }, { 0x4092, 0 }, { 0x4093, 0 },
};
u16 PolarShadowFrames3[26][2] = {
    { 0x4080, 0 }, { 0x4081, 0 }, { 0x4082, 0 }, { 0x4083, 0 }, { 0x4084, 0 }, { 0x4085, 0 },
    { 0x4086, 0 }, { 0x4087, 0 }, { 0x4088, 0 }, { 0x4089, 0 }, { 0x408A, 0 }, { 0x408B, 0 },
    { 0x408C, 0 }, { 0x408D, 0 }, { 0x408E, 0 }, { 0x408F, 0 }, { 0x4090, 0 }, { 0x4091, 0 },
    { 0x4092, 0 }, { 0x4093, 0 }, { 0x4080, 0 }, { 0x4081, 0 }, { 0x4082, 0 }, { 0x4083, 0 },
    { 0x4084, 0 }, { 0x4085, 0 },
};
u16 PolarShadowFrames4[41][2] = {
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 },
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4186, 0 },
    { 0x4187, 0 }, { 0x4188, 0 }, { 0x4189, 0 }, { 0x4189, 0 }, { 0x4189, 0 }, { 0x4189, 0 },
    { 0x4189, 0 }, { 0x4189, 0 }, { 0x4189, 0 }, { 0x4189, 0 }, { 0x4189, 0 }, { 0x4189, 0 },
    { 0x4189, 0 }, { 0x4188, 0 }, { 0x4187, 0 }, { 0x4186, 0 }, { 0x4180, 0 }, { 0x4180, 0 },
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 },
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 },
};
u16 PolarShadowFrames5[35][2] = {
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 },
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4186, 0 },
    { 0x4187, 0 }, { 0x4188, 0 }, { 0x4189, 0 }, { 0x4189, 0 }, { 0x4189, 0 }, { 0x4189, 0 },
    { 0x4189, 0 }, { 0x4189, 0 }, { 0x4189, 0 }, { 0x4188, 0 }, { 0x4187, 0 }, { 0x4186, 0 },
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 },
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 },
};
u16 PolarShadowFrames6[42][2] = {
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 },
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4186, 0 }, { 0x4187, 0 }, { 0x4188, 0 },
    { 0x4189, 0 }, { 0x4189, 0 }, { 0x4189, 0 }, { 0x4189, 0 }, { 0x4189, 0 }, { 0x4189, 0 },
    { 0x4189, 0 }, { 0x4189, 0 }, { 0x4189, 0 }, { 0x4188, 0 }, { 0x4187, 0 }, { 0x4186, 0 },
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 },
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 },
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 },
};
u16 PolarShadowFrames7[46][2] = {
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 },
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4200, 0 }, { 0x4201, 0 },
    { 0x4202, 0 }, { 0x4203, 0 }, { 0x4204, 0 }, { 0x4203, 0 }, { 0x4202, 0 }, { 0x4201, 0 },
    { 0x4200, 0 }, { 0x4200, 0 }, { 0x4201, 0 }, { 0x4202, 0 }, { 0x4203, 0 }, { 0x4204, 0 },
    { 0x4203, 0 }, { 0x4202, 0 }, { 0x4201, 0 }, { 0x4200, 0 }, { 0x4200, 0 }, { 0x4201, 0 },
    { 0x4202, 0 }, { 0x4203, 0 }, { 0x4204, 0 }, { 0x4203, 0 }, { 0x4202, 0 }, { 0x4201, 0 },
    { 0x4200, 0 }, { 0x4200, 0 }, { 0x4201, 0 }, { 0x4202, 0 }, { 0x4203, 0 }, { 0x4204, 0 },
    { 0x4203, 0 }, { 0x4202, 0 }, { 0x4201, 0 }, { 0x4200, 0 },
};
u16 PolarShadowFrames8[46][2] = {
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 },
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4200, 0 }, { 0x4200, 0 },
    { 0x4201, 0 }, { 0x4201, 0 }, { 0x4202, 0 }, { 0x4202, 0 }, { 0x4203, 0 }, { 0x4203, 0 },
    { 0x4204, 0 }, { 0x4204, 0 }, { 0x4203, 0 }, { 0x4203, 0 }, { 0x4202, 0 }, { 0x4202, 0 },
    { 0x4201, 0 }, { 0x4201, 0 }, { 0x4200, 0 }, { 0x4200, 0 }, { 0x4200, 0 }, { 0x4200, 0 },
    { 0x4201, 0 }, { 0x4201, 0 }, { 0x4202, 0 }, { 0x4202, 0 }, { 0x4203, 0 }, { 0x4203, 0 },
    { 0x4204, 0 }, { 0x4204, 0 }, { 0x4203, 0 }, { 0x4203, 0 }, { 0x4202, 0 }, { 0x4202, 0 },
    { 0x4201, 0 }, { 0x4201, 0 }, { 0x4200, 0 }, { 0x4200, 0 },
};
u16 PolarShadowFrames9[50][2] = {
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 },
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4200, 0 }, { 0x4201, 0 }, { 0x4202, 0 },
    { 0x4203, 0 }, { 0x4204, 0 }, { 0x4203, 0 }, { 0x4202, 0 }, { 0x4201, 0 }, { 0x4200, 0 },
    { 0x4201, 0 }, { 0x4202, 0 }, { 0x4203, 0 }, { 0x4204, 0 }, { 0x4203, 0 }, { 0x4202, 0 },
    { 0x4201, 0 }, { 0x4200, 0 }, { 0x4201, 0 }, { 0x4202, 0 }, { 0x4203, 0 }, { 0x4204, 0 },
    { 0x4203, 0 }, { 0x4202, 0 }, { 0x4203, 0 }, { 0x4204, 0 }, { 0x4203, 0 }, { 0x4202, 0 },
    { 0x4201, 0 }, { 0x4200, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 },
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 },
    { 0x4180, 0 }, { 0x4180, 0 },
};
u16 PolarShadowFrames10[63][2] = {
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 },
    { 0x4180, 0 }, { 0x4200, 0 }, { 0x4200, 0 }, { 0x4200, 0 }, { 0x4200, 0 }, { 0x4200, 0 },
    { 0x4200, 0 }, { 0x4200, 0 }, { 0x4200, 0 }, { 0x4200, 0 }, { 0x4200, 0 }, { 0x4200, 0 },
    { 0x4200, 0 }, { 0x4200, 0 }, { 0x4200, 0 }, { 0x4200, 0 }, { 0x4200, 0 }, { 0x4200, 0 },
    { 0x4200, 0 }, { 0x4200, 0 }, { 0x4200, 0 }, { 0x4200, 0 }, { 0x4200, 0 }, { 0x4200, 0 },
    { 0x4200, 0 }, { 0x4200, 0 }, { 0x4200, 0 }, { 0x4200, 0 }, { 0x4200, 0 }, { 0x4200, 0 },
    { 0x4200, 0 }, { 0x4200, 0 }, { 0x4200, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 },
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 },
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 },
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 },
    { 0x4180, 0 }, { 0x4180, 0 }, { 0x4180, 0 },
};
s32 PolarHeroMass[4] = {0};
s32 PolarShadowState[4] = {0};

s32 PolarAI_PickVictim(s32 arg0) {
    s32 start;
    s32 r;
    s32 i;
    s32 j;
    s32 k;
    s32 total;
    s32 result;
    s32 v;
    s32 neg;

    start = Random(4);
    r = Random(0x1000);
    neg = -1;
    i = start;
    while (1) {
        if (i >= start + 4) {
            break;
        }
        if ((i & 3) == arg0) {
            goto next;
        }
        j = i & 3;
        result = 0;
        total = 0;
        if (Heroes[j].hero_status & 0x2000) {
            v = -1;
        } else {
            for (k = 0; k < 4; k++) {
                if (k != arg0 && !(Heroes[k].hero_status & 0x2000)) {
                    total += AiPlayerSkill[k].quality << 7;
                }
            }
            if (total != 0) {
                result = (AiPlayerSkill[j].quality << 19) / total;
            }
            v = result;
        }
        if (r < v) {
            return neg = i & 3;
        }
    next:
        i++;
    }
    return neg;
}

void PolarInitScene(void) {
    s32 i;
    SceneObj *obj;

    i = 0;
    PolarTiltZ = 0;
    PolarTiltX = 0;
    bossmapsub = NULL;
    PolarSceneA = NULL;
    PolarSceneTimer = 0;
    obj =((SceneObj *)MapScene->subs);
    PolarSceneA = NULL;
    PolarSceneB = NULL;
    MapSceneRot.vx = 0;
    MapSceneRot.vz = 0;
    do {
        Heroes[i].hero_eng->flags |= 0x200000;
        i++;
    } while (i < 4);
    while (1) {
        if (obj == NULL) {
            break;
        }
        if (obj->flags & 0x8000) {
            if (obj->model->colors != NULL) {
                entity_address(obj->index, (u8 *)obj->model->colors);
                if (((PolarModeView *)GameInfo)->mode == 0x12) {
                    if (!(((PolarModeView *)GameInfo)->flags & 0x80)) {
                        if (obj->index == 0x5001 || obj->index == 0x5004) {
                            obj->flags |= 0x200000;
                        }
                    } else {
                        if (obj->index == 0x5001 || obj->index == 0x5004) {
                            obj->flags |= 0x200000;
                        }
                        if (obj->index == 0x5005) {
                            PolarSceneA = obj;
                        }
                        if (obj->index == 0x5003) {
                            PolarSceneB = obj;
                            PolarSceneCount = obj->unk8;
                        }
                    }
                }
                if (((PolarModeView *)GameInfo)->mode == 0x16 &&obj->index == 0x5003) {
                    bossmapsub = (subengine *)obj;
                    obj->flags |= 0x200000;
                }
            }
        }
        obj = obj->next;
    }
}

void PolarUpdateScene(void)
{
  s32 i;
  s32 sz;
  s32 sx;
  s32 n;
  s32 count;
  s32 k;
  s32 r;
  s32 step;
  s32 sign;
  s32 a;
  s32 b;
  HeroInfo *h;
  u8 *p;
  EXTengine *eng;
  PolarMissileExt *ext;
  SceneObj *o;
  o = PolarSceneA;
  if (o != 0)
  {
    o->unk52 = 1;
    o->unk51 = 0;
    o->unk12 += 6;
  }
  if (PolarSceneB != 0)
  {
    PolarSceneTimer = (PolarSceneTimer + 0x20) & 0xFFF;
  }
  if (((PolarGameView *) GameInfo)->unk10 != 0)
  {
    return;
  }
  if ((((PolarModeView *) GameInfo)->mode == 0x12) || (((PolarModeView *) GameInfo)->mode == 0x16))
  {
    sx = 0;
    sz = 0;
    if (((PolarModeView *) GameInfo)->mode == 0x12)
    {
      i = 0;
      if (!(((PolarModeView *) GameInfo)->flags & 0x80))
      {

        for (i = 0; i < 4; i++)
        {
          if (!(Heroes[i].hero_status & 0xFA00))
          {
            k = Heroes[i].hero_move->mass;
            sx += Heroes[i].hero_eng->unk10.x * k;
            sz += Heroes[i].hero_eng->unk10.z * k;
          }
        }


        if ((PolarProximity != 0) && (PolarProximity->unk58 != 0))
        {
          r = PolarProximity->unk58->mode;
          if (((r == 1) || (r == 5)) || (r == 7))
          {
            eng = PolarProximity->eng;
            sx += eng->unk10.x << 10;
            sx >>= 8;
            sx /= 5;
            sz += eng->unk10.z << 10;
            sz >>= 8;
            sz /= 5;
          }
          else
          {
            sx >>= 10;
            sz >>= 10;
          }
        }
      }
    }
    else
    {
      n = 1;
      if (((PolarGameView *) GameInfo)->unk6C != 0)
      {
        n = 2;
      }
      count = 0;
      for (i = 0; i < n; i++)
      {
        if (!(Heroes[i].hero_status & 0xFA00))
        {
          sx += Heroes[i].hero_eng->unk10.x;
          sz += Heroes[i].hero_eng->unk10.z;
        }
      }

      p = PolarBears;
      loop:
      if (p != 0)
      {
        if ((((NAS *) p)->Engine != 0) && (((NAS *) p)->Engine->unk10.y == 0))
        {
          ext = FindExtension((NAS *) p, 0x10000000);
          if (ext != 0)
          {
            sx += ((NAS *) p)->Engine->unk10.x;
            sz += ((NAS *) p)->Engine->unk10.z;
            if (ext->unk4C != 0)
            {
              sx *= ext->unk4C;
              sz *= ext->unk4C;
              sx >>= 6;
              sz >>= 6;
              ext->unk4C = (ext->unk4C * 200) >> 8;
            }
          }
          count++;
        }
        p = *((u8 **) (p + 0x20));
        goto loop;
      }

      sx /= count + 1;
      sz /= count + 1;
    }
    PolarTiltZ -= ((PolarTiltZ - (sx / 15)) << 20) >> 22;
    PolarTiltX -= ((PolarTiltX - ((-sz) / 16)) << 20) >> 22;
    if (PolarTiltZ < (-0x80))
    {
      PolarTiltZ = -0x80;
    }
    if (PolarTiltZ > 0x80)
    {
      PolarTiltZ = 0x80;
    }
    if (PolarTiltX < (-0x40))
    {
      PolarTiltX = -0x40;
    }
    if (PolarTiltX > 0x40)
    {
      PolarTiltX = 0x40;
    }
    MapSceneRot.vy = 0;
    MapSceneRot.vx = PolarTiltX;
    MapSceneRot.vz = PolarTiltZ;
    if ((((PolarModeView *) GameInfo)->flags & 0x80) && (((PolarModeView *) GameInfo)->mode == 0x12))
    {
      a = (PolarSceneWord2 * rcossin_tbl[(PolarSceneWord0 - 0x400) & 0xFFF].unk2) >> 12;
      b = (PolarSceneWord2 * rcossin_tbl[PolarSceneWord0 & 0xFFF].unk2) >> 12;
      PolarSceneWord0 = (rcossin_tbl[(PolarSceneWord1 - 0x400) & 0xFFF].unk2 * 3) >> 2;
      PolarSceneWord1 = (PolarSceneWord1 + PolarSceneWord3) & 0xFFF;
      if (PolarSceneWord1 == 0)
      {
        r = Random(4);
        step = (r == 0) * 0x10;
        if (r == 1)
        {
          step = 0x10;
        }
        if (r == 2)
        {
          step = 0x20;
        }
        if (r == 3)
        {
          step = 0x20;
        }
        PolarSceneWord2 = 0x100;
        sign = Random(2);
        if (sign == 0)
        {
          PolarSceneWord3 = step;
        }
        if (sign == 1)
        {
          PolarSceneWord3 = -step;
        }
      }
      PolarSceneWord0 &= 0xFFF;
      MapSceneRot.vx -= ((a - MapSceneRot.vx) << 20) >> 22;
      MapSceneRot.vz -= ((b - MapSceneRot.vz) << 20) >> 22;
    }
    RotMatrix(&MapSceneRot, &PolarSceneMatrix);
  }
  else
  {
    MapSceneRot.vx = 0;
    MapSceneRot.vz = 0;
    MapSceneRot.vy = 0;
  }
}

bool IsPtInALLZone(SLONG_XYZ *arg0, s32 arg1) {
    MATRIX m;
    SVECTOR rot;
    VECTOR v;
    VECTOR *vp;
    u8 *base;
    s32 off;
    u8 *p;
    u8 *a2;
    u8 *a3;
    TrigEntry *t;
    s32 n;
    s32 i;
    s32 cnt;
    s32 ry;

    base = ((u8 *)MapScene->map);
    off = *(s32 *)(base + 0x14);
    p = (u8 *)&((s32 *)(base + 0x14 + off))[arg1 + 1] + ((s32 *)(base + 0x14 + off))[arg1 + 1];
    if (p != NULL) {
        i = 0;
        cnt = 0;
        vp = &v;
        a2 = p + 0x50;
        a3 = p;
        ry = ((((get_angle(arg0->x, arg0->z) - 0x7D) & 0xFFF) >> 8) + 1 & 0xF) << 8;
        t = rcossin_tbl;
        n = *(s32 *)(p + 0x38);
        for (;;) {
            s32 ang;

            if (i >= n) {
                break;
            }
            ang = *(s32 *)(a3 + 0x64) - ry;
            v.vx = *(s32 *)(a2 - 8);
            v.vz = *(s32 *)a2;
            v.vy = 0;
            rot.vx = 0;
            rot.vy = ry;
            rot.vz = 0;
            RotMatrix(&rot, &m);
            ApplyMatrixLV(&m, vp, vp);
            v.vx = arg0->x - v.vx;
            v.vz = arg0->z - v.vz;
            ang = -ang;
            if (((v.vx * t[ang & 0xFFF].unk2) >> 12) - ((v.vz * t[(ang - 0x400) & 0xFFF].unk2) >> 12) < 0) {
                cnt++;
            }
            a3 += 0x38;
            i++;
            a2 += 0x38;
        }
        if (cnt == n) {
            return 0;
        }
    }
    return 1;
}

bool CheckMelted(SLONG_XYZ *arg0, s32 arg1) {
    s32 v[3];
    PolarLink *l;
    u32 sec;
    u8 *base;
    s32 off;
    u8 *p;
    u8 *a2;
    u8 *a3;
    TrigEntry *t;
    s32 n;
    s32 i;
    s32 ang;

    if (GameMode == 0x14 && PolarBeamOwner != NULL) {
        l = PolarBeamOwner->unk58;
        if (l != NULL) {
            sec = (u32)((get_angle(arg0->x, arg0->z) - 0x7D) & 0xFFF) >> 8;
            if (l->sectors[sec].state == 1) {
                arg1 = 1;
            }
            if (l->sectors[sec].state == 3 && l->sectors[sec].val >= 0x72 && !IsPtInALLZone(arg0, 2)) {
                return 0;
            }
            if (l->sectors[sec].state == 2 && l->sectors[sec].val >= 0x72 && !IsPtInALLZone(arg0, 3)) {
                return 0;
            }
            if (l->sectors[sec].state == 4 && l->sectors[sec].val >= 0x72) {
                if (!IsPtInALLZone(arg0, 2)) {
                    return 0;
                }
                if (!IsPtInALLZone(arg0, 3)) {
                    return 0;
                }
            }
        }
    }
    base = ((u8 *)MapScene->map);
    off = *(s32 *)(base + 0x14);
    p = (u8 *)&((s32 *)(base + 0x14 + off))[arg1 + 1] + ((s32 *)(base + 0x14 + off))[arg1 + 1];
    i = 0;
    if (p != NULL) {
        n = *(s32 *)(p + 0x38);
        t = rcossin_tbl;
        a2 = p + 0x50;
        a3 = p;
        for (;;) {
            if (i >= n) {
                return 1;
            }
            ang = *(s32 *)(a3 + 0x64);
            v[0] = arg0->x - *(s32 *)(a2 - 8);
            v[2] = arg0->z - *(s32 *)a2;
            ang = -ang;
            if (((v[0] * t[ang & 0xFFF].unk2) >> 12) - ((v[2] * t[(ang - 0x400) & 0xFFF].unk2) >> 12) < 0) {
                return 0;
            }
            a3 += 0x38;
            i++;
            a2 += 0x38;
        }
    }
    return 1;
}


s32 PolarBombHitHeroes(NAS *nas, s32 dist) {
    PolarThrown *self;
    HeroInfo *h;
    s32 *ext;
    EXTheromove *move;
    NAS *obj;
    PolarBombHit *b;
    s32 i;
    s32 r;
    s32 dx;
    s32 dz;
    s32 vol;
    s32 spd;
    TrigEntry *t;

    self = FindExtension(nas, 0x10000000);
    i = 0;
    if (self == NULL) {
        return 0;
    }
    vol = 0x1E00;
    spd = 0xBB8;
    t = rcossin_tbl;
    h = Heroes;
    while (1) {
    if ((u32)i < 4) {
        ext = FindExtension(h->hero, 0x20000000);
        if (ext == NULL || (h->hero_status & 0x2000) || i == self->owner
            || h->hero_immunity != 0 || h->hero_mode == 0x4D || h->hero_mode == 0x50) {
            goto next;
        }
        dx = nas->Engine->unk10.x - h->hero_eng->unk10.x;
        dz = nas->Engine->unk10.z - h->hero_eng->unk10.z;
        if (dist <= dx * dx + dz * dz) {
            goto next;
        }
        if (ext[9] == 1) {
            r = Random(4);
            if (r == 0) {
                PlaySfx(0x490, NULL, NULL, 0x1000, vol, 0);
            }
            if (r == 1) {
                PlaySfx(0x491, NULL, NULL, 0x1000, vol, 0);
            }
            if (r == 2) {
                PlaySfx(0x492, NULL, NULL, 0x1000, vol, 0);
            }
            if (r == 3) {
                PlaySfx(0x493, NULL, NULL, 0x1000, vol, 0);
            }
            switch (h->hero_name) {
            case 1:
                PlaySfx(3, NULL, NULL, 0x1000, vol, 0);
                break;
            case 2:
                PlaySfx(0x35, NULL, NULL, 0x1000, vol, 0);
                break;
            case 3:
                PlaySfx(0x67, NULL, NULL, 0x1000, vol, 0);
                break;
            case 4:
                PlaySfx(0x99, NULL, NULL, 0x1000, vol, 0);
                break;
            case 5:
                PlaySfx(0x12F, NULL, NULL, 0x1000, vol, 0);
                break;
            case 7:
            case 8:
                PlaySfx(0xFD, NULL, NULL, 0x1000, vol, 0);
                break;
            }
            h->hero_move->forward_angle = nas->Engine->unk1E;
            h->hero_eng->unk5D = 2;
            h->hero_move->forward_angle = 0x800;
            h->hero_eng->unk1E = 0x800;
            h->hero_eng->unk5E = 1;
            if (h->hero_mode != 0x57) {
                SetHeroAction(i, 0x57);
            }
        }
        if (ext[9] == 0) {
            r = Random(4);
            if (r == 0) {
                PlaySfx(0x490, NULL, NULL, 0x1000, vol, 0);
            }
            if (r == 1) {
                PlaySfx(0x491, NULL, NULL, 0x1000, vol, 0);
            }
            if (r == 2) {
                PlaySfx(0x492, NULL, NULL, 0x1000, vol, 0);
            }
            if (r == 3) {
                PlaySfx(0x493, NULL, NULL, 0x1000, vol, 0);
            }
            switch (h->hero_name) {
            case 1:
                PlaySfx(3, NULL, NULL, 0x1000, vol, 0);
                break;
            case 2:
                PlaySfx(0x35, NULL, NULL, 0x1000, vol, 0);
                break;
            case 3:
                PlaySfx(0x67, NULL, NULL, 0x1000, vol, 0);
                break;
            case 4:
                PlaySfx(0x99, NULL, NULL, 0x1000, vol, 0);
                break;
            case 5:
                PlaySfx(0x12F, NULL, NULL, 0x1000, vol, 0);
                break;
            case 7:
            case 8:
                PlaySfx(0xFD, NULL, NULL, 0x1000, vol, 0);
                break;
            }
            move = h->hero_move;
            if (move != NULL) {
                obj = CreateNAS(h->hero_eng->unk10.x, h->hero_eng->unk10.y, h->hero_eng->unk10.z, 0x1611, 0x11);
                if (obj != NULL) {
                    b = FindExtension(obj, 0x10000000);
                    if (b != NULL) {
                        b->angle = nas->Engine->unk1E;
                        obj->Engine->unk1E = nas->Engine->unk1E;
                        h->hero_move->forward_angle = nas->Engine->unk1E + 0x800;
                        b->start = h->hero_eng->unk10;
                        b->pos.x = h->hero_eng->unk10.x - ((t[(nas->Engine->unk1E - 0x400) & 0xFFF].unk2 * spd) >> 12);
                        b->pos.z = h->hero_eng->unk10.z - ((t[nas->Engine->unk1E & 0xFFF].unk2 * spd) >> 12);
                        b->pos.y = 0;
                        SetHeroAction(i, 0x56);
                        h->hero_immunity = 0x3C;
                        move->mass -= 0x46;
                    }
                }
            }
        }
        return 1;
    next:
        h++;
        i++;
        continue;
    }
    return 0;
    }
}

s32 PolarBombOwnerEngine(NAS *nas)
{
  s32 *ext;
  HeroInfo *hero;
  u32 i;
  ext = FindExtension(nas, 0x10000000);
  i = 0;
  if (ext == 0)
  {
    return 0;
  }
  hero = Heroes;
  loop:
  if (i < 4)
  {
 do { if (!(hero->hero_status & 0x2000)) { s32 dx; s32 dz; EXTengine *a; EXTengine *b; a = nas->Engine; b = hero->hero_eng; dx = a->unk10.x - b->unk10.x; dz = a->unk10.z - b->unk10.z; if (((dx * dx) + (dz * dz)) > 0x15F8F) { goto next; } if (FindExtension(hero->hero, 0x20000000) != 0) { ext[5] = i; } return 1; } next: hero++; i++; } while (0);
    goto loop;
  }

  return 0;
}

void InitPolarBomb(NAS *m) {
    PolarBombExt *ext;
    EXTengine *e;

    ext = AddExt(m, 0x84, 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->entity = 0x201A;
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
        m->Engine->unk5D = 2;
        m->Engine->sub->flags |= 0x02000000;
        ext->unk30 = 2;
        ext->unk74 = 0x56;
        ext->unk34 = 0;
        ext->unk48 = 0;
        ext->unk10 = 0;
        ext->unkC = 0;
        m->FUX = NULL;
        e = AddExtEngine(m, 2);
        if (e != NULL) {
            e->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
            e->sub->entity = 0x202F;
            e->Engine->unk5C = 1;
            e->Engine->unk5D = 0;
            e->Engine->unk2C.x = 0xC00;
            e->Engine->unk2C.y = 0xC00;
            e->Engine->unk2C.z = 0xC00;
            e->unk10 = m->Engine->unk10;
            e->flags |= 0x8041;
            e->sub->unk76 = 0x200;
            e->sub->flags |= 0x40000000;
            m->FUX = (EXT *)e;
        }
    }
}

void PlotPolarBomb(NAS *m) {
    PolarBombExt *ext;
    NAS *nas;
    NAS *smoke;
    ExtSpawn5910 *fx;
    s32 *bx;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    switch (ext->unkC) {
    case 0:
        if ((CheckMelted(&m->Engine->unk10, 0) ^ 1) != 0) {
            ext->unk48 = 0x100;
            m->Engine->sub->unk68 = 0x200;
            if (m->Engine->unk10.y >= ext->unk48) {
                PlaySfx(0x494, NULL, NULL, 0x1000, 0x1E00, 0);
                KillNAS(m);
                break;
            }
        }
        if (ArcUpdate(ext->from, ext->to, &ext->unk30, &m->Engine->unk10.x) != 0) {
            PlaySfx((CheckMelted(&m->Engine->unk10, 0) ^ 1) != 0 ? 0x494 : 0x48F, NULL, NULL, 0x1000, 0x1E00, 0);
            nas = CreateNAS(m->Engine->unk10.x, m->Engine->unk10.y, m->Engine->unk10.z, 0x1605, 0x11);
            if (nas != NULL) {
                nas->Engine->flags |= 0x200000;
                fx = (ExtSpawn5910 *)nas->FUX;
                if (fx != NULL) {
                    fx->unkC = 1;
                    fx->unk18 = 0x100;
                    fx->unk20 = 1;
                    fx->unk28 = 0;
                }
                nas->InitDelete(nas);
            }
            ext->unk10 = 0;
            ext->unkC = 1;
            break;
        }
        if (ext->unk10-- < 0) {
            smoke = CreateNAS(m->Engine->unk10.x, m->Engine->unk10.y, m->Engine->unk10.z, 0x1612, 0x11);
            if (smoke != NULL) {
                smoke->Engine->unk2C.x = 0x5DC;
                smoke->Engine->unk2C.y = 0x5DC;
                smoke->Engine->unk2C.z = 0x5DC;
                bx = FindExtension(smoke, 0x10000000);
                if (bx != NULL) {
                    bx[3] = 0x400;
                    bx[4] = 0xC8;
                    bx[5] = 0x10;
                }
            }
            ext->unk10 = Random(2);
        }
        if (m->FUX != NULL) {
            ((EXTengine *)m->FUX)->unk10 = m->Engine->unk10;
            ((EXTengine *)m->FUX)->unk10.y = 0;
            ((EXTengine *)m->FUX)->unk2C.x = (0x100 - abs(m->Engine->unk10.y)) * 8 + 0xC00;
            ((EXTengine *)m->FUX)->unk2C.y = (0x100 - abs(m->Engine->unk10.y)) * 8 + 0xC00;
            ((EXTengine *)m->FUX)->unk2C.z = (0x100 - abs(m->Engine->unk10.y)) * 8 + 0xC00;
            ((EXTengine *)m->FUX)->unk5E = 1;
        }
        m->Engine->unk1C += 0x100;
        m->Engine->unk5E = 1;
        if (PolarBombHitHeroes(m, 0x15F90) != 0) {
            ext->unkC = 1;
            ext->unk10 = 0;
        }
        break;
    case 1:
        m->Engine->sub->flags &= ~0x8000;
        if (m->FUX != NULL) {
            ((EXTengine *)m->FUX)->sub->flags &= ~0x8000;
        }
        if (ext->unk10 == 0) {
            CreateNAS(m->Engine->unk10.x, m->Engine->unk10.y, m->Engine->unk10.z, 0x1614, 0x11);
            PolarBombHitHeroes(m, 0x35EE4);
        }
        ext->unk10++;
        if (ext->unk10 >= 0x1F) {
            KillNAS(m);
        }
        break;
    }
}

void PlotBombGFX(NAS *m) {
    s32 *ext;
    s32 *hx;
    s32 *hero;
    s32 *gx;
    s32 *game;
    s32 *bx;
    s32 ok;
    s32 v;
    s32 p;
    u16 st;
    HeroInfo *h;
    EXTengine *e;
    NAS *n;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    switch (ext[3]) {
    case 0:
        m->Engine->unk1E += 0x20;
        m->Engine->unk5E = 1;
        if (PolarBombOwnerEngine(m) == 0) {
            return;
        }
        hx = FindExtension(Heroes[ext[5]].hero, 0x20000000);
        if (hx == NULL) {
            return;
        }
        ok = 1;
        gx = FindExtension((NAS *)PolarBeamOwner, 0x40000000);
        if (gx != NULL) {
            v = gx[3];
            if (v == 5) {
                ok = gx[27] != ext[5];
            }
            if (v == 6 && gx[27] < 4 && gx[27] == ext[5]) {
                ok = 0;
            }
        }
        PlaySfx(0x48E, NULL, NULL, 0x1000, 0x1E00, 0);
        if (hx[10] == 0 && ok) {
            hx[10] = 1;
            ext[3] = 1;
            hx[40] = (s32)m;
            return;
        }
        n = BombMaster;
        if (n == NULL) {
            return;
        }
        goto dec;
    case 1:
        hero = FindExtension(Heroes[ext[5]].hero, 0x20000000);
        if (hero == NULL) {
            return;
        }
        if (m->FUX != NULL) {
            ((EXTengine *)m->FUX)->sub->flags &= ~0x8000;
        }
        game = FindExtension((NAS *)PolarBeamOwner, 0x40000000);
        if (game != NULL) {
            p = ext[5];
            st = Heroes[p].hero_status;
            if (st & 0x2000) {
                n = BombMaster;
                if (n != NULL) {
                dec:
                    bx = FindExtension(n, 0x10000000);
                    if (bx != NULL) {
                        bx[6]--;
                    }
                }
                goto kill;
            }
            if (st & 0x200) {
                n = BombMaster;
                if (n == NULL) {
                    goto kill;
                }
                goto dec2;
            }
            if ((game[3] == 5 && game[27] == p && (n = BombMaster) != NULL)
                || (game[3] == 6 && game[27] < 4 && game[27] == ext[5] && (n = BombMaster) != NULL)) {
            dec2:
                bx = FindExtension(n, 0x10000000);
                if (bx != NULL) {
                    bx[6]--;
                    hero[10] = 0;
                }
                goto kill;
            }
        }
        hero = FindExtension(Heroes[ext[5]].hero, 0x20000000);
        if (hero == NULL) {
            return;
        }
        m->Engine->unk10 = Heroes[ext[5]].hero_eng->unk10;
        h = &Heroes[ext[5]];
        if (h->hero_name == 1) {
            m->Engine->unk10.y -= (h->hero_eng->unk2C.x * 0x258) >> 12;
        } else {
            m->Engine->unk10.y -= (h->hero_eng->unk2C.x * 0x300) >> 12;
        }
        m->Engine->unk1E += 0x20;
        e = m->Engine;
        e->unk2C.z = 0x800;
        e->unk2C.y = 0x800;
        e->unk2C.x = 0x800;
        m->Engine->unk5E = 1;
        if (hero[10] != 0) {
            return;
        }
        if (BombMaster == NULL) {
            return;
        }
        bx = FindExtension(BombMaster, 0x10000000);
        if (bx != NULL) {
            bx[6]--;
        }
        if (hero[40] != 0) {
            KillNAS((NAS *)hero[40]);
            hero[40] = 0;
        }
    kill:
        KillNAS(m);
        break;
    }
}

void PolarInitHeroAngle(s32 player) {
    HeroInfo *base;
    HeroInfo *h;

    base = Heroes;
    h = &base[player];
    if (h->hero_frozen == 0) {
        if (!(h->hero_ctrl & 1)) {
            if (h->hero_mode != 0x52) {
                h->hero_eng->unk1E += angle_diff(h->hero_eng->unk1E, h->hero_move->forward_angle) >> 2;
            } else {
                h->hero_eng->unk1E += angle_diff(h->hero_eng->unk1E, h->hero_move->forward_angle) >> 5;
            }
        }
        Heroes[player].hero_ctrl &= ~1;
        Heroes[player].hero_eng->unk5E = 1;
    }
}

void ResetMass(s32 player) {
    EXTheromove *move = Heroes[player].hero_move;
    s32 *ext;

    if (move != NULL) {
        switch (GameInfo[player + 0x50]) {
        case 1:
            move->mass = 0x100;
            break;
        case 2:
            move->mass = 0x100;
            break;
        case 3:
            move->mass = 0x100;
            break;
        case 4:
            move->mass = 0x100;
            break;
        case 5:
        case 6:
        case 7:
        case 8:
            move->mass = 0x180;
            break;
        }
        ext = FindExtension(Heroes[player].hero, 0x20000000);
        if (ext != NULL && ext[9] == 1) {
            move->mass -= 0x46;
        }
    }
}

void PolarInitHeroEngine(s32 arg0) {
    EXTengine *e = Heroes[arg0].hero_eng;
    EXTheromove *m = Heroes[arg0].hero_move;
    s32 step;

    HeroHooks.HeroFacing();
    HeroApplyJump(arg0);
    if (m->velocity_calc == 3) {
        if (m->x_vel != m->x_max) {
            if (m->x_max != 0) {
                step = m->x_acc;
            } else {
                step = m->x_dec;
            }
            if (m->x_vel < m->x_max) {
                m->x_vel += step;
                if (m->x_vel > m->x_max) {
                    m->x_vel = m->x_max;
                }
            } else {
                m->x_vel -= step;
                if (m->x_vel < m->x_max) {
                    m->x_vel = m->x_max;
                }
            }
        }
        if (m->z_vel != m->z_max) {
            if (m->z_max != 0) {
                step = m->z_acc;
            } else {
                step = m->z_dec;
            }
            if (m->z_vel < m->z_max) {
                m->z_vel += step;
                if (m->z_vel > m->z_max) {
                    m->z_vel = m->z_max;
                }
            } else {
                m->z_vel -= step;
                if (m->z_vel < m->z_max) {
                    m->z_vel = m->z_max;
                }
            }
        }
        e->unk10.z += m->z_vel;
        e->unk10.x += m->x_vel;
    }
}

void PolarSpawnObjects(void) {
    SLONG_XYZ pos;
    HeroInfo *hero;
    NAS *nas;
    ExtSpawn5910 *ext;
    u8 *tex;
    s32 i;
    s32 start;
    SLONG_XYZ *p;
    s32 n;
    u16 status;

    i = Random(4);
    start = i;
    while (1) {
        if (i >= start + 4) {
            break;
        }
        hero = &Heroes[i & 3];
        status = hero->hero_status;
        if (status & 0x2000) {
            goto next;
        }
        if (status & 0x200) {
            goto next;
        }
        if (PolarRoundState != 0) {
            hero->hero_status = status | 0x2000;
            pos.x = hero->hero_eng->unk10.x;
            pos.y = hero->hero_eng->unk10.y - 0x80;
            pos.z = hero->hero_eng->unk10.z;
            tex = &HeroModelBank[DB_PolarObjects];
            p = &pos;
            nas = CreateNAS(pos.x, p->y, p->z, 0x1605, 0x11);
            n = 5;
            if (nas != NULL) {
                ext = FindExtension(nas, 0x10000000);
                if (ext != NULL) {
                    ext->unkC = n;
                    ext->unk18 = 0x100;
                    if (tex != NULL) {
                        ext->unk20 = n;
                        nas->Engine->sub->u6C.dbase = tex;
                    } else {
                        ext->unk20 = 1;
                    }
                    ext->unk28 = 0;
                    nas->InitDelete(nas);
                }
            }
            do {
                SetHeroAction(i & 3, 0x50);
            } while (0);
        } else {
            PolarRoundState = 1;
            do {
                SetHeroAction(i & 3, 0x58);
            } while (0);
        }
    next:
        i++;
    }
}

/* the victory sequence name (PolarVictorySeqs) and an empty string; their place in .rodata
 * is between functions' jump tables, so they are defined here */
const char PolarAnimWin[] = "WIN";
const char PolarEmptyString[] = "";

void InitialisePOLARFirst(void) {
    PolarHooks *h;
    PolarInitView *g;
    void **p;
    void **p2;
    void **p3;
    void **p4;
    void **p5;
    void *t;
    void *t4;
    PolarInitView *g2;
    PolarInitView *g3;

    h = &HeroHooks;
    h->unk8 = NULL;
    h->HeroModels = PolarHeroModelsB;
    PolarBeamOwner = NULL;
    BombMaster = NULL;
    PolarFirstFlag = 0;
    PolarArenaRadius2 = 0x400000;
    if (((PolarInitView *)GameInfo)->mode != 0x15) {
        h->CreateShadow = PolarInitShadow;
        h->UpdateShadow = PolarUpdateShadow;
    } else {
        h->CreateShadow = ModelCreateShadow;
        h->UpdateShadow = ModelUpdateShadow;
        h->unk8 = PolarHeroModelsB;
        h->HeroModels = PolarHeroModels;
    }
    g = (PolarInitView *)GameInfo;
    if (g->mode == 0x14) {
        HeroHooks.HeroModels = PolarHeroModelsCut;
    }
    HeroHooks.KeepInBounds = DoNowt;
    HeroHooks.InitHero = (s32)PolarInitHero;
    HeroHooks.PreMoveHero = PolarPreMoveHero;
    HeroHooks.MoveHero = PolarInitHeroEngine;
    HeroHooks.HeroBot = Polar_HeroBot;
    HeroHooks.TestAttack = PolarTestAttack;
    HeroHooks.HeroFacing = PolarInitHeroAngle;
    HeroHooks.VictorySeqs = PolarVictorySeqs;
    LevelSpecs.FinishQuick = PolarSpawnObjects;
    HeroesAllOut = 0;
    switch (g->mode) {
    case 0x12:
        AiBotTweakTable = PolarDayAiBotTweak;
        AiHumanTweakTable = PolarDayAiHumanTweak;
        AiHumanRoundTweakTable = PolarDayAiHumanRoundTweak;
        p = &AiSkillTable;
        if (GameTeams[0] != 0) {
            t = PolarDayAiSkillAlt;
        } else {
            t = PolarDayAiSkill;
        }
        *p = t;
        AiTeamSkillTable = PolarDayAiTeamSkill;
        return;
    case 0x14:
        AiBotTweakTable = PolarCutAiBotTweak;
        AiHumanTweakTable = PolarCutAiHumanTweak;
        AiHumanRoundTweakTable = PolarCutAiHumanRoundTweak;
        p2 = &AiSkillTable;
        if (GameTeams[0] != 0) {
            t = PolarCutAiSkillAlt;
        } else {
            t = PolarCutAiSkill;
        }
        *p2 = t;
        AiTeamSkillTable = PolarCutAiTeamSkill;
        return;
    case 0x15:
        AiBotTweakTable = PolarEndAiBotTweak;
        AiHumanTweakTable = PolarEndAiHumanTweak;
        AiHumanRoundTweakTable = PolarEndAiHumanRoundTweak;
        p3 = &AiSkillTable;
        if (GameTeams[0] != 0) {
            t = PolarEndAiSkillAlt;
        } else {
            t = PolarEndAiSkill;
        }
        *p3 = t;
        AiTeamSkillTable = PolarEndAiTeamSkill;
        return;
    case 0x16:
        AiBotTweakTable = PolarBossAiBotTweak;
        AiHumanTweakTable = PolarBossAiHumanTweak;
        AiHumanRoundTweakTable = PolarBossAiHumanRoundTweak;
        p4 = &AiSkillTable;
        if (GameTeams[0] != 0) {
            t4 = PolarBossAiSkillAlt;
        } else {
            t4 = PolarBossAiSkill;
        }
        *p4 = t4;
        g2 = (PolarInitView *)GameInfo;
        g2->flags |= 0x100;
        if (g2->w6C == 0) {
            g2->b51 = 9;
        }
        g2->b52 = 9;
        g2->b53 = 9;
        return;
    case 0x2C:
        g3 = (PolarInitView *)GameInfo;
        g3->flags |= 0x100;
        g3->b51 = 9;
        g3->b52 = 9;
        g3->b53 = 9;
        return;
    default:
        AiBotTweakTable = PolarDefaultAiBotTweak;
        AiHumanTweakTable = PolarDefaultAiHumanTweak;
        AiHumanRoundTweakTable = PolarDefaultAiHumanRoundTweak;
        p5 = &AiSkillTable;
        if (GameTeams[0] != 0) {
            t = PolarDefaultAiSkillAlt;
        } else {
            t = PolarDefaultAiSkill;
        }
        *p5 = t;
        AiTeamSkillTable = PolarDefaultAiTeamSkill;
        return;
    }
}

void PolarAI_SteerToTarget(player)
s32 player;
{
    HeroInfo *h;
    HeroInfo *h2;
    PolarWanderFux *fux;
    u32 pad;
    u16 ang;
    u16 t;
    EXTbotai *ai;
    s32 off;
    HeroInfo *base;

    base = Heroes;
    off = player * sizeof(HeroInfo);
    h = (HeroInfo *)(off + (u8 *)base);
    fux = (PolarWanderFux *)h->hero->FUX;
    if (fux == NULL) {
        return;
    }
    h->hero_ai->AIpad |= PolarPadToward(&fux->pos, &fux->vel);
    if (h->hero_frozen == 0) {
        ((PolarPadAng *)Pads)[player].buttons = h->hero_ai->AIpad;
    }
    ai = h->hero_ai;
    t = ai->AItimer1--;
    if (t == 0) {
        pad = h->hero_ai->AIpad;
        ang = 0;
        if (pad & 0x80) {
            ang = 0xC00;
        }
        if (pad & 0x20) {
            ang = 0x400;
        }
        if (pad & 0x10) {
            ang = 0;
        }
        if (pad & 0x40) {
            ang = 0x800;
        }
        if ((pad & 0x90) == 0x90) {
            ang = 0xE00;
        }
        if ((pad & 0xC0) == 0xC0) {
            ang = 0xA00;
        }
        if ((pad & 0x30) == 0x30) {
            ang = 0x200;
        }
        if ((pad & 0x60) == 0x60) {
            ang = 0x600;
        }
        h2 = (HeroInfo *)(off + (s32)base);
        h2->hero_ai->AIang = ang;
        h2->hero_ai->AItimer1 = fux->unk3C;
    }
    if (Heroes[player].hero_frozen == 0) {
        ((PolarPadAng *)Pads)[player].angle = Heroes[player].hero_ai->AIang;
    }
}

void PolarAI_TryAttack(s32 player) {
    HeroInfo *base;
    HeroInfo *h;
    PolarWanderFux *fux;
    Ext20 *ext;
    s32 t;

    base = Heroes;
    h = base + player;
    fux = (PolarWanderFux *)h->hero->FUX;
    if (fux == NULL) {
        return;
    }
    if (TestForTeamMate(player, fux->target) != 0) {
        return;
    }
    DistXZSq((SLONG_XYZ *)&base[fux->target].hero_eng->unk10, (SLONG_XYZ *)&h->hero_eng->unk10);
    if (PolarAI_ShotBlocked(player) == 0) {
        return;
    }
    ext = FindExtension(h->hero, 0x20000000);
    if (ext == NULL) {
        return;
    }
    if (ext->val < PolarHeroAttackPower(player)) {
        return;
    }
    if (h->hero_move == NULL) {
        return;
    }
    if (fux->timer-- > 0) {
        return;
    }
    if (h->hero_ai->AIpad & 0x8000) {
        return;
    }
    if (AiPlayerSkill[player].level > 0x20) {
        AiPlayerSkill[player].level = 0x20;
    }
    t = PolarAttackOdds[AiPlayerSkill[player].level];
    h->hero_ai->AIpad |= 0x8000;
    fux->timer = 0x40 - t;
}

void PolarAI_PickTarget(s32 player) {
    s32 dist;
    HeroInfo *base;
    HeroInfo *h;
    Ext20 *ext;
    s32 target;

    base = Heroes;
    h = base + player;
    ext = FindExtension(h->hero, 0x20000000);
    if (ext == NULL) {
        return;
    }
    if (h->hero_mode != 0x4E) {
        return;
    }
    target = FindClosetPlayer(player, &dist);
    if (target == -1 && h->hero_anim->Aindex == 0x1E) {
        ext->unk14 = 8;
        return;
    }
    if (dist <= 0x3FFFF && Player2PlayerRayTest(player, target) != 0) {
        RollPolarOdds(player, ext);
    }
    if (Heroes[player].hero_anim->Aindex == 0x1E) {
        RollPolarOdds(player, ext);
    }
}

void PolarAI_StartAttack(s32 player) {
    PolarWanderFux *fux;
    s32 i;
    s32 n;
    SLONG_XYZ pos;
    HeroInfo *h;
    HeroInfo *me;
    HeroInfo *base;
    HeroInfo *p;
    s32 best;
    s32 min;

    fux = (PolarWanderFux *)Heroes[player].hero->FUX;
    if (fux == NULL) {
        return;
    }
    for (n = i = 0, p = Heroes; i < 4; i++, p++) {
        if (p->hero_status & 0x2000) {
            n++;
        }
        fux->state = 5;
        if (n >= 3) {
            return;
        }
    }
    fux->target = Random(4);
    n = 0;
    fux->unk3C = 1;
    for (;;) {
        if (TestForTeamMate(player, fux->target) != 0) {
            n++;
            fux->target = (fux->target + 1) & 3;
        }
        if (fux->target != player && !(Heroes[fux->target].hero_status & 0x2000)) {
            break;
        }
        fux->target = (fux->target + 1) & 3;
        n++;
        if (((PolarGameView *)GameInfo)->unk6C != 0 && n >= 3) {
            fux->state = 5;
            fux->func = PolarEmptyStub6;
            return;
        }
    }
    if (!(fux->unk34 & 2)) {
        return;
    }
    best = -1;
    i = 0;
    base = Heroes;
    me = &base[player];
    h = base;
loop:
    do {
        if (i < 4) {
            min = 0x07FFFFFF;
            if (i != player && TestForTeamMate(player, i) == 0 && !(h->hero_status & 0x2000)) {
                pos = h->hero_eng->unk10;
                if (DistXZSq((SLONG_XYZ *)&pos, (SLONG_XYZ *)&me->hero_eng->unk10) < min) {
                    best = i;
                }
            }
            h++;
            i++;
            goto loop;
        }
    } while (0);
    if (best == -1) {
        fux->state = 5;
        fux->func = PolarEmptyStub6;
        return;
    }
    fux->target = best;
}

void PolarAI_DoAttack(s32 player) {
    SLONG_XYZ pos;
    s32 dist;
    s32 best;
    PolarWanderFux *fux;
    s32 *ext;
    s32 i;
    s32 skip;
    s32 tilted;
    s32 r;

    fux = (PolarWanderFux *)Heroes[player].hero->FUX;
    skip = 0;
    if (fux == NULL) {
        return;
    }
    dist = 0x7FFFFFFF;
    best = 0;
    fux->unk3C = 1;
    if (fux->unk34 & 2) {
        if ((Heroes[fux->target].hero_status & 0x2000) || Heroes[fux->target].hero_mode == 0x50) {
            for (i = 0; ; i++) {
                s32 d;

                if (i >= 4) {
                    break;
                }
                if (i != player && !(Heroes[i].hero_status & 0x2000) && !skip
                    && Heroes[fux->target].hero_mode != 0x50
                    && Heroes[fux->target].hero_immunity == 0
                    && TestForTeamMate(player, i) == 0) {
                    pos = Heroes[i].hero_eng->unk10;
                    d = DistXZSq((SLONG_XYZ *)&pos, (SLONG_XYZ *)&Heroes[player].hero_eng->unk10);
                    if (d < dist) {
                        dist = d;
                        best = i;
                    }
                }
            }
            fux->target = best;
        }
    }
    ext = FindExtension(Heroes[player].hero, 0x20000000);
    if (ext != NULL) {
        if (ext[9] == 0) {
            skip = 0;
        }
        if (ext[9] == 1 && Heroes[fux->target].hero_mode == 0x4E) {
            skip = 1;
        }
    }
    if (GameMode == 0x15 && DoesPlayerHaveBomb(player) != 0) {
        IsPlayerInRange(player, fux->target);
    }
    tilted = 0;
    if (abs(MapSceneRot.vx) > 0x40 || abs(MapSceneRot.vx) > 0x40) {
        tilted = 1;
    }
    if (tilted && (PolarAIFlags[player] & 4)) {
        fux->state = 0xB;
        PolarAI_ClearState(player);
        fux->func = PolarAI_DoChaseBomber;
        return;
    }
    if ((Heroes[fux->target].hero_status & 0x2000) || skip || Heroes[fux->target].hero_mode == 0x50) {
        if (fux->unk34 & 2) {
            for (i = 0; ; i++) {
                s32 d;

                if (i >= 4) {
                    break;
                }
                if (i != player && !(Heroes[i].hero_status & 0x2000) && !skip
                    && Heroes[i].hero_mode != 0x50) {
                    pos = Heroes[i].hero_eng->unk10;
                    d = DistXZSq((SLONG_XYZ *)&pos, (SLONG_XYZ *)&Heroes[player].hero_eng->unk10);
                    if (d < dist) {
                        dist = d;
                        best = i;
                    }
                }
            }
            fux->target = best;
        } else {
            r = PolarAI_PickVictim(player);
            if (r == -1) {
                for (i = 0; ; i++) {
                    s32 d;

                    if (i >= 4) {
                        break;
                    }
                    if (i != player && !(Heroes[i].hero_status & 0x2000) && !skip
                        && Heroes[fux->target].hero_mode != 0x50
                        && Heroes[fux->target].hero_immunity == 0
                        && TestForTeamMate(player, i) == 0) {
                        pos = Heroes[i].hero_eng->unk10;
                        d = DistXZSq((SLONG_XYZ *)&pos, (SLONG_XYZ *)&Heroes[player].hero_eng->unk10);
                        if (d < dist) {
                            dist = d;
                            best = i;
                        }
                    }
                }
                fux->state = 1;
                fux->target = best;
                fux->func = PolarAI_DoAttack;
                fux->unk34 |= 2;
            } else {
                fux->target = r;
            }
        }
    }
    if (fux->unk24-- <= 0) {
        fux->state = 5;
        fux->func = PolarEmptyStub6;
        fux->unk34 &= ~2;
    }
    if (skip || TestForTeamMate(player, fux->target) != 0) {
        fux->state = 5;
        fux->func = PolarEmptyStub6;
        fux->unk34 &= ~2;
        return;
    }
    PolarAI_TryAttack(player);
    fux->vel = Heroes[fux->target].hero_eng->unk10;
    fux->pos = Heroes[player].hero_eng->unk10;
    PolarAI_SteerToTarget(player);
}

void PolarAI_DoHunt(s32 player) {
    HeroInfo *base;
    HeroInfo *h;
    PolarWanderFux *fux;
    s32 dist;
    s32 t;

    if (EnsureMateNotObstructingMate(player) != 0) {
        return;
    }
    if (GameMode == 0x15) {
        if (DoesPlayerHaveBomb(player) != 0) {
            t = FindClosetPlayer(player, &dist);
            if (t != -1) {
                IsPlayerInRange(player, t);
            }
        }
    }
    base = Heroes;
    h = &base[player];
    fux = (PolarWanderFux *)h->hero->FUX;
    if (fux != NULL) {
        fux->unk3C = 1;
        if (PolarBeamOwner != NULL) {
            fux->vel = PolarBeamOwner->unk54->unk10;
        }
        fux->pos = h->hero_eng->unk10;
        if (fux->unk24-- < 0) {
            fux->state = 5;
            fux->func = PolarEmptyStub6;
            fux->unk34 &= ~1;
        }
        PolarAI_TryAttack(player);
        PolarAI_SteerToTarget(player);
    }
}

bool IsPlayerInRange(u32 player, u32 target) {
    HeroInfo *base;
    HeroInfo *h;
    PolarWanderFux *fux;
    EXTengine *a;
    EXTengine *b;
    s32 dx;
    s32 dz;

    base = Heroes;
    h = &base[player];
    fux = (PolarWanderFux *)h->hero->FUX;
    if (fux != NULL) {
        if (TestForTeamMate(player, target) != 0) {
            return 0;
        }
        a = h->hero_eng;
        b = base[target].hero_eng;
        dx = a->unk10.x - b->unk10.x;
        dz = a->unk10.z - b->unk10.z;
        if (dx * dx + dz * dz > 0x3D08FF) {
            return 0;
        }
        if (fux->unk54 < 0x1F) {
            return 0;
        }
        if (Player2PlayerRayTest(player, target) != 0) {
            h->hero_ai->AIpad |= 0x2000;
            return 1;
        }
    }
    return 0;
}

void DoGetBomb(u32 player) {
    PolarWanderFux *fux;
    NAS *nas;
    s32 r;

    if (EnsureMateNotObstructingMate(player) != 0) {
        return;
    }
    fux = (PolarWanderFux *)Heroes[player].hero->FUX;
    if (fux == NULL) {
        return;
    }
    if (PolarHasBomb2(player)) {
        PolarAI_StartAttack(player);
        fux->unk24 = Random(0x78) + 0x3C;
        r = PolarAI_PickVictim(player);
        if (r == -1) {
            fux->unk34 |= 2;
        } else {
            fux->target = r;
            fux->unk34 &= ~2;
        }
        fux->state = 1;
        fux->func = PolarAI_DoAttack;
        return;
    }
    nas = PolarFreeBomb();
    if (nas != NULL) {
        fux->unk3C = 1;
        fux->vel = nas->Engine->unk10;
        fux->pos = Heroes[player].hero_eng->unk10;
        if (fux->unk24-- < 0) {
            fux->state = 5;
            fux->func = PolarEmptyStub6;
        }
        PolarAI_TryAttack(player);
        PolarAI_SteerToTarget(player);
        return;
    }
    fux->state = 5;
    fux->func = PolarEmptyStub6;
}

void PolarAI_AvoidBomber(s32 player) {
    SLONG_XYZ pos;
    s32 dist;
    HeroInfo *h;
    HeroInfo *me;
    HeroInfo *o;
    PolarWanderFux *fux;
    EXTheromove *move;
    EXTengine *a;
    EXTengine *b;
    s32 best;
    s32 i;
    s32 d;
    s32 t;
    HeroInfo *base;
    HeroInfo *base2;

    base = Heroes;
    h = &base[player];
    fux = (PolarWanderFux *)h->hero->FUX;
    if (fux == NULL) {
        return;
    }
    dist = 0x7FFFFFFF;
    move = h->hero_move;
    best = 0;
    if (GameMode == 0x15) {
        if (PolarHasBomb(player)) {
            t = FindClosetPlayer(player, &dist);
            if (t != -1) {
                IsPlayerInRange(player, t);
            }
        }
    }
    i = 0;
    base2 = Heroes;
    me = &base2[player];
    o = base2;
loop:
    if (i < 4) {
        do {
            if (i != player && !(o->hero_status & 0x2000)) {
                pos = o->hero_eng->unk10;
                d = DistXZSq((SLONG_XYZ *)&pos, (SLONG_XYZ *)&me->hero_eng->unk10);
                if (d < dist) {
                    dist = d;
                    best = i;
                }
            }
            o++;
        } while (0);
        i++;
        goto loop;
    }
    if (move != NULL) {
        a = Heroes[player].hero_eng;
        b = Heroes[best].hero_eng;
        move->forward_angle = get_angle(a->unk10.x - b->unk10.x, a->unk10.z - b->unk10.z) & 0xFFF;
    }
    if (dist <= 0xFFFFF) {
        fux->unk2C = best;
        fux->unk24 = 0x5A;
        PolarAI_TryAttack(player);
        fux->state = 6;
        fux->func = PolarAI_AvoidMelt;
        fux->unk34 |= 1;
    }
    if (fux->unk24-- < 0) {
        fux->state = 5;
        fux->func = PolarEmptyStub6;
        fux->unk34 &= ~1;
    }
    if (PolarAIFlags[player] & 1) {
        fux->state = 0xB;
        PolarAI_ClearState(player);
        fux->func = PolarAI_DoChaseBomber;
    }
}

void PolarAI_DoWander(s32 player) {
    HeroInfo *h;
    PolarWanderFux *fux;
    s32 dx;
    s32 dz;
    HeroInfo *base;

    base = Heroes;
    h = &base[player];
    fux = (PolarWanderFux *)h->hero->FUX;
    if (fux != NULL) {
        if (EnsureMateNotObstructingMate(player) != 0) {
            return;
        }
        fux->pos = h->hero_eng->unk10;
        dx = fux->vel.x - fux->pos.x;
        dz = fux->vel.z - fux->pos.z;
        if (dx * dx + dz * dz > 0x24000) {
            PolarAI_SteerToTarget(player);
        } else {
            fux->unk24 = Random(0x78);
            fux->state = 0xA;
            fux->func = PolarAI_AvoidBomber;
        }
    }
    if (GameMode == 0x12 && (PolarAIFlags[player] & 1)) {
        fux->state = 0xB;
        PolarAI_ClearState(player);
        fux->func = PolarAI_DoChaseBomber;
    }
}

void PolarAI_DoChase(s32 player) {
    PolarWanderFux *fux;
    HeroInfo *h;
    s32 i;
    s32 n;

    fux = (PolarWanderFux *)Heroes[player].hero->FUX;
    if (fux == NULL) {
        return;
    }
    i = 0;
    n = 0;
    for (; i < 4; i++) {
        if (Heroes[i].hero_status & 0x2000) {
            n++;
        }
        fux->state = 5;
        fux->func = PolarEmptyStub6;
        if (n >= 3) {
            return;
        }
    }
    if (Heroes[fux->target].hero_status & 0x2000) {
        fux->state = 4;
        fux->func = PolarEmptyStub5;
    }
    if (fux->unk24-- < 0) {
        fux->state = 5;
        fux->func = PolarEmptyStub6;
    }
    h = &Heroes[fux->target];
    if (h->hero_mode == 0x4E) {
        fux->state = 5;
        fux->func = PolarEmptyStub6;
        return;
    }
    fux->vel = h->hero_eng->unk10;
    fux->pos = Heroes[player].hero_eng->unk10;
    PolarAI_SteerToTarget();
}

void PolarAI_DoBeam(s32 player) {
    s32 dist;
    HeroInfo *base;
    PolarWanderFux *fux;
    EXTengine *eng;
    EXTengine *e;
    s32 r;
    s32 dx;
    s32 dz;

    if (EnsureMateNotObstructingMate(player) != 0) {
        return;
    }
    base = Heroes;
    fux = (PolarWanderFux *)base[player].hero->FUX;
    if (fux == NULL) {
        return;
    }
    eng = base[player].hero_eng;
    if (eng == NULL) {
        return;
    }
    if (GameMode == 0x15) {
        if (PolarHasBomb(player)) {
            r = FindClosetPlayer(player, &dist);
            if (r != -1) {
                IsPlayerInRange(player, r);
            }
        }
    }
    fux->unk3C = 1;
    if (PolarBeamOwner != NULL) {
        fux->vel = PolarBeamOwner->unk54->unk10;
    }
    fux->pos = Heroes[player].hero_eng->unk10;
    e = PolarBeamOwner->unk54;
    dx = eng->unk10.x - e->unk10.x;
    dz = eng->unk10.z - e->unk10.z;
    if (dx * dx + dz * dz < 0x19000) {
        fux->state = 5;
        fux->func = PolarEmptyStub6;
    }
    if (PolarBeamMode == 1) {
        fux->state = 5;
        fux->func = PolarEmptyStub6;
    }
    PolarAI_SteerToTarget(player);
}

bool Player2PlayerRayTest(u32 player, u32 target) {
    VECTOR vel;
    VECTOR out;
    VECTOR pos;
    SVECTOR d;
    SVECTOR pad;
    HeroInfo *base;
    HeroInfo *h;
    EXTengine *a;
    EXTengine *b;
    s32 ang;
    s32 ret;

    base = Heroes;
    h = &base[player];
    if (h->hero->FUX == NULL) {
        goto ret0;
    }
    a = h->hero_eng;
    ang = a->unk1E;
    b = base[target].hero_eng;
    vel.vx = PolarIceRadius * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2 >> 12;
    vel.vz = PolarIceRadius * rcossin_tbl[ang & 0xFFF].unk2 >> 12;
    pos.vx = 0;
    pos.vz = 0;
    d.vx = b->unk10.x - a->unk10.x;
    d.vz = b->unk10.z - a->unk10.z;
    d.vy = 0;
    d.pad = 200;
    ret = 1;
    if (PolarRayHit(&pos, &vel, &d, &out) == ret) {
        return ret;
    }
ret0:
    return 0;
}

s32 PolarAI_ShotBlocked(s32 self) {
    SLONG_XYZ pos;
    s32 dist;
    s32 *dp;
    s32 best;
    HeroInfo *h;
    HeroInfo *me;
    HeroInfo *base;
    s32 i;
    s32 d;
    s32 min;
    u16 st;

    dp = &dist;
    best = -1;
    min = 0x07FFFFFF;
    i = 0;
    base = Heroes;
    me = &base[self];
    h = base;
loop:
    if (i < 4) {
        if (i != self && !((st = h->hero_status) & 0x2000) && !(st & 0x8000)) {
            pos = h->hero_eng->unk10;
            d = DistXZSq((SLONG_XYZ *)&pos, (SLONG_XYZ *)&me->hero_eng->unk10);
            if (d < min) {
                min = d;
                best = i;
            }
        }
        h++;
        i++;
        goto loop;
    }
    *dp = min;
    if (best != -1) {
        st = 0;
        if (TestForTeamMate(self, best) == 0) {
            if (dist <= 0x3FFFF) {
                st = 1;
                d = Player2PlayerRayTest(self, best);
                if (d != 0) {
                    return st;
                }
            }
        }
    }
    return 0;
}

bool EnsureMateNotObstructingMate(u32 self) {
    SLONG_XYZ pos;
    s32 dist;
    s32 *dp;
    s32 best;
    HeroInfo *h;
    HeroInfo *me;
    HeroInfo *base;
    s32 i;
    s32 d;
    s32 min;
    u16 st;
    s32 target;

    if (GameTeams[0] == 0) {
        return 0;
    }
    dp = &dist;
    best = -1;
    min = 0x07FFFFFF;
    i = 0;
    base = Heroes;
    me = &base[self];
    h = base;
loop:
    do {
        if (i < 4) {
            if (i != self && !((st = h->hero_status) & 0x2000) && !(st & 0x8000)) {
                pos = h->hero_eng->unk10;
                d = DistXZSq((SLONG_XYZ *)&pos, (SLONG_XYZ *)&me->hero_eng->unk10);
                if (d < min) {
                    min = d;
                    best = i;
                }
            }
            h++;
            i++;
            goto loop;
        }
    } while (0);
    target = best;
    *dp = min;
    if (target == -1) {
        goto ret0;
    }
    d = TestForTeamMate(self, target) != 1;
    if (d) {
        return 0;
    }
    if (dist > 0x7FFFF) {
        return 0;
    }
    if (Player2PlayerRayTest(self, target) == 0) {
        goto ret0;
    }
    if (Player2PlayerRayTest(target, self) == 0) {
        goto ret0;
    }
    PolarGrab(self);
    PolarGrab(target);
    return 1;
ret0:
    return 0;
}

void PolarAI_AvoidMelt(s32 player)
{
  VECTOR v1;
  VECTOR out;
  VECTOR v2;
  SVECTOR d[2];
  struct 
  {
    SLONG_XYZ v;
    s32 pad;
  } pos;
  SVECTOR d2[2];
  s32 dist;
  EXTheromove *mv;
  s32 *dp;
  s32 rnd;
  s32 best;
  s32 bestd;
  s32 i;
  s32 j;
  s32 found;
  s32 found2;
  s32 done;
  s32 r;
  s32 ang;
  s32 dx;
  s32 dz;
  HeroInfo *h;
  HeroInfo *o;
  HeroInfo *hp;
  HeroInfo *me2;
  HeroInfo *hb;
  s32 ok;
  HeroInfo *b1;
  HeroInfo *me1;
  EXTengine *my;
  EXTengine *ve;
  PolarWanderFux *fux;
  s32 k;
  bestd = 0x7FFFFFFF;
  best = 0;
  if (((PolarModeView *) GameInfo)->mode == 0x12)
  {
    PolarIceRadius = 0x9F4;
  }
  hb = Heroes;
  h = &hb[player];
  fux = (PolarWanderFux *) h->hero->FUX;
  if (fux == 0)
  {
    return;
  }
  mv = h->hero_move;
  fux->unk24--;
  if (((PolarModeView *) GameInfo)->mode == 0x15)
  {
    if (PolarHasBomb2(player))
    {
      s32 min;
      HeroInfo *me;
      HeroInfo *base;
      u16 st;
      dp = &dist;
      r = -1;
      min = 0x07FFFFFF;
      i = 0;
      base = Heroes;
      me = &base[player];
      o = base;
      loop0:
      if (i < 4)
      {
        do
        {
          s32 dd;
          if (((i != player) && (!((st = o->hero_status) & 0x2000))) && (!(st & 0x8000)))
          {
            pos.v = o->hero_eng->unk10;
            dd = DistXZSq((SLONG_XYZ *)(&pos.v), (SLONG_XYZ *)(&me->hero_eng->unk10));
            if (dd < min)
            {
              min = dd;
              r = i;
            }
          }
          o++;
          i++;
        }
        while (0);
        goto loop0;
      }

      *dp = min;
      if (r != (-1))
      {
        IsPlayerInRange(player, r);
      }
    }
  }
  PolarAI_TryAttack(player);
  if (fux->unk34 & 1)
  {
    {
      s32 idle;
      s32 n;
      n = 0;
      do
      {
        if (n == player)
        {
          idle = 0;
          goto chk;
        }
        if (PolarAIFlags[n] & 4)
        {
          goto zero;
        }
        if (PolarAIFlags[n] & 2)
        {
          goto zero;
        }
        n++;
      }
      while (n < 4);
      idle = 1;
      chk:
      if (idle)
      {
        fux->state = 5;
        goto tail;
      }

      goto rest;
      zero:
      idle = 0;

      goto chk;
    }
    rest:
    i = 0;

    b1 = Heroes;
    me1 = &b1[player];
    o = b1;
    loop:
    if (i < 4)
    {
      do
      {
        s32 dd;
        if ((i != player) && (!(o->hero_status & 0x2000)))
        {
          pos.v = o->hero_eng->unk10;
          dd = DistXZSq((SLONG_XYZ *)(&pos.v), (SLONG_XYZ *)(&me1->hero_eng->unk10));
          if (dd < bestd)
          {
            bestd = dd;
            best = i;
          }
        }
        o++;
      }
      while (0);
      i++;
      goto loop;
    }

    fux->unk2C = best;
  }
  if (mv != 0)
  {
    mv->forward_angle = get_angle(Heroes[player].hero_eng->unk10.x - Heroes[fux->unk2C].hero_eng->unk10.x, Heroes[player].hero_eng->unk10.z - Heroes[fux->unk2C].hero_eng->unk10.z) & 0xFFF;
  }
  my = Heroes[player].hero_eng;
  ve = Heroes[fux->unk2C].hero_eng;
  d[0].vx = (*((u16 *) (&ve->unk10.x))) - (*((u16 *) (&my->unk10.x)));
  d[0].vz = (*((u16 *) (&ve->unk10.z))) - (*((u16 *) (&my->unk10.z)));
  d[0].vy = 0;
  d[0].pad = 0xFA;
  dx = ve->unk10.x - my->unk10.x;
  dz = ve->unk10.z - my->unk10.z;
  if (((dx * dx) + (dz * dz)) > 0x283E3F)
  {
    goto end;
  }
  rnd = Random(8);
  v1.vx = (PolarIceRadius * rcossin_tbl[(fux->angle - 0x400) & 0xFFF].unk2) >> 12;
  v1.vz = (PolarIceRadius * rcossin_tbl[fux->angle & 0xFFF].unk2) >> 12;
  done = 0;
  v2.vx = 0;
  v2.vz = 0;
  if (PolarRayHit(&v2, &v1, d, &out) == 0)
  {
    found2 = 0;
    j = 0;
    hp = Heroes;
    loop2:
    if (j < 4)
    {
      do
      {
        EXTengine *e2;
        e2 = hp->hero_eng;
        if (((j != player) && (j != fux->unk2C)) && (!(hp->hero_status & 0x2000)))
        {
          d2[0].vx = (*((u16 *) (&e2->unk10.x))) - (*((u16 *) (&my->unk10.x)));
          d2[0].vz = (*((u16 *) (&e2->unk10.z))) - (*((u16 *) (&my->unk10.z)));
          d2[0].vy = 0;
          d2[0].pad = 0xFA;
          if (PolarRayHit(&v2, &v1, d2, &out) == 1)
          {
            found2 = 1;
          }
        }
        hp++;
      }
      while (0);
      j++;
      goto loop2;
    }

    if (!found2)
    {
      HeroInfo *b2;
      b2 = Heroes;
      me2 = &b2[player];
      pos.v.x = me2->hero_eng->unk10.x - v1.vx;
      pos.v.z = me2->hero_eng->unk10.z - v1.vz;
      if (GameMode != 0x14)
      {
        ok = CheckMelted(&pos.v, 0);
        goto join;
        here:
        fux->vel = pos.v;

        fux->pos = Heroes[player].hero_eng->unk10;
        PolarAI_SteerToTarget(player);
        fux->angle = ang;
        goto end;
      }
      else
      {
        ok = PointInZone((s32 *) (&pos.v), 1);
      }
      join:
      if (ok != 0)
      {
        fux->vel = pos.v;
        fux->pos = me2->hero_eng->unk10;
        PolarAI_SteerToTarget(player);
        done = 1;
      }

    }
  }
  if (done != 1)
  {
    for (j = rnd;; j++)
    {
      if (j >= (rnd + 8))
      {
        goto end;
      }
      ang = PolarMeltOffsets[j & 7];
      v1.vx = (PolarIceRadius * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
      v1.vz = (PolarIceRadius * rcossin_tbl[ang & 0xFFF].unk2) >> 12;
      v2.vx = 0;
      v2.vz = 0;
      if (PolarRayHit(&v2, &v1, d, &out) != 0)
      {
        continue;
      }

      found = 0;
      k = 0;
      pos.v.x = Heroes[player].hero_eng->unk10.x - v1.vx;
      pos.v.z = Heroes[player].hero_eng->unk10.z - v1.vz;
      for (;;)
      {
        EXTengine *e3;
        if (k >= 4)
        {
          break;
        }
        e3 = Heroes[k].hero_eng;
        if (((k != player) && (k != fux->unk2C)) && (!(Heroes[k].hero_status & 0x2000)))
        {
          d2[0].vx = (*((u16 *) (&e3->unk10.x))) - (*((u16 *) (&my->unk10.x)));
          d2[0].vz = (*((u16 *) (&e3->unk10.z))) - (*((u16 *) (&my->unk10.z)));
          d2[0].vy = 0;
          d2[0].pad = 0xFA;
          if (PolarRayHit(&v2, &v1, d2, &out) == 1)
          {
            found = 1;
          }
        }
        k++;
      }


      if (!found)
      {
        if (GameMode != 0x14)
        {
          if (CheckMelted(&pos.v, 0) != 0)
          {
            fux->vel = pos.v;
            fux->pos = Heroes[player].hero_eng->unk10;
            PolarAI_SteerToTarget(player);
            fux->angle = ang;
            goto end;
          }
        }
        else
          if (PointInZone((s32 *) (&pos.v), 1) != 0)
        {
          goto here;
        }
      }
    }

  }
  end:
  if (PolarAIFlags[player] & 1)
  {
    fux->state = 0xB;
    PolarAI_ClearState(player);
    fux->func = PolarAI_DoChaseBomber;
  }

  if (fux->unk24 <= 0)
  {
    fux->state = 5;
    tail:
    fux->func = PolarEmptyStub6;

    fux->unk34 &= ~1;
  }
}

void PolarAI_DoIceWalk(s32 player) {
    VECTOR v1;
    VECTOR out;
    VECTOR v2;
    SVECTOR d[2];
    struct {
        SLONG_XYZ v;
        s32 pad;
    } pos;
    SVECTOR d2[2];
    SVECTOR d3[2];
    EXTheromove *mv;
    s32 rnd;
    s32 best;
    s32 bestd;
    s32 i;
    s32 j;
    s32 found;
    s32 found2;
    s32 ang;
    s32 dx;
    s32 ax;
    s32 az;
    s32 dz;
    s32 prox;
    HeroInfo *base;
    HeroInfo *h;
    HeroInfo *o;
    HeroInfo *hp;
    HeroInfo *q;
    EXTengine *my;
    EXTengine *ve;
    PolarWanderFux *fux;

    bestd = 0x7FFFFFFF;
    best = 0;
    base = Heroes;
    h = &base[player];
    fux = (PolarWanderFux *)h->hero->FUX;
    prox = 0;
    if (fux == NULL) {
        return;
    }
    ve = NULL;
    mv = h->hero_move;
    if (GameMode == 0x12) {
        PolarIceRadius = 0x9F4;
    }
    fux->unk24--;
    if (fux->unk34 & 1) {
        HeroInfo *me;
        EXTengine *a;
        EXTengine *b;

        i = 0;
        me = h;
        o = base;
    loop:
        if (i < 4) {
            do {
                s32 dist;
                if (i != player && !(o->hero_status & 0x2000)) {
                    pos.v = o->hero_eng->unk10;
                    dist = DistXZSq((SLONG_XYZ *)&pos.v, (SLONG_XYZ *)&me->hero_eng->unk10);
                    if (dist < bestd) {
                        bestd = dist;
                        best = i;
                    }
                }
                o++;
            } while (0);
            i++;
            goto loop;
        }
        fux->unk2C = best;
        if (((PolarModeView *)GameInfo)->mode == 0x12 && PolarProximity != NULL) {
            a = PolarProximity->eng;
            b = Heroes[player].hero_eng;
            dx = a->unk10.x - b->unk10.x;
            dz = a->unk10.z - b->unk10.z;
            dx = dx * dx + dz * dz;
            if (dx < bestd) {
                prox = 1;
                fux->unk2C = 5;
            }
        }
    }
    if (mv != NULL) {
        if (prox == 0) {
            ax = Heroes[player].hero_eng->unk10.x - Heroes[fux->unk2C].hero_eng->unk10.x;
            az = Heroes[player].hero_eng->unk10.z - Heroes[fux->unk2C].hero_eng->unk10.z;
        } else {
            ax = Heroes[player].hero_eng->unk10.x - PolarProximity->eng->unk10.x;
            az = Heroes[player].hero_eng->unk10.z - PolarProximity->eng->unk10.z;
        }
        mv->forward_angle = get_angle(ax, az) & 0xFFF;
    }
    my = Heroes[player].hero_eng;
    if (prox == 0) {
        ve = Heroes[fux->unk2C].hero_eng;
        goto aim;
escape:
        fux->vel = pos.v;
        fux->pos = Heroes[player].hero_eng->unk10;
        PolarAI_SteerToTarget(player);
        fux->angle = ang;
        goto end;
    } else if (PolarProximity != NULL) {
        ve = PolarProximity->eng;
    }
aim:
    d[0].vx = *(u16 *)&ve->unk10.x - *(u16 *)&my->unk10.x;
    d[0].vz = *(u16 *)&ve->unk10.z - *(u16 *)&my->unk10.z;
    d[0].vy = 0;
    d[0].pad = 0x10E;
    dx = ve->unk10.x - my->unk10.x;
    dz = ve->unk10.z - my->unk10.z;
    dx = dx * dx + dz * dz;
    if (dx > 0x283E3F) {
        goto end;
    }
    rnd = Random(8);
    v1.vx = (PolarIceRadius * rcossin_tbl[(fux->angle - 0x400) & 0xFFF].unk2) >> 12;
    v1.vz = (PolarIceRadius * rcossin_tbl[fux->angle & 0xFFF].unk2) >> 12;
    bestd = 0;
    v2.vx = 0;
    v2.vz = 0;
    if (PolarRayHit(&v2, &v1, d, &out) == 0) {
        found2 = 0;
        j = 0;
        hp = Heroes;
    loop2:
        if (j < 4) {
            do {
                EXTengine *e2;
                e2 = hp->hero_eng;
                if (j != player && j != fux->unk2C && !(hp->hero_status & 0x2000)) {
                    d2[0].vx = *(u16 *)&e2->unk10.x - *(u16 *)&my->unk10.x;
                    d2[0].vz = *(u16 *)&e2->unk10.z - *(u16 *)&my->unk10.z;
                    d2[0].vy = 0;
                    d2[0].pad = 0x10E;
                    if (PolarRayHit(&v2, &v1, d2, &out) == 1) {
                        found2 = 1;
                    }
                }
                hp++;
            } while (0);
            j++;
            goto loop2;
        }
        if (!found2) {
            HeroInfo *hb;
            HeroInfo *me2;

            hb = Heroes;
            me2 = &hb[player];
            pos.v.x = me2->hero_eng->unk10.x - v1.vx;
            pos.v.z = me2->hero_eng->unk10.z - v1.vz;
            if (CheckMelted(&pos.v, 0) != 0) {
                fux->vel = pos.v;
                fux->pos = me2->hero_eng->unk10;
                PolarAI_SteerToTarget(player);
                bestd = 1;
            }
        }
    }
    if (bestd != 1) {
        for (j = rnd; ; j++) {
            if (j >= rnd + 8) {
                goto end;
            }
            ang = PolarMeltOffsets[j & 7];
            v1.vx = (PolarIceRadius * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
            v1.vz = (PolarIceRadius * rcossin_tbl[ang & 0xFFF].unk2) >> 12;
            v2.vx = 0;
            v2.vz = 0;
            if (PolarRayHit(&v2, &v1, d, &out) != 0) {
                continue;
            }
            found = 0;
            i = 0;
            pos.v.x = Heroes[player].hero_eng->unk10.x - v1.vx;
            q = Heroes;
            pos.v.z = Heroes[player].hero_eng->unk10.z - v1.vz;
        loop3:
            if (i < 4) {
                do {
                    EXTengine *e3;
                    e3 = q->hero_eng;
                    if (i != player && i != fux->unk2C && !(q->hero_status & 0x2000)) {
                        d3[0].vx = *(u16 *)&e3->unk10.x - *(u16 *)&my->unk10.x;
                        d3[0].vz = *(u16 *)&e3->unk10.z - *(u16 *)&my->unk10.z;
                        d3[0].vy = 0;
                        d3[0].pad = 0x10E;
                        if (PolarRayHit(&v2, &v1, d3, &out) == 1) {
                            found = 1;
                        }
                    }
                    q++;
                } while (0);
                i++;
                goto loop3;
            }
            if (!found && CheckMelted(&pos.v, 0) != 0) {
                goto escape;
            }
        }
    }
end:
    if (PolarAIFlags[player] & 1) {
        fux->state = 0xB;
        PolarAI_ClearState(player);
        fux->func = PolarAI_DoChaseBomber;
    }
    if (fux->unk24 <= 0) {
        fux->state = 5;
        fux->func = PolarEmptyStub6;
        fux->unk34 &= ~1;
    }
}

void PolarAI_DoChaseBomber(player)
s32 player;
{
    s32 dist;
    HeroInfo *base;
    HeroInfo *h;
    PolarWanderFux *fux;
    EXTengine *eng;
    s32 r;
    s32 dx;
    s32 dz;

    if (EnsureMateNotObstructingMate(player) != 0) {
        return;
    }
    fux = (PolarWanderFux *)Heroes[player].hero->FUX;
    if (fux == NULL) {
        return;
    }
    if (GameMode == 0x15) {
        if (PolarHasBomb(player)) {
            r = PolarClosest(player, &dist);
            if (r != -1) {
                IsPlayerInRange(player, r);
            }
        }
    }
    base = Heroes;
    h = &base[player];
    eng = h->hero_eng;
    if (eng == NULL) {
        return;
    }
    fux->unk3C = 1;
    fux->vel = *(SLONG_XYZ *)&PolarCentre;
    fux->pos = h->hero_eng->unk10;
    dx = eng->unk10.x - PolarCentre.vx;
    dz = eng->unk10.z - PolarCentre.vz;
    if (dx * dx + dz * dz < 0x19000) {
        fux->state = 5;
        fux->func = PolarEmptyStub6;
    }
    if (fux->unk24-- <= 0) {
        fux->state = 5;
        fux->func = PolarEmptyStub6;
    }
    PolarAI_SteerToTarget(player);
}

s32 TestBeamChance(u32 player) {
    s32 mode;
    PolarWanderFux *fux;
    HeroInfo *h;
    PolarLink *l;
    s32 r;
    HeroInfo *base;

    if (GameMode != 0x14) {
        if ((mode = PolarBeamMode) != 2) {
            return 0;
        }
        base = Heroes;
        h = &base[player];
        fux = (PolarWanderFux *)h->hero->FUX;
        if (fux != NULL && PolarBeamOwner != NULL) {
            if (HeroesAllOut != 0) {
                fux->state = mode;
                fux->func = PolarAI_DoBeam;
                return 1;
            }
            l = PolarBeamOwner->unk58;
            if (l == NULL) {
                goto fail;
            }
            if (l->unk6C == player) {
                return 0;
            }
            r = Random(0x1000);
            fux->flags |= 1;
            if (r < PolarAIChances[h->hero_name + 0x17]) {
                fux->state = mode;
                fux->func = PolarAI_DoBeam;
                return 1;
            }
        }
    }
fail:
    return 0;
}

s32 TestAttackChance(s32 player) {
    PolarWanderFux *fux;
    PolarWanderFux *ext;
    HeroInfo *h;
    s32 r;
    s32 t;
    HeroInfo *base;

    base = Heroes;
    h = &base[player];
    fux = (PolarWanderFux *)h->hero->FUX;
    if (fux == NULL) {
        return 0;
    }
    r = Random(0x1000);
    ext = FindExtension(h->hero, 0x20000000);
    if (ext != NULL && ext->unk24 == 1) {
        return 0;
    }
    if (r >= PolarAIChances[Heroes[player].hero_name - 1]) {
        return 0;
    }
    PolarAI_StartAttack(player);
    fux->unk24 = Random(0x78) + 0x3C;
    t = PolarAI_PickVictim(player);
    if (t == -1) {
        fux->unk34 |= 2;
    } else {
        fux->target = t;
        fux->unk34 &= ~2;
    }
    fux->state = 1;
    fux->func = PolarAI_DoAttack;
    return 1;
}

s32 TestHuntChance(s32 player) {
    PolarWanderFux *fux;
    PolarWanderFux *f;
    HeroInfo *h;
    s32 r;
    HeroInfo *base;

    if (GameMode != 0x14) {
        base = Heroes;
        h = &base[player];
        fux = (PolarWanderFux *)h->hero->FUX;
        if (fux != NULL) {
            r = Random(0x1000);
            if (PlayerActionCount(8, player) != 0) {
                r = 0x7FFF;
            }
            if (r < PolarAIChances[h->hero_name + 0x27]) {
                f = (PolarWanderFux *)h->hero->FUX;
                if (f != NULL) {
                    f->unk24 = Random(0x168) + 0x5A;
                }
                fux->state = 8;
                fux->func = PolarAI_DoHunt;
                return 1;
            }
            return 0;
        }
        return 0;
    }
    return 0;
}

s32 TestWanderChance(s32 player) {
    PolarWanderFux *fux;
    PolarWanderFux *ext;
    PolarWanderFux *f;
    HeroInfo *h;
    SLONG_XYZ v;
    s32 r;
    s32 spd;

    fux = (PolarWanderFux *)Heroes[player].hero->FUX;
    if (fux == NULL) {
        return 0;
    }
    ext = FindExtension(Heroes[player].hero, 0x20000000);
    if (ext != NULL && ext->unk24 == 1) {
        return 0;
    }
    if (Random(0x1000) >= PolarAIChances[Heroes[player].hero_name + 0x2F]) {
        return 0;
    }
    h = &Heroes[player];
    r = Random(0x1000);
    f = (PolarWanderFux *)h->hero->FUX;
    spd = 0x708;
    if (f != NULL) {
        v.x = (rcossin_tbl[r & 0xFFF].unk2 * spd) >> 12;
        v.z = (rcossin_tbl[(r - 0x400) & 0xFFF].unk2 * spd) >> 12;
        f->vel = v;
        f->pos = h->hero_eng->unk10;
    }
    fux->state = 9;
    fux->func = PolarAI_DoWander;
    return 1;
}

bool TestBombChance(u32 player) {
    PolarWanderFux *fux;
    PolarWanderFux *f;
    HeroInfo *h;

    HeroInfo *base;

    if (GameMode == 0x15) {
        base = Heroes;
        h = &base[player];
        fux = (PolarWanderFux *)h->hero->FUX;
        if (fux != NULL && Random(0x1000) < PolarAIChances[h->hero_name + 0x3F]) {
            f = (PolarWanderFux *)h->hero->FUX;
            if (f != NULL) {
                f->unk24 = Random(0x168) + 0x5A;
            }
            fux->state = 0xD;
            fux->func = DoGetBomb;
            return 1;
        }
        return 0;
    }
    return 0;
}

bool TestChaseChance(u32 player) {
    PolarWanderFux *fux;
    HeroInfo *h;
    s32 start;
    s32 i;
    HeroInfo *base;

    base = Heroes;
    h = base + player;
    fux = (PolarWanderFux *)h->hero->FUX;
    if (fux == NULL) {
        return 0;
    }
    start = Random(3);
    if (Random(0x1000) < PolarAIChances[h->hero_name + 0x1F]) {
        for (i = start; ; i++) {
            s32 t;

            if (i >= start + 4) {
                break;
            }
            t = i & 3;
            if (t != player && !(Heroes[t].hero_status & 0x2000) && (PolarAIFlags[t] & 1)) {
                fux->target = t;
                PolarStartChase(player);
                fux->state = 7;
                fux->func = PolarAI_DoChase;
                return 1;
            }
        }
    }
    return 0;
}

s32 PolarAI_TeamChoice(s32 hero) {
    s16 lead;
    s16 other;
    s32 mate;
    s32 r;

    r = Random(0x1000);
    lead = ((PolarTeamView *)GameInfo)->team0;
    if (hero == lead || (mate = Heroes[lead].hero_teammate, hero == mate)) {
        if (r < 0x800) {
            other = ((PolarTeamView *)GameInfo)->team1;
            if (!(Heroes[other].hero_status & 0x2000)) {
                return other;
            }
            mate = Heroes[other].hero_teammate;
            if (Heroes[mate].hero_status & 0x2000) {
                return -1;
            }
            return mate;
        } else {
            lead = ((PolarTeamView *)GameInfo)->team1;
            mate = Heroes[lead].hero_teammate;
            if (!(Heroes[mate].hero_status & 0x2000)) {
                return mate;
            }
            if (!(Heroes[lead].hero_status & 0x2000)) {
                return lead;
            }
            return -1;
        }
    } else if (r < 0x800) {
        if (!(Heroes[lead].hero_status & 0x2000)) {
            return lead;
        }
        if (Heroes[mate].hero_status & 0x2000) {
            return -1;
        }
        return mate;
    } else {
        if (!(Heroes[mate].hero_status & 0x2000)) {
            return mate;
        }
        if (!(Heroes[lead].hero_status & 0x2000)) {
            return lead;
        }
    }
    return -1;
}

void DoChooseNewAction(u32 player) {
    s32 i;
    PolarWanderFux *fux;
    PolarWanderFux *f;
    Ext20 *ext;
    EXTengine *eng;
    HeroInfo *h;
    HeroInfo *me;
    s32 *fl;
    s32 *all;
    PolarModeView *g;
    PolarLink *l;
    PolarWanderFux *o;
    s32 one;
    s32 found;
    s32 j;
    s32 n;
    s32 r;
    HeroInfo *base;
    HeroInfo *hb;
    HeroInfo *hp;
    HeroInfo *e;

    base = Heroes;
    h = &base[player];
    fux = (PolarWanderFux *)h->hero->FUX;
    if (fux == NULL) {
        return;
    }
    ext = FindExtension(h->hero, 0x20000000);
    if (ext != NULL) {
        if (((PolarModeView *)GameInfo)->mode == 0x12) {
            PolarArenaRadius2 = 1000000;
        }
        h->hero_ai->AImode = 1;
        eng = h->hero_eng;
        if (h->hero_mode != 0x49) {
            fux->unk50 = 0;
        }
        i = 0;
        if (fux->state != 1) {
            fux->unk58 = 0;
        }
        g = (PolarModeView *)GameInfo;
        one = 1;
        for (;; i++) {
            NAS *nas;
            BombMakerExt *b;

            if (i >= 4) {
                break;
            }
            o = (PolarWanderFux *)Heroes[i].hero->FUX;
            if (o == NULL) {
                continue;
            }
            PolarAIFlags[i] = 0;
            if (Heroes[i].hero_mode == 0x4B || Heroes[i].hero_mode == 0x5A) {
                PolarAIFlags[i] = 0x80;
            }
            if (Heroes[i].hero_mode == 0x56) {
                PolarAIFlags[i] |= 0x800;
            }
            if (g->mode == 0x12) {
                PolarProx *p = PolarProximity;
                PolarMode *m;
                s32 dx;
                s32 dz;

                if (p != NULL && (m = p->unk58) != NULL) {
                    dx = p->eng->unk10.x - Heroes[player].hero_eng->unk10.x;
                    dz = p->eng->unk10.z - Heroes[player].hero_eng->unk10.z;
                    if (dx * dx + dz * dz <= 0xB0643 && (m->mode == one || m->mode == 5)) {
                        PolarAIFlags[i] |= 0x100;
                    }
                }
            }
            if (g->mode == 0x14 && Heroes[i].hero_mode == 0x52) {
                PolarAIFlags[i] |= 0x200;
            }
            if (g->mode == 0x15) {
                nas = PolarBomb;
            loop:
                if (nas != NULL) {
                    b = FindExtension(nas, 0x10000000);
                    if (b != NULL && b->unkC == one && b->unk14 == i) {
                        found = 1;
                        goto done;
                    }
                    nas = nas->Group.Child;
                    goto loop;
                }
                found = 0;
            done:
                if (found) {
                    PolarAIFlags[i] |= 0x400;
                    o->unk54++;
                } else {
                    o->unk54 = 0;
                    if (PolarFreeBomb() != NULL && o->func == PolarEmptyStub6 && ext->state == one) {
                        f = (PolarWanderFux *)Heroes[i].hero->FUX;
                        if (f != NULL) {
                            f->unk24 = Random(0x168) + 0x5A;
                        }
                        o->state = 0xD;
                        o->func = DoGetBomb;
                    }
                }
            }
        }
        PolarHeroAnimState(player);
        PolarAI_PickTarget(player);
        PolarAI_TryAttack(player);
        if (Heroes[player].hero_mode == 0x4E || (Heroes[player].hero_status & 0x2000)) {
            goto stop;
        }
        if (eng != NULL) {
            if (PolarArenaRadius2 < DistXZSq((SLONG_XYZ *)&PolarCentre, (SLONG_XYZ *)&eng->unk10)) {
                PolarAIFlags[player] |= 1;
            }
            if (PolarBeamOwner != NULL && (l = PolarBeamOwner->unk58) != NULL) {
                if (l->unk14 == 2) {
                    PolarBeamMode = l->unk14;
                } else {
                    PolarBeamMode = 1;
                    fux->flags &= ~1;
                }
                if (l->unk6C != 5) {
                    if (l->unkC == 5) {
                        PolarAIFlags[l->unk6C] |= 2;
                    }
                    if (l->unkC == 6) {
                        PolarAIFlags[l->unk6C] |= 0x40;
                    }
                }
                if (l->unk10 != -1) {
                    switch (l->unk28[l->unk10]) {
                    case 1:
                        PolarAIFlags[l->unk10] |= 4;
                        break;
                    case 0:
                        PolarAIFlags[l->unk10] |= 8;
                        break;
                    case 2:
                        PolarAIFlags[l->unk10] |= 0x20;
                        break;
                    }
                }
            }
        }
        if (!(fux->flags & 1)) {
            TestBeamChance(player);
        }
        if (fux->state == 5) {
            r = Random(5);
            for (n = 0; n < 5; n++) {
                if (PolarAITests[r](player) != 0) {
                    break;
                }
                if (++r >= 5) {
                    r = 0;
                }
            }
        }
        i = 0;
        hb = Heroes;
        all = PolarAIFlags;
        hp = &hb[player];
        fl = all;
        me = hb;
    loop2:
        if (i < 4) {
            do {
            do {
            if (i == player || me->hero_mode == 0x4E || (me->hero_status & 0x2000)) {
                goto next2;
            }
            if (*fl & 0x400) {
                EXTengine *a = hp->hero_eng;
                EXTengine *c = me->hero_eng;
                s32 dx;
                s32 dz;

                dx = a->unk10.x - c->unk10.x;
                dz = a->unk10.z - c->unk10.z;
                if (dx * dx + dz * dz <= 0x4D1C3F) {
                    all[6] = -1;
                    POLAR_ATTACK(fux, hp, player, i, f, j);
                }
            }
            if (*fl & 4) {
                all[6] = -1;
                if (fux->state != 6 || fux->unk24 == 0) {
                    fux->unk24 = 0xD2;
                }
                fux->state = 6;
                fux->func = PolarAI_AvoidMelt;
                fux->unk2C = i;
                fux->unk3C = 1;
                fux->unk34 |= 1;
                if (TestForTeamMate(player, i)) {
                    POLAR_ATTACK(fux, hp, player, i, f, j);
                }
            }
            if (*fl & 8) {
                all[6] = -1;
                POLAR_ATTACK(fux, hp, player, i, f, j);
            }
            if (*fl & 0x20) {
                all[6] = -1;
                POLAR_ATTACK(fux, hp, player, i, f, j);
            }
            if (*fl & 2) {
                all[6] = -1;
                if (fux->unk24 <= 0 && (fux->state != 6 || fux->unk24 == 0)) {
                    fux->unk24 = 0x168;
                }
                fux->state = 6;
                fux->func = PolarAI_AvoidMelt;
                fux->unk2C = i;
                fux->unk3C = 0;
                fux->unk34 |= 1;
                if (TestForTeamMate(player, i)) {
                    POLAR_ATTACK(fux, hp, player, i, f, j);
                }
            }
            if (*fl & 0x40) {
                all[6] = -1;
                POLAR_ATTACK(fux, hp, player, i, f, j);
            }
            if (*fl & 0x80) {
                all[6] = -1;
                POLAR_ATTACK(fux, hp, player, i, f, j);
            }
            if ((*fl & 0x200) && all[6] == 1) {
                all[6] = -1;
                POLAR_ATTACK(fux, hp, player, i, f, j);
            }
        next2:
            fl++;
            } while (0);
            } while (0);
            me++;
            i++;
            goto loop2;
        }
        i = 0;
        if (PolarAIFlags[player] & 4) {
            fux->unk3C = 2;
            if (fux->state != 1 || fux->unk24 == 0) {
                fux->unk24 = 0xD2;
            }
            if (fux->unk58 == 0) {
                s32 t = PolarAI_PickVictim(player);

                fux->unk58 = 1;
                if (t == -1) {
                    fux->state = 1;
                    fux->target = i;
                    fux->func = PolarAI_DoAttack;
                    fux->unk34 |= 2;
                } else {
                    fux->func = PolarAI_DoAttack;
                    fux->target = t;
                    fux->state = 1;
                    fux->unk34 &= ~2;
                }
            }
        }
        if (PolarAIFlags[player] & 8) {
            fux->unk34 |= 1;
            if (fux->state != 6 || fux->unk24 == 0) {
                fux->unk24 = 0xD2;
            }
            fux->state = 6;
            fux->unk3C = 2;
            fux->unk2C = i;
            fux->func = PolarAI_AvoidMelt;
        }
        if (PolarAIFlags[player] & 2) {
            fux->unk3C = 0;
            if (fux->state != 1 || fux->unk24 == 0) {
                fux->unk24 = 0x168;
            }
            if (fux->unk58 == 0) {
                s32 t = PolarAI_PickVictim(player);

                fux->unk58 = 1;
                if (t == -1) {
                    fux->state = 1;
                    fux->target = i;
                    fux->func = PolarAI_DoAttack;
                    fux->unk34 |= 2;
                } else {
                    fux->func = PolarAI_DoAttack;
                    fux->target = t;
                    fux->state = 1;
                    fux->unk34 &= ~2;
                }
            }
        }
        if (PolarAIFlags[player] & 0x40) {
            fux->unk34 |= 1;
            if (fux->state != 6 || fux->unk24 == 0) {
                fux->unk24 = 0xD2;
            }
            fux->state = 6;
            fux->unk3C = 2;
            fux->unk2C = i;
            fux->func = PolarAI_AvoidMelt;
        }
        if (PolarAIFlags[player] & 0x100) {
            fux->unk34 |= 1;
            if (fux->state != 6 || fux->unk24 == 0) {
                fux->unk24 = 0xD2;
            }
            fux->state = 0xC;
            fux->unk3C = 1;
            fux->unk2C = i;
            fux->func = PolarAI_DoIceWalk;
        }
        if (PolarAIFlags[player] & 0x200) {
            fux->unk34 |= 1;
            if (fux->state != 6 || fux->unk24 == 0) {
                fux->unk24 = 0xD2;
            }
            fux->state = 6;
            fux->unk3C = 2;
            fux->unk2C = i;
            fux->func = PolarAI_AvoidMelt;
        }
        if (PolarAIFlags[player] & 0x800) {
            fux->unk34 |= 1;
            if (fux->state != 6 || fux->unk24 == 0) {
                fux->unk24 = 0xD2;
            }
            fux->state = 6;
            fux->unk3C = 2;
            fux->unk2C = i;
            fux->func = PolarAI_AvoidMelt;
        }
    }
    {
        HeroInfo *hb2 = Heroes;

        e = &hb2[player];
    }
    if ((u16)(e->hero_mode - 0x4B) < 2) {
        e->hero_ai->AImode = 0;
    stop:
        fux->state = 5;
        fux->func = PolarEmptyStub6;
    }
}

void InitialisePOLAR(void) {
    SceneObj *obj;
    camera_info_tag *cam;
    s32 i;
    s32 *speed;
    HeroInfo *h;
    PolarView *v;
    u8 *g;
    EXTheromove *move;
    PolarFux *fux;
    PolarAi *ai;
    subengine *sub;
    EXTengine *eng;
    EXTengine *eng2;
    NAS *nas;
    u16 status;
    u16 ang;
    s32 fast;
    s32 slow;
    PolarView *w;
    HeroInfo *hp;
    s32 j;
    void (*fn)(void);

    PolarBears = NULL;
    Init_Clock(0x140, 0x28, 0);
    PolarInitScene();
    PolarUpdateWord = 0;
    PolarRoundState = 0;
    HeroHooks.RunSpeeds = PolarHeroSfx;
    PolarIceRadius = 0x658;
    obj = ((SceneObj *)MapScene->subs);
    PolarInitWord = 0x800;
    while (obj != NULL) {
        obj->flags |= 0x1000000;
        obj = obj->next;
    }
    PolarRoundState = 0;
    cam = &MainCamera;
    cam->at.x = 0;
    cam->at.y = 0;
    cam->at.z = 0;
    if (((PolarView *)GameInfo)->mode != 0x16) {
        if (((PolarView *)GameInfo)->flags & 0x40) {
            Set_Clock(0x45, 0x81C0);
        } else {
            Set_Clock(0x130, 0x81C0);
        }
        if (((PolarView *)GameInfo)->mode == 0x13) {
            if (((PolarView *)GameInfo)->flags & 0x40) {
                Set_Clock(0x100, 0x81C0);
            } else {
                Set_Clock(0x130, 0x81C0);
            }
            if (((PolarView *)GameInfo)->flags & 0x40) {
                if (((PolarView *)GameInfo)->unk6C != 0) {
                    Set_Clock(0x45, 0x81C0);
                }
            }
        }
        if (((PolarView *)GameInfo)->mode == 0x12) {
            if (((PolarView *)GameInfo)->flags & 0x40) {
                Set_Clock(0x30, 0x81C0);
            } else {
                Set_Clock(0x130, 0x81C0);
            }
            if (((PolarView *)GameInfo)->unk6C != 0) {
                if (((PolarView *)GameInfo)->flags & 0x40) {
                    Set_Clock(0x25, 0x81C0);
                }
            }
        }
        if (((PolarView *)GameInfo)->mode == 0x14) {
            if (((PolarView *)GameInfo)->flags & 0x40) {
                Set_Clock(0x25, 0x81C0);
            } else {
                Set_Clock(0x130, 0x81C0);
            }
            if (((PolarView *)GameInfo)->unk6C != 0) {
                if (((PolarView *)GameInfo)->flags & 0x40) {
                    Set_Clock(0x20, 0x81C0);
                }
            }
        }
        if (((PolarView *)GameInfo)->mode == 0x15) {
            if (((PolarView *)GameInfo)->flags & 0x40) {
                Set_Clock(0x30, 0x81C0);
            } else {
                Set_Clock(0x130, 0x81C0);
            }
            if (((PolarView *)GameInfo)->unk6C != 0
                && (((PolarView *)GameInfo)->flags & 0x40)) {
                Set_Clock(0x25, 0x81C0);
            }
        }
    }
    PolarInitDebug();
    dbase_load_all_mesh_anims((s32 *)&HeroModelBank[DB_Polar]);
    j = 0;
    fast = 0x1000;
    slow = 0x2000;
    speed = PolarAIPace;
    hp = Heroes;
    PolarBeamTimer = 0;
    PolarClock = 0;
    MapSceneRot.vx = 0;
    MapSceneRot.vz = 0;
    MapSceneRot.vy = 0;
    PolarTiltZ = 0;
    PolarTiltX = 0;
    polarbossnas = NULL;
    PolarAnimFlag = 0;
    CameraMode = 1;
    do {
        if (hp->hero_ai->AImode != 0) {
            *speed = fast;
        } else {
            *speed = slow;
        }
        speed++;
        j++;
        hp++;
    } while (j < 4);
    i = 0;
    v = (PolarView *)GameInfo;
    fn = PolarEmptyStub6;
    h = Heroes;
    g = (u8 *)v;
loop:
    if (i < 4) {
        move = h->hero_move;
        if (move != NULL) {
            if (((PolarView *)g)->unk50[0] == 9) {
                status = h->hero_status;
                h->hero_status = status | 0x2000;
                if (((PolarView *)g)->unk50[0] == 9) {
                    h->hero_status = status | 0xA000;
                }
            }
            PolarInitHeroMove(i);
            SetHeroAction(i, 0x49);
            if (v->mode == 0x15 && (v->flags & 0x80) && ((PolarView *)g)->unk5C[0] == 0) {
                SetHeroAction(i, 0x54);
            }
            h->hero_eng->unk5C = 1;
            h->hero_eng->unk5D = 0;
            h->hero_eng->unk10.y = 0;
            eng = h->hero_eng;
            eng->unk2C.z = 0x1000;
            eng->unk2C.y = 0x1000;
            eng->unk2C.x = 0x1000;
            if (v->mode == 0x12) {
                dbase_load_all_mesh_anims((s32 *)&HeroModelBank[DB_ArenaPolarTiltSeal]);
            }
            if (v->mode == 0x15) {
                dbase_load_all_mesh_anims((s32 *)&HeroModelBank[DB_PolarObjects]);
            }
            ResetMass(i);
            eng2 = h->hero_eng;
            ang = ((get_angle(eng2->unk10.x - PolarCentre.vx, eng2->unk10.z - PolarCentre.vz) + 0x800) & 0xFFF) + 0x800;
            move->forward_angle = ang;
            h->hero_eng->unk1E = ang;
            h->hero_eng->unk5E = 1;
            h->hero_shadow->unk1E = h->hero_eng->unk1E;
            h->hero_ai->AItimer1 = 0;
            fux = (PolarFux *)h->hero->FUX;
            if (fux == NULL) {
                fux = AddExt(h->hero, 0x60, 0x10000000);
                if (fux != NULL) {
                    h->hero->FUX = (EXT *)fux;
                    fux->unk24 = 0;
                    fux->unk38 = 0;
                    fux->unk48 = 0;
                    fux->unk30 = 5;
                    fux->unk44 = fn;
                    h->hero_frozen = 0;
                    fux->unk34 = 0;
                    fux->unk4C = 0;
                    fux->unk50 = 0;
                    fux->unk54 = 0;
                    fux->unk58 = 0;
                    sub = h->hero_eng->sub;
                    sub->flags |= 0x2000000;
                    h->hero_eng->sub->unk68 = 0;
                }
                ai = AddExt(h->hero, 0xB8, 0x20000000);
            } else {
                fux->unk24 = 0;
                fux->unk38 = 0;
                fux->unk48 = 0;
                fux->unk30 = 5;
                fux->unk44 = fn;
                h->hero_frozen = 0;
                fux->unk34 = 0;
                fux->unk4C = 0;
                fux->unk50 = 0;
                fux->unk54 = 0;
                fux->unk58 = 0;
                fux->unk5C = 0;
                sub = h->hero_eng->sub;
                sub->flags |= 0x2000000;
                h->hero_eng->sub->unk68 = 0;
                ai = FindExtension(h->hero, 0x20000000);
            }
            if (ai != NULL) {
                ai->unk10 = 0x1E;
                ai->unkC = 0;
                ai->unk14 = 0;
                ai->unk18 = 0;
                ai->unk1C = 0;
                ai->unk20 = 0x100;
                ai->unk24 = 0;
                ai->unk28 = 0;
                ai->unk98 = 0;
                ai->unkA0 = 0;
                ai->unkA4 = 0;
                ai->unkA8 = 0;
                ai->unkAC = 0;
                ai->unkB0 = NULL;
                ai->unkB0 = AddExtEngine(h->hero, 0x14);
                ai->unkB4 = 0;
            }
            PolarAIFlags[i] = 0;
            PolarAIFlags[6] = 0;
        }
        h++;
        g++;
        i++;
        goto loop;
    }
    if (GameMode != 0x16) {
        nas = CreateNAS(0, 0, 0, 0x1606, 0x11);
        if (nas != NULL) {
            nas->UserInfo.user1 = 0;
        }
        nas = CreateNAS(0, 0, 0, 0x1606, 0x11);
        if (nas != NULL) {
            nas->UserInfo.user1 = 1;
        }
        nas = CreateNAS(0, 0, 0, 0x1606, 0x11);
        if (nas != NULL) {
            nas->UserInfo.user1 = 2;
        }
        nas = CreateNAS(0, 0, 0, 0x1606, 0x11);
        if (nas != NULL) {
            nas->UserInfo.user1 = 3;
        }
    }
    w = (PolarView *)GameInfo;
    if (w->mode == 0x15) {
        CreateNAS(0, 0, 0, 0x160E, 0x11);
    }
    if (w->mode == 0x16) {
        dbase_load_all_mesh_anims((s32 *)&HeroModelBank[DB_RoboPolar]);
        SetPanelHeroIconB(0x82, 0x3031);
        InitBossHealthIcons();
        CreateNAS(0, 0, 0, 0x1625, 0x11);
    }
    if (w->mode == 0x12) {
        if (!(w->flags & 0x80)) {
            CreateNAS(0, 0, 0, 0x160C, 0x11);
        } else {
            PlaySfx(0x47E, NULL, NULL, 0x1000, 0x1E00, 0);
        }
    }
}

void UpdatePOLAR(void) {
    VECTOR pos;
    s32 dead;
    s32 n;
    s32 kind;
    s32 i;
    s32 snd;
    s32 j;
    s32 drop;
    PolarLink *l;
    EXTheromove *mv;

    dead = 0;
    if (((PolarModeView *)GameInfo)->mode == 0x12
        && (((PolarModeView *)GameInfo)->flags & 0x80)
        && PolarClock == 0) {
        if (PolarUpdateWord-- < 0) {
            n = Random(3);
            if (n == 0) {
                n = 1;
            }
            kind = Random(6);
            drop = 0;
            while (1) {
                if (drop >= n) {
                    break;
                }
                pos.vx = -Random(0x1300) + Random(0x1300);
                pos.vz = 0x1D00;
                pos.vy = 0x200;
                if (++kind == 6) {
                    kind = 0;
                }
                if (kind == 0) {
                    PolarSpawnDrop(&pos, 0);
                }
                if (kind == 1) {
                    PolarSpawnDrop(&pos, 2);
                }
                if (kind == 2) {
                    PolarSpawnDrop(&pos, 3);
                }
                if (kind == 3) {
                    PolarSpawnDrop(&pos, 4);
                }
                if (kind == 4) {
                    PolarSpawnDrop(&pos, 5);
                }
                if (kind == 5) {
                    PolarSpawnDrop(&pos, 6);
                }
                PolarUpdateWord = (Random(2) + 1) * 30;
                snd = Random(4);
                if (snd == 0) {
                    PlaySfx(0x47F, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (snd == 1) {
                    PlaySfx(0x480, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (snd == 2) {
                    PlaySfx(0x481, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (snd == 3) {
                    PlaySfx(0x482, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                drop++;
            }
        }
    }
    if (GameMode == 0x2C && (Pads[0].unk54 & 0x2000)) {
        ScriptFxOn ^= 1;
    }
    if (PolarBeamOwner != NULL) {
        l = PolarBeamOwner->unk58;
        if (l != NULL) {
            if (PolarAIFlags[6] == 0 && l->unk10 != -1) {
                PolarAIFlags[6] = 1;
            }
            if (PolarAIFlags[6] == -1 && l->unk10 == PolarAIFlags[6]) {
                PolarAIFlags[6] = 0;
            }
        }
    }
    PolarUpdateScene();
    for (j = 0; j < 4; j++) {
        if (Heroes[j].hero_ai->AImode != 0) {
            PolarAIPace[j] = 0x1000;
        } else {
            PolarAIPace[j] = 0x2000;
        }
    }
    if (BombMaster != NULL) {
        FindExtension(BombMaster, 0x10000000);
    }
    if (HeroesAllOut == 0) {
        HeroesAllOut = 1;
        i = 0;
        do {
            if (!(Heroes[i].hero_status & 0xFA00)) {
                if (GameInfo[i + 0x5C] == 0) {
                    HeroesAllOut = 0;
                }
                if (((PolarGameView *)GameInfo)->unk6C != 0
                    && GameInfo[Heroes[i].hero_teammate + 0x5C] == 0) {
                    HeroesAllOut = 0;
                }
            }
            i++;
        } while (i < 4);
    }
    i = 0;
    PolarAnimFlag++;
    while (1) {
        if (i >= 4) {
            break;
        }
        if (Heroes[i].hero_eng->unk10.y > 0) {
            RemoveCamera(Heroes[i].hero);
        }
        if (Heroes[i].hero_mode == 0x50) {
            dead++;
        }
        if (dead == 3) {
            PolarClock = 1;
        }
        if (Heroes[i].hero_status & 0x200) {
            mv = Heroes[i].hero_move;
            PolarClock = 1;
            if (mv != NULL) {
                mv->z_dir = 0;
                mv->x_max = 0;
                mv->z_max = 0;
                mv->x_vel = 0;
                mv->z_vel = 0;
            }
        }
        i++;
    }
    if (GameExitFlags & 0x800) {
        PolarClock = 1;
    }
    if (GameExitFlags & 1) {
        PolarClock = 1;
    }
}

s32 PolarHeroCollisions(s32 player) {
    s32 hit;
    s32 range;
    HeroInfo *o;
    EXTengine *eng;
    EXTengine *e; /* one pointer for every engine/extension in turn (its register needs it) */
    EXTheromove *mv;
    HeroInfo *h;
    HeroInfo *hp;
    HeroInfo *hq;
    PolarAi *ai;
    s32 swapped;
    TrigEntry *t;
    u8 *p;
    u32 i;
    s32 dx;
    s32 r;
    s32 dz;
    s32 d;
    s32 ang;
    s32 pick;
    s32 spd;
    s32 snd;
    s32 k;

    hit = 0;
    eng = Heroes[player].hero_eng;
    range = 0x100;
    if (eng != NULL) {
    i = 0;
    while (1) {
        if (i >= 4) {
            break;
        }
        if (player != i && !(Heroes[i].hero_status & 0x2000)) {
            e = Heroes[i].hero_eng;
            if (e != NULL) {
                dx = e->unk10.x - eng->unk10.x;
                dz = e->unk10.z - eng->unk10.z;
                d = csqrt(dx * dx + dz * dz) >> 6;
                if (d < range) {
                    h = &Heroes[player];
                    e = (EXTengine *)h->hero_move;
                    snd = Random(4);
                    ang = get_angle(dx, dz) & 0xFFF;
                    d = range - d + 8;
                    PolarHeroesCollide(player, i);
                    spd = ((EXTheromove *)e)->x_vel * ((EXTheromove *)e)->x_vel + ((EXTheromove *)e)->z_vel * ((EXTheromove *)e)->z_vel;
                    hit = 1;
                    if (spd > 0x400) {
                        if (h->hero_mode == 0x52) {
                            PlaySfx(0x48B, NULL, NULL, 0x1000, 0x1E00, 0);
                        } else {
                            if (snd == 0) {
                                PlaySfx(0x446, NULL, NULL, 0x1000, 0x1E00, 0);
                            }
                            if (snd == 1) {
                                PlaySfx(0x447, NULL, NULL, 0x1000, 0x1E00, 0);
                            }
                            if (snd == 2) {
                                PlaySfx(0x448, NULL, NULL, 0x1000, 0x1E00, 0);
                            }
                            if (snd == 3) {
                                PlaySfx(0x449, NULL, NULL, 0x1000, 0x1E00, 0);
                            }
                        }
                    }
                    if (spd > 0x1000) {
                        pad_rumble_small(player, 0xA);
                        ((void (*)(s32, s32, s32))pad_rumble_big)(player, 0xA, 0xFF);
                        hp =&Heroes[player];
                        if (hp->hero_mode == 0x52) {
                            goto snow;
                        }
                        if (hp->hero_mode == 0x4C) {
                            goto done;
                        }
                        if (hp->hero_mode == 0x4B) {
                            goto done;
                        }
                        if (hp->hero_mode == 0x5A) {
                            goto done;
                        }
                        e = FindExtension(hp->hero, 0x20000000);
                        if (e != NULL) {
                            ((PolarAi *)e)->unkB0->unk10 = hp->hero_eng->unk10;
                            if (((PolarAi *)e)->unkB0 != NULL) {
                            switch (hp->hero_name) {
                            case 1:
                                r = Random(4);
                                if (r == 0) {
                                    PlaySfx(3, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (r == 1) {
                                    PlaySfx(4, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (r == 2) {
                                    PlaySfx(5, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (r == 3) {
                                    PlaySfx(6, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                break;
                            case 2:
                                r = Random(4);
                                if (r == 0) {
                                    PlaySfx(0x35, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (r == 1) {
                                    PlaySfx(0x36, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (r == 2) {
                                    PlaySfx(0x37, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (r == 3) {
                                    PlaySfx(0x38, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                break;
                            case 3:
                                r = Random(4);
                                if (r == 0) {
                                    PlaySfx(0x67, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (r == 1) {
                                    PlaySfx(0x68, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (r == 2) {
                                    PlaySfx(0x69, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (r == 3) {
                                    PlaySfx(0x6A, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                break;
                            case 4:
                                r = Random(4);
                                if (r == 0) {
                                    PlaySfx(0x99, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (r == 1) {
                                    PlaySfx(0x9A, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (r == 2) {
                                    PlaySfx(0x9B, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (r == 3) {
                                    PlaySfx(0x9C, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                break;
                            case 5:
                                pick = Random(4);
                                if (pick == 0) {
                                    PlaySfx(0x12F, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (pick == 1) {
                                    PlaySfx(0x130, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (pick == 2) {
                                    PlaySfx(0x131, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (pick == 3) {
                                    PlaySfx(0x132, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                break;
                            case 7:
                                pick = Random(4);
                                if (pick == 0) {
                                    PlaySfx(0xFD, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (pick == 1) {
                                    PlaySfx(0xFE, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (pick == 2) {
                                    PlaySfx(0xFF, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (pick == 3) {
                                    PlaySfx(0x100, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                break;
                            case 8:
                                pick = Random(4);
                                if (pick == 0) {
                                    PlaySfx(0xCB, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (pick == 1) {
                                    PlaySfx(0xCC, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (pick == 2) {
                                    PlaySfx(0xCD, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (pick == 3) {
                                    PlaySfx(0xCE, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                break;
                            case 6:
                                pick = Random(4);
                                if (pick == 0) {
                                    PlaySfx(0x161, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (pick == 1) {
                                    PlaySfx(0x162, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (pick == 2) {
                                    PlaySfx(0x163, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (pick == 3) {
                                    PlaySfx(0x164, &((PolarAi *)e)->unkB0->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                break;
                            }
                            }
                        }
                        goto done;
                    snow:
                        printf("snow\n");
                        PlaySfx(0x48B, NULL, NULL, 0x1000, 0x1E00, 0);
                    }
                done:
                    if (PolarBeamOwner != NULL) {
                        e = FindExtension((NAS *)PolarBeamOwner, 0x40000000);
                        if (e != NULL) {
                            swapped = 0;
                            if (player == ((PolarGame *)e)->unk6C && ((PolarGame *)e)->unk68 > 0) {
                                if (((PolarGame *)e)->unk48[i] == 0) {
                                    ((PolarGame *)e)->unk6C = i;
                                    ((PolarGame *)e)->unk48[player] = 0xF;
                                    PlaySfx(0x454, NULL, NULL, 0x1000, 0x1E00, 0);
                                    swapped = 1;
                                    ((PolarGame *)e)->unk48[i] = 0;
                                }
                            }
                            if (i == ((PolarGame *)e)->unk6C && swapped == 0 && ((PolarGame *)e)->unk68 > 0) {
                                if (((PolarGame *)e)->unk48[player] == 0) {
                                    ((PolarGame *)e)->unk6C = player;
                                    ((PolarGame *)e)->unk48[i] = 0xF;
                                    PlaySfx(0x454, NULL, NULL, 0x1000, 0x1E00, 0);
                                    ((PolarGame *)e)->unk48[player] = 0;
                                }
                            }
                        }
                    }
                    eng->unk10.x -= ((d + 2) * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                    eng->unk10.z -= ((d + 2) * rcossin_tbl[ang].unk2) >> 12;
                }
            }
        }
        i++;
    }
    if (GameMode != 0x16) {
        p = PolarBears;
        o = Heroes;
        hq = &o[player];
        while (1) {
            if (p == NULL) {
                break;
            }
            if (((NAS *)p)->UserInfo.status != 0) {
                e = *(EXTengine **)(p + 0x54);
                if (e != NULL && e != eng && PolarBoxOverlap(e, 0, hq->hero_eng, 0) != 0) {
                    mv = FindExtension((NAS *)p, 0x20000000);
                    if (mv != NULL) {
                        hit = 1;
                        PolarMovesSwap(hq->hero_move, mv);
                    }
                }
            }
            p = *(u8 **)(p + 0x20);
        }
    } else {
        range = 0x1F4;
        p = PolarBears;
        t = rcossin_tbl;
        while (1) {
            if (p == NULL) {
                break;
            }
            e = *(EXTengine **)(p + 0x54);
            if (e != NULL && e != eng) {
                ai = FindExtension((NAS *)p, 0x10000000);
                if (ai != NULL && ai->unkB4 == 0 && abs(e->unk10.y - eng->unk10.y) <= range) {
                    dx = e->unk10.x - eng->unk10.x;
                    dz = e->unk10.z - eng->unk10.z;
                    d = csqrt(dx * dx + dz * dz) >> 6;
                    if (d < range) {
                        e = FindExtension((NAS *)p, 0x20000000);
                        if (e != NULL) {
                            ang = get_angle(dx, dz) & 0xFFF;
                            d = range - d + 2;
                            eng->unk10.x -= (d * t[(ang - 0x400) & 0xFFF].unk2) >> 12;
                            eng->unk10.z -= (d * t[ang].unk2) >> 12;
                            PolarHeroBounceOff(player, (EXTheromove *)e);
                            hit = 1;
                            if (((EXTheromove *)e)->x_vel * ((EXTheromove *)e)->x_vel + ((EXTheromove *)e)->z_vel * ((EXTheromove *)e)->z_vel > 0x2400) {
                                k = Random(3);
                                if (k == 0) {
                                    PlaySfx(0x236, NULL, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (k == 1) {
                                    PlaySfx(0x237, NULL, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (k == 2) {
                                    PlaySfx(0x238, NULL, NULL, 0x1000, 0x1E00, 0);
                                }
                            }
                        }
                    }
                }
            }
            p = *(u8 **)(p + 0x20);
        }
    }
    }
    return hit;
}

s32 PolarHeroAnimState(s32 player) {
    HeroInfo *h;
    PolarWanderFux *ext;
    PolarWanderFux *fux;
    u16 name;
    HeroInfo *base;
    s32 state;

    if (PolarAnimFlag < 0xD2) {
        goto fail;
    }
    name = Heroes[player].hero_name;
    if (name != 4 && name != 1 && name != 7 && name != 3 && name != 2) {
        return 0;
    }
    base = Heroes;
    h = &base[player];
    ext = (PolarWanderFux *)FindExtension(h->hero, 0x20000000);
    if (ext == NULL) {
        goto fail;
    }
    if (ext->unk24 != 0) {
        return 0;
    }
    fux = (PolarWanderFux *)h->hero->FUX;
    if (fux == NULL) {
        goto fail;
    }
    state = 6;
    if (fux->state != state && fux->state != 9) {
        return 0;
    }
    if (h->hero_mode != 0x49) {
        return 0;
    }
    if (fux->unk50 != 0) {
        return 0;
    }
    if (h->hero_name == 1) {
        h->hero_anim->Atable = CRA_seq_polartaunt;
        h->hero_anim->Aindex = 1;
    }
    if (h->hero_name == 2) {
        h->hero_anim->Atable = COC_seq_polartaunt;
        h->hero_anim->Aindex = 1;
    }
    if (h->hero_name == 7) {
        h->hero_anim->Atable = DNG_seq_polartaunt;
        h->hero_anim->Aindex = 1;
    }
    if (h->hero_name == 8) {
        h->hero_anim->Atable = KAN_seq_polartaunt;
        h->hero_anim->Aindex = 1;
    }
    if (h->hero_name == 5) {
        h->hero_anim->Atable = TIN_seq_polartaunt;
        h->hero_anim->Aindex = 1;
    }
    if (h->hero_name == state) {
        h->hero_anim->Atable = KON_seq_polartaunt;
        h->hero_anim->Aindex = 1;
    }
    if (h->hero_name == 3) {
        h->hero_anim->Atable = CTX_seq_polartaunt;
        h->hero_anim->Aindex = 1;
    }
    if (h->hero_name == 4) {
        h->hero_anim->Atable = BRO_seq_polartaunt;
        h->hero_anim->Aindex = 1;
    }
    fux->unk50 = 1;
    return 1;
fail:
    return 0;
}

bool TestForTeamMate(u32 arg0, u32 arg1) {
    u8 *g = GameInfo;

    if (*(s32 *)(g + 0x6C) != 0) {
        if (Heroes[arg1].hero_status & 0x2000) {
            return 0;
        }
        if (arg0 == *(s16 *)(g + 0x48) && arg1 == Heroes[arg0].hero_teammate) {
            return 1;
        }
        if (arg1 == GameTeamLeader[0] && arg0 == Heroes[arg1].hero_teammate) {
            return 1;
        }
        if (arg0 == GameTeamLeader[1] && arg1 == Heroes[arg0].hero_teammate) {
            return 1;
        }
        if (arg1 == GameTeamLeader[1] && arg0 == Heroes[arg1].hero_teammate) {
            return 1;
        }
    }
    return 0;
}

void PolarBeamRelease(s32 arg0, s32 arg1, s32 arg2) {
    HeroInfo *base;
    HeroInfo *h;
    EXTengine *eng;
    EXTengine *shadow;

    switch (arg1) {
    case 0:
        if (arg2 == 0) {
            break;
        }
        goto common;
    case 1:
        if (arg2 != arg1) {
        common:
            base = Heroes;
            h = &base[arg0];
            eng = h->hero_eng;
            shadow = h->hero_shadow;
            if (eng != NULL && shadow != NULL) {
                eng->unk2C.x = 0x1000;
                eng->unk2C.y = 0x1000;
                eng->unk2C.z = 0x1000;
                shadow->unk2C.x = 0x1000;
                shadow->unk2C.y = 0x1000;
                shadow->unk2C.z = 0x1000;
                ResetMass(arg0);
                PolarHeroMass[arg0] = HeroHooks.RunSpeeds[h->hero_name * 3];
            }
        }
        break;
    case 2:
        if (arg2 != arg1 && Heroes[arg0].hero_mode != 0x49) {
            SetHeroAction(arg0, 0x49);
        }
        break;
    }
}

void InitTransmuterBeam(NAS *m) {
    TransBeamExt *ext;
    UkaBitExt *bx;
    EXTengine *e;
    subengine *sub;
    subengine *s2;
    NAS *n;
    s32 i;

    ext = AddExt(m, 0x1E0, 0x40000000);
    if (ext == NULL) {
        return;
    }
    BeamCount = 0;
    BeamDelay = 0;
    BeamState = 0;
    BeamTarget = 0;
    BeamTimer = 0;
    ext->unkC = 5;
    PolarBeamOwner = (PolarOwner *)m;
    m->FUX = (EXT *)ext;
    for (i = 0; i < 4; i++) {
        ext->unk18[i] = -1;
        ext->unk28[i] = -1;
        ext->unk38[i] = 0;
        ext->unk48[i] = 0;
        ext->engs[i] = 0;
    }
    ext->unk10 = -1;
    ext->unk6C = 5;
    ext->unk58 = 0;
    ext->unk5C = 0;
    ext->unk60 = 0x78;
    ext->unk64 = Random(0x5A) + 0xD2;
    ext->unk14 = 1;
    ext->unk68 = -1;
    ext->pos = *(SLONG_XYZ *)&m->Engine->unk10;
    ext->unk98 = 0x5DC;
    ext->unk94 = 0;
    ext->unkA0 = 8;
    ext->unk9C = 0;
    ext->unk94 = Random(0x3C);
    m->UserInfo.user1 = 0;
    ext->unk1DC = 0x1E0;
    ext->bit1 = n = CreateNAS(m->Engine->unk10.x, m->Engine->unk10.y, m->Engine->unk10.z, 0x1605, 0x11);
    if (n != NULL) {
        bx = FindExtension(n, 0x10000000);
        if (bx != NULL) {
            if (((PolarModeView *)GameInfo)->mode == 0x12) {
                bx->unkC = 1;
                bx->unk20 = 4;
                goto tex1;
            }
            if (((PolarModeView *)GameInfo)->flags & 0x80) {
                if (((PolarModeView *)GameInfo)->mode == 0x13) {
                    bx->unkC = 1;
                }
                if (((PolarModeView *)GameInfo)->mode != 0x15) {
                    goto skip1;
                }
            }
            bx->unk20 = 4;
            bx->unkC = 4;
        tex1:
            ext->bit1->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
        skip1:
            bx->unk18 = 0x100;
            bx->unk20 |= 8;
            ext->bit1->Engine->flags |= 0x200000;
            ext->bit1->InitDelete(ext->bit1);
        }
    }
    ext->bit2 = n = CreateNAS(m->Engine->unk10.x, m->Engine->unk10.y, m->Engine->unk10.z, 0x1605, 0x11);
    if (n != NULL) {
        n->Engine->flags |= 0x200000;
        bx = FindExtension(ext->bit2, 0x10000000);
        if (bx != NULL) {
            if (((PolarModeView *)GameInfo)->mode == 0x12) {
                bx->unkC = 2;
                bx->unk20 = 4;
                goto tex2;
            }
            if (((PolarModeView *)GameInfo)->flags & 0x80) {
                if (((PolarModeView *)GameInfo)->mode == 0x13) {
                    bx->unkC = 2;
                }
                if (((PolarModeView *)GameInfo)->mode != 0x15) {
                    goto skip2;
                }
            }
            bx->unk20 = 4;
            bx->unkC = 3;
        tex2:
            ext->bit2->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
        skip2:
            bx->unk18 = 0x100;
            ext->bit2->InitDelete(ext->bit2);
            ext->unkAC = 0;
        }
    }
    m->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
    m->Engine->sub->entity = 0x2013;
    m->Engine->unk5D = 2;
    m->Engine->flags |= 0x200000;
    ext->eng = NULL;
    e = AddExtEngine(m, 2);
    if (e != NULL) {
        ext->eng = e;
        e->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
        e->sub->entity = 0x2003;
        e->sub->unk51 = 0;
        s2 = e->sub;
        s2->unk14 = 0;
        s2->unk12 = 0;
        s2->unk10 = 0;
        e->sub->unk50 = 1;
        sub = e->sub;
        sub->unk20.z = 0x200;
        sub->unk20.y = 0x200;
        sub->unk20.x = 0x200;
        e->sub->flags &= ~0x8000;
        e->flags |= 0x200000;
    }
    for (i = 0; i < 4; i++) {
        e = AddExtEngine(m, i + 3);
        if (e != NULL) {
            ext->engs[i] = e;
            e->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
            e->sub->entity = 0x2019;
            e->sub->unk51 = 0;
            s2 = e->sub;
            s2->unk14 = 0;
            s2->unk12 = 0;
            s2->unk10 = 0;
            e->sub->unk50 = 1;
            s2 = e->sub;
            s2->unk20.z = 0x800;
            s2->unk20.y = 0x800;
            s2->unk20.x = 0x800;
            e->sub->flags &= ~0x8000;
            e->flags |= 0x200000;
        }
    }
}

/* The hit switch's cases 0-2 in PlotTransmuterBeam: hero i's timer, then its
   effect kind in both slots. As an inline call, case 1's kind 1 gets its own
   argument pseudo (copy_to_mode_reg), which loop.c needs (MATCHING.md, Diff
   patterns). */
static inline void TBSetHeroEffect(TransBeamExt *e, s32 i, s32 k) {
    e->unk38[i] = 0xB4;
    e->unk18[i] = k;
    e->unk28[i] = k;
}

/* The spawn's `if (TB_MODE13()) c++;` is dead (c is never read): flow empties
   it and jump2 deletes the test, but until then its read keeps the mode test's
   view base live, which is why retail holds that base in a1 for a single lh
   (MATCHING.md, Diff patterns). */
void PlotTransmuterBeam(NAS *m) {
    TransBeamExt *ext;
    HeroInfo *h;
    HeroInfo *h3;
    HeroInfo *h5;
    HeroInfo *hb1;
    HeroInfo *hb2;
    HeroInfo *hb3;
    HeroInfo *hb4;
    s32 k;
    s32 k2;
    s32 k3;
    s32 t2;
    s32 t3;
    s32 t4;
    s32 t5;
    EXTengine *e;
    EXTengine *he;
    EXTengine *hs;
    EXTengine *hq;
    EXTengine *e1;
    EXTengine *e2;
    EXTengine *e3;
    EXTengine *me;
    EXTengine *ce;
    EXTheromove *mv;
    NAS *n;
    ExtSpawn5910 *bx;
    PolarAi *ai;
    PolarAi *a1x;
    WeightDeathExt *wx;
    s32 i;
    s32 j;
    s32 best;
    s32 closest;
    s32 t;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 d;
    s32 yaw;
    s32 pitch;
    u16 mode;
    s16 c;

    ext = FindExtension(m, 0x40000000);
    if (ext == NULL) {
        return;
    }
    best = 0x7FFFFFFF;
    closest = 0;
    if (PolarClock != 0) {
        PolarBeamTimer = 1;
        if (ext->unkAC != NULL) {
            KillNAS(ext->unkAC);
            ext->unkAC = NULL;
        }
        ext->bit2->Engine->flags &= ~0x8000;
    }
    if (TB_MODE13()) {
        if (TBV->unk6C != 0 && ext->unk1DC == 0) {
            t = BeamTarget;
            BeamTarget = t ^ 1;
            if (Heroes[t ^ 1].hero_status & 0x2000) {
                BeamTarget = t;
            }
        }
        if (ext->unk1DC < 0) {
            mode = Heroes[BeamTarget].hero_mode;
            if (mode != 0x4B && mode != 0x5A) {
                BeamCount = 1;
            }
        } else {
            ext->unk1DC--;
        }
    }
    if (TB_MODE13() && TBV->unk10 == 0 && PolarClock == 0 && ext->unk14 == 1) {
        if (++m->UserInfo.user1 == 0x3C) {
            e1 = m->Engine;
            n = CreateNAS(e1->unk10.x, 0x80, e1->unk10.z, 0x1605, 0x11);
            if (n != NULL) {
                n->Engine->flags |= 0x200000;
                bx = (ExtSpawn5910 *)n->FUX;
                if (bx != NULL) {
                    bx->unk18 = 0x200;
                    bx->unkC = 0;
                    bx->unk20 = 5;
                    n->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
                    n->Engine->flags |= 0x200000;
                    bx->unk28 = 6;
                }
                n->InitDelete(n);
                m->UserInfo.user2 = (s32)n;
                m->UserInfo.user1 = 0;
                PlaySfx(0x46D, NULL, NULL, 0x1000, 0x1E00, 0);
            } else {
                m->UserInfo.user2 = 0;
                m->UserInfo.user1 = 0;
            }
        }
        if (m->UserInfo.user1 < 0x20 && m->UserInfo.user2 != 0) {
            if (TB_MODE13()) {
                if (ext->bit1 != NULL) {
                    ext->bit1->Engine->sub->flags |= 0x8000;
                }
                if (ext->bit2 != NULL) {
                    ext->bit2->Engine->sub->flags |= 0x8000;
                }
            }
            for (i = 0;; i++) {
                s32 lx;
                s32 lz;

                if (i >= 2) {
                    break;
                }
                mode = Heroes[i].hero_mode;
                if (mode == 0x4D) {
                    goto next1;
                }
                if (mode == 0x4E) {
                    goto next1;
                }
                if (mode == 0x4B) {
                    goto next1;
                }
                if (mode == 0x4F) {
                    goto next1;
                }
                if (mode == 0x58) {
                    goto next1;
                }
                if (Heroes[i].hero_status & 0x2000) {
                    goto next1;
                }
                if (TBV->unk5C[i] != 0) {
                    goto next1;
                }
                if (i == 1 && TBV->unk6C == 0) {
                    goto next1;
                }
                lx = m->Engine->unk10.x - Heroes[i].hero_eng->unk10.x;
                lz = m->Engine->unk10.z - Heroes[i].hero_eng->unk10.z;
                if (lx * lx + lz * lz > 0x18FFF) {
                    goto next1;
                }
                t = Random(0x1000);
                if (Heroes[i].hero_mode == 0x5A) {
                    goto next1;
                }
                Heroes[i].hero_move->x_vel = rcossin_tbl[(t - 0x400) & 0xFFF].unk2 * 128 >> 12;
                Heroes[i].hero_move->z_vel = rcossin_tbl[t & 0xFFF].unk2 * 128 >> 12;
                SetHeroAction(i, 0x5A);
                BeamTimer = 1;
            next1:;
            }
            ((NAS *)m->UserInfo.user2)->Engine->unk10.x = m->Engine->unk10.x;
            ((NAS *)m->UserInfo.user2)->Engine->unk10.z = m->Engine->unk10.z;
        }
    }
    for (i = 0;; i++) {
        s32 lx;
        s32 lz;
        s32 ld;

        if (i >= 4) {
            break;
        }
        if (!(Heroes[i].hero_status & 0x2000)) {
            lx = m->Engine->unk10.x - Heroes[i].hero_eng->unk10.x;
            lz = m->Engine->unk10.z - Heroes[i].hero_eng->unk10.z;
            ld = lx * lx + lz * lz;
            if (ld < best) {
                closest = i;
                best = ld;
            }
            if (ext->unk48[i] > 0) {
                ext->unk48[i]--;
            }
        }
    }
    if (ext->unk14 == 1) {
        if (BeamCount != 0) {
            h3 = Heroes;
            h3 = &h3[BeamTarget];
            if (h3->hero_mode == 0x4B) {
                BeamTimer = 1;
            }
            if (BeamDelay == 0x13B || BeamTimer != 0) {
                s32 a;
                s32 sa;
                s32 ox;
                s32 oz;
                EXTengine *me;
                TrigEntry *tr;

                tr = rcossin_tbl;
                a = BeamState >> 1;
                me = m->Engine;
                ox = me->unk10.x - (ext->pos.x + (((ext->unk98 << 8) >> 8) * tr[(tr[(a - 0x400) & 0xFFF].unk2 - 0x400) & 0xFFF].unk2 >> 12));
                sa = tr[a & 0xFFF].unk2;
                oz = me->unk10.z - (ext->pos.z + (ext->unk98 * tr[sa & 0xFFF].unk2 >> 12));
                me->unk10.x -= ox >> 4;
                m->Engine->unk10.z -= oz >> 4;
                if (ox * ox + oz * oz < 0x1000) {
                    BeamCount = 0;
                    BeamDelay = 0;
                    BeamTimer = 0;
                    ext->unk1DC = (Random(4) + 0x12) * 0x1E;
                }
            } else {
                EXTengine *me;
                EXTengine *te;
                s32 ddx;
                s32 ddz;

                me = m->Engine;
                te = h3->hero_eng;
                BeamDelay++;
                ddx = me->unk10.x - te->unk10.x;
                ddz = me->unk10.z - te->unk10.z;
                me->unk10.x -= ddx >> 5;
                m->Engine->unk10.z -= ddz >> 5;
            }
        } else {
            TrigEntry *tr;

            tr = rcossin_tbl;
            t2 = BeamState >> 1;
            m->Engine->unk10.x = ext->pos.x + (((ext->unk98 << 8) >> 8) * tr[(tr[(t2 - 0x400) & 0xFFF].unk2 - 0x400) & 0xFFF].unk2 >> 12);
            BeamTimer = 0;
            t5 = tr[t2 & 0xFFF].unk2;
            m->Engine->unk10.z = ext->pos.z + (ext->unk98 * tr[t5 & 0xFFF].unk2 >> 12);
        }
        if (PolarClock == 0) {
            m->Engine->unk10.y = ext->pos.y + (rcossin_tbl[((ext->unk9C << 4) - 0x400) & 0xFFF].unk2 * 64 >> 12);
        } else {
            m->Engine->unk10.y -= 0x10;
        }
        ext->unk9C += ext->unkA0;
        if (BeamCount == 0) {
            BeamState += ext->unkA0;
        }
        if (ext->bit1 != NULL) {
            ext->bit1->Engine->unk10 = m->Engine->unk10;
            ext->bit1->Engine->sub->unk68 = 0x200;
            if (GameMode == 0x12) {
                ext->bit1->Engine->unk10.y -= 0x40;
            } else {
                ext->bit1->Engine->unk10.y -= 0x60;
            }
        }
    }
    m->Engine->unk5E = 1;
    if (ext->bit2 != NULL) {
        ext->bit2->Engine->unk10 = m->Engine->unk10;
        ext->bit2->Engine->sub->unk68 = 0x100;
        ext->bit2->Engine->unk10.y = 0x5E;
    }
    me = m->Engine;
    ce = Heroes[closest].hero_eng;
    dx = me->unk10.x - ce->unk10.x;
    dz = me->unk10.z - ce->unk10.z;
    dy = (me->unk10.y - ce->unk10.y) + 0x1C2;
    d = csqrt(dx * dx + dz * dz) >> 6;
    yaw = get_angle(dx, dz) & 0xFFF;
    pitch = get_angle(-dy, d);
    if (ext->unk14 == 1 && BeamCount == 0 && PolarBeamTimer == 0) {
        ext->unk58++;
        if (!TB_MODE13() || m->UserInfo.user1 >= 0x1F) {
            if (abs(ext->unk58 - ext->unk64) == 0xF) {
                PlaySfx(0x463, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (ext->unk58 >= ext->unk64) {
                ext->unk5C = Random(4);
                ext->unk58 = 0;
                ext->unkC = BeamFrames[ext->unk5C];
                if (HeroesAllOut != 0) {
                    if (ext->unkC == 0) {
                        ext->unkC = 6;
                    }
                    if (ext->unkC == 1) {
                        ext->unkC = 5;
                    }
                }
                e2 = m->Engine;
                n = CreateNAS(e2->unk10.x, 0x5E, e2->unk10.z, 0x1605, 0x11);
                if (n != NULL) {
                    ext->unkAC = n;
                    n->Engine->flags |= 0x200000;
                    bx = FindExtension(n, 0x10000000);
                    if (bx != NULL) {
                        if (TBV->mode == 0x12) {
                            bx->unkC = 1;
                        } else {
                            bx->unkC = 0;
                            if (TB_MODE13()) {
                                c++;
                            }
                        }
                        if (GameMode == 0x15) {
                            bx->unkC = 0;
                        }
                        bx->unk18 = 0x100;
                        bx->unk20 = 1;
                        bx->unk28 = 0;
                        n->InitDelete(n);
                    }
                }
                ext->unk14 = 2;
                PlaySfx(0x44C, &m->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
            }
        }
    }
    if (ext->unk14 == 2) {
        if (PolarClock != 0) {
            ext->unk14 = 1;
            if (m->Engine->sfx != NULL) {
                StopSfx(m->Engine->sfx);
            }
        }
        if (++ext->unk58 >= ext->unk60) {
            ext->unk14 = 1;
            PlaySfx(0x44D, NULL, NULL, 0x1000, 0x1E00, 0);
            if (m->Engine->sfx != NULL) {
                StopSfx(m->Engine->sfx);
            }
            ext->unk58 = 0;
        }
        pitch = 0x400;
    }
    m->Engine->unk1C -= ((m->Engine->unk1C - pitch) << 20) >> 22;
    m->Engine->unk1E -= ((m->Engine->unk1E - yaw) << 20) >> 22;
    m->Engine->unk20 = 0;
    if (TB_MODE13()) {
        if (ext->bit1 != NULL) {
            ext->bit1->Engine->sub->entity = 0x2008;
        }
        if (ext->bit2 != NULL) {
            ext->bit2->Engine->sub->entity = 0x2008;
        }
    }
    if (ext->unk14 & 2) {
        if (TB_MODE13()) {
            if (ext->bit1 != NULL) {
                ext->bit1->Engine->sub->entity = 0x200C;
            }
            if (ext->bit2 != NULL) {
                ext->bit2->Engine->sub->entity = 0x200C;
            }
        }
        for (i = 0;; i++) {
            s32 lx;
            s32 lz;
            s32 ld;

            if (i >= 4) {
                break;
            }
            lx = Heroes[i].hero_eng->unk10.x - m->Engine->unk10.x;
            lz = Heroes[i].hero_eng->unk10.z - m->Engine->unk10.z;
            ld = lx * lx + lz * lz;
            if (Heroes[i].hero_status & 0x2000 || ext->unk10 != -1 || ld > 0x18FFF) {
                continue;
            }
            ext->unk10 = i;
            ext->unk14 = 1;
            ext->unk58 = 0;
            if (m->Engine->sfx != NULL) {
                StopSfx(m->Engine->sfx);
            }
            if (ext->unkAC != NULL) {
                KillNAS(ext->unkAC);
                ext->unkAC = NULL;
            }
            e3 = m->Engine;
            n = CreateNAS(e3->unk10.x, 0x5E, e3->unk10.z, 0x1605, 0x11);
            if (n != NULL) {
                n->Engine->flags |= 0x200000;
                bx = (ExtSpawn5910 *)n->FUX;
                if (bx != NULL) {
                    bx->unkC = 3;
                    if (TBV->mode == 0x15) {
                        bx->unkC = 2;
                    }
                    if (TBV->mode == 0x13) {
                        bx->unkC = 3;
                    }
                    if (TBV->mode == 0x12 && (TBV->flags & 0x80)) {
                        bx->unkC = 7;
                    }
                    bx->unk18 = 0x80;
                    bx->unk20 = 1;
                    bx->unk28 = 0;
                }
                n->InitDelete(n);
            }
            switch (ext->unkC) {
            case 0:
                PolarBeamRelease(i, ext->unk28[i], 0);
                ext->unk60 = 0x78;
                ext->unk64 = Random(0x3C) + 0xD2;
                TBSetHeroEffect(ext, i, 0);
                Heroes[i].hero_move->mass = 0x50;
                PolarHeroMass[i] = 0x40;
                ext->unk70 = 0;
                PlaySfx(0x44E, NULL, NULL, 0x1000, 0x1E00, 0);
                SetHeroAction(i, 0x4F);
                break;
            case 1:
                PolarBeamRelease(i, ext->unk28[i], 1);
                ext->unk60 = 0x78;
                ext->unk64 = Random(0x3C) + 0xD2;
                TBSetHeroEffect(ext, i, 1);
                Heroes[i].hero_move->mass = 0x258;
                PolarHeroMass[i] = 0x20;
                ext->unk70 = 0;
                PlaySfx(0x44F, NULL, NULL, 0x1000, 0x1E00, 0);
                SetHeroAction(i, 0x4F);
                break;
            case 2:
                PolarBeamRelease(i, ext->unk28[i], 2);
                ext->unk60 = 0x78;
                ext->unk64 = Random(0x3C) + 0xD2;
                TBSetHeroEffect(ext, i, 2);
                ext->unk70 = 0xF;
                PlaySfx(0x450, NULL, NULL, 0x1000, 0x1E00, 0);
                SetHeroAction(i, 0x4F);
                break;
            case 5:
                PolarBeamRelease(i, ext->unk28[i], 5);
                ext->unk60 = 0x78;
                ext->unk64 = 0x168;
                ext->unk38[i] = 0;
                ext->unk18[i] = 5;
                ext->unk70 = 0xF;
                ext->unk28[i] = 5;
                PlaySfx(0x451, NULL, NULL, 0x1000, 0x1E00, 0);
                PlaySfx(0x452, &ext->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                SetHeroAction(i, 0x4F);
                a1x = FindExtension(Heroes[ext->unk10].hero, 0x20000000);
                if (a1x != NULL) {
                    a1x->unk28 = 0;
                }
                break;
            case 6:
                ext->unk60 = 0x78;
                ext->unk64 = 0x168;
                ext->unk38[i] = 0;
                ext->unk18[i] = 6;
                ext->unk70 = 0xF;
                ext->unk28[i] = 6;
                PlaySfx(0x464, NULL, NULL, 0x1000, 0x1E00, 0);
                e = m->Engine;
                PlaySfx(0x465, &e->sfx, &e->unk10, 0x1000, 0x1E00, 0);
                e = ext->eng;
                PlaySfx(0x452, &e->sfx, &e->unk10, 0x1000, 0x1E00, 0);
                SetHeroAction(i, 0x4F);
                a1x = FindExtension(Heroes[ext->unk10].hero, 0x20000000);
                if (a1x != NULL) {
                    a1x->unk28 = 0;
                }
                break;
            }
        }
    }
    if (ext->unk10 != -1) {
        hb1 = Heroes;
        h5 = &hb1[ext->unk10];
        he = h5->hero_eng;
        hs = h5->hero_shadow;
        if (PolarRoundState != 0) {
            if (m->Engine->sfx != NULL) {
                StopSfx(m->Engine->sfx);
            }
            if (ext->eng->sfx != NULL) {
                StopSfx(ext->eng->sfx);
            }
            if (Heroes[ext->unk10].hero_status & 0x2000) {
                return;
            }
        }
        if (ext->unk38[ext->unk10] > 0) {
            ext->unk38[ext->unk10]--;
        }
        hs->unk5C = 1;
        if (ext->unk70 > 0) {
            ext->unk70--;
        }
        switch (ext->unk18[ext->unk10]) {
        case 0:
            if (ext->unk70 <= 0) {
                if (he->unk2C.x > 0xA00) {
                    he->unk2C.x -= 0x100;
                    he->unk2C.y -= 0x100;
                    he->unk2C.z -= 0x100;
                    hs->unk2C.x -= 0x100;
                    hs->unk2C.y -= 0x100;
                    hs->unk2C.z -= 0x100;
                } else {
                    ai = FindExtension(Heroes[ext->unk10].hero, 0x20000000);
                    if (ai != NULL) {
                        if (ai->unk24 == 0) {
                            SetHeroAction(ext->unk10, 0x49);
                        }
                        if (ai->unk24 == 1) {
                            SetHeroAction(ext->unk10, 0x54);
                        }
                    }
                    ext->unk18[ext->unk10] = -1;
                }
            }
            break;
        case 1:
            if (ext->unk70 <= 0) {
                if (he->unk2C.x < 0x1771) {
                    he->unk2C.x += 0x100;
                    he->unk2C.y += 0x100;
                    he->unk2C.z += 0x100;
                    hs->unk2C.x += 0x100;
                    hs->unk2C.y += 0x100;
                    hs->unk2C.z += 0x100;
                } else {
                    ext->unk18[ext->unk10] = -1;
                    ai = FindExtension(Heroes[ext->unk10].hero, 0x20000000);
                    if (ai != NULL) {
                        if (ai->unk24 == 0) {
                            SetHeroAction(ext->unk10, 0x49);
                        }
                        if (ai->unk24 == 1) {
                            SetHeroAction(ext->unk10, 0x54);
                        }
                    }
                }
            }
            break;
        case 2:
            if (ext->unk70 <= 0 && Heroes[ext->unk10].hero_mode != 0x4C) {
                SetHeroAction(ext->unk10, 0x4C);
                mv = Heroes[ext->unk10].hero_move;
                if (mv != NULL) {
                    mv->x_max = 0;
                    mv->z_max = 0;
                    mv->x_vel = 0;
                    mv->z_vel = 0;
                }
                ext->unk18[ext->unk10] = -1;
            }
            break;
        case 5:
            if (ext->unk70 <= 0) {
                ext->unk6C = ext->unk10;
                if (ext->eng != NULL) {
                    ext->eng->unk10 = Heroes[ext->unk6C].hero_eng->unk10;
                    ext->eng->sub->flags |= 0x8000;
                }
                ext->unk68 = 0x12C;
                ext->unk18[ext->unk10] = -1;
                ai = FindExtension(Heroes[ext->unk10].hero, 0x20000000);
                if (ai != NULL) {
                    if (ai->unk24 == 0) {
                        SetHeroAction(ext->unk10, 0x49);
                    }
                    if (ai->unk24 == 1) {
                        SetHeroAction(ext->unk10, 0x54);
                    }
                }
            }
            break;
        case 6:
            k2 = 0;
            if (ext->unk70 <= 0) {
                ext->unk6C = ext->unk10;
                do {
                    if (k2 == ext->unk6C && !(Heroes[k2].hero_status & 0x2000) && ext->engs[k2] != NULL) {
                        ext->engs[k2]->unk10 = Heroes[k2].hero_eng->unk10;
                        ext->engs[k2]->sub->flags |= 0x8000;
                    }
                    k2++;
                } while (k2 < 4);
                ext->unk68 = 0x12C;
                ext->unk18[ext->unk10] = -1;
                ai = FindExtension(Heroes[ext->unk10].hero, 0x20000000);
                if (ai != NULL) {
                    if (ai->unk24 == 0) {
                        SetHeroAction(ext->unk10, 0x49);
                    }
                    if (ai->unk24 == 1) {
                        SetHeroAction(ext->unk10, 0x54);
                    }
                }
            }
            break;
        }
        if (ext->unk38[ext->unk10] == 0) {
            switch (ext->unk28[ext->unk10]) {
            case 0:
                if (he->unk2C.x < 0x1000) {
                    he->unk2C.x += 0x100;
                    he->unk2C.y += 0x100;
                    he->unk2C.z += 0x100;
                    hs->unk2C.x += 0x100;
                    hs->unk2C.y += 0x100;
                    hs->unk2C.z += 0x100;
                } else {
                    ext->unk28[ext->unk10] = -1;
                    PolarHeroMass[ext->unk10] = HeroHooks.RunSpeeds[Heroes[ext->unk10].hero_name * 3];
                    ext->unk58 = 0;
                    ResetMass(ext->unk10);
                    ext->unk10 = -1;
                    he->unk2C.x = 0x1000;
                }
                break;
            case 1:
                if (he->unk2C.x > 0x1000) {
                    he->unk2C.x -= 0x100;
                    he->unk2C.y -= 0x100;
                    he->unk2C.z -= 0x100;
                    hs->unk2C.x -= 0x100;
                    hs->unk2C.y -= 0x100;
                    hs->unk2C.z -= 0x100;
                } else {
                    ext->unk28[ext->unk10] = -1;
                    ResetMass(ext->unk10);
                    PolarHeroMass[ext->unk10] = HeroHooks.RunSpeeds[Heroes[ext->unk10].hero_name * 3];
                    ext->unk58 = 0;
                    ext->unk10 = -1;
                    he->unk2C.x = 0x1000;
                }
                break;
            case 2:
                a1x = FindExtension(Heroes[ext->unk10].hero, 0x20000000);
                if (a1x != NULL) {
                    if (a1x->unk24 == 0) {
                        if (Heroes[ext->unk10].hero_mode != 0x49) {
                            SetHeroAction(ext->unk10, 0x49);
                            ext->unk64 = 0xD2;
                        } else {
                            ext->unk28[ext->unk10] = -1;
                            ext->unk58 = 0;
                        }
                    } else {
                        if (Heroes[ext->unk10].hero_mode != 0x54) {
                            SetHeroAction(ext->unk10, 0x54);
                            ext->unk64 = 0xD2;
                        } else {
                            ext->unk28[ext->unk10] = -1;
                            ext->unk58 = 0;
                        }
                    }
                    ext->unk10 = -1;
                }
                break;
            case 5:
                if (ext->unk6C != 5) {
                    if (ext->unk68 > 0) {
                        t3 = --ext->unk68;
                        if (ext->eng->sfx != NULL) {
                            sfx_set_pitch(ext->eng->sfx, (0x12C - t3) * 0xF00 / 300 + 0x1E00);
                        }
                        if (ext->eng != NULL) {
                            ext->eng->unk10 = Heroes[ext->unk6C].hero_eng->unk10;
                            if (PolarRoundState == 0) {
                                ext->eng->sub->flags |= 0x8000;
                            } else {
                                ext->eng->sub->flags &= ~0x8000;
                            }
                            ext->eng->unk1E += 0x80;
                            ext->eng->unk5E = 1;
                            switch (Heroes[ext->unk6C].hero_name) {
                            case 1:
                                ext->eng->unk10.y -= 0x258;
                                break;
                            case 2:
                                ext->eng->unk10.y -= 0x300;
                                break;
                            case 3:
                                ext->eng->unk10.y -= 0x300;
                                break;
                            case 4:
                                ext->eng->unk10.y -= 0x300;
                                break;
                            case 5:
                                ext->eng->unk10.y -= 0x300;
                                break;
                            case 6:
                                ext->eng->unk10.y -= 0x300;
                                break;
                            case 7:
                                ext->eng->unk10.y -= 0x364;
                                break;
                            case 8:
                                ext->eng->unk10.y -= 0x364;
                                break;
                            default:
                                ext->eng->unk10.y -= 0x300;
                                break;
                            }
                        }
                    }
                    if (ext->unk68 == 0) {
                        hb2 = Heroes;
                        h = &hb2[ext->unk6C];
                        if (h->hero_mode != 0x4D) {
                            h->hero_status |= 0x2000;
                            hq = Heroes[ext->unk6C].hero_eng;
                            if (hq != NULL) {
                                NAS *dn;

                                dn = CreateNAS(hq->unk10.x, hq->unk10.y - 0x898, hq->unk10.z, 0x1602, 0x11);
                                if (dn != NULL) {
                                    wx = FindExtension(dn, 0x10000000);
                                    if (wx != NULL) {
                                        wx->unkC = hq->unk10.y;
                                        wx->unk1C = ext->unk6C;
                                    }
                                }
                            }
                            SetHeroAction(ext->unk6C, 0x4D);
                            ext->eng->sub->flags &= ~0x8000;
                            ext->unk64 = 0xD2;
                            if (ext->eng->sfx != NULL) {
                                StopSfx(ext->eng->sfx);
                            }
                            ext->unk28[ext->unk10] = -1;
                            ext->unk58 = 0;
                            ext->unk10 = -1;
                            ext->unk6C = 5;
                        }
                    }
                    if (PolarClock != 0 || (hb3 = Heroes, h = &hb3[ext->unk6C], h->hero_mode == 0x50) || (h->hero_status & 0x2000)) {
                        if (ext->eng->sfx != NULL) {
                            StopSfx(ext->eng->sfx);
                        }
                        ext->eng->sub->flags &= ~0x8000;
                        ext->unk64 = 0xD2;
                        if (ext->unk10 != -1) {
                            ext->unk28[ext->unk10] = -1;
                        }
                        ext->unk58 = 0;
                        ext->unk10 = -1;
                        ext->unk6C = 5;
                    }
                }
                break;
            case 6:
                if (ext->unk6C != 5) {
                    if (ext->unk68 > 0) {
                        t4 = --ext->unk68;
                        if (ext->eng->sfx != NULL) {
                            sfx_set_pitch(ext->eng->sfx, (0x12C - t4) * 0xF00 / 300 + 0x1E00);
                        }
                        if (ext->eng != NULL) {
                            k = 0;
                            do {
                                if (k != ext->unk6C || (Heroes[k].hero_status & 0x2000)) {
                                    ext->engs[k]->sub->flags &= ~0x8000;
                                } else if (ext->engs[k] != NULL) {
                                    ext->engs[k]->unk10 = Heroes[k].hero_eng->unk10;
                                    if (PolarRoundState == 0) {
                                        ext->engs[k]->sub->flags |= 0x8000;
                                    } else {
                                        ext->engs[k]->sub->flags &= ~0x8000;
                                    }
                                    ext->engs[k]->unk1E += 0x80;
                                    ext->engs[k]->unk5E = 1;
                                    switch (Heroes[k].hero_name) {
                                    case 1:
                                        ext->engs[k]->unk10.y -= 0x258;
                                        break;
                                    case 2:
                                        ext->engs[k]->unk10.y -= 0x300;
                                        break;
                                    case 3:
                                        ext->engs[k]->unk10.y -= 0x300;
                                        break;
                                    case 4:
                                        ext->engs[k]->unk10.y -= 0x300;
                                        break;
                                    case 5:
                                        ext->engs[k]->unk10.y -= 0x300;
                                        break;
                                    case 6:
                                        ext->engs[k]->unk10.y -= 0x300;
                                        break;
                                    case 7:
                                        ext->engs[k]->unk10.y -= 0x300;
                                        break;
                                    case 8:
                                        ext->engs[k]->unk10.y -= 0x300;
                                        break;
                                    default:
                                        ext->engs[k]->unk10.y -= 0x300;
                                        break;
                                    }
                                }
                                k++;
                            } while (k < 4);
                        }
                    }
                    if (ext->unk68 == 0) {
                        for (j = 0; j < 4; j++) {
                            h = &Heroes[j];
                            if (j != ext->unk6C && !(h->hero_status & 0x2000) && h->hero_mode != 0x50) {
                                if (TBV->unk6C == 0 || TestForTeamMate(ext->unk6C, j) == 0) {
                                    if (h->hero_mode != 0x4B) {
                                        SetHeroAction(j, 0x4B);
                                        ext->engs[ext->unk6C]->sub->flags &= ~0x8000;
                                    }
                                }
                            }
                        }
                        ext->unk64 = 0x14A;
                        ext->unk28[ext->unk10] = -1;
                        if (m->Engine->sfx != NULL) {
                            StopSfx(m->Engine->sfx);
                        }
                        PlaySfx(0x466, NULL, NULL, 0x1000, 0x1E00, 0);
                        if (ext->eng->sfx != NULL) {
                            StopSfx(ext->eng->sfx);
                        }
                        ext->unk58 = 0;
                        ext->unk10 = -1;
                        ext->unk6C = 5;
                    }
                    hb4 = Heroes;
                    h = &hb4[ext->unk6C];
                    mode = h->hero_mode;
                    if (mode == 0x56 || mode == 0x50 || (h->hero_status & 0x2000) || PolarClock != 0) {
                        k3 = 0;
                        do {
                            if (k3 == ext->unk6C) {
                                ext->engs[k3]->sub->flags &= ~0x8000;
                            }
                            k3++;
                        } while (k3 < 4);
                        ext->unk64 = 0xD2;
                        ext->unk28[ext->unk10] = -1;
                        ext->unk58 = 0;
                        ext->unk10 = -1;
                        ext->unk6C = 5;
                        if (m->Engine->sfx != NULL) {
                            StopSfx(m->Engine->sfx);
                        }
                        if (ext->eng->sfx != NULL) {
                            StopSfx(ext->eng->sfx);
                        }
                    }
                }
                break;
            }
        }
    }
    if (PolarRoundState != 0) {
        s32 n2;

        for (n2 = 0; n2 < 4; n2++) {
            ext->engs[n2]->flags &= ~0x8000;
        }
        ext->eng->flags &= ~0x8000;
    }
}

void PlotWeightDeath(NAS *m) {
    WeightDeathExt *ext;
    HeroInfo *h;
    HeroInfo *base;
    HeroInfo *p;
    HeroInfo *q;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    if (ext->unk24-- >= 0) {
        return;
    }
    m->Engine->unk10.y += ext->unk10;
    ext->unk10 += ext->unk14;
    if (ext->unkC < m->Engine->unk10.y) {
        m->Engine->unk10.y -= abs(m->Engine->unk10.y - ext->unkC);
        if (++ext->unk18 < 5) {
            ext->unk10 = -ext->unk10 >> 3;
            if (ext->unk18 == 1) {
                PlaySfx(0x455, NULL, NULL, 0x1000, 0x1E00, 0);
            }
        }
    }
    if (ext->unk20 > 0x3C) {
        HeroInfo *b = Heroes;

        q = &b[ext->unk1C];
        if (!(q->hero_status & 0x200)) {
            q->hero_status |= 0x2000;
        }
        m->Engine->sub->flags &= ~0x8000;
    }
    if (ext->unk20 > 0x78) {
        base = Heroes;
        p = &base[ext->unk1C];
        if (!(p->hero_status & 0x200)) {
            p->hero_status |= 0x2000;
            base[ext->unk1C].hero_status |= 0x8000;
        }
    } else {
        ext->unk20++;
    }
    if (ext->unk20 > 0xF) {
        base = Heroes;
        h = &base[ext->unk1C];
        if (!(h->hero_status & 0x200)) {
            h->hero_eng->flags &= ~0x8000;
            base[ext->unk1C].hero_shadow->flags &= ~0x8000;
        }
    }
}

void GravityMotionUpdate(NAS *nas, GravityMotion *m) {
    EXTengine *eng;
    s32 y;
    s32 floor;

    eng = nas->Engine;
    if (eng != NULL) {
        eng->unk10.x += m->vx;
        eng->unk10.y += m->vy;
        eng->unk10.z += m->vz;
        m->vx += m->ax;
        m->vy += m->ay;
        m->vz += m->az;
        if (m->flags & 4) {
            nas->Engine->unk1C += m->rx;
        }
        if (m->flags & 8) {
            nas->Engine->unk1E += m->ry;
        }
        if (m->flags & 0x10) {
            nas->Engine->unk20 += m->rz;
        }
        nas->Engine->unk5E = 1;
        floor = m->floor;
        y = eng->unk10.y;
        if (y >= floor) {
            eng->unk10.y -= abs(y - floor);
            if (m->flags & 1) {
                m->vy = -m->vy >> 1;
            }
            if (m->flags & 2) {
                KillTriggerNas(nas);
            }
        }
    }
}

s32 PolarRayHit(VECTOR *a, VECTOR *b, SVECTOR *c, VECTOR *out) {
    VECTOR e;
    VECTOR d;
    VECTOR *p;
    s32 qa;
    s32 qb;
    s32 qc;
    s32 disc;
    s32 t0;
    s32 t1;

    d.vx = b->vx - a->vx;
    d.vy = 0;
    d.vz = b->vz - a->vz;
    e.vx = c->vx - a->vx;
    e.vy = 0;
    e.vz = c->vz - a->vz;
    qa = (d.vx * d.vx + d.vz * d.vz) >> 8;
    p = &e;
    qb = ((d.vx * p->vx + d.vz * p->vz) * 2) >> 8;
    qc = (p->vx * p->vx + p->vy * p->vy + p->vz * p->vz - c->pad * c->pad) >> 8;
    disc = qb * qb - qa * qc * 4;
    if (disc >= 0) {
        disc = csqrt(disc) >> 6;
        t0 = ((-qb + disc) << 8) / (qa * 2);
        t1 = ((-qb - disc) << 8) / (qa * 2);
        if (t0 < 0 && t1 < 0) {
            return 0;
        }
        if (t0 < t1) {
            if (t0 < 0) {
                out->vx = a->vx + ((d.vx * t1) >> 8);
                out->vy = a->vy + ((d.vy * t1) >> 8);
                out->vz = a->vz + ((d.vz * t1) >> 8);
                return 1;
            } else {
                out->vx = a->vx + ((d.vx * t0) >> 8);
                out->vy = a->vy + ((d.vy * t0) >> 8);
                out->vz = a->vz + ((d.vz * t0) >> 8);
                return 1;
            }
        } else if (t1 > 0) {
            out->vx = a->vx + ((d.vx * t1) >> 8);
            out->vy = a->vy + ((d.vy * t1) >> 8);
            out->vz = a->vz + ((d.vz * t1) >> 8);
            return 1;
        } else {
            out->vx = a->vx + ((d.vx * t0) >> 8);
            out->vy = a->vy + ((d.vy * t0) >> 8);
            out->vz = a->vz + ((d.vz * t0) >> 8);
            return 1;
        }
    }
    return 0;
}

void PlotScratchMaster(NAS *m) {
    subengine *s;
    EXTengine *e;
    s32 dx;
    s32 v;
    s32 w;
    s32 x;
    u16 ang;

    s = m->Engine->sub;
    while (s != NULL) {
        if (!(s->flags & 0x8000)) {
            break;
        }
        s = s->next;
    }
    e = Heroes[m->UserInfo.user1].hero_shadow;
    if (e != NULL) {
        if ((e->unk10.x - e->OldXYZ.x) * (e->unk10.x - e->OldXYZ.x)
            + (e->unk10.z - e->OldXYZ.z) * (e->unk10.z - e->OldXYZ.z) > 0 && s != NULL) {
            if (Heroes[m->UserInfo.user1].hero_mode == 0x49) {
                if (!(m->UserInfo.user2++ & 1) && !(s->flags & 0x8000)) {
                    *(SLONG_XYZ *)&s->unk04 = Heroes[m->UserInfo.user1].hero_eng->unk10;
                    if (Heroes[m->UserInfo.user1].hero_eng->unk2C.x > 0x1000) {
                        s->unk50 = 1;
                        dx = Heroes[m->UserInfo.user1].hero_eng->unk2C.x;
                        s->unk20.z = dx;
                        s->unk20.y = dx;
                        s->unk20.x = dx;
                    }
                    if (Heroes[m->UserInfo.user1].hero_eng->unk2C.x < 0x1000) {
                        s->unk50 = 1;
                        v = Heroes[m->UserInfo.user1].hero_eng->unk2C.x;
                        s->unk20.z = v;
                        s->unk20.y = v;
                        s->unk20.x = v;
                    }
                    if (GameMode == 0x15) {
                        s->flags = 0x40008021;
                        s->entity = 0x2030;
                    } else {
                        s->flags = 0x40008061;
                    }
                    ang = Heroes[m->UserInfo.user1].hero_eng->unk1E + 0x800;
                    s->unk52 = 1;
                    s->unk76 = 0x400;
                    s->unk12 = ang;
                }
            }
            if (Heroes[m->UserInfo.user1].hero_mode == 0x4A
                && Heroes[m->UserInfo.user1].hero_anim->Aindex == 9
                && !(s->flags & 0x8000)) {
                if (Heroes[m->UserInfo.user1].hero_eng->unk2C.x > 0x1000) {
                    s->unk50 = 1;
                    w = Heroes[m->UserInfo.user1].hero_eng->unk2C.x;
                    s->unk20.z = w;
                    s->unk20.y = w;
                    s->unk20.x = w;
                }
                if (Heroes[m->UserInfo.user1].hero_eng->unk2C.x < 0x1000) {
                    s->unk50 = 1;
                    x = Heroes[m->UserInfo.user1].hero_eng->unk2C.x;
                    s->unk20.z = x;
                    s->unk20.y = x;
                    s->unk20.x = x;
                }
                *(SLONG_XYZ *)&s->unk04 = Heroes[m->UserInfo.user1].hero_eng->unk10;
                if (GameMode == 0x15) {
                    s->flags = 0x40008021;
                    s->entity = 0x2030;
                } else {
                    s->flags = 0x40008061;
                }
                ang = Heroes[m->UserInfo.user1].hero_eng->unk1E + 0x800;
                s->unk76 = 0;
                s->unk52 = 1;
                s->unk12 = ang;
            }
        }
    }
    for (s = m->Engine->sub; s != NULL; s = s->next) {
        if (s->flags & 0x8000) {
            s->unk76 += 0x60;
            if ((s16)s->unk76 > 0xC00) {
                s->flags = 0;
                s->unk20.z = 0x1000;
                s->unk20.y = 0x1000;
                s->unk20.x = 0x1000;
            }
        }
    }
}

void InitIceBerg(NAS *m) {
    MapTriggerInfo *info;

    if (m->trigger != NULL) {
        info = m->trigger->InfoStruct;
        m->UserInfo.user1 = info->val1;
        m->UserInfo.user2 = info->val2;
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
        if (GameMode == 0x12) {
            if (m->UserInfo.user1 == 0) {
                m->Engine->sub->entity = 0x200D;
            }
            if (m->UserInfo.user1 == 1) {
                m->Engine->sub->entity = 0x200E;
            }
            if (m->UserInfo.user1 == 2) {
                m->Engine->sub->entity = 0x200F;
            }
        } else {
            if (m->UserInfo.user1 == 0) {
                m->Engine->sub->entity = 0x2010;
            }
            if (m->UserInfo.user1 == 1) {
                m->Engine->sub->entity = 0x2011;
            }
            if (m->UserInfo.user1 == 2) {
                m->Engine->sub->entity = 0x2012;
            }
        }
        m->Engine->sub->flags |= 0x02000000;
        m->Engine->sub->unk51 = 0;
        if (m->UserInfo.user1 == 1) {
            m->Engine->sub->unk12 = 0x800;
            m->Engine->sub->unk68 = 0x200;
        }
        m->Engine->sub->unk68 = 0x200;
    }
}

s32 PolarIceHit(SLONG_XYZ *pos, s32 player) {
    volatile s32 t0;
    volatile s32 t1;
    SceneObj *o;
    TrigEntry *t;
    s32 i;
    s32 dx;
    s32 dz;
    s32 d;
    s32 ang;
    s32 r;
    EXTengine *eng;
    EXTheromove *mv;
    NAS *nas;
    EXTmotion *m;
    EXTmotion *m2;
    s32 k;
    s32 sp;

    i = 0;
    o = PolarIceObjects[0];
    if (GameMode == 0x13) {
        t0 = VSync(1);
        t = rcossin_tbl;
        k = 0x100;
        while (1) {
            if (o != NULL) {
            if (o->flags & 0x8000) {
                dx = o->unk4 - pos->x;
                dz = o->unkC - pos->z;
                if (abs(dx) < 0x14B && abs(dz) < 0x14B) {
                    d = dx * dx + dz * dz;
                    if (d <= 0x1A963) {
                        ang = get_angle(dx, dz) & 0xFFF;
                        eng = Heroes[player].hero_eng;
                        d = csqrt(d) >> 6;
                        d = 0x14A - d;
                        d += 2;
                        eng->unk10.x -= d * t[(ang - 0x400) & 0xFFF].unk2 >> 12;
                        eng->unk10.z -= d * t[ang].unk2 >> 12;
                        ang = get_angle(eng->unk10.x, eng->unk10.z) & 0xFFF;
                        mv = Heroes[player].hero_move;
                        if (mv != NULL) {
                            sp = csqrt(mv->x_vel * mv->x_vel + mv->z_vel * mv->z_vel) >> 6;
                            mv->x_vel = -(sp * t[(-ang + 0x400) & 0xFFF].unk2 >> 13);
                            mv->z_vel = -(sp * t[-ang & 0xFFF].unk2 >> 13);
                            sp *= Heroes[player].hero_move->mass;
                            if ((HeroHooks.RunSpeeds[Heroes[player].hero_name * 3] * 0xC000 >> 8) < sp) {
                                nas = CreateNAS(o->unk4, o->unk8 - 0x80, o->unkC, 0x1604, 0x11);
                                if (nas != NULL) {
                                    m = (EXTmotion *)nas->FUX;
                                    if (m != NULL) {
                                        r = Random(3);
                                        m->flags = 0x1E;
                                        m->Xvel = t[(ang - 0x200) & 0xFFF].unk2 * 0x11 >> 11;
                                        m->Zvel = t[(ang + 0x200) & 0xFFF].unk2 * 0x11 >> 11;
                                        m->Yvel = -0x20;
                                        m->Xa = -0x100;
                                        m->Ya = -0x80;
                                        m->Za = k;
                                        m->Yacc = 3;
                                        if (r == 0) {
                                            PlaySfx(0x46A, NULL, NULL, 0x1000, 0x1E00, 0);
                                        }
                                        if (r == 1) {
                                            PlaySfx(0x46B, NULL, NULL, 0x1000, 0x1E00, 0);
                                        }
                                        if (r == 2) {
                                            PlaySfx(0x46C, NULL, NULL, 0x1000, 0x1E00, 0);
                                        }
                                    }
                                    if ((u32)(ang - 0x400) > 0x800) {
                                        nas->Engine->sub->unk68 = 0x200;
                                    }
                                }
                                nas = CreateNAS(o->unk4, o->unk8 - 0x80, o->unkC, 0x1604, 0x11);
                                if (nas != NULL) {
                                    m2 = (EXTmotion *)nas->FUX;
                                    if (m2 != NULL) {
                                        m2->flags = 0x1E;
                                        m2->Xvel = t[ang].unk2 * 0xB >> 10;
                                        m2->Zvel = t[(ang + 0x400) & 0xFFF].unk2 * 0xB >> 10;
                                        m2->Yvel = -0x2A;
                                        m2->Ya = 0x80;
                                        m2->Za = -0x100;
                                        m2->Xa = k;
                                        m2->Yacc = 3;
                                    }
                                    if ((u32)(ang - 0x400) > 0x800) {
                                        nas->Engine->sub->unk68 = 0x200;
                                    }
                                }
                                nas = CreateNAS(o->unk4, o->unk8 - 0x80, o->unkC, 0x1604, 0x11);
                                if (nas != NULL) {
                                    m2 = (EXTmotion *)nas->FUX;
                                    if (m2 != NULL) {
                                        m2->flags = 0x1E;
                                        m2->Xvel = t[(ang - 0x400) & 0xFFF].unk2 * 0xB >> 10;
                                        m2->Zvel = t[ang].unk2 * 0xB >> 10;
                                        m2->Yvel = -0x48;
                                        m2->Ya = -0x80;
                                        m2->Xa = k;
                                        m2->Za = k;
                                        m2->Yacc = 3;
                                    }
                                    if ((u32)(ang - 0x400) > 0x800) {
                                        nas->Engine->sub->unk68 = 0x200;
                                    }
                                }
                                o->flags &= ~0x8000;
                            }
                        }
                        t1 = VSync(1);
                        return 1;
                    }
                }
            }
            o = PolarIceObjects[i];
            i++;
            continue;
            }
            break;
        }
        t1 = VSync(1);
    }
    return 0;
}

/* a debug printf format nothing prints with in retail, in place in .rodata as above */
const char PolarMsgFoundColSubs[] = "found col subs %d\n";

void UkaMaskInitRisers(void) {
    s32 i;
    SceneObj *obj;
    SceneObj *scan;
    s32 n;
    VECTOR v;
    VECTOR *vp;

    scan = ((SceneObj *)MapScene->subs);
    for (i = 15; i >= 0; i--) {
        UkaMaskObjects[i] = NULL;
    }
    n = 0;
    while (scan != NULL) {
        if ((scan->flags & 0x8000) && scan->index == 0x4000) {
            UkaMaskObjects[n++] = scan;
        }
        scan = scan->next;
    }
    i = 0;
    while (1) {
        static const VECTOR up = { 0, 0, 0x1000, 0 };

        if (i >= 16) {
            break;
        }
        v = up;
        vp = &v;
        obj = UkaMaskObjects[i];
        ApplyMatrixLV((MATRIX *)((u8 *)obj + 0x30), vp, vp);
        i++;
        UkaMaskRisers[(((get_angle(v.vx, vp->vz) + 1) & 0xFFF) >> 8) - 1 & 0xF] = (PolarRiserObj *)obj;
    }
}

#define UKA_SEG_SET(j, st, a) \
    { m->seg[j].state = (st); m->seg[j].angle = (a); m->seg[j].t = 0; m->seg[j].count++; }

void UkaMaskSetSegments(s32 i, UkaMaskSet *m) {
    s32 next;
    s32 prev;

    next = (i + 1) & 0xF;
    prev = (i - 1) & 0xF;
    if (m->seg[i].count == 0) {
        UKA_SEG_SET(i, 1, 0x4000);
        switch (m->seg[next].state) {
        case 0:
            UKA_SEG_SET(next, 2, 0x4300);
            break;
        case 3:
            UKA_SEG_SET(next, 4, 0x4200);
            break;
        }
        switch (m->seg[prev].state) {
        case 0:
            UKA_SEG_SET(prev, 3, 0x4380);
            break;
        case 2:
            UKA_SEG_SET(prev, 4, 0x4100);
            break;
        }
    } else if (m->seg[i].count == 1) {
        switch (m->seg[i].state) {
        case 3:
            UKA_SEG_SET(i, 1, 0x4280);
            break;
        case 2:
            UKA_SEG_SET(i, 1, 0x4180);
            break;
        }
        switch (m->seg[prev].state) {
        case 0:
            UKA_SEG_SET(prev, 3, 0x4380);
            break;
        case 2:
            UKA_SEG_SET(prev, 4, 0x4100);
            break;
        }
        switch (m->seg[next].state) {
        case 0:
            UKA_SEG_SET(next, 2, 0x4300);
            break;
        case 3:
            UKA_SEG_SET(next, 4, 0x4200);
            break;
        }
    } else if (m->seg[i].count == 2) {
        if (m->seg[i].state == 4) {
            UKA_SEG_SET(i, 1, 0x4080);
        }
    }
}

void InitUkaPolarMask(NAS *m) {
    UkaPolarExt *ext;
    UkaBitExt *bx;
    EXTengine *e;
    subengine *sub;
    subengine *s2;
    NAS *n;
    s32 r;
    s32 i;

    ext = AddExt(m, 0x1E0, 0x40000000);
    if (ext == NULL) {
        return;
    }
    UkaMaskInitRisers();
    ext->unk14 = 4;
    ext->unkB4 = 0x12C;
    ext->unk1B8 = 0xC;
    ext->unk1CC = 0xC;
    m->FUX = (EXT *)ext;
    m->Engine->sub->u6C.dbase = MapScene->dbase;
    *(u16 *)((u8 *)m->Engine->sub + 0x74) = 0x200C;
    PolarBeamOwner = (PolarOwner *)m;
    if (GameLevelFlags & 0x80) {
        m->Engine->sub->entity = 0x200C;
    }
    m->UserInfo.user1 = 0;
    m->Engine->unk10.x = 0;
    m->Engine->unk10.z = 0;
    ext->unk10 = -1;
    r = Random(4);
    if (r == 0) {
        ext->angle = -0x100;
    }
    if (r == 1) {
        ext->angle = 0x300;
    }
    if (r == 2) {
        ext->angle = 0x700;
    }
    if (r == 3) {
        ext->angle = 0xB00;
    }
    ext->unk1D0 = (rcossin_tbl[(ext->angle - 0x400) & 0xFFF].unk2 * 0x115) >> 9;
    ext->unk1D8 = (rcossin_tbl[ext->angle & 0xFFF].unk2 * 0x115) >> 9;
    ext->unk1C4 = 1;
    ext->unk1C8 = 1;
    ext->unk68 = -1;
    ext->pos = *(SLONG_XYZ *)&m->Engine->unk10;
    ext->unk64 = 0x5A;
    ext->unk6C = 5;
    for (i = 0; i < 16; i++) {
        ext->seg[i].state = 0;
        ext->seg[i].count = 0;
        ext->seg[i].t = 0;
        ext->seg[i].angle = 0;
    }
    ext->mask = n = CreateNAS(0, 0, 0, 0x160B, 0x11);
    if (n != NULL) {
        AddCamera(n);
    }
    ext->bit = n = CreateNAS(m->Engine->unk10.x, m->Engine->unk10.y, m->Engine->unk10.z, 0x1605, 0x11);
    if (n != NULL) {
        n->Engine->flags |= 0x200000;
        bx = FindExtension(ext->bit, 0x10000000);
        if (bx != NULL) {
            bx->unkC = 3;
            bx->unk18 = 0x100;
            bx->unk20 = 4;
            ext->bit->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
            ext->bit->InitDelete(ext->bit);
            ext->unkAC = 0;
        }
    }
    e = AddExtEngine(m, 2);
    if (e != NULL) {
        ext->eng = e;
        e->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
        e->sub->entity = 0x2003;
        e->sub->unk51 = 0;
        s2 = e->sub;
        s2->unk14 = 0;
        s2->unk12 = 0;
        s2->unk10 = 0;
        e->sub->unk50 = 1;
        sub = e->sub;
        sub->unk20.z = 0x200;
        sub->unk20.y = 0x200;
        sub->unk20.x = 0x200;
        e->sub->flags &= ~0x8000;
    }
}

void PlotUkaPolarMask(NAS *m) {
    UkaPolarExt *ext;
    UkaBitExt *bx;
    EXTheromove *mv;
    NAS *n;
    s32 seg;
    s32 r;
    s32 tx;
    s32 ty;
    s32 tz;
    s32 i;
    s32 j;
    s32 p;
    s32 v;
    s32 d;
    s32 cnt;
    s32 cs;
    s32 sn;
    s32 ga;
    s32 ang;

    ext = FindExtension(m, 0x40000000);
    if (ext == NULL) {
        return;
    }
    r = Random(4);
    if (PolarClock != 0) {
        RemoveCamera(ext->mask);
    }
    if (PolarClock != 0) {
        if (ext->unkAC != NULL) {
            KillNAS(ext->unkAC);
            ext->unkAC = NULL;
        }
    }
    if (PolarClock != 0) {
        if (m->Engine->sfx != NULL) {
            StopSfx(m->Engine->sfx);
        }
        if (ext->eng->sfx != NULL) {
            StopSfx(ext->eng->sfx);
        }
        ext->bit->Engine->flags &= ~0x8000;
    }
    seg = ((get_angle(m->Engine->unk10.x, m->Engine->unk10.z) + 1) & 0xFFF) >> 8;
    cs = rcossin_tbl[(ext->unk9C - 0x400) & 0xFFF].unk2;
    sn = rcossin_tbl[ext->unk9C & 0xFFF].unk2;
    ty = (cs * 3) >> 5;
    tx = tz = sn * 128 >> 12;
    if (ext->unk14 != 2) {
        m->Engine->unk10.y = ext->pos.y + (cs * 64 >> 12);
    }
    m->Engine->unk5E = 1;
    if (ext->unk1CC < 0) {
        ext->unk9C += 0x40;
    } else {
        ext->unk9C -= 0x40;
    }
    for (j = 0; j < 16; j++) {
        if (ext->seg[j].count != 0 && ext->seg[j].t < 0x74) {
            ext->seg[j].t += 4;
            UkaMaskRisers[j]->y = ext->seg[j].angle + ext->seg[j].t;
        }
    }
    for (i = 0; i < 4; i++) {
        if (ext->unk48[i] > 0) {
            ext->unk48[i]--;
        }
    }
    if (ext->unk64 < 0) {
        for (i = r; ; i++) {
            HeroInfo *h;
            s32 s;
            s32 kind;

            if (i >= r + 4) {
                break;
            }
            if (ext->unk14 == 1 || PolarClock != 0 || ext->unk10 != -1) {
                continue;
            }
            s = i & 3;
            h = &Heroes[s];
            if (h->hero_status & 0x2000) {
                continue;
            }
            if (DistXZSq((SLONG_XYZ *)&m->Engine->unk10, (SLONG_XYZ *)&h->hero_eng->unk10) > 0x18FFF) {
                continue;
            }
            {
                EXTheromove *hm = h->hero_move;

                hm->x_vel = 0;
                hm->z_vel = 0;
                hm->x_max = 0;
                hm->z_max = 0;
            }
            ext->unk5C = Random(4);
            kind = UkaMaskFrames[ext->unk5C];
            ext->unk10 = s;
            ext->unk14 = 3;
            ext->unkC = kind;
            PlaySfx(0x44C, &m->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
            ext->unkAC = CreateNAS(m->Engine->unk10.x, 0x5E, m->Engine->unk10.z, 0x1605, 0x11);
            if (ext->unkAC != NULL) {
                bx = FindExtension(ext->unkAC, 0x10000000);
                if (bx != NULL) {
                    bx->unkC = 0;
                    bx->unk18 = 0x100;
                    bx->unk20 = 1;
                    bx->unk28 = 0;
                    ext->unkAC->InitDelete(ext->unkAC);
                }
            }
            switch (kind) {
            case 5:
                ext->unk64 = 0x14A;
                ext->unk38[i & 3] = 0;
                ext->unk18[i & 3] = kind;
                ext->unk70 = 0xF;
                ext->unk28[i & 3] = kind;
                PlaySfx(0x451, NULL, NULL, 0x1000, 0x1E00, 0);
                SetHeroAction(i & 3, 0x4F);
                ext->unkB4 = 0x3C;
                PlaySfx(0x452, &ext->eng->sfx, &ext->eng->unk10, 0x1000, 0x1E00, 0);
                break;
            case 2:
                ext->unk64 = Random(0x3C) + 0x5A;
                ext->unk38[i & 3] = 0;
                ext->unk18[i & 3] = kind;
                ext->unk28[i & 3] = kind;
                ext->unk70 = 0xF;
                PlaySfx(0x450, NULL, NULL, 0x1000, 0x1E00, 0);
                SetHeroAction(i & 3, 0x4F);
                ext->unkB4 = 0x3C;
                break;
            case 0:
                ext->unk64 = Random(0x3C) + 0x96;
                ext->unk38[i & 3] = 0;
                ext->unk18[i & 3] = 0;
                ext->unk28[i & 3] = 0;
                Heroes[i & 3].hero_move->mass = 0x50;
                PolarHeroMass[i & 3] = 0x40;
                ext->unk70 = 0xF;
                PlaySfx(0x44E, NULL, NULL, 0x1000, 0x1E00, 0);
                SetHeroAction(i & 3, 0x4F);
                ext->unkB4 = 0x3C;
                break;
            case 8:
                ext->unk64 = Random(0x3C) + 0x12C;
                ext->unk38[i & 3] = 0;
                ext->unk18[i & 3] = kind;
                ext->unk28[i & 3] = kind;
                Heroes[i & 3].hero_move->mass = 0x190;
                ext->unk70 = 0xF;
                PolarHeroMass[i & 3] = 0x10;
                PlaySfx(0x48A, NULL, NULL, 0x1000, 0x1E00, 0);
                SetHeroAction(i & 3, 0x4F);
                ext->unkB4 = 0x3C;
                break;
            }
        }
    }
    switch (ext->unk14) {
    case 2:
        m->Engine->unk10.y -= 0x10;
        ext->bit->Engine->flags &= ~0x8000;
        break;
    case 4:
        if (ext->unk64-- < 0) {
            ext->unk14 = 0;
        }
        break;
    case 0:
        if (PolarClock != 0) {
            ext->unk14 = 2;
            if (m->Engine->sfx != NULL) {
                StopSfx(m->Engine->sfx);
            }
            return;
        }
        if (m->Engine->unk10.x < ext->unk1D0) {
            m->Engine->unk10.x += 0x20;
        }
        if (ext->unk1D0 < m->Engine->unk10.x) {
            m->Engine->unk10.x -= 0x20;
        }
        if (m->Engine->unk10.z < ext->unk1D8) {
            m->Engine->unk10.z += 0x20;
        }
        if (ext->unk1D8 < m->Engine->unk10.z) {
            m->Engine->unk10.z -= 0x20;
        }
        ext->mask->Engine->unk10 = m->Engine->unk10;
        ext->mask->Engine->unk10.y = 0x100;
        if (ext->unk1B8 < ext->unk1CC) {
            ext->unk1B8 += ext->unk1C4;
            if (ext->unk1B8 > ext->unk1CC) {
                ext->unk1B8 = ext->unk1CC;
            }
        }
        if (ext->unk1B8 > ext->unk1CC) {
            ext->unk1B8 -= ext->unk1C4;
            if (ext->unk1B8 < ext->unk1CC) {
                ext->unk1B8 = ext->unk1CC;
            }
        }
        m->UserInfo.user1 += ext->unk1B8;
        ga = get_angle(m->Engine->unk10.x, m->Engine->unk10.z) & 0xFFF;
        d = ((ga - ext->angle) << 20) >> 20;
        abs(d);
        if (ext->unk1B8 == ext->unk1CC) {
            s32 brake = abs(d) <= abs(ext->unk1B8) * 8;

            if (brake) {
                ext->unk1CC = 0;
            }
        }
        {
            s32 dx = m->Engine->unk10.x - ext->unk1D0;
            s32 dz = m->Engine->unk10.z - ext->unk1D8;

            p = dx * dx + dz * dz;
        }
        if (p < 0x1000) {
            v = Random(0x1000);
            ext->unk14 = 1;
            ext->unkB4 = 0x3C;
            m->UserInfo.user1 -= ext->unk1B8;
            PlaySfx(0x489, NULL, NULL, 0x1000, 0x1E00, 0);
            PlaySfx(0x44C, &m->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
            UkaMaskSetSegments(seg, (UkaMaskSet *)ext);
            n = CreateNAS(m->Engine->unk10.x, 0x5E, m->Engine->unk10.z, 0x1605, 0x11);
            if (n != NULL) {
                bx = FindExtension(n, 0x10000000);
                if (bx != NULL) {
                    bx->unkC = 0;
                    bx->unk18 = 0x100;
                    bx->unk20 = 1;
                    bx->unk28 = 0;
                    n->InitDelete(n);
                    ext->unkAC = n;
                }
            }
            for (cnt = 0; cnt < 16; cnt++) {
                if (ext->seg[v++ & 0xF].state != 1) {
                    break;
                }
                if (cnt == 0xF) {
                    ext->unk14 = 2;
                    return;
                }
            }
            v &= 0xF;
            ga = get_angle(m->Engine->unk10.x, m->Engine->unk10.z) & 0xFFF;
            ext->angle = ((v << 8) - 0x40) & 0xFFF;
            ext->unk1D0 = (rcossin_tbl[(ext->angle - 0x400) & 0xFFF].unk2 * 0x115) >> 9;
            ext->unk1D8 = (rcossin_tbl[ext->angle & 0xFFF].unk2 * 0x115) >> 9;
            UkaMaskTimer++;
            ext->unk1B8 = 0;
            if (((ga - ext->angle) << 20) < 0) {
                ext->unk1CC = 0xC;
            } else {
                ext->unk1CC = -0xC;
            }
        }
        break;
    case 1:
        tx = 0;
        ty = 0;
        tz = 0;
        if (--ext->unkB4 == -1) {
            ext->unk14 = 0;
            ext->unkB4 = 0xF0;
            if (m->Engine->sfx != NULL) {
                StopSfx(m->Engine->sfx);
            }
        }
        break;
    case 3:
        switch (ext->unk18[ext->unk10]) {
        case 5:
            if (ext->unk70-- <= 0) {
                ext->unk6C = ext->unk10;
                if (ext->eng != NULL) {
                    ext->eng->unk10 = Heroes[ext->unk6C].hero_eng->unk10;
                    ext->eng->sub->flags |= 0x8000;
                }
                ext->unk68 = 0x12C;
                ext->unk18[ext->unk10] = -1;
                SetHeroAction(ext->unk10, 0x49);
                if (m->Engine->sfx != NULL) {
                    StopSfx(m->Engine->sfx);
                }
            }
            break;
        case 2:
            if (ext->unk70-- <= 0) {
                if (Heroes[ext->unk10].hero_mode != 0x4C) {
                    SetHeroAction(ext->unk10, 0x4C);
                    mv = Heroes[ext->unk10].hero_move;
                    if (mv != NULL) {
                        mv->x_max = 0;
                        mv->z_max = 0;
                        mv->x_vel = 0;
                        mv->z_vel = 0;
                    }
                    ext->unk18[ext->unk10] = -1;
                    if (m->Engine->sfx != NULL) {
                        StopSfx(m->Engine->sfx);
                    }
                }
            }
            break;
        case 0: {
            EXTengine *e = Heroes[ext->unk10].hero_eng;
            EXTengine *sh = Heroes[ext->unk10].hero_shadow;

            if (ext->unk70-- <= 0) {
                if (e->unk2C.x > 0xA00) {
                    e->unk2C.x -= 0x100;
                    e->unk2C.y -= 0x100;
                    e->unk2C.z -= 0x100;
                    sh->unk2C.x -= 0x100;
                    sh->unk2C.y -= 0x100;
                    sh->unk2C.z -= 0x100;
                } else {
                    ext->unk18[ext->unk10] = -1;
                    SetHeroAction(ext->unk10, 0x49);
                }
                if (m->Engine->sfx != NULL) {
                    StopSfx(m->Engine->sfx);
                }
            }
            break;
        }
        case 8:
            if (ext->unk70-- <= 0) {
                if (Heroes[ext->unk10].hero_mode != 0x52) {
                    SetHeroAction(ext->unk10, 0x52);
                    Heroes[ext->unk10].hero_eng->unk10.y -= 0x14C;
                    {
                        HeroInfo *h8 = &Heroes[ext->unk10];
                        EXTheromove *m8 = h8->hero_move;

                        if (m8 != NULL) {
                            ang = get_angle(h8->hero_eng->unk10.x, h8->hero_eng->unk10.z) & 0xFFF;
                            Heroes[ext->unk10].hero_eng->unk1E = ang;
                            Heroes[ext->unk10].hero_move->forward_angle = ang;
                            m8->x_max = -(rcossin_tbl[(ang - 0x400) & 0xFFF].unk2 * 64 >> 12);
                            m8->z_max = -(rcossin_tbl[ang].unk2 * 64 >> 12);
                            m8->x_vel = m8->x_max;
                            m8->z_vel = m8->z_max;
                        }
                    }
                    ext->unk18[ext->unk10] = -1;
                    if (m->Engine->sfx != NULL) {
                        StopSfx(m->Engine->sfx);
                    }
                }
            }
            break;
        }
        if (ext->unk18[ext->unk10] != -1) {
            break;
        }
        switch (ext->unk28[ext->unk10]) {
        case 0: {
            EXTengine *e = Heroes[ext->unk10].hero_eng;
            EXTengine *sh = Heroes[ext->unk10].hero_shadow;

            if (ext->unkB4-- < 0) {
                if (m->Engine->unk10.x < 0) {
                    m->Engine->unk10.x += 0x18;
                }
                if (m->Engine->unk10.x > 0) {
                    m->Engine->unk10.x -= 0x18;
                }
                if (m->Engine->unk10.z < 0) {
                    m->Engine->unk10.z += 0x18;
                }
                if (m->Engine->unk10.z > 0) {
                    m->Engine->unk10.z -= 0x18;
                }
            }
            if (ext->unk64-- < 0) {
                if (e->unk2C.x < 0x1000) {
                    e->unk2C.x += 0x100;
                    e->unk2C.y += 0x100;
                    e->unk2C.z += 0x100;
                    sh->unk2C.x += 0x100;
                    sh->unk2C.y += 0x100;
                    sh->unk2C.z += 0x100;
                } else {
                    ext->unk28[ext->unk10] = -1;
                    PolarHeroMass[ext->unk10] = HeroHooks.RunSpeeds[Heroes[ext->unk10].hero_name * 3];
                    ResetMass(ext->unk10);
                    ext->unk10 = -1;
                    ext->unk14 = 0;
                    e->unk2C.x = 0x1000;
                    e->unk2C.y = 0x1000;
                    e->unk2C.z = 0x1000;
                }
            }
            break;
        }
        case 2:
            if (ext->unkB4-- < 0) {
                if (m->Engine->unk10.x < 0) {
                    m->Engine->unk10.x += 0x18;
                }
                if (m->Engine->unk10.x > 0) {
                    m->Engine->unk10.x -= 0x18;
                }
                if (m->Engine->unk10.z < 0) {
                    m->Engine->unk10.z += 0x18;
                }
                if (m->Engine->unk10.z > 0) {
                    m->Engine->unk10.z -= 0x18;
                }
            }
            if (ext->unk64-- < 0) {
                ext->unk14 = 0;
                ext->unk28[ext->unk10] = -1;
                if (Heroes[ext->unk10].hero_mode != 0x49) {
                    SetHeroAction(ext->unk10, 0x49);
                    ext->unk10 = -1;
                }
            }
            break;
        case 8:
            if (ext->unkB4-- < 0) {
                if (m->Engine->unk10.x < 0) {
                    m->Engine->unk10.x += 0x18;
                }
                if (m->Engine->unk10.x > 0) {
                    m->Engine->unk10.x -= 0x18;
                }
                if (m->Engine->unk10.z < 0) {
                    m->Engine->unk10.z += 0x18;
                }
                if (m->Engine->unk10.z > 0) {
                    m->Engine->unk10.z -= 0x18;
                }
            }
            if (ext->unk64-- < 0) {
                ResetMass(ext->unk10);
                ext->unk14 = 0;
                PolarHeroMass[ext->unk10] = HeroHooks.RunSpeeds[Heroes[ext->unk10].hero_name * 3];
                ext->unk28[ext->unk10] = -1;
                if (Heroes[ext->unk10].hero_mode != 0x49) {
                    SetHeroAction(ext->unk10, 0x49);
                    Heroes[ext->unk10].hero_eng->unk10.y += 0x14C;
                    ext->unk10 = -1;
                }
            }
            break;
        case 5:
            if (ext->unkB4-- < 0) {
                if (m->Engine->unk10.x < 0) {
                    m->Engine->unk10.x += 0x18;
                }
                if (m->Engine->unk10.x > 0) {
                    m->Engine->unk10.x -= 0x18;
                }
                if (m->Engine->unk10.z < 0) {
                    m->Engine->unk10.z += 0x18;
                }
                if (m->Engine->unk10.z > 0) {
                    m->Engine->unk10.z -= 0x18;
                }
            }
            ext->unk64--;
            if (ext->unk6C == 5) {
                break;
            }
            if (ext->unkAC != NULL) {
                KillNAS(ext->unkAC);
                ext->unkAC = NULL;
            }
            if (ext->eng->sfx != NULL) {
                sfx_set_pitch(ext->eng->sfx, (0x12C - ext->unk68) * 0xF00 / 300 + 0x1E00);
            }
            if (ext->unk68 > 0) {
                ext->unk68--;
                if (ext->eng != NULL) {
                    ext->eng->unk10 = Heroes[ext->unk6C].hero_eng->unk10;
                    ext->eng->sub->flags |= 0x8000;
                    ext->eng->unk1E += 0x80;
                    ext->eng->unk5E = 1;
                    switch (Heroes[ext->unk6C].hero_name) {
                    case 1:
                        ext->eng->unk10.y -= 0x258;
                        break;
                    case 7:
                        ext->eng->unk10.y -= 0x364;
                        break;
                    case 8:
                        ext->eng->unk10.y -= 0x364;
                        break;
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    default:
                        ext->eng->unk10.y -= 0x300;
                        break;
                    }
                }
            }
            if (ext->unk68 == 0) {
                if (Heroes[ext->unk6C].hero_mode != 0x4D) {
                    EXTengine *he;

                    Heroes[ext->unk6C].hero_status |= 0x2000;
                    he = Heroes[ext->unk6C].hero_eng;
                    if (he != NULL) {
                        NAS *sp = CreateNAS(he->unk10.x, he->unk10.y - 0x898, he->unk10.z, 0x1602, 0x11);

                        if (sp != NULL) {
                            UkaBitExt *sx = FindExtension(sp, 0x10000000);

                            if (sx != NULL) {
                                sx->unkC = he->unk10.y;
                                sx->unk1C = ext->unk6C;
                            }
                        }
                    }
                    if (ext->eng->sfx != NULL) {
                        StopSfx(ext->eng->sfx);
                    }
                    SetHeroAction(ext->unk6C, 0x4D);
                    ext->eng->sub->flags &= ~0x8000;
                    ext->unk64 = 0xD2;
                    ext->unk28[ext->unk10] = -1;
                    ext->unk58 = 0;
                    ext->unk10 = -1;
                    ext->unk6C = 5;
                    ext->unk14 = 0;
                    return;
                }
            }
            if (PolarClock != 0 || Heroes[ext->unk6C].hero_mode == 0x50
                || (Heroes[ext->unk6C].hero_status & 0x2000)) {
                ext->eng->sub->flags &= ~0x8000;
                ext->unk64 = 0xD2;
                if (ext->eng->sfx != NULL) {
                    StopSfx(ext->eng->sfx);
                }
                if (ext->unk10 != -1) {
                    ext->unk28[ext->unk10] = -1;
                }
                ext->unk58 = 0;
                ext->unk10 = -1;
                ext->unk6C = 5;
                ext->unk14 = 0;
            }
            break;
        }
        break;
    }
    m->Engine->unk1C += (tx - m->Engine->unk1C) >> 2;
    m->Engine->unk1E += (ty - m->Engine->unk1E) >> 2;
    m->Engine->unk20 += (tz - m->Engine->unk20) >> 2;
    if (ext->bit != NULL) {
        ext->bit->Engine->unk10 = m->Engine->unk10;
        ext->bit->Engine->sub->unk68 = 0x100;
        ext->bit->Engine->unk10.y = 0x5E;
    }
}

s32 PolarBoxOverlap(EXTengine *a, s32 b, EXTengine *c, s32 d)
{
  VECTOR v;
  VECTOR dir;
  PolarBox box;
  MATRIX m;
  MATRIX *new_var2;
  SVECTOR rot;
  PolarBox *bp;
  s32 *rp;
  PolarBox *new_var;
  s32 dist;
  s32 side;
  PolarBox *new_var5;
  VECTOR *new_var3;
  char new_var4;
  s32 dx;
  s32 dz;
  s32 push;
  dist = 0x07FFFFFF;
  side = 0;
  box.min.x = -0xC8;
  box.min.z = -0x180;
  box.max.x = 0xC8;
  box.max.z = 0x180;
  box.r = 0x100;
  if (b != 0)
  {
    box.r = 0x12C;
  }

  new_var5 = (PolarBox *) (&m);
  v.vx = c->unk10.x - ((unsigned long long) a->unk10.x);
  v.vz = c->unk10.z - a->unk10.z;
  rot.vx = -a->unk1C;
  new_var4 = (rot.vy = -a->unk1E);
  rot.vz = -a->unk20;
  RotMatrixZYX(&rot, (MATRIX *) (bp = new_var5));
  new_var2 = (MATRIX *) bp;
  new_var = (bp = &box);
  ApplyMatrixLV(new_var2, &v, new_var3 = &v);
  rp = &box.r;
  if (v.vx < bp->min.x)
  {
    side = 2;
  }

  if (v.vz < bp->min.z)
  {
    side |= 4;
  }
  if (bp->max.x < v.vx)
  {
    side |= 1;
  }
  if (bp->max.z < v.vz)
  {
    side |= 8;
  }
  switch (side)
  {
    case 4:
      dist = abs(v.vz - bp->min.z);
      dir.vx = 0;
      dir.vz = -0x1000;
      break;

    case 6:
      dx = v.vx - bp->min.x;
      dz = v.vz - bp->min.z;
      dist = csqrt((dx * dx) + (dz * dz)) >> 6;
      dir.vx = -0x1000;
      dir.vz = -0x1000;
      break;

    case 5:
      dx = v.vx - bp->max.x;
      dz = v.vz - bp->min.z;
      dist = csqrt((dx * dx) + (dz * dz)) >> 6;
      dir.vx = 0x1000;
      dir.vz = -0x1000;
      break;

    case 8:
      dist = abs(v.vz - bp->max.z);
      dir.vx = 0;
      dir.vz = 0x1000;
      break;

    case 10:
      dx = v.vx - bp->min.x;
      dz = v.vz - bp->max.z;
      dist = csqrt((dx * dx) + (dz * dz)) >> 6;
      dir.vx = -0x1000;
      dir.vz = 0x1000;
      break;

    case 9:
      dx = v.vx - bp->max.x;
      dz = v.vz - bp->max.z;
      dist = csqrt((dx * dx) + (dz * dz)) >> 6;
      dir.vx = 0x1000;
      dir.vz = 0x1000;
      break;

    case 2:
      dist = abs(v.vx - bp->min.x);
      dir.vx = -0x1000;
      dir.vz = 0;
      break;

    case 1:
      dist = abs(v.vx - bp->max.x);
      dir.vx = 0x1000;
      dir.vz = 0;
      break;

  }

  if (dist < (*rp))
  {
    push = abs((*rp) - dist);
    rot.vx = a->unk1C;
    rot.vy = a->unk1E;
    rot.vz = a->unk20;
    RotMatrix(&rot, &m);
    ApplyMatrixLV(&m, &dir, &dir);
    c->unk10.x += (push * dir.vx) >> 12;
    c->unk10.z += (push * dir.vz) >> 12;
    return 1;
  }
  return 0;
}

void InitPolarWalrus(NAS *m) {
    PolarWalrusExt *ext;
    PolarWalrusExt2 *e2;

    PolarProximity = (PolarProx *)m;
    m->Engine->flags |= 0x200000;
    ext = AddExt(m, 0x94, 0x10000000);
    if (ext != NULL) {
        ext->cursor = ScriptCursorFirst(m, &WalrusScriptInit);
        m->FUX = (EXT *)ext;
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_ArenaPolarTiltSeal];
        m->Engine->sub->entity = 0x4000;
        m->UserInfo.status = 0;
        AddToGroup((NAS **)&PolarBears, (NAS *)m);
        m->Engine->unk10.x = (rcossin_tbl[(WalrusState - 0x400) & 0xFFF].unk2 * 0x307) >> 10;
        m->Engine->unk10.z = (rcossin_tbl[WalrusState & 0xFFF].unk2 * 0x307) >> 10;
        m->Engine->unk10.y = 0x400;
        m->Engine->unk1E = get_angle(m->Engine->unk10.x, m->Engine->unk10.z) & 0xFFF;
        m->Engine->sub->flags &= ~0x8000;
        ext->state = 0;
        m->Engine->unk5C = 1;
        m->Engine->unk5D = 0;
        m->Engine->unk2C.x = 0x1F40;
        m->Engine->unk2C.y = 0x1F40;
        m->Engine->unk2C.z = 0x1F40;
        m->Engine->unk5E = 1;
        m->Engine->sub->unk68 = 0x200;
        ext->arc[0] = 0x10;
        ext->unk58 = 0x28;
        ext->arc[1] = 0;
        ext->arc[6] = 0;
        ext->from = *(SLONG_XYZ *)&m->Engine->unk10;
        ext->to.x = (rcossin_tbl[(WalrusState - 0x400) & 0xFFF].unk2 * 0x7D) >> 8;
        ext->to.y = 0;
        ext->to.z = (rcossin_tbl[WalrusState & 0xFFF].unk2 * 0x7D) >> 8;
        m->Engine->sub->flags |= 0x02000000;
        ext->timer = Random(8) * 0x1E + 0x10E;
        ext->unk90 = 0;
        m->UserInfo.user1 = 0;
    }
    e2 = AddExt(m, 0x54, 0x20000000);
    if (e2 != NULL) {
        e2->unk4E = 0x400;
        e2->unk20 = 0;
        e2->unk40 = 0;
        e2->unk4C = 0;
    }
}

void PlotPolarWalrus(NAS *m) {
    PolarWalrusExt *ext;
    PolarWalrusExt2 *e2;
    s32 *gx;
    s32 v;
    s32 r;
    s32 ang;
    SLONG_XYZ pos;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    m->Engine->unk5E = 1;
    if (PolarClock != 0) {
        RemoveCamera(m);
    }
    e2 = FindExtension(m, 0x20000000);
    if (e2 == NULL) {
        return;
    }
    e2->unk20 = ext->unk48;
    e2->unk40 = ext->unk4C;
    switch (ext->state) {
    case 0:
        if (ext->timer-- < 0 && PolarBeamOwner != NULL) {
            gx = FindExtension((NAS *)PolarBeamOwner, 0x40000000);
            if (gx != NULL && PolarClock == 0) {
                v = gx[5];
                if (v == 1 && gx[4] == -1) {
                    ext->unk84 = CreateNAS(0, 0, 0, 0x1610, 0x11);
                    ext->state = 2;
                    ext->timer = 0x5A;
                    PolarBeamTimer = v;
                }
            }
        }
        break;
    case 2:
        if (PolarClock == 0 && ext->timer-- < 0) {
            ext->state = 3;
            PlaySfx(0x473, NULL, NULL, 0x1000, 0x1E00, 0);
            m->Engine->sub->flags |= 0x8000;
            AddCamera(m);
            ext->timer = 0;
        }
        break;
    case 3:
        if (++ext->timer < 0x1E) {
            m->Engine->sub->unk68 = 0x200;
        } else {
            m->Engine->sub->unk68 = 0;
        }
        RunNasAnim(m);
        ArcUpdate(&ext->from.x, &ext->to.x, ext->arc, &m->Engine->unk10.x);
        e2->unk20 = ext->unk48;
        e2->unk40 = ext->unk4C;
        if (ext->cursor->unk1E == 0x18) {
            ext->state = 4;
            ext->cursor->unk1E = 0;
            ext->cursor->unk1C = 0;
            ext->cursor->cur = (s32)WalrusSteps0;
        }
        break;
    case 4:
        m->Engine->sub->unk68 = 0;
        e2->unk20 = ext->unk48;
        e2->unk40 = ext->unk4C;
        RunNasAnim(m);
        if (ArcUpdate(&ext->from.x, &ext->to.x, ext->arc, &m->Engine->unk10.x) != 0) {
            r = Random(4);
            ext->state = 1;
            ext->timer = 0x96;
            ext->cursor->unk1E = 0;
            ext->cursor->unk1C = 0;
            ext->cursor->cur = (s32)WalrusSteps1;
            PlaySfx(0x475, NULL, NULL, 0x1000, 0x1E00, 0);
            if (r == 0) {
                PlaySfx(0x46F, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 1) {
                PlaySfx(0x470, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 2) {
                PlaySfx(0x471, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 3) {
                PlaySfx(0x472, NULL, NULL, 0x1000, 0x1E00, 0);
            }
        }
        break;
    case 1:
        m->UserInfo.status = 1;
        m->Engine->sub->flags |= 0x8000;
        if (RunNasAnim(m)) {
            ext->state = 5;
            ext->cursor->unk1E = 0;
            ext->cursor->unk1C = 0;
            ext->cursor->cur = (s32)WalrusSteps2;
            ext->timer = 0;
            ext->maxSpeed = 0x10;
            ext->speed = 0;
            ext->unk90 = m->Engine->unk1E + 0x800;
            if (ext->unk84 != NULL) {
                KillNAS(ext->unk84);
                ext->unk84 = NULL;
            }
        }
        break;
    case 5:
        RunNasAnim(m);
        if (ext->cursor->unk1E == 7) {
            PlaySfx(0x476, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (ext->cursor->unk1E == 0xD) {
            PlaySfx(0x477, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        m->Engine->unk1E -= (s32)((m->Engine->unk1E - ext->unk90) << 20) >> 24;
        if (ext->speed != ext->maxSpeed && ext->speed < ext->maxSpeed) {
            ext->speed++;
            if (ext->speed > ext->maxSpeed) {
                ext->speed = ext->maxSpeed;
            }
            if (ext->speed < ext->maxSpeed) {
                ext->speed--;
                if (ext->speed < ext->maxSpeed) {
                    ext->speed = ext->maxSpeed;
                }
            }
        }
        if (++ext->timer == 0x1E) {
            PlaySfx(0x47A, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        m->Engine->unk10.x -= (ext->speed * rcossin_tbl[(m->Engine->unk1E - 0x400) & 0xFFF].unk2) >> 12;
        m->Engine->unk10.z -= (ext->speed * rcossin_tbl[m->Engine->unk1E & 0xFFF].unk2) >> 12;
        pos.x = m->Engine->unk10.x + ((rcossin_tbl[(m->Engine->unk1E - 0x400) & 0xFFF].unk2 * 0x80) >> 12);
        pos.z = m->Engine->unk10.z + ((rcossin_tbl[m->Engine->unk1E & 0xFFF].unk2 * 0x80) >> 12);
        e2->unk20 = 0;
        e2->unk40 = 0;
        if ((CheckMelted(&pos, 0) ^ 1) != 0) {
            ext->state = 7;
            ext->cursor->unk1E = 0;
            ext->cursor->unk1C = 0;
            ext->cursor->cur = (s32)WalrusSteps3;
            m->Engine->sub->unk68 = 0x19C;
            PlaySfx(0x474, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        break;
    case 7:
        if (RunNasAnim(m)) {
            ext->state = 6;
            PolarBeamTimer = 0;
        }
        break;
    case 6:
        ext->timer = Random(0x384) + 0x5A;
        ext->state = 0;
        m->UserInfo.status = 0;
        m->Engine->sub->flags &= ~0x8000;
        m->Engine->sub->unk68 = 0x200;
        ang = WalrusState;
        m->Engine->unk10.x = (rcossin_tbl[(ang + 0x400) & 0xFFF].unk2 * 0x307) >> 10;
        m->Engine->unk10.z = (rcossin_tbl[(ang + 0x800) & 0xFFF].unk2 * 0x307) >> 10;
        m->Engine->unk10.y = 0x400;
        WalrusState += 0x800;
        ext->from = *(SLONG_XYZ *)&m->Engine->unk10;
        ext->to.x = (rcossin_tbl[(WalrusState - 0x400) & 0xFFF].unk2 * 0x7D) >> 8;
        ext->to.y = 0;
        ext->to.z = (rcossin_tbl[WalrusState & 0xFFF].unk2 * 0x7D) >> 8;
        m->Engine->unk1E = get_angle(m->Engine->unk10.x, m->Engine->unk10.z) & 0xFFF;
        ext->cursor->unk1E = 0;
        ext->cursor->unk1C = 0;
        ext->cursor->cur = (s32)WalrusFrames;
        ext->arc[0] = 0x10;
        ext->unk58 = 0x28;
        ext->arc[1] = 0;
        RemoveCamera(m);
        break;
    }
}

void InitWalrusFish(NAS *m) {
    WalrusFishExt *ext;

    ext = AddExt(m, 0x88, 0x10000000);
    if (ext != NULL) {
        ext->cursor = ScriptCursorFirst(m, &WalrusFishScriptInit);
        m->Engine->flags |= 0x200000;
        m->Engine->unk10.x = (rcossin_tbl[(WalrusState - 0x400) & 0xFFF].unk2 * 0x307) >> 10;
        m->Engine->unk10.z = (rcossin_tbl[WalrusState & 0xFFF].unk2 * 0x307) >> 10;
        m->Engine->unk10.y = 0x400;
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_ArenaPolarTiltSeal];
        m->Engine->sub->entity = 0x2002;
        m->Engine->unk5C = 1;
        m->Engine->unk5D = 0;
        m->Engine->unk2C.x = 0x1F40;
        m->Engine->unk2C.y = 0x1F40;
        m->Engine->unk2C.z = 0x1F40;
        ext->state = 0;
        m->Engine->sub->unk68 = 0x200;
        ext->arc[0] = 0x10;
        ext->unk54 = 0x28;
        ext->arc[1] = 0;
        ext->arc[6] = 0;
        ext->from = *(SLONG_XYZ *)&m->Engine->unk10;
        ext->to.x = (rcossin_tbl[(WalrusState - 0x400) & 0xFFF].unk2 * 0x7D) >> 8;
        ext->to.y = 0;
        ext->to.z = (rcossin_tbl[WalrusState & 0xFFF].unk2 * 0x7D) >> 8;
        m->Engine->sub->flags |= 0x02000000;
        ext->spin = 0x80;
        ext->timer = 0;
        PlaySfx(0x478, NULL, NULL, 0x1000, 0x1E00, 0);
        m->Engine->unk5E = 1;
    }
}

void PlotWalrusFish(NAS *m) {
    WalrusFishExt *ext;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    m->Engine->unk1E += ext->spin;
    m->Engine->unk1C += 0x100;
    m->Engine->unk5E = 1;
    switch (ext->state) {
    case 0:
        if (++ext->timer == 0xF) {
            m->Engine->sub->unk68 = 0;
        }
        if (ArcUpdate(&ext->from.x, &ext->to.x, ext->arc, &m->Engine->unk10.x) != 0) {
            ext->state = 2;
            PlaySfx(0x479, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        break;
    case 2:
        RunNasAnim(m);
        m->Engine->unk10.x += ext->vx;
        m->Engine->unk10.z += ext->vz;
        ext->spin >>= 1;
        m->Engine->unk1C >>= 1;
        if (ext->vx < 0) {
            if (++ext->vx <= 0) {
                goto skip_x;
            }
            ext->vx = 0;
        }
        if (ext->vx > 0) {
            if (--ext->vx < 0) {
                ext->vx = 0;
            }
        }
    skip_x:
        if (ext->vz < 0) {
            if (++ext->vz <= 0) {
                goto skip_z;
            }
            ext->vz = 0;
        }
        if (ext->vz > 0) {
            if (--ext->vz < 0) {
                ext->vz = 0;
            }
        }
    skip_z:
        break;
    }
}

void PlotSmokeTrail(NAS *m) {
    ExtSmokeTrail *ext;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    if (ext->flags & 1) {
        m->Engine->unk10.y -= ext->rise;
        m->Engine->unk10.x = ext->baseX + ((ext->amp * rcossin_tbl[(ext->angle - 0x400) & 0xFFF].unk2) >> 12);
        ext->amp -= ext->ampDecay;
        if (ext->amp < 0) {
            ext->amp = 0;
        }
        m->Engine->sub->entity = 0x202E;
        ext->angle += 0x80;
    }
    switch (m->UserInfo.status) {
    case 0:
        m->Engine->sub->unk76 -= ext->fadeIn;
        if ((s16)m->Engine->sub->unk76 < 0) {
            m->Engine->sub->unk76 = 0;
            m->UserInfo.status = 1;
        }
        break;
    case 1:
        m->Engine->sub->unk76 += ext->fadeOut;
        m->Engine->unk2C.x -= ext->shrink;
        m->Engine->unk2C.y -= ext->shrink;
        m->Engine->unk2C.z -= ext->shrink;
        if ((s16)m->Engine->sub->unk76 >= 0xDAD) {
            KillNAS(m);
            return;
        }
        break;
    }
    m->Engine->unk5E = 1;
}

void InitSimpleExplosion(NAS *m) {
    ExtSimpleExplosion *ext;
    subengine *sub;
    s32 i;

    ext = AddExt(m, 0x108, 0x10000000);
    if (ext != NULL) {
        sub = m->Engine->sub;
        eng_add_subs(m->Engine, 0x10);
        if (sub != NULL) {
            i = 0;
            do {
                sub->entity = 0x2034;
                sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
                ext->vel[i].x = Random(0x4C) - Random(0x4C);
                ext->vel[i].z = Random(0x4C) - Random(0x4C);
                ext->vel[i].y = Random(0x4C) - Random(0x4C);
                sub->flags = 0x40008041;
                sub->unk50 = 1;
                sub->unk51 = 0;
                sub->unk20.z = 0x1000;
                sub->unk20.y = 0x1000;
                sub->unk20.x = 0x1000;
                ext->unk104 = 0;
                sub->unk76 = 0;
                sub = sub->next;
                i++;
            } while (sub != NULL);
        }
    }
}

void PlotDingoTail(NAS *m) {
    BrioOwner *owner;
    s32 user;

    user = m->UserInfo.user1;
    if (user != 0) {
        owner = (BrioOwner *)user;
        if (owner->type != 0x51) {
            KillNAS(m);
            return;
        }
        m->Engine->unk1C += 0x200;
        m->Engine->unk5E = 1;
        m->Engine->unk10 = owner->engine->unk10;
        m->Engine->unk1E = owner->engine->unk1E - 0x400;
        m->Engine->unk2C.x = owner->engine->unk2C.x;
        m->Engine->unk2C.y = owner->engine->unk2C.y;
        m->Engine->unk2C.z = owner->engine->unk2C.z;
        if (owner->subtype == 7) {
            m->Engine->unk10.y = owner->engine->unk10.y - ((owner->engine->unk2C.x * 0x208) >> 12);
        } else {
            m->Engine->unk10.y = owner->engine->unk10.y - ((owner->engine->unk2C.x * 0x1A4) >> 12);
        }
        if (owner->anim->frame >= 0xD) {
            m->Engine->sub->unk76 += 0x400;
        }
    }
}

void PlotBrioPack(NAS *m) {
    BrioOwner *owner;
    NAS *nas;
    s32 user;

    user = m->UserInfo.user1;
    if (user != 0) {
        owner = (BrioOwner *)user;
        if (owner->type != 0x51) {
            KillNAS(m);
            return;
        }
        if (owner->anim->frame == 0xC) {
            nas = CreateNAS(owner->engine->unk10.x,
                               owner->engine->unk10.y - ((owner->engine->unk2C.x * 0x1A4) >> 12),
                               owner->engine->unk10.z + 0x40, 0x1613, 0x11);
            if (nas != NULL) {
                nas->UserInfo.user1 = (s32)owner;
            }
        }
        if (owner->anim->frame >= 0xA) {
            m->Engine->unk10 = owner->engine->unk10;
            m->Engine->unk1E = owner->engine->unk1E;
            m->Engine->unk2C.x = owner->engine->unk2C.x;
            m->Engine->unk2C.y = owner->engine->unk2C.y;
            m->Engine->unk2C.z = owner->engine->unk2C.z;
            m->Engine->unk10.y = owner->engine->unk10.y - ((owner->engine->unk2C.x * 0x1A4) >> 12);
        }
        if (owner->anim->frame >= 0x18) {
            m->Engine->sub->unk76 += 0x400;
        }
    }
    m->Engine->unk5E = 1;
}

void InitPackEffect(NAS *m) {
    ExtSimpleExplosion *ext;
    subengine *sub;
    s32 i;

    ext = AddExt(m, 0x108, 0x10000000);
    if (ext != NULL) {
        sub = m->Engine->sub;
        eng_add_subs(m->Engine, 0xC);
        if (sub != NULL) {
            i = 0;
            do {
                sub->entity = 0x2033;
                sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
                ext->vel[i].x = Random(0x38) - Random(0x38);
                ext->vel[i].z = Random(0x38) - Random(0x38);
                ext->vel[i].y = Random(0x38) - Random(0x38);
                sub->flags = 0x40008021;
                sub->unk50 = 1;
                sub->unk51 = 0;
                sub->unk20.z = 0x1000;
                sub->unk20.y = 0x1000;
                sub->unk20.x = 0x1000;
                ext->unk104 = 0;
                sub->unk76 = 0;
                sub = sub->next;
                i++;
            } while (sub != NULL);
        }
    }
}

void InitPolarBoss(NAS *nas) {
    PolarBossExt *ext = AddExt(nas, 0x94, 0x10000000);
    NAS *s;
    Ext20 *e;

    if (ext != NULL) {
        AddCamera(nas);
        ResetHeroGlobals();
        BearminatorNas = nas;
        ext->cursor = ScriptCursorFirst(nas, &PolarBossScriptInit);
        GotoLinkAnim((EXTanimator *)ext->cursor, 0);
        RunNasAnim(nas);
        ext->unk84 = CreateNAS(nas->Engine->unk10.x, nas->Engine->unk10.y, nas->Engine->unk10.z, 0x1622, 0x11);
        s = CreateNAS(nas->Engine->unk10.x, 0, nas->Engine->unk10.z, 0x1620, 0x11);
        if (s != NULL) {
            ext->unk88 = s;
            AddCamera(s);
        }
        ext->unk90 = 0;
        nas->Engine->sub->u6C.dbase = MapScene->dbase;
        nas->Engine->sub->unk68 = 0x300;
        nas->Engine->sub->flags |= 0x02000000;
        nas->Engine->unk5D = 0;
        BossState.health = 0x14;
        e = FindExtension(Heroes[3].hero, 0x20000000);
        if (e != NULL) {
            e->val = 0xFF;
        }
        ext->state = 5;
        ext->unk38 = 0x12;
        ext->unk3C = 4;
        ext->unk3E = 4;
        ext->unk5C = 4;
        ext->unk5E = 4;
        ext->unk34 = 0;
        ext->unk58 = 0;
        ext->unk54 = 0;
        ext->unk68 = 0;
        ext->ang = 0;
        ext->unk70 = 1;
        ext->unk74 = 0;
        ext->phase = 0;
        ext->unk80 = 0;
        ext->unk8C = 0xC;
        PlaySfx(0x226, &nas->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
        nas->Engine->OldXYZ.x = (rcossin_tbl[(((ext->ang + 0x20) & 0xFFF) - 0x400) & 0xFFF].unk2 * 3) >> 2;
        nas->Engine->OldXYZ.z = ((rcossin_tbl[(ext->ang + 0x20) & 0xFFF].unk2 * 0x400) >> 12) + 0x2000;
        get_angle(nas->Engine->OldXYZ.x - nas->Engine->unk10.x, nas->Engine->OldXYZ.z - nas->Engine->unk10.z);
        nas->Engine->unk1E = 0x800;
        nas->Engine->unk5E = 1;
    }
}

void PlotPolarBoss(NAS *m) {
    PolarBossExt *ext;
    PolarBossAnimExt *mate;
    PolarBossAnimExt *pe;
    PolarBossExt *be;
    NAS *nas;
    HeroInfo *h;
    SLONG_XYZ pos;
    s32 ang;
    s32 i;
    s32 n;
    s32 rot;
    s32 dx;
    s32 dz;
    s32 k;
    s32 t;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    if (((PolarBossGame *)GameInfo)->teams != 0) {
        h = Heroes;
        if ((h[((PolarBossGame *)GameInfo)->lead].hero_status & 0x2000)
            && (h[h[((PolarBossGame *)GameInfo)->lead].hero_teammate].hero_status & 0x2000)) {
            CameraMode = 0;
        }
    } else if (Heroes[0].hero_status & 0x2000) {
        CameraMode = 0;
    }
    polarbossnas = m;
    m->UserInfo.user1 = RunNasAnim(m);
    if (!(Heroes[0].hero_mode & 0x2000)) {
        ang = get_angle(m->Engine->unk10.x - Heroes[0].hero_eng->unk10.x,
                        m->Engine->unk10.z - Heroes[0].hero_eng->unk10.z) & 0xFFF;
    } else {
        ang = get_angle(m->Engine->unk10.x - Heroes[1].hero_eng->unk10.x,
                        m->Engine->unk10.z - Heroes[1].hero_eng->unk10.z) & 0xFFF;
    }
    if (ext->state != 8) {
        ext->unk84->Engine->unk1E = ang;
    }
    mate = FindExtension(ext->unk84, 0x10000000);
    if (mate != NULL) {
        if (((PolarBossGame *)GameInfo)->teams != 0) {
            h = Heroes;
            if ((h[((PolarBossGame *)GameInfo)->lead].hero_status & 0x2000)
                && (h[h[((PolarBossGame *)GameInfo)->lead].hero_teammate].hero_status & 0x2000)
                && ext->state != 9 && ext->unk90 == 0) {
                ext->state = 9;
                GotoLinkAnim(mate->anim, 7);
                return;
            }
        } else if ((Heroes[0].hero_status & 0x2000) && ext->unk90 == 0 && ext->state != 9) {
            PlaySfx(0x23B, NULL, NULL, 0x1000, 0x1E00, 0);
            ext->state = 9;
            GotoLinkAnim(mate->anim, 7);
            return;
        }
    }
    if (ext->unk68-- < 0) {
        for (k = 0;; k++) {
            s32 d;

            if (k >= 4) {
                break;
            }
            d = 0x700;
            pos.x = m->Engine->unk10.x + PolarSinMul((u16)m->Engine->unk1E, d);
            pos.z = m->Engine->unk10.z + PolarSinMul(m->Engine->unk1E + 0x400, d);
            pos.x += PolarRandSpread(0x400);
            pos.z += PolarRandSpread(0x400);
            d = 0x500;
            pos.x = m->Engine->unk10.x - PolarSinMul((u16)m->Engine->unk1E, d);
            pos.z = m->Engine->unk10.z - PolarSinMul(m->Engine->unk1E + 0x400, d);
            pos.x += PolarRandSpread(0x5F4);
            pos.z += PolarRandSpread(0x5F4);
        }
        ext->unk68 = Random(4) + 5;
    }
    switch (ext->state) {
    case 5:
        if (ext->unk84->Engine->sfx != NULL) {
            StopSfx(ext->unk84->Engine->sfx);
        }
        m->Engine->OldXYZ.x = (rcossin_tbl[(((ext->ang + 0x20) & 0xFFF) - 0x400) & 0xFFF].unk2 * 3) >> 2;
        m->Engine->OldXYZ.z = ((rcossin_tbl[(ext->ang + 0x20) & 0xFFF].unk2 * 0x400) >> 12) + 0x2000;
        ext->unk84->Engine->unk10 = m->Engine->unk10;
        ext->unk88->Engine->unk10 = m->Engine->unk10;
        ext->unk88->Engine->unk10.y -= 0x400;
        rot = (get_angle(m->Engine->OldXYZ.x - m->Engine->unk10.x,
                         m->Engine->OldXYZ.z - m->Engine->unk10.z) & 0xFFF) + 0x400;
        m->Engine->unk1E -= ((m->Engine->unk1E - rot) << 20) >> 22;
        m->Engine->unk10.x = (rcossin_tbl[(ext->ang - 0x400) & 0xFFF].unk2 * 3) >> 2;
        m->Engine->unk10.z = ((rcossin_tbl[ext->ang & 0xFFF].unk2 * 0x400) >> 12) + 0x2000;
        ext->ang = (ext->ang + ext->unk8C) & 0xFFF;
        if (ext->unk70 != 0) {
            if (ext->phase == 0 || ext->phase == 2 || ext->phase == 4 || ext->phase == 6) {
                if ((u32)(ext->ang - 0x709) < 0xC7) {
                    ext->state = 6;
                    ext->unk70 = 0;
                    ext->unk8C = 4;
                }
            }
            if (ext->phase == 1 || ext->phase == 3 || ext->phase == 5 || ext->phase == 7) {
                if (ext->phase == 7) {
                    FindExtension(Heroes[0].hero, 0x20000000);
                    FindExtension(Heroes[1].hero, 0x20000000);
                }
                if ((u32)(ext->ang - 0x12C) >= 0xE11) {
                    if (ext->unk80 == 0 && ext->phase < 4) {
                        ext->phase--;
                        return;
                    }
                    ext->state = 6;
                    ext->unk70 = 0;
                    ext->unk80 = 0;
                }
            }
        }
        break;
    case 6:
        if (ext->phase == 0 || ext->phase == 2 || ext->phase == 4 || ext->phase == 6) {
            n = 0;
            t = 0;
            pos.z = 0;
            pos.y = 0;
            pos.x = 0;
            pe = FindExtension(ext->unk84, 0x10000000);
            if (pe != NULL) {
                GotoLinkAnim(pe->anim, 1);
            }
            if (ext->phase == 0) {
                n = 1;
                GotoLinkAnim((EXTanimator *)ext->cursor, n);
                RunNasAnim(m);
            }
            if (ext->phase == 2) {
                n = 2;
                GotoLinkAnim((EXTanimator *)ext->cursor, 3);
                RunNasAnim(m);
            }
            if (ext->phase == 4) {
                GotoLinkAnim((EXTanimator *)ext->cursor, 5);
                n = 3;
                RunNasAnim(m);
            }
            if (ext->phase == 6) {
                GotoLinkAnim((EXTanimator *)ext->cursor, 5);
                n = 3;
                RunNasAnim(m);
            }
            dz = 0;
            dx = dz;
            i = dz;
            for (;; i++) {
                PolarShardExt *se;
                NAS *nas;

                if (i >= n) {
                    break;
                }
                if (n == 1) {
                    dx = 0;
                    dz = -0x700;
                }
                if (n == 2) {
                    if (i == 0) {
                        dx = -0x380;
                        dz = -0x700;
                    }
                    if (i == 1) {
                        dx = 0x380;
                        dz = -0x700;
                    }
                }
                if (n == 3) {
                    if (i == 0) {
                        dx = -0x380;
                        dz = -0x700;
                    }
                    if (i == 1) {
                        dx = 0x380;
                        dz = -0x700;
                    }
                    if (i == 2) {
                        dx = 0;
                        dz = -0x700;
                    }
                }
                nas = CreateNAS(m->Engine->unk10.x + dx, m->Engine->unk10.y - 0x5DC,
                                   m->Engine->unk10.z + dz, 0x161B, 0x11);
                if (nas != NULL) {
                    se = FindExtension(nas, 0x10000000);
                    if (se != NULL) {
                        if (ext->phase == 0 && i == 0) {
                            t = Random(0x1000);
                        }
                        if (ext->phase == 2) {
                            if (i == 0) {
                                t = Random(0x400);
                            }
                            if (i == 1) {
                                t = Random(0x400) + 0x400;
                                if (Random(0x1000) < PolarShardChance()) {
                                    se->unkB4 = 1;
                                }
                            }
                        }
                        if (ext->phase == 4) {
                            if (i == 0) {
                                t = Random(0x2AA) + 0x800;
                            }
                            if (i == 1) {
                                t = Random(0x2AA) + 0xAAA;
                                if (Random(0x1000) < PolarShardChance()) {
                                    se->unkB4 = 1;
                                }
                            }
                            if (i == 2) {
                                t = Random(0x2AA) + 0xD54;
                                if (Random(0x1000) < PolarShardChance()) {
                                    se->unkB4 = 1;
                                }
                            }
                            t -= 0x400;
                        }
                        if (ext->phase == 6) {
                            if (i == 0) {
                                t = Random(0x78) + 0xC00;
                            }
                            if (i == 1) {
                                t = Random(0x78) + 0xD7D;
                                if (Random(0x1000) < PolarShardChance()) {
                                    se->unkB4 = 1;
                                }
                            }
                            if (i == 2) {
                                t = Random(0x78) + 0xEAA;
                                if (Random(0x1000) < PolarShardChance()) {
                                    se->unkB4 = 1;
                                }
                            }
                            t -= 0x400;
                        }
                        se->unk94 = t;
                    }
                }
            }
            ext->unk74 = 1;
            ext->state = 7;
            ext->phase++;
            if (m->Engine->sfx != NULL) {
                StopSfx(m->Engine->sfx);
            }
            PlaySfx(0x227, &ext->unk84->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
            return;
        }
        if (ext->phase == 1 || ext->phase == 3 || ext->phase == 5 || ext->phase == 7) {
            NAS *nas;

            pe = FindExtension(ext->unk84, 0x10000000);
            if (pe != NULL) {
                GotoLinkAnim(pe->anim, 1);
            }
            GotoLinkAnim((EXTanimator *)ext->cursor, 7);
            nas = CreateNAS(m->Engine->unk10.x, m->Engine->unk10.y, m->Engine->unk10.z, 0x1621, 0x11);
            if (nas != NULL) {
                be = FindExtension(nas, 0x10000000);
                if (be != NULL) {
                    be->unk70 = 0;
                    if (ext->phase == 1) {
                        be->ang = (rcossin_tbl[0xA00].unk2 * 3) >> 3;
                        be->unk74 = (rcossin_tbl[0xE00].unk2 * 3) >> 3;
                    }
                    if (ext->phase == 3) {
                        be->ang = (rcossin_tbl[0xE00].unk2 * 3) >> 3;
                        be->unk74 = (rcossin_tbl[0x200].unk2 * 3) >> 3;
                    }
                    if (ext->phase == 5) {
                        be->ang = (rcossin_tbl[0x200].unk2 * 3) >> 3;
                        be->unk74 = (rcossin_tbl[0x600].unk2 * 3) >> 3;
                    }
                    if (ext->phase == 7) {
                        be->ang = (rcossin_tbl[0x600].unk2 * 3) >> 3;
                        be->unk74 = (rcossin_tbl[0xA00].unk2 * 3) >> 3;
                    }
                }
            }
            ext->state = 7;
            ext->phase++;
            return;
        }
        break;
    case 7:
        if (CountBears() == 0) {
            ext->state = 5;
            PlaySfx(0x226, &m->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
            ext->unk70 = 1;
            if (ext->phase & 1) {
                SLONG_XYZ *p;
                void *tex;

                pos.y = -0x700;
                pos.x = 0;
                pos.z = 0;
                p = &pos;
                tex = &HeroModelBank[DB_PolarObjects];
                nas = CreateNAS(p->x, p->y, p->z, 0x1605, 0x11);
                if (nas != NULL) {
                    ExtSpawn5910 *sp = FindExtension(nas, 0x10000000);
                    if (sp != NULL) {
                        sp->unkC = 5;
                        sp->unk18 = 0x100;
                        if (tex != NULL) {
                            sp->unk20 = 5;
                            nas->Engine->sub->u6C.dbase = tex;
                        } else {
                            sp->unk20 = 1;
                        }
                        sp->unk28 = 0;
                        nas->InitDelete(nas);
                    }
                }
                CreateNAS(0, -0x600, 0, 0x1623, 0x11);
            }
        }
        break;
    case 9:
        if (mate->anim->Aindex == 1) {
            if (++ext->unk90 == 4) {
                GotoLinkAnim(mate->anim, 0);
                ext->state = 10;
                return;
            }
            PlaySfx(0x23B, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        break;
    }
    m->Engine->unk5E = 1;
}

s32 BearminatorFindPoint(s32 *arg0, s32 arg1) {
    s32 v[3];
    u8 *base;
    s32 off;
    u8 *p;
    u8 *a2;
    u8 *a3;
    TrigEntry *t;
    s32 n;
    s32 i;
    s32 ang;

    base = ((u8 *)MapScene->map);
    off = *(s32 *)(base + 0x14);
    p = (u8 *)&((s32 *)(base + 0x14 + off))[arg1 + 1] + ((s32 *)(base + 0x14 + off))[arg1 + 1];
    if (p != NULL) {
        i = 0;
        n = *(s32 *)(p + 0x38);
        t = rcossin_tbl;
        a2 = p + 0x50;
        a3 = p;
        for (;;) {
            if (i >= n) {
                return 1;
            }
            ang = *(s32 *)(a3 + 0x64);
            v[0] = arg0[0] - *(s32 *)(a2 - 8);
            v[2] = arg0[2] - *(s32 *)a2;
            ang = -ang;
            if (((v[0] * t[ang & 0xFFF].unk2) >> 12) - ((v[2] * t[(ang - 0x400) & 0xFFF].unk2) >> 12) < -0x80) {
                return 0;
            }
            a3 += 0x38;
            i++;
            a2 += 0x38;
        }
    }
    return 1;
}

s32 BearminatorTarget(NAS *m) {
    if (bossmapsub->entity == 0x2008) {
        if (BearminatorFindPoint((s32 *)&m->Engine->unk10, 1)) {
            return 1;
        }
        if (BearminatorFindPoint((s32 *)&m->Engine->unk10, 0)) {
            return 0;
        }
    }
    if (bossmapsub->entity == 0x2009) {
        if (BearminatorFindPoint((s32 *)&m->Engine->unk10, 2)) {
            return 1;
        }
        if (BearminatorFindPoint((s32 *)&m->Engine->unk10, 0)) {
            return 0;
        }
    }
    if (bossmapsub->entity == 0x200A) {
        if (BearminatorFindPoint((s32 *)&m->Engine->unk10, 3)) {
            return 0;
        }
    }
    if (bossmapsub->entity == 0x5003) {
        if (BearminatorFindPoint((s32 *)&m->Engine->unk10, 0)) {
            return 0;
        }
    }
    return 1;
}

void BearminatorEaseCam(PolarEaseCam *c, s32 *pos) {
    s32 step;

    if (c->x.cur != c->x.target) {
        if (c->x.target != 0) {
            step = c->x.step;
        } else {
            step = c->x.stepZero;
        }
        if (c->x.cur < c->x.target) {
            c->x.cur += step;
            if (c->x.cur > c->x.target) {
                c->x.cur = c->x.target;
            }
        } else {
            c->x.cur -= step;
            if (c->x.cur < c->x.target) {
                c->x.cur = c->x.target;
            }
        }
    }
    if (c->z.cur != c->z.target) {
        if (c->z.target != 0) {
            step = c->z.step;
        } else {
            step = c->z.stepZero;
        }
        if (c->z.cur < c->z.target) {
            c->z.cur += step;
            if (c->z.cur > c->z.target) {
                c->z.cur = c->z.target;
            }
        } else {
            c->z.cur -= step;
            if (c->z.cur < c->z.target) {
                c->z.cur = c->z.target;
            }
        }
    }
    pos[0] -= c->x.cur;
    pos[2] -= c->z.cur;
}

void InitBearminator(NAS *m) {
    BearminatorExt *ext;
    BearminatorExt2 *e2;
    NAS *n;
    EXTengine *e;
    EXTengine *f;
    s32 r;
    s16 v;

    ext = AddExt(m, 0xB8, 0x10000000);
    if (ext != NULL) {
        r = Random(2);
        if (r == 0) {
            PlaySfx(0x22B, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 1) {
            PlaySfx(0x22C, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        PlaySfx(0x22E, NULL, NULL, 0x1000, 0x1E00, 0);
        e2 = AddExt(m, 0x54, 0x20000000);
        if (e2 != NULL) {
            m->Engine->sub->u6C.dbase = &HeroModelBank[DB_RoboPolar];
            n = CreateNAS(m->Engine->unk10.x, 0, m->Engine->unk10.z, 0x1620, 0x11);
            if (n != NULL) {
                ext->unk98 = n;
                AddCamera(n);
            }
            ext->unkAC = e2;
            ext->unkB4 = 0;
            m->Engine->unk5C = 1;
            m->Engine->unk2C.x = 0x1F40;
            m->Engine->unk2C.y = 0x1F40;
            m->Engine->unk2C.z = 0x1F40;
            m->Engine->unk5E = 1;
            m->Engine->flags |= 0x200000;
            AddToGroup((NAS **)&PolarBears, (NAS *)m);
            ext->cursor = ScriptCursorFirst(m, &BearminatorScriptInit);
            ext->unkC = 3;
            e2->unk20 = 0;
            e2->unk4E = v = ((u16)AiPlayerSkill[0].level * 8) - 0x30;
            if (v < 0x10) {
                e2->unk4E = 0x10;
            }
            e2->unk40 = 0;
            e2->unk28 = 4;
            e2->unk2A = 1;
            e2->unk48 = 4;
            e2->unk4A = 1;
            ext->unkA0 = 0;
            ext->unk14 = 0;
            ext->unkA4 = 0;
            ext->unk18 = m->Engine->unk1E;
            ext->unk38 = 0xB;
            ext->unk7C = 0x96;
            ext->unk3C = 0;
            ext->unk50 = Heroes[0].hero_eng->unk10.y;
            ext->unk8C = 0;
            ext->unk1C = 0x5A;
            ext->unkA8 = 0;
            ext->unkB0 = 0;
            m->FUX = NULL;
            e = AddExtEngine(m, 2);
            if (e != NULL) {
                e->sub->u6C.dbase = &HeroModelBank[DB_RoboPolar];
                e->sub->entity = 0x4000;
                e->sub->unk68 = 0x80;
                e->sub->flags |= 0x02000000;
                e->unk10 = m->Engine->unk10;
                e->unk5D = 0;
                e->flags |= 0x200000;
                e->sub->flags = 0x40008021;
                e->sub->flags &= ~0x8000;
                e->unk1E = m->Engine->unk1E;
                e->unk20 = -0x800;
                m->FUX = (EXT *)e;
                e->unk2C.x = 0x1F40;
                e->unk2C.y = 0x1F40;
                e->unk2C.z = 0x1F40;
                e->sub->unk76 = 0xC00;
                e->unk5E = 1;
            }
            ext->unk9C = NULL;
            f = AddExtEngine(m, 3);
            if (f != NULL) {
                f->sub->u6C.dbase = &HeroModelBank[DB_RoboPolar];
                f->sub->entity = 0x2006;
                f->unk10 = m->Engine->unk10;
                f->unk5D = 0;
                f->flags |= 0x200000;
                f->sub->flags &= ~0x8000;
                f->unk1E = m->Engine->unk1E;
                ext->unk9C = f;
                f->unk5E = 1;
            }
        }
    }
}

void PlotBearminator(NAS *m) {
    BearminatorExt *ext;
    PolarEaseCam *cam;
    EXTengine *fux;
    u8 *p;
    s32 done;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    fux = (EXTengine *)m->FUX;
    cam = FindExtension(m, 0x20000000);
    if (cam == NULL) {
        return;
    }
    done = RunNasAnim(m);
    p = PolarBears;
    for (;;) {
        EXTengine *n;
        EXTengine *e;
        s32 dx;
        s32 dz;
        s32 d;
        s32 a;
        s32 s;

        if (p == NULL) {
            break;
        }
        n = ((NAS *)p)->Engine;
        if (n != NULL) {
            e = m->Engine;
            if (n != e) {
                dx = n->unk10.x - e->unk10.x;
                dz = n->unk10.z - e->unk10.z;
                d = dx * dx + dz * dz;
                if (d <= 0x3D08F) {
                    s = 0x7D00 - csqrt(d);
                    a = get_angle(dx, dz) & 0xFFF;
                    m->Engine->unk10.x -= ((s + 2) * rcossin_tbl[(a - 0x400) & 0xFFF].unk2) >> 18;
                    m->Engine->unk10.z -= ((s + 2) * rcossin_tbl[a].unk2) >> 18;
                }
            }
        }
        p = *(u8 **)(p + 0x20);
    }
    switch (ext->unkC) {
    case 8:
        CameraMode = 0;
        break;
    case 3:
        m->Engine->sub->flags &= ~0x8000;
        if (BearminatorNas->UserInfo.user1 != 0) {
            s32 r;

            r = Random(0x29C) + 0x400;
            ext->vel[0] = (r * rcossin_tbl[(ext->unk94 - 0x400) & 0xFFF].unk2) >> 12;
            ext->vel[1] = 0;
            ext->vel[2] = (r * rcossin_tbl[ext->unk94 & 0xFFF].unk2) >> 12;
            *(SLONG_XYZ *)ext->from = m->Engine->unk10;
            ArcUpdate(ext->from, ext->vel, &ext->unk38, &m->Engine->unk10.x);
            m->Engine->sub->flags |= 0x8000;
            ext->unkC = 4;
        }
        break;
    case 9:
        if (--ext->unkB0 == -1) {
            CreateNAS(m->Engine->unk10.x, m->Engine->unk10.y, m->Engine->unk10.z, 0x1612, 0x11);
            ext->unkB0 = 1;
        }
        m->Engine->sub->entity = 0x4100;
        m->Engine->unk1C += 0x200;
        if (ext->unk4C < 0) {
            RemoveCamera(ext->unk98);
        }
        if (ArcUpdate(ext->from, ext->vel, &ext->unk38, &m->Engine->unk10.x) == 0) {
            break;
        }
        goto kill;
    case 4:
        m->Engine->sub->entity = 0x4100;
        m->Engine->unk1C += 0x200;
        if (ArcUpdate(ext->from, ext->vel, &ext->unk38, &m->Engine->unk10.x) != 0) {
            s32 r;

            if (ext->unkB4 != 0) {
                m->Engine->unk10.y = 0;
                ext->unkC = 9;
                r = Random(2);
                if (r == 0) {
                    PlaySfx(0x228, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (r == 1) {
                    PlaySfx(0x229, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                if (r == 2) {
                    PlaySfx(0x22A, NULL, NULL, 0x1000, 0x1E00, 0);
                }
                ext->unk4C = -((ext->unk4C * 0xAA) >> 8);
                return;
            }
            GotoLinkAnim((EXTanimator *)ext->cursor, 2);
            ext->unkC = 5;
            r = Random(2);
            if (r == 0) {
                PlaySfx(0x228, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 1) {
                PlaySfx(0x229, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 2) {
                PlaySfx(0x22A, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (GameTeams[0] != 0) {
                ext->unkA0 = r;
            } else {
                ext->unkA0 = 0;
            }
            m->Engine->unk1C = 0;
            fux->sub->flags |= 0x8000;
            m->Engine->unk10.y = 0;
            return;
        }
        break;
    case 5: {
        HeroInfo *hi;
        EXTengine *h;
        NAS *n;

        ((PolarEaseCam *)ext->unkAC)->x.cur = 0;
        ((PolarEaseCam *)ext->unkAC)->z.cur = 0;
        ((PolarEaseCam *)ext->unkAC)->x.target = 0;
        ((PolarEaseCam *)ext->unkAC)->z.target = 0;
        if (ext->unk1C >= 7) {
            if (ext->unkA4-- < 0) {
                PlaySfx(0x23C, NULL, NULL, 0x1000, 0x1E00, 0);
                ext->unkA4 = ((0xF - (0x5A - ext->unk1C) * 3) / 90 + 3) * 2;
            }
        }
        ext->unkA8++;
        if (ext->unk1C == 6) {
            PlaySfx(0x23D, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (((PolarBearView *)GameInfo)->unk6C != 0) {
            HeroInfo *hb = Heroes;

            hi = &hb[((PolarBearView *)GameInfo)->lead];
            if (hi->hero_status & 0x2000) {
                ext->unkA0 = hi->hero_teammate;
            }
            if (hb[hb[((PolarBearView *)GameInfo)->lead].hero_teammate].hero_status & 0x2000) {
                ext->unkA0 = ((PolarBearView *)GameInfo)->lead;
            }
            hi = &hb[((PolarBearView *)GameInfo)->lead];
            if ((hi->hero_status & 0x2000) && (hb[hi->hero_teammate].hero_status & 0x2000)) {
                ext->unkC = 8;
                GotoLinkAnim((EXTanimator *)ext->cursor, 3);
                return;
            }
        } else if (Heroes[0].hero_status & 0x2000) {
            ext->unkC = 8;
            GotoLinkAnim((EXTanimator *)ext->cursor, 3);
        }
        h = Heroes[ext->unkA0].hero_eng;
        ext->unk18 = (get_angle(h->unk10.x - m->Engine->unk10.x, h->unk10.z - m->Engine->unk10.z) + 0x800) & 0xFFF;
        ext->unk9C->sub->flags |= 0x8000;
        ext->unk9C->unk10 = m->Engine->unk10;
        ext->unk9C->unk10.y -= 0x19A;
        ext->unk9C->unk5E = 1;
        m->Engine->unk1E += ((ext->unk18 - m->Engine->unk1E) << 20) >> 22;
        ext->unk9C->unk1E = m->Engine->unk1E;
        if (ext->unk1C-- < 0) {
            ext->unkC = 2;
            n = CreateNAS(m->Engine->unk10.x, m->Engine->unk10.y - 0x19A, m->Engine->unk10.z, 0x161C, 0x11);
            if (n != NULL) {
                n->Engine->unk1E = m->Engine->unk1E;
                ext->unk9C->sub->flags &= ~0x8000;
            }
            cam->x.target = 0;
            cam->z.target = 0;
            ext->unk1C = 0x5A;
            GotoLinkAnim((EXTanimator *)ext->cursor, 0);
        }
        break;
    }
    case 7: {
        EXTengine *e;

        e = m->Engine;
        if (e->unk10.z > 0) {
            e->sub->unk68 = 0x400;
        } else {
            e->sub->unk68 = 0x200;
        }
        m->Engine->sub->flags |= 0x02000000;
        fux->sub->flags &= ~0x8000;
        if (done != 0) {
        kill:
            PlaySfx(0x45D, NULL, NULL, 0x1000, 0x1E00, 0);
            KillNAS(ext->unk98);
            KillNAS(m);
            return;
        }
        break;
    }
    case 0: {
        HeroInfo *hi;
        EXTengine *h;
        s32 near;
        s32 r;
        s32 s;

        r = Random(3);
        s = ((EXTanimator *)ext->cursor)->Aindex;
        if (s == 1) {
            if (r == 0) {
                PlaySfx(0x230, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == s) {
                PlaySfx(0x231, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 2) {
                PlaySfx(0x232, NULL, NULL, 0x1000, 0x1E00, 0);
            }
        }
        if (BearminatorTarget(m) != 0) {
            goto fall;
        }
        if (ext->unk14-- < 0) {
            if (((PolarBearView *)GameInfo)->unk6C != 0) {
                HeroInfo *hb = Heroes;

                hi = &hb[((PolarBearView *)GameInfo)->lead];
                if (hi->hero_status & 0x2000) {
                    ext->unkA0 = hi->hero_teammate;
                }
                if (hb[hb[((PolarBearView *)GameInfo)->lead].hero_teammate].hero_status & 0x2000) {
                    ext->unkA0 = ((PolarBearView *)GameInfo)->lead;
                }
                hi = &hb[((PolarBearView *)GameInfo)->lead];
                if ((hi->hero_status & 0x2000) && (hb[hi->hero_teammate].hero_status & 0x2000)) {
                    ext->unkC = 8;
                    GotoLinkAnim((EXTanimator *)ext->cursor, 3);
                    return;
                }
            } else if (Heroes[0].hero_status & 0x2000) {
                GotoLinkAnim((EXTanimator *)ext->cursor, 3);
                ext->unkC = 8;
            }
            h = Heroes[ext->unkA0].hero_eng;
            ext->unk18 = (get_angle(h->unk10.x - m->Engine->unk10.x, h->unk10.z - m->Engine->unk10.z) + 0x800) & 0xFFF;
            ext->unk14 = 1;
        }
        m->Engine->unk1E += ((ext->unk18 - m->Engine->unk1E) << 20) >> 21;
        {
            s32 k;

            k = AiPlayerSkill[0].level + 0x20;
            cam->x.target = (k * rcossin_tbl[(m->Engine->unk1E - 0x400) & 0xFFF].unk2) >> 13;
            cam->z.target = (k * rcossin_tbl[m->Engine->unk1E & 0xFFF].unk2) >> 13;
        }
        if (Heroes[ext->unkA0].hero_mode == 0x4E) {
            ext->unkC = 6;
            return;
        }
        BearminatorEaseCam(cam, &m->Engine->unk10.x);
        if (ext->unk1C-- < 0) {
            hi = &Heroes[ext->unkA0];
            if (hi->hero_mode != 0x4E) {
                s32 dx;
                s32 dz;

                dx = m->Engine->unk10.x - hi->hero_eng->unk10.x;
                dz = m->Engine->unk10.z - hi->hero_eng->unk10.z;
                near = dx * dx + dz * dz < 0x40000;
                if (near != 0)
                if (near == 1) {
                    ext->unkC = near;
                    cam->x.target = cam->x.cur = (rcossin_tbl[(m->Engine->unk1E - 0x400) & 0xFFF].unk2 * 0x4B) >> 12;
                    cam->z.target = cam->z.cur = (rcossin_tbl[m->Engine->unk1E & 0xFFF].unk2 * 0x4B) >> 12;
                    GotoLinkAnim((EXTanimator *)ext->cursor, 1);
                    ext->unk1C = 0x5A - (AiPlayerSkill[0].level - 0x10) * 4;
                }
            }
        }
        break;
    }
    case 6: {
        EXTengine *e;
        EXTengine *h;
        s32 x;
        s32 z;
        s32 dx;
        s32 dz;
        s32 a;

        if (BearminatorTarget(m) != 0) {
        fall:
            ext->unkC = 7;
            PlaySfx(0x23E, NULL, NULL, 0x1000, 0x1E00, 0);
            GotoLinkAnim((EXTanimator *)ext->cursor, 4);
            return;
        }
        if (Heroes[0].hero_mode != 0x4E) {
            ext->unkC = 0;
            return;
        }
        if (ext->unk14-- < 0) {
            e = m->Engine;
            x = e->unk10.x;
            z = e->unk10.z;
            dx = -x;
            dz = -z;
            if (dx * dx + dz * dz > 0x400000) {
                a = (get_angle(dx, dz) + 0x800) & 0xFFF;
                ext->unk18 = a;
                m->Engine->unk1E += ((a - m->Engine->unk1E) << 20) >> 21;
                cam->x.target = (rcossin_tbl[(m->Engine->unk1E - 0x400) & 0xFFF].unk2 * 3) >> 9;
                cam->z.target = (rcossin_tbl[m->Engine->unk1E & 0xFFF].unk2 * 3) >> 9;
            } else {
                h = Heroes[ext->unkA0].hero_eng;
                dx = h->unk10.x;
                dz = h->unk10.z;
                cam->x.target = 0;
                cam->z.target = 0;
                ext->unk18 = (get_angle(dx - x, dz - z) + 0x800) & 0xFFF;
            }
            ext->unk14 = 1;
        }
        BearminatorEaseCam(cam, &m->Engine->unk10.x);
        break;
    }
    case 1: {
        HeroInfo *hi;
        EXTengine *h;
        s32 r;
        s32 s;

        r = Random(3);
        s = ((EXTanimator *)ext->cursor)->Aindex;
        if (s == 1) {
            if (r == 0) {
                PlaySfx(0x233, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == s) {
                PlaySfx(0x234, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (r == 2) {
                PlaySfx(0x235, NULL, NULL, 0x1000, 0x1E00, 0);
            }
        }
        if (BearminatorTarget(m) != 0) {
            ext->unkC = 7;
            GotoLinkAnim((EXTanimator *)ext->cursor, 4);
            return;
        }
        if (ext->unk14-- < 0) {
            if (((PolarBearView *)GameInfo)->unk6C != 0) {
                HeroInfo *hb = Heroes;

                hi = &hb[((PolarBearView *)GameInfo)->lead];
                if (hi->hero_status & 0x2000) {
                    ext->unkA0 = hi->hero_teammate;
                }
                if (hb[hb[((PolarBearView *)GameInfo)->lead].hero_teammate].hero_status & 0x2000) {
                    ext->unkA0 = ((PolarBearView *)GameInfo)->lead;
                }
                hi = &hb[((PolarBearView *)GameInfo)->lead];
                if ((hi->hero_status & 0x2000) && (hb[hi->hero_teammate].hero_status & 0x2000)) {
                    ext->unkC = 8;
                    GotoLinkAnim((EXTanimator *)ext->cursor, 3);
                    return;
                }
            } else if (Heroes[0].hero_status & 0x2000) {
                GotoLinkAnim((EXTanimator *)ext->cursor, 3);
                ext->unkC = 8;
            }
            h = Heroes[ext->unkA0].hero_eng;
            ext->unk18 = (get_angle(h->unk10.x - m->Engine->unk10.x, h->unk10.z - m->Engine->unk10.z) + 0x800) & 0xFFF;
            ext->unk14 = 1;
        }
        m->Engine->unk1E += ((ext->unk18 - m->Engine->unk1E) << 20) >> 21;
        cam->x.target = (rcossin_tbl[(m->Engine->unk1E - 0x400) & 0xFFF].unk2 * 0x4B) >> 12;
        cam->z.target = (rcossin_tbl[m->Engine->unk1E & 0xFFF].unk2 * 0x4B) >> 12;
        BearminatorEaseCam(cam, &m->Engine->unk10.x);
        if (done != 0) {
            ext->unkC = 0;
            GotoLinkAnim((EXTanimator *)ext->cursor, 0);
            return;
        }
        break;
    }
    case 2:
        ext->unkC = 0;
        break;
    }
    fux->sub->entity = m->Engine->sub->entity;
    fux->unk1E = m->Engine->unk1E;
    fux->unk10 = m->Engine->unk10;
    if (ext->unk98 != NULL) {
        ext->unk98->Engine->unk10 = m->Engine->unk10;
        ext->unk98->Engine->unk10.y = 0;
    }
    fux->unk5E = 1;
    m->Engine->unk5E = 1;
}

void PlotPolarMissile(NAS *m) {
    PolarMissileExt *ext;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    switch (ext->unk10) {
    case 0:
        ext->unk10 = 1;
        ext->pos = m->Engine->unk10;
        break;
    case 1:
        if (--ext->unk60 == -1) {
            PolarMissileBlast(m, 0);
            ext->unk60 = 1;
        }
        break;
    }
    ext->pos.x -= (ext->unk5C * rcossin_tbl[(m->Engine->unk1E - 0x400) & 0xFFF].unk2) >> 12;
    ext->pos.z -= (ext->unk5C * rcossin_tbl[(u16)m->Engine->unk1E & 0xFFF].unk2) >> 12;
    if (PolarMissileHitHero(&ext->pos) != -1) {
    blast:
        PolarMissileBlast(m, 1);
    } else {
        m->Engine->unk5E = 1;
        m->Engine->unk10 = ext->pos;
        if (ext->unk58 >= 3) {
            goto blast;
        }
        if (csqrt(m->Engine->unk10.x * m->Engine->unk10.x
                  + m->Engine->unk10.z * m->Engine->unk10.z) >> 6 <= 0xFA0) {
            return;
        }
        PolarMissileBlast(m, 1);
    }
    KillNAS(m);
}

void PlotBigMissile(NAS *m) {
    BigMissileArcExt *ext;
    PolarBossExt *boss;
    ExtSpawn5910 *sext;
    NAS *nas;
    NAS *res;
    SLONG_XYZ *pos;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    if (ext->state != 0) {
        return;
    }
    if (ArcUpdate(&ext->from.x, &ext->to.x, ext->arc, &m->Engine->unk10.x) == 0) {
        return;
    }
    m->Engine->unk10.y = 0x20;
    pos = &m->Engine->unk10;
    nas = CreateNAS(pos->x, pos->y, pos->z, 0x1605, 0x11);
    if (nas != NULL) {
        sext = FindExtension(nas, 0x10000000);
        if (sext != NULL) {
            sext->unk18 = 0x100;
            sext->unkC = 0;
            sext->unk20 = 1;
            sext->unk28 = 0;
            nas->InitDelete(nas);
        }
        res = nas;
    } else {
        res = NULL;
    }
    if (res != NULL) {
        res->Engine->flags |= 0x200000;
    }
    boss = FindExtension(polarbossnas, 0x10000000);
    if (boss != NULL) {
        boss->unk70 = 1;
        if (boss->phase == 2 && bossmapsub != NULL) {
            bossmapsub->entity = 0x2008;
        }
        if (boss->phase == 4 && bossmapsub != NULL) {
            bossmapsub->entity = 0x2009;
        }
        if (boss->phase == 6 && bossmapsub != NULL) {
            bossmapsub->entity = 0x200A;
        }
        if (boss->phase == 8 && bossmapsub != NULL) {
            bossmapsub->flags &= ~0x8000;
            if (!(Heroes[0].hero_mode & 0x2000)) {
                SetHeroAction(0, 0x4E);
            }
            if (GameTeams[0] != 0 && !(Heroes[1].hero_mode & 0x2000)) {
                SetHeroAction(1, 0x4E);
            }
        }
        boss->state = 5;
    }
    PlaySfx(0x22F, NULL, NULL, 0x1000, 0x1E00, 0);
    KillNAS(m);
}

void SniperAimUpdate(SniperAim *a, s32 *out) {
    s32 step;

    if (a->x.cur != a->x.target) {
        if (a->x.target != 0) {
            step = a->x.step;
        } else {
            step = a->x.stepZero;
        }
        if (a->x.cur < a->x.target) {
            a->x.cur += step;
            if (a->x.target < a->x.cur) {
                a->x.cur = a->x.target;
            }
        } else {
            a->x.cur -= step;
            if (a->x.cur < a->x.target) {
                a->x.cur = a->x.target;
            }
        }
    }
    if (a->y.cur != a->y.target) {
        if (a->y.target != 0) {
            step = a->y.step;
        } else {
            step = a->y.stepZero;
        }
        if (a->y.cur < a->y.target) {
            a->y.cur += step;
            if (a->y.target < a->y.cur) {
                a->y.cur = a->y.target;
            }
        } else {
            a->y.cur -= step;
            if (a->y.cur < a->y.target) {
                a->y.cur = a->y.target;
            }
        }
    }
    out[0] -= a->x.cur;
    out[1] -= a->y.cur;
}

void InitSniper(NAS *m) {
    SniperExt *ext;
    EXTengine *e;
    EXTengine *f;

    ext = AddExt(m, 0x9C, 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->entity = 0x2006;
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_RoboPolar];
        ext->unk8C = 8;
        ext->unk90 = 0;
        ext->unk98 = 0;
        ext->unkC = 0;
        ext->unk18 = 0;
        ext->unk1C = 0;
        ext->unk24 = 0;
        ext->unk28 = 0;
        ext->unk54 = 0;
        ext->unk74 = 0;
        ext->unk50 = 0;
        ext->unk70 = 0;
        ext->unk58 = 0xC;
        ext->unk78 = 0xC;
        ext->unk5A = 1;
        ext->unk7A = 1;
        ext->unk84 = 0;
        ext->unk88 = 0;
        m->Engine->unk2C.x = 0x1830;
        m->Engine->unk2C.y = 0x1830;
        m->Engine->unk2C.z = 0x1830;
        m->Engine->unk5E = 1;
        m->Engine->flags |= 0x200000;
        ext->unk10 = NULL;
        ext->unk20 = NULL;
        e = AddExtEngine(m, 2);
        if (e != NULL) {
            ext->unk10 = e;
            e->sub->u6C.dbase = MapScene->dbase;
            e->sub->entity = 0x2018;
            e->sub->flags = 0x22000000;
            e->sub->flags &= ~0x8000;
            e->unk10.x = 0;
            e->unk10.y = 0;
            e->unk10.z = 0;
            e->unk10.x = 0;
            e->unk10.y = 0;
            e->unk10.z = 0;
            e->sub->unk68 = -0x600;
        }
        e = AddExtEngine(m, 3);
        if (e != NULL) {
            ext->unk20 = e;
            e->sub->u6C.dbase = MapScene->dbase;
            e->sub->entity = 0x2019;
            e->sub->flags = 0x22000000;
            e->sub->flags &= ~0x8000;
            e->unk10.x = 0;
            e->unk10.y = 0;
            e->unk10.z = 0;
            e->unk10.x = 0;
            e->unk10.y = 0;
            e->unk10.z = 0;
            e->sub->unk68 = -0x3F4;
        }
        f = AddExtEngine(m, 4);
        if (f != NULL) {
            ext->unk94 = f;
            f->sub->u6C.dbase = &HeroModelBank[DB_RoboPolar];
            f->sub->entity = 0x2006;
            f->sub->flags |= 0x02000000;
            f->unk10 = m->Engine->unk10;
            f->unk5D = 0;
            f->flags |= 0x200000;
            f->sub->flags = 0x40008021;
            f->unk1E = m->Engine->unk1E;
            f->unk20 = -0x800;
            f->unk2C.x = 0x1830;
            f->unk2C.y = 0x1830;
            f->unk2C.z = 0x1830;
            f->sub->unk76 = 0xC00;
            f->unk5E = 1;
        }
    }
}

void PlotSniper(NAS *m) {
    SniperExt *ext;
    PolarBossExt *boss;
    subengine *sub;
    SniperMissileExt *mext;
    NAS *nas;
    _sfx_handler *sfx;
    s32 i;
    s32 dx;
    s32 dy;
    s32 y;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    boss = FindExtension(polarbossnas, 0x10000000);
    if (boss != NULL) {
        m->Engine->unk5E = 1;
        switch (ext->unkC) {
        case 0:
            if (ext->unk98 < 3) {
                m->Engine->unk10.y += ext->unk90;
                ext->unk90 += ext->unk8C;
                y = m->Engine->unk10.y;
                if (y >= -0x1F) {
                    m->Engine->unk10.y -= abs(y - 0x20);
                    ext->unk98++;
                    ext->unk90 = -(ext->unk90 >> 1);
                }
            }
            m->Engine->unk1E += 0x40;
            if (++ext->unk1C == 0x96) {
                boss->unk8C = 0xC;
                PolarMissileBlast(m, 1);
                PlaySfx(0x43D, NULL, NULL, 0x1000, 0x1E00, 0);
                KillNAS(m);
                return;
            }
            if (ext->unk98 > 0) {
                for (i = 0;; i++) {
                    EXTengine *e;
                    EXTengine *he;

                    if (i >= 2) {
                        break;
                    }
                    e = m->Engine;
                    he = Heroes[i].hero_eng;
                    if ((he->unk10.x - e->unk10.x) * (he->unk10.x - e->unk10.x)
                        + (he->unk10.z - e->unk10.z) * (he->unk10.z - e->unk10.z) <= 0x23FFF) {
                        SNIPER_CAM->at.x = polarbossnas->Engine->unk10.x;
                        SNIPER_CAM->at.y = polarbossnas->Engine->unk10.y + 0x100;
                        SNIPER_CAM->at.z = polarbossnas->Engine->unk10.z;
                        SNIPER_CAM->eye.x = 0;
                        SNIPER_CAM->eye.y = 0;
                        ext->unk14 = SNIPER_CAM->eye.z;
                        SNIPER_CAM->eye.z = 0x1A80;
                        SNIPER_CAM->xform.angle.vx = 0xDC;
                        SNIPER_CAM->xform.angle.vy = 0;
                        SNIPER_CAM->xform.angle.vz = 0;
                        m->Engine->sub->flags &= ~0x8000;
                        ext->unk10->sub->flags |= 0x8000;
                        ext->unkC = 1;
                        CameraMode = 0;
                        ext->unk10->sub->unk04 = SNIPER_CAM->at.x;
                        ext->unk10->sub->unk08 = SNIPER_CAM->at.y - 0x600;
                        ext->unk10->sub->unk0C = SNIPER_CAM->at.z;
                        ext->unk20->sub->flags |= 0x8000;
                        ext->unkC = 1;
                        ext->unk20->sub->unk04 = SNIPER_CAM->at.x;
                        ext->unk20->sub->unk08 = SNIPER_CAM->at.y - 0x600;
                        ext->unk20->sub->unk0C = SNIPER_CAM->at.z;
                        ext->unk24 = -0x5DC;
                        ext->unk28 = 0x400;
                        PlaySfx(0x23F, NULL, NULL, 0x1000, 0x1E00, 0);
                        PlaySfx(0x240, &m->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
                        if (!(Heroes[0].hero_mode & 0x2000)) {
                            SetHeroAction(0, 0x4D);
                            Heroes[0].hero_sub->flags &= ~0x8000;
                        }
                        if (GameTeams[0] != 0 && !(Heroes[1].hero_mode & 0x2000)) {
                            SetHeroAction(1, 0x4D);
                            Heroes[1].hero_sub->flags &= ~0x8000;
                        }
                        ext->unk88 = i;
                        return;
                    }
                }
            }
            break;
        case 1:
            ext->unk10->sub->unk04 = SNIPER_CAM->at.x;
            ext->unk54 = 0;
            ext->unk74 = 0;
            ext->unk18++;
            if (SNIPER_PAD[ext->unk88].unk50 & 0x80) {
                ext->unk54 = 0xC4;
            }
            if (SNIPER_PAD[ext->unk88].unk50 & 0x20) {
                ext->unk54 = -0xC4;
            }
            if (SNIPER_PAD[ext->unk88].unk50 & 0x10) {
                ext->unk74 = 0xC4;
            }
            if (SNIPER_PAD[ext->unk88].unk50 & 0x40) {
                ext->unk74 = -0xC4;
            }
            if (ext->unk84 == 0) {
                SniperAimUpdate((SniperAim *)&ext->pad2C[4], &ext->unk24);
                sub = ext->unk20->sub;
                dx = SNIPER_CAM->at.x - sub->unk04;
                dy = SNIPER_CAM->at.y - sub->unk08;
                if (dx < -0x700) {
                    SNIPER_CAM->at.x += 0x41;
                    i = abs(ext->unk50);
                    ext->unk50 = 0;
                    ext->unk24 -= i;
                }
                if (dx > 0x700) {
                    SNIPER_CAM->at.x -= 0x41;
                    i = abs(ext->unk50);
                    ext->unk50 = 0;
                    ext->unk24 += i;
                }
                if (dy > 0xAF0) {
                    i = abs(ext->unk70);
                    ext->unk70 = 0;
                    ext->unk28 += i;
                }
                if (dy < 0x190) {
                    i = abs(ext->unk70);
                    ext->unk70 = 0;
                    ext->unk28 -= i;
                }
                if (SNIPER_CAM->at.x < -0x1000) {
                    SNIPER_CAM->at.x = -0x1000;
                }
                if (SNIPER_CAM->at.x > 0x1000) {
                    SNIPER_CAM->at.x = 0x1000;
                }
                ext->unk20->sub->unk04 = SNIPER_CAM->at.x + ext->unk24;
                {
                    s32 ay = ext->unk28 - 0x600;
                    ext->unk20->sub->unk08 = SNIPER_CAM->at.y + ay;
                }
                if (ext->unk84 == 0 && (SNIPER_PAD[ext->unk88].unk54 & 0x8000)) {
                    ext->unk84 = 1;
                    ext->unk18 = 0xD2;
                    nas = CreateNAS(Heroes[ext->unk88].hero_eng->unk10.x,
                                       Heroes[ext->unk88].hero_eng->unk10.y,
                                       Heroes[ext->unk88].hero_eng->unk10.z, 0x1624, 0x11);
                    if (nas != NULL) {
                        mext = FindExtension(nas, 0x10000000);
                        if (mext != NULL) {
                            mext->from = *(SLONG_XYZ *)&ext->unk20->sub->unk04;
                            mext->to = Heroes[ext->unk88].hero_eng->unk10;
                            mext->to.x = SNIPER_CAM->at.x;
                            mext->to.y = SNIPER_CAM->at.y - 0x700;
                            mext->to.z = SNIPER_CAM->at.z - SNIPER_CAM->eye.z + 0x300;
                            nas->Engine->unk10 = mext->to;
                        }
                    }
                }
            }
            if (ext->unk18 == 0x10E) {
                SNIPER_CAM->at.x = 0;
                SNIPER_CAM->at.y = 0;
                SNIPER_CAM->at.z = 0;
                SNIPER_CAM->eye.z = ext->unk14;
                if (!(Heroes[0].hero_mode & 0x2000)) {
                    SetHeroAction(0, 0x49);
                    Heroes[0].hero_sub->flags |= 0x8000;
                }
                if (GameTeams[0] != 0 && !(Heroes[1].hero_mode & 0x2000)) {
                    SetHeroAction(1, 0x49);
                    Heroes[1].hero_sub->flags |= 0x8000;
                }
                sfx = m->Engine->sfx;
                CameraMode = 1;
                if (sfx != NULL) {
                    StopSfx(sfx);
                }
                boss->unk8C = 0xC;
                KillNAS(m);
                return;
            }
            break;
        }
    }
    if (ext->unk94 != NULL) {
        ext->unk94->unk10 = m->Engine->unk10;
        ext->unk94->unk10.y = -m->Engine->unk10.y + 0x40;
        ext->unk94->unk1E = m->Engine->unk1E;
        ext->unk94->unk5E = 1;
    }
}

void PlotSniperMissile(NAS *m) {
    SniperMissileExt *ext;
    PolarBossExt *boss;
    s32 *anim;
    s32 *gun;
    s32 dx, dz, dy, dist;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    boss = FindExtension(polarbossnas, 0x10000000);
    if (boss == NULL) {
        return;
    }
    m->Engine->unk5E = 1;
    switch (ext->unkC) {
    case 0:
        dx = ext->to.x - ext->from.x;
        dz = ext->to.z - ext->from.z;
        dy = ext->to.y - ext->from.y;
        dist = csqrt(dx * dx + dz * dz) >> 6;
        ext->yaw = get_angle(dx, dz) & 0xFFF;
        ext->pitch = get_angle(dy, dist) & 0xFFF;
        ext->unkC = 1;
        m->Engine->unk1E = ext->yaw;
        m->Engine->unk1C = -ext->pitch;
        break;
    case 1:
        ext->unk34++;
        m->Engine->unk10.x -= rcossin_tbl[(ext->yaw - 0x400) & 0xFFF].unk2 * 0x100 >> 12;
        m->Engine->unk10.z -= rcossin_tbl[ext->yaw & 0xFFF].unk2 * 0x100 >> 12;
        m->Engine->unk10.y -= rcossin_tbl[(ext->pitch - 0x400) & 0xFFF].unk2 * 0x100 >> 12;
        if (--ext->unk30 == -1) {
            PolarMissileBlast(m, 0);
            ext->unk30 = 1;
        }
        if (XZBoxToSphere(polarbossnas->Engine, m->Engine, 0) != 0 || ext->unk34 == 0x3C) {
            PolarMissileBlast(m, 1);
            PlaySfx(0x22F, NULL, NULL, 0x1000, 0x1E00, 0);
            boss->unk8C = 0xC;
            anim = FindExtension(boss->unk84, 0x10000000);
            if (anim != NULL) {
                GotoLinkAnim((EXTanimator *)anim[3], 8);
                PlaySfx(0x23B, NULL, NULL, 0x1000, 0x1E00, 0);
            }
        } else {
            if (XZBoxToSphere(boss->unk84->Engine, m->Engine, 1) == 0) {
                return;
            }
            PolarMissileBlast(m, 1);
            anim = FindExtension(boss->unk84, 0x10000000);
            if (anim != NULL) {
                GotoLinkAnim((EXTanimator *)anim[3], 3);
            }
            boss->unk80 = 1;
            boss->unk8C = 0xC;
            GotoLinkAnim((EXTanimator *)boss->cursor, 9);
            PlaySfx(0x22F, NULL, NULL, 0x1000, 0x1E00, 0);
            PlaySfx(0x23A, NULL, NULL, 0x1000, 0x1E00, 0);
            BossState.health -= 7;
            gun = FindExtension(Heroes[3].hero, 0x20000000);
            if (gun != NULL) {
                gun[8] -= 0x55;
            }
            if (BossState.health <= 0) {
                BossState.health = 0;
                BossState.flags |= 0x8000;
                anim = FindExtension(boss->unk84, 0x10000000);
                if (anim != NULL) {
                    GotoLinkAnim((EXTanimator *)anim[3], 5);
                    boss->unk84->Engine->sub->unk68 = 0x238;
                    boss->state = 8;
                }
            }
        }
        KillNAS(m);
        break;
    }
}

s32 XZBoxToSphere(EXTengine *a, EXTengine *b, s32 c) {
    VECTOR d;
    VECTOR n;
    MATRIX m;
    SVECTOR ang;
    SVECTOR sv[2];
    PolarBoxHalf lo;
    PolarBoxHalf hi;
    PolarCollBox *src;
    SVECTOR *p;
    s32 dist;
    s32 side;

    dist = 0x7FFFFFF;
    side = 0;
    src = (PolarCollBox *)SubFindFlagPoint(a->sub, c, side);
    if (src == NULL) {
        printf("No Coll box on subengine in XZBoxToSphere\n");
        return 0;
    }
    p = (SVECTOR *)SubFindFlagPoint(b->sub, c, 0x80000000);
    if (p == NULL) {
        p = sv;
        p->pad = 0x100;
        sv[0].vx = 0;
        p->vy = 0;
        p->vz = 0;
    }
    lo = src->lo;
    hi = src->hi;
    if (a->unk5C == 1) {
        lo.v[0] = (lo.v[0] * a->unk2C.x) >> 12;
        lo.v[1] = (lo.v[1] * a->unk2C.y) >> 12;
        lo.v[2] = (lo.v[2] * a->unk2C.z) >> 12;
        hi.v[0] = (hi.v[0] * a->unk2C.x) >> 12;
        hi.v[1] = (hi.v[1] * a->unk2C.y) >> 12;
        hi.v[2] = (hi.v[2] * a->unk2C.z) >> 12;
    }
    if (a->sub->unk50 == 1) {
        lo.v[0] = (lo.v[0] * a->sub->unk20.x) >> 12;
        lo.v[1] = (lo.v[1] * a->sub->unk20.y) >> 12;
        lo.v[2] = (lo.v[2] * a->sub->unk20.z) >> 12;
        hi.v[0] = (hi.v[0] * a->sub->unk20.x) >> 12;
        hi.v[1] = (hi.v[1] * a->sub->unk20.y) >> 12;
        hi.v[2] = (hi.v[2] * a->sub->unk20.z) >> 12;
    }
    d.vx = b->unk10.x - a->unk10.x;
    d.vy = b->unk10.y - a->unk10.y;
    d.vz = b->unk10.z - a->unk10.z;
    ang.vx = -a->unk1C;
    ang.vy = -a->unk1E;
    ang.vz = -a->unk20;
    RotMatrixZYX(&ang, &m);
    ApplyMatrixLV(&m, &d, &d);
    if (d.vx < lo.v[0]) {
        side = 2;
    }
    if (d.vy < hi.v[1]) {
        side |= 0x10;
        if (GameMode == 0x16) {
            return 0;
        }
    }
    if (lo.v[1] < d.vy && GameMode == 0x16) {
        return 0;
    }
    if (d.vz < lo.v[2]) {
        side |= 4;
    }
    if (hi.v[0] < d.vx) {
        side |= 1;
    }
    if (hi.v[2] < d.vz) {
        side |= 8;
    }
    n.vy = 0;
    switch (side) {
    case 20:
        dist = abs(d.vy - hi.v[1]);
        n.vx = 0;
        n.vy = -0x1000;
        n.vz = 0;
        break;
    case 22:
        dist = abs(d.vy - hi.v[1]);
        n.vx = 0;
        n.vy = -0x1000;
        n.vz = 0;
        break;
    case 21:
        dist = abs(d.vy - hi.v[1]);
        n.vx = 0;
        n.vy = -0x1000;
        n.vz = 0;
        break;
    case 4:
        dist = abs(d.vz - lo.v[2]);
        n.vx = 0;
        n.vz = -0x1000;
        break;
    case 6:
        dist = csqrt((d.vx - lo.v[0]) * (d.vx - lo.v[0]) + (d.vz - lo.v[2]) * (d.vz - lo.v[2])) >> 6;
        n.vx = -0x1000;
        n.vz = -0x1000;
        break;
    case 5:
        dist = csqrt((d.vx - hi.v[0]) * (d.vx - hi.v[0]) + (d.vz - lo.v[2]) * (d.vz - lo.v[2])) >> 6;
        n.vx = 0x1000;
        n.vz = -0x1000;
        break;
    case 24:
        dist = abs(d.vy - hi.v[1]);
        n.vx = 0;
        n.vy = -0x1000;
        n.vz = 0;
        break;
    case 26:
        dist = abs(d.vy - hi.v[1]);
        n.vx = 0;
        n.vy = -0x1000;
        n.vz = 0;
        break;
    case 25:
        dist = abs(d.vy - hi.v[1]);
        n.vx = 0;
        n.vy = -0x1000;
        n.vz = 0;
        break;
    case 8:
        dist = abs(d.vz - hi.v[2]);
        n.vx = 0;
        n.vz = 0x1000;
        break;
    case 10:
        dist = csqrt((d.vx - lo.v[0]) * (d.vx - lo.v[0]) + (d.vz - hi.v[2]) * (d.vz - hi.v[2])) >> 6;
        n.vx = -0x1000;
        n.vz = 0x1000;
        break;
    case 9:
        dist = csqrt((d.vx - hi.v[0]) * (d.vx - hi.v[0]) + (d.vz - hi.v[2]) * (d.vz - hi.v[2])) >> 6;
        n.vx = 0x1000;
        n.vz = 0x1000;
        break;
    case 2:
        dist = abs(d.vx - lo.v[0]);
        n.vx = -0x1000;
        n.vz = 0;
        break;
    case 16:
        dist = abs(d.vy - hi.v[1]);
        n.vx = 0;
        n.vy = -0x1000;
        n.vz = 0;
        break;
    case 1:
        dist = abs(d.vx - hi.v[0]);
        n.vx = 0x1000;
        n.vz = 0;
        break;
    }
    if (dist < p->pad) {
        s32 push;

        push = abs(p->pad - dist);
        ang.vx = a->unk1C;
        ang.vy = a->unk1E;
        ang.vz = a->unk20;
        RotMatrix(&ang, &m);
        ApplyMatrixLV(&m, &n, &n);
        if (c == 0) {
            b->unk10.x += (push * n.vx) >> 12;
            b->unk10.z += (push * n.vz) >> 12;
        }
        return 1;
    }
    return 0;
}

void InitSniperMissile(NAS *m) {
    SniperMissileExt *ext;

    ext = AddExt(m, 0x38, 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->entity = 0x2006;
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_RoboPolar];
        ext->unkC = 0;
        m->Engine->unk5C = 1;
        m->Engine->unk2C.x = 0x2AF8;
        m->Engine->unk2C.y = 0x2AF8;
        m->Engine->unk2C.z = 0x2AF8;
        m->Engine->unk5D = 2;
        ext->unk30 = 0;
        ext->unk34 = 0;
        PlaySfx(0x522, &m->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
    }
}

void InitBigMissile(NAS *m) {
    BigMissileExt *ext;

    ext = AddExt(m, 0x7C, 0x10000000);
    if (ext != NULL) {
        m->Engine->unk5D = 2;
        m->Engine->unk5C = 1;
        m->Engine->sub->entity = 0x201A;
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
        m->Engine->unk2C.x = 0x2000;
        m->Engine->unk2C.y = 0x2000;
        m->Engine->unk2C.z = 0x2000;
        m->Engine->unk5E = 1;
        ext->unkC = 0x10;
        ext->unk50 = 0x96;
        ext->unk10 = 0;
        ext->unk24 = Heroes[0].hero_eng->unk10.y;
        ext->unk78 = 0;
        ext->pos = m->Engine->unk10;
        PlaySfx(0x22D, NULL, NULL, 0x1000, 0x1E00, 0);
        PlaySfx(0x22B, NULL, NULL, 0x1000, 0x1E00, 0);
    }
}

void PlotBossTransmuter(NAS *m) {
}

void InitBossTransmuter(NAS *m) {
}

s32 CountBears(void) {
    u8 *p;
    s32 n;

    n = 0;
    for (p = PolarBears; p != NULL; p = *(u8 **)(p + 0x20)) {
        n++;
    }
    return n;
}

void InitPolarMissile(NAS *m) {
    PolarMissileExt *ext;

    ext = AddExt(m, 0x70, 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->entity = 0x2006;
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_RoboPolar];
        m->Engine->unk5D = 2;
        m->Engine->unk5C = 1;
        m->Engine->sub->unk51 = 2;
        m->Engine->sub->flags |= 0x02000000;
        ext->unk5C = 0x8C;
        ext->unk2C = 0x3000;
        ext->unk38 = 0x700;
        ext->unk4C = 4;
        ext->unk10 = 0;
        ext->unk18 = 0;
        ext->unk58 = 0;
        ext->unk44 = 0;
        ext->unk48 = 0x100;
        ext->unk54 = 0;
        ext->unk60 = 0;
        ext->unk50 = ext->unk2C;
        PlaySfx(0x522, &m->Engine->sfx, &m->Engine->unk10, 0x1000, 0x1E00, 0);
    }
}

s32 PolarBossDazeHero(u8 *arg0) {
    HeroInfo *hero;
    u32 i;
    u32 n;

    n = 1;
    if (GameTeams[0] != 0) {
        n = 2;
    }
    i = 0;
    hero = Heroes;
loop:
    if (i < n) {
        if (!(hero->hero_mode & 0x2000) &&
            DistXZSq(arg0, &hero->hero_eng->unk10) < 0x7BC4) {
            if (hero->hero_mode != 0x4B) {
                SetHeroAction(i, 0x4B);
            }
            PlaySfx(0x43D, NULL, NULL, 0x1000, 0x1E00, 0);
            return i;
        }
        hero++;
        i++;
        goto loop;
    }
    return -1;
}

void PolarBossFace(NAS *m) {
    u8 *p;
    EXTengine *e;
    s32 dx;
    s32 dz;
    s32 d;
    s32 ang;
    TrigEntry *t;

    p = PolarBears;
    t = rcossin_tbl;
loop:
    if (p != NULL) {
        e = *(EXTengine **)(p + 0x54);
        if (e != NULL && e != m->Engine) {
            dx = e->unk10.x - m->Engine->unk10.x;
            dz = e->unk10.z - m->Engine->unk10.z;
            if (dx * dx + dz * dz < 0x3D090) {
                d = 0x7D00 - csqrt(dx * dx + dz * dz);
                ang = get_angle(dx, dz) & 0xFFF;
                m->Engine->unk10.x -= ((d + 2) * t[(ang - 0x400) & 0xFFF].unk2) >> 18;
                m->Engine->unk10.z -= ((d + 2) * t[ang].unk2) >> 18;
            }
        }
        p = *(u8 **)(p + 0x20);
        goto loop;
    }
}

s32 PolarBossNear(EXTengine *eng, s32 player) {
    HeroInfo *base;
    HeroInfo *h;
    EXTengine *e;
    s32 dx;
    s32 dz;

    base = Heroes;
    h = base + player;
    if (h->hero_mode != 0x4E) {
        e = h->hero_eng;
        dx = eng->unk10.x - e->unk10.x;
        dz = eng->unk10.z - e->unk10.z;
        if (dx * dx + dz * dz < 0x40000) {
            return 1;
        }
    }
    return 0;
}

void PlotBearCam(NAS *m) {
}

void InitBearCam(NAS *m) {
}

void PlotRockProjectile(NAS *m) {
}

void InitRockProjectile(NAS *m) {
}

s32 PolarBossSpeedUp(void) {
    u8 *g = GameInfo;
    s32 k;

    if (g[0xC1] < 10) {
        return 0;
    }
    k = 0xB8;
    if (g[0xC1] >= 30) {
        return 0xE66;
    }
    return (g[0xC1] - 9) * k;
}

void PlotBearBoss(NAS *m) {
    if (FindExtension(m, 0x10000000) != NULL) {
        m->Engine->unk5E = 1;
        RunNasAnim(m);
    }
}

void InitBearBoss(NAS *m) {
    Ext10000000 *ext;

    ext = AddExt(m, 0x10, 0x10000000);
    if (ext != NULL) {
        m->Engine->unk5D = 0;
        m->Engine->sub->unk68 = 0x300;
        m->Engine->sub->flags |= 0x02000000;
        m->Engine->sub->u6C.dbase = MapScene->dbase;
        ext->unkC = (s32)ScriptCursorFirst(m, &BearBossScriptInit);
        GotoLinkAnim((EXTanimator *)ext->unkC, 0);
        RunNasAnim(m);
    }
}

void PlotPackEffect(NAS *m) {
    ExtSimpleExplosion *ext;
    subengine *sub;
    s32 i;

    ext = FindExtension(m, 0x10000000);
    if (ext != NULL) {
        sub = m->Engine->sub;
        ext->unk104++;
        if (sub != NULL) {
            i = 0;
            do {
                sub->unk04 += ext->vel[i].x;
                sub->unk08 += ext->vel[i].z;
                sub->unk0C += ext->vel[i].y;
                sub->unk76 += 0x100;
                sub->unk20.x -= 0x80;
                sub->unk20.y -= 0x80;
                sub->unk20.z -= 0x80;
                sub = sub->next;
                i++;
            } while (sub != NULL);
        }
        if (ext->unk104 == 0x2D) {
            KillNAS(m);
        }
    }
}

void PlotCortexPack(NAS *m) {
    m->Engine->unk5E = 1;
}

void InitCortexPack(NAS *m) {
    m->Engine->sub->u6C.dbase = &HeroModelBank[DB_CharsPolar2Dingo];
    m->Engine->sub->entity = 0x2003;
    m->Engine->unk5D = 2;
    m->Engine->unk5C = 1;
    m->Engine->sub->flags |= 0x40000021;
    m->Engine->sub->unk76 = 0;
}

void InitBrioPack(NAS *m) {
    m->Engine->flags |= 0x200000;
    m->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
    m->Engine->sub->entity = 0x2031;
    m->Engine->unk5D = 2;
    m->Engine->unk5C = 1;
    m->Engine->sub->flags |= 0x40000021;
    m->Engine->sub->unk76 = 0;
    m->UserInfo.user2 = 0;
    m->UserInfo.user1 = 0;
}

void InitDingoTail(NAS *m) {
    m->Engine->flags |= 0x200000;
    m->Engine->sub->u6C.dbase = (s32)&HeroModelBank[DB_CharsPolar2Dingo];
    m->Engine->sub->entity = 0x2003;
    m->Engine->unk5D = 2;
    m->Engine->unk5C = 1;
    m->Engine->sub->flags |= 0x40000021;
    m->Engine->sub->unk76 = 0;
}

void PlotSimpleExplosion(NAS *m) {
    ExtSimpleExplosion *ext;
    subengine *sub;
    s32 i;

    ext = FindExtension(m, 0x10000000);
    if (ext != NULL) {
        sub = m->Engine->sub;
        ext->unk104++;
        if (sub != NULL) {
            i = 0;
            do {
                sub->unk04 += ext->vel[i].x;
                sub->unk08 += ext->vel[i].z;
                sub->unk0C += ext->vel[i].y;
                sub->unk76 += 0x100;
                sub->unk20.x -= 0x80;
                sub->unk20.y -= 0x80;
                sub->unk20.z -= 0x80;
                sub = sub->next;
                i++;
            } while (sub != NULL);
        }
        if (ext->unk104 == 0x2D) {
            KillNAS(m);
        }
    }
}

void InitSmokeTrail(NAS *m) {
    ExtSmokeTrail *ext;

    ext = AddExt(m, 0x30, 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
        m->Engine->sub->entity = 0x2020;
        m->Engine->sub->flags |= 0x40000041;
        m->Engine->sub->unk76 = 0x1000;
        m->UserInfo.status = 0;
        ext->fadeIn = 0x400;
        ext->fadeOut = 0xC8;
        ext->shrink = 0x60;
        ext->flags = 0;
        ext->baseX = m->Engine->unk10.x;
        m->Engine->unk5C = 1;
        m->Engine->unk5D = 0;
        ext->angle = Random(0x1000);
    }
}

void PlotBearDeath(NAS *m) {
    BearDeathExt *ext;

    ext = FindExtension(m, 0x10000000);
    if (ext != NULL) {
        RunNasAnim(m);
        m->Engine->unk1C += 0x100;
        m->Engine->unk5E = 1;
        if (ext->unk40 > 0) {
            m->Engine->sub->unk68 = 0x200;
        }
        if (++ext->unk80 == 0x1F) {
            RemoveCamera(m);
        }
        if (--ext->unk84 < 0) {
            CreateNAS(m->Engine->unk10.x, m->Engine->unk10.y, m->Engine->unk10.z, 0x1612, 0x11);
            ext->unk84 = Random(7) + 1;
        }
        if (ArcUpdate(&ext->from.x, &ext->to.x, &ext->unk2C, &m->Engine->unk10.x) != 0) {
            KillNAS(m);
        }
    }
}

void InitBearDeath(NAS *m) {
    BearDeathExt *ext;

    ext = AddExt(m, 0x88, 0x10000000);
    if (ext != NULL) {
        ScriptCursorFirst(m, &BearDeathScriptInit);
        m->Engine->sub->u6C.dbase = ((s32)MapScene->dbase);
        m->Engine->unk5D = 2;
        m->Engine->sub->flags |= 0x02000000;
        ext->unk2C = 2;
        ext->unk70 = 0x32;
        ext->unk44 = 0x100;
        ext->unk30 = 0;
        ext->unk80 = 0;
        ext->unk84 = 5;
        AddCamera(m);
    }
}

void RiserSetReset(PolarRiserSet *arg0) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (arg0->slot[i].active != 0 && arg0->slot[i].height < 0x74) {
            arg0->slot[i].height += 4;
            UkaMaskRisers[i]->y = arg0->slot[i].base + arg0->slot[i].height;
        }
    }
}

void PlotBreathMaster(NAS *m) {
}

void InitBreathMaster(NAS *m) {
}

void PlotSnowMaster(NAS *m) {
}

void InitSnowMaster(NAS *m) {
}

void PolarInitDebug(void) {
    s32 i;
    SceneObj *obj;
    SceneObj **out;

    obj = ((SceneObj *)MapScene->subs);
    for (i = 79; i >= 0; i--) {
        PolarIceObjects[i] = NULL;
    }
    i = 0;
    out = PolarIceObjects;
    while (1) {
        if (obj == NULL) {
            break;
        }
        if (obj->flags & 0x8000) {
            if (obj->model->colors != NULL) {
                u8 *p = entity_address(obj->index, (u8 *)obj->model->colors);
                if (p != NULL && *(s32 *)(p + 0x28) != 0) {
                    *out++ = obj;
                    i++;
                }
            }
            obj = obj->next;
        }
    }
    printf(PolarMsgFoundColSubs, i - 1);
}

void PlotIceBerg(NAS *arg0) {
    u8 *temp_a1;
    u8 *temp_v1;

    temp_a1 = (*(s32*)((u8*)(arg0) + 84));
    (*(s32*)((u8*)(temp_a1) + 16)) = (s32) ((*(s32*)((u8*)(temp_a1) + 16)) + ((s32) (*(s32*)((u8*)(arg0) + 100)) >> 8));
    temp_v1 = (*(s32*)((u8*)(arg0) + 84));
    if ((*(s32*)((u8*)(temp_v1) + 16)) >= 0x1AF5) {
        (*(s32*)((u8*)(temp_v1) + 16)) = -0x1C84;
    }
}

void InitScratchMaster(NAS *m) {
    EXTengine *eng;
    subengine *sub;

    if (eng_add_subs(m->Engine, 0x10) != 0) {
        eng = m->Engine;
        sub = eng->sub;
        eng->flags |= 0x200000;
        m->UserInfo.user1 = 0;
        m->UserInfo.user2 = 0;
        while (sub != NULL) {
            sub->flags = 0x40000000;
            sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
            sub->entity = 0x2002;
            sub->unk51 = 2;
            sub->unk14 = 0;
            sub->unk12 = 0;
            sub->unk10 = 0;
            sub->unk76 = 0x800;
            sub = sub->next;
        }
    }
}

void PolarMovesSwap(EXTheromove *a, EXTheromove *b) {
    s32 ma;
    s32 mb;
    s32 d;
    s32 q;
    s32 r;
    s32 total;

    ma = a->mass;
    mb = b->mass;
    if (ma > 0 && mb > 0) {
        d = -((a->x_vel - b->x_vel) << 8);
        q = (d * mb + ((ma * a->x_vel + mb * b->x_vel) << 8)) / (total = mb + ma);
        r = q - d;
        a->x_vel = q >> 8;
        b->x_vel = r >> 8;
        d = -((a->z_vel - b->z_vel) << 8);
        q = (d * mb + ((ma * a->z_vel + mb * b->z_vel) << 8)) / total;
        r = q - d;
        a->z_vel = q >> 8;
        b->z_vel = r >> 8;
    }
}

void PlotGravityObject(GravityObject *obj) {
    s32 target = obj->target;

    if (target != 0) {
        GravityMotionUpdate(obj, target);
    }
}

void InitGravityObject(NAS *m) {
    GravityMotion *g;
    s32 t;

    g = AddExt(m, 0x34, 0x10000000);
    if (g != NULL) {
        m->Engine->sub->entity = 0x200C;
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
        m->Engine->sub->flags |= 0x02000000;
        g->vx = (rcossin_tbl[0xE00].unk2 * 27) >> 11;
        t = rcossin_tbl[0x200].unk2;
        g->vy = -0x54;
        g->ax = 0;
        g->az = 0;
        g->ay = 2;
        g->flags = 0;
        g->vz = (t * 27) >> 11;
        g->floor = *(u16 *)&m->Engine->unk10.y + 0x400;
        m->FUX = (EXT *)g;
    }
}

void InitWeightDeath(NAS *m) {
    WeightDeathExt *ext;

    ext = AddExt(m, 0x28, 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
        m->Engine->sub->entity = 0x2003;
        ext->unk10 = 0x40;
        ext->unk14 = 8;
        ext->unk18 = 0;
        ext->unk20 = 0;
        ext->unk24 = 0;
        PlaySfx(0x453, NULL, NULL, 0x1000, 0x1E00, 0);
    }
}

s32 PolarOfficeBox(s32 i) {
    u8 *p;
    EXTengine *eng;
    EXTengine *e;
    HeroInfo *base;

    p = PolarBears;
    base = Heroes;
    eng = base[i].hero_eng;
    for (; p != NULL; p = *(u8 **)(p + 0x20)) {
        e = *(EXTengine **)(p + 0x54);
        if (e != NULL && e != eng) {
            if (PolarBoxOverlap(e, 1, base[i].hero_eng, 0) != 0) {
                return 1;
            }
        }
    }
    return 0;
}

void PolarCamReset(void) {
    MainCamera.at.x = -0x40;
    MainCamera.at.y = -0x340;
    MainCamera.at.z = -0x8E0;
    MainCamera.xform.angle.vx = 0xC6;
    MainCamera.xform.angle.vy = 0;
    MainCamera.xform.angle.vz = 0;
    MainCamera.eye.x = 0;
    MainCamera.eye.z = 0x7FF;
    MainCamera.eye.y = 0;
    MainCamera.dist = 0x10F;
}

void PolarEmptyStub3(void) {
}

void POLARtriggers(NASTrigger *arg0, s32 arg1) {
}

void FirstTriggersPOLAR(void) {
    u8 *var_v1;

    var_v1 = ((s32)MapScene->subs);
    if (var_v1 != NULL) {
        do {
            (*(s32*)((u8*)(var_v1) + 0)) = (s32) ((*(s32*)((u8*)(var_v1) + 0)) | 0x01000000);
            var_v1 = (*(s32*)((u8*)(var_v1) + 92));
        } while (var_v1 != NULL);
    }
}

void Polar_HeroBot(u32 player) {
    PolarWanderFux *fux;
    HeroInfo *base;
    HeroInfo *h;

    base = Heroes;
    h = base + player;
    fux = (PolarWanderFux *)h->hero->FUX;
    if (fux != NULL) {
        h->hero_ai->AIpad = 0;
        DoChooseNewAction(player);
        ((void (*)(s32))fux->func)(player);
        set_joypad((u8 *)&Pads[player], h->hero_ai->AIpad);
    }
}

void PolarAI_SetTarget(s32 player, s32 target) {
    PolarWanderFux *fux;
    s32 r;

    fux = (PolarWanderFux *)Heroes[player].hero->FUX;
    if (fux == NULL) {
        return;
    }
    if (TestForTeamMate(player, target) == 0) {
        return;
    }
    r = PolarAI_TeamChoice(player);
    if (r == -1) {
        fux->state = 5;
        fux->func = PolarEmptyStub6;
        return;
    }
    fux->target = r;
}

void PolarAI_Proximity(s32 player, s32 idx) {
    PolarProx *p;
    PolarMode *m;
    EXTengine *a;
    EXTengine *b;
    s32 dx;
    s32 dz;
    s32 mode;

    if (GameMode == 0x12) {
        p = PolarProximity;
        if (p != NULL) {
            m = p->unk58;
            if (m != NULL) {
                a = p->eng;
                b = Heroes[player].hero_eng;
                dx = a->unk10.x - b->unk10.x;
                dz = a->unk10.z - b->unk10.z;
                if (dx * dx + dz * dz <= 0xB0643) {
                    mode = m->mode;
                    if (mode == 1 || mode == 5) {
                        PolarAIFlags[idx] |= 0x100;
                    }
                }
            }
        }
    }
}

s32 PlayerActionCount(s32 state, s32 player) {
    PolarWanderFux *f;
    s32 i;
    s32 n;

    HeroInfo *h;

    n = 0;
    i = 0;
    h = Heroes;
loop:
    if (i < 4) {
        f = (PolarWanderFux *)h->hero->FUX;
        if (f != NULL && f->state == state) {
            n++;
        }
        h++;
        i++;
        goto loop;
    }
    return n;
}

s32 PolarAI_Never(void) {
    return 0;
}

s32 PolarAI_RandomPause(s32 player) {
    PolarWanderFux *fux;
    HeroInfo *h;
    HeroInfo *base;

    base = Heroes;
    h = base + player;
    fux = (PolarWanderFux *)h->hero->FUX;
    if (fux == NULL) {
        return 0;
    }
    if (Random(0x1000) >= PolarAIChances[h->hero_name + 7]) {
        return 0;
    }
    fux->state = 6;
    fux->func = PolarAI_AvoidMelt;
    return 1;
}

void PolarBeamLinkState(void) {
    PolarLink *l;

    if (PolarBeamOwner != NULL) {
        l = PolarBeamOwner->unk58;
        if (l != NULL) {
            if (PolarAIFlags[6] == 0 && l->unk10 != -1) {
                PolarAIFlags[6] = 1;
            }
            if (PolarAIFlags[6] == -1 && l->unk10 == PolarAIFlags[6]) {
                PolarAIFlags[6] = 0;
            }
        }
    }
}

void PolarEmptyStub4(void) {
}

void PolarEmptyStub5(void) {
}

void PolarAI_ClearState(s32 player) {
    PolarWanderFux *f;

    f = (PolarWanderFux *)Heroes[player].hero->FUX;
    if (f != NULL) {
        f->unk24 = 0x5A;
    }
}

u32 FindClosetPlayer(u32 self, s32 *dist) {
    SLONG_XYZ pos;
    s32 best;
    HeroInfo *h;
    HeroInfo *me;
    HeroInfo *base;
    s32 i;
    s32 d;
    s32 min;
    u16 st;

    best = -1;
    min = 0x07FFFFFF;
    i = 0;
    base = Heroes;
    me = &base[self];
    h = base;
loop:
    if (i < 4) {
        if (i != self &&!((st = h->hero_status) & 0x2000) && !(st & 0x8000)) {
            pos = h->hero_eng->unk10;
            d = DistXZSq((SLONG_XYZ *)&pos, (SLONG_XYZ *)&me->hero_eng->unk10);
            if (d < min) {
                min = d;
                do {
                    best = i;
                } while (0);
            }
        }
        do {
            h++;
        } while (0);
        i++;
        goto loop;
    }
    *dist = min;
    return best;
}

s32 PolarAI_AnyBusy(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (PolarAIFlags[i] & 8) {
            return 1;
        }
        if (PolarAIFlags[i] & 0x20) {
            return 1;
        }
    }
    return 1;
}

s32 PolarAI_NoneBusy(s32 arg0) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (i == arg0 || (PolarAIFlags[i] & 4)) {
            return 0;
        }
        if (PolarAIFlags[i] & 2) {
            return 0;
        }
    }
    return 1;
}

void PolarAI_Reset(s32 player) {
    PolarWanderFux *fux;

    fux = (PolarWanderFux *)Heroes[player].hero->FUX;
    if (fux != NULL) {
        fux->unk2C = 0;
        fux->unk3C = 0;
        fux->unk34 |= 1;
    }
}

void PolarAI_CountDead(s32 player) {
    PolarWanderFux *f;
    s32 j;
    s32 n;

    f = (PolarWanderFux *)Heroes[player].hero->FUX;
    if (f == NULL) {
        return;
    }
    for (n = j = 0; j < 4; j++) {
        if (Heroes[j].hero_status & 0x2000) {
            n++;
        }
        f->state = 4;
        f->func = PolarEmptyStub5;
        if (n >= 3) {
            return;
        }
    }
    f->unk3C = 1;
    f->unk24 = Random(0x3C) + 5;
}

void PolarAI_Timer1(s32 player) {
    PolarWanderFux *f;
    HeroInfo *h;
    HeroInfo *base;
    SLONG_XYZ v;
    s32 r;
    s32 spd;

    r = Random(0x1000);
    base = Heroes;
    h = &base[player];
    f = (PolarWanderFux *)h->hero->FUX;
    spd = 0x708;
    if (f != NULL) {
        v.x = (rcossin_tbl[r & 0xFFF].unk2 * spd) >> 12;
        v.z = (rcossin_tbl[(r - 0x400) & 0xFFF].unk2 * spd) >> 12;
        f->vel = v;
        f->pos = h->hero_eng->unk10;
    }
}

void PolarAI_Timer2(s32 player) {
    PolarWanderFux *fux;

    fux = (PolarWanderFux *)Heroes[player].hero->FUX;
    if (fux != NULL) {
        fux->unk24 = Random(0x168) + 0x5A;
    }
}

bool DoesPlayerHaveBomb(u32 arg0) {
    NAS *nas;
    BombMakerExt *ext;
    s32 one;

    nas = PolarBomb;
    one = 1;
loop:
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL && ext->unkC == one && ext->unk14 == arg0) {
            return 1;
        }
        nas = nas->Group.Child;
        goto loop;
    }
    return 0;
}

NAS *IsBombAvailable(void) {
    NAS *nas;
    Ext10000000 *ext;

    nas = PolarBomb;
loop:
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL && ext->unkC == 0) {
            return nas;
        }
        nas = nas->Group.Child;
        goto loop;
    }
    return NULL;
}

void PolarAI_Timer3(s32 player) {
    PolarWanderFux *fux;

    fux = (PolarWanderFux *)Heroes[player].hero->FUX;
    if (fux != NULL) {
        fux->unk24 = Random(0x168) + 0x5A;
    }
}

void PolarEmptyStub6(void) {
}

void PolarEmptyStub7(void) {
}

s32 PolarPadAngle(s32 arg0) {
    s32 var_a1;

    var_a1 = 0;
    if (arg0 & 0x80) {
        var_a1 = 0xC00;
    }
    if (arg0 & 0x20) {
        var_a1 = 0x400;
    }
    if (arg0 & 0x10) {
        var_a1 = 0;
    }
    if (arg0 & 0x40) {
        var_a1 = 0x800;
    }
    if ((arg0 & 0x90) == 0x90) {
        var_a1 = 0xE00;
    }
    if ((arg0 & 0xC0) == 0xC0) {
        var_a1 = 0xA00;
    }
    if ((arg0 & 0x30) == 0x30) {
        var_a1 = 0x200;
    }
    if ((arg0 & 0x60) == 0x60) {
        var_a1 = 0x600;
    }
    return var_a1;
}

s32 PolarCompareXZ(u8 *arg0, u8 *arg1) {
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a2;
    s32 temp_v0;
    s32 var_v1;

    temp_v0 = (*(s32*)((u8*)(arg0) + 0));
    temp_a0 = (*(s32*)((u8*)(arg1) + 0));
    var_v1 = (temp_a0 < temp_v0) << 7;
    if (temp_v0 < temp_a0) {
        var_v1 |= 0x20;
    }
    temp_a2 = (*(s32*)((u8*)(arg0) + 8));
    temp_a0_2 = (*(s32*)((u8*)(arg1) + 8));
    if (temp_a0_2 < temp_a2) {
        var_v1 |= 0x40;
    }
    if (temp_a2 < temp_a0_2) {
        var_v1 |= 0x10;
    }
    return var_v1;
}

void InitBombGFX(NAS *m) {
    BombGFXExt *ext;
    EXTengine *e;

    ext = AddExt(m, 0x1C, 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->entity = 0x2018;
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
        ext->unkC = 0;
        m->Engine->unk5C = 1;
        AddToGroup((NAS **)&PolarBomb, m);
        m->FUX = NULL;
        e = AddExtEngine(m, 2);
        if (e != NULL) {
            e->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
            e->sub->entity = 0x202F;
            e->unk5D = 0;
            e->unk10 = m->Engine->unk10;
            e->flags |= 0x8041;
            e->sub->unk76 = 0x200;
            e->sub->flags |= 0x40000000;
            m->FUX = (EXT *)e;
        }
    }
}

void PlotBombMaker(NAS *m) {
    BombMakerExt *ext;
    s32 x;
    s32 z;

    ext = FindExtension(m, 0x10000000);
    if (ext != NULL) {
        if (ext->unk18 < 0) {
            ext->unk18 = 0;
        }
        switch (ext->unkC) {
        case 0:
            if (ext->unk18 <= 0) {
                if (ext->timer-- < 0 && PolarClock == 0) {
                    ext->unkC = 2;
                    ext->timer = 0x5A;
                    ext->unk18++;
                }
            }
            break;
        case 2:
            x = Random(0x800) - Random(0x800);
            z = Random(0x800);
            CreateNAS(x, 0, z - Random(0x800), 0x160F, 0x11);
            PlaySfx(0x3FA, NULL, NULL, 0x1000, 0x1E00, 0);
            ext->unkC = 0;
            break;
        }
    }
}

void InitBombMaker(NAS *m) {
    BombMakerExt *ext;

    ext = AddExt(m, 0x1C, 0x10000000);
    if (ext != NULL) {
        ext->timer = 0xB4;
        ext->unkC = 0;
        ext->unk18 = 0;
        BombMaster = m;
    }
}

u32 PolarAngleOf(u8 *arg0) {
    return (u32) ((get_angle((*(s32*)((u8*)(arg0) + 0)), (*(s32*)((u8*)(arg0) + 8))) - 0x7D) & 0xFFF) >> 8;
}

bool DoNowt(EXTengine *eng) {
    return 0;
}

void POLARSetUp(void) {
    LevelSpecs.LevelPreInit = InitialisePOLARFirst;
    LevelSpecs.LevelInit = InitialisePOLAR;
    LevelSpecs.LevelRestartInit = InitialisePOLAR;
    LevelSpecs.LevelRoutine = UpdatePOLAR;
    LevelSpecs.PostTriggers = POLARtriggers;
}

s32 PolarAI_TargetShare(s32 arg0, s32 arg1) {
    s32 total;
    s32 result;
    s32 i;

    result = 0;
    total = 0;
    if (Heroes[arg1].hero_status & 0x2000) {
        return -1;
    }
    for (i = 0; i < 4; i++) {
        if (i != arg0 && !(Heroes[i].hero_status & 0x2000)) {
            total += AiPlayerSkill[i].quality << 7;
        }
    }
    if (total != 0) {
        result = (AiPlayerSkill[arg1].quality << 19) / total;
    }
    return result;
}

void PolarAI_ResetPace(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (Heroes[i].hero_ai->AImode != 0) {
            PolarAIPace[i] = 0x1000;
        } else {
            PolarAIPace[i] = 0x2000;
        }
    }
}

void PolarHeroesCollide(s32 p1, s32 p2) {
    EXTheromove *move1 = Heroes[p1].hero_move;
    EXTheromove *move2 = Heroes[p2].hero_move;
    s32 m1 = move1->mass;
    s32 m2 = move2->mass;
    s32 k = 0x100;
    s32 v1;
    s32 v2;
    s32 d;
    s32 a;
    s32 b;

    if (m1 > 0 && m2 > 0) {
        if (GameInfo[p1 + 0x5C] != 0) {
            if (AiPlayerSkill[p1].level < 9) {
                m1 -= (9 - AiPlayerSkill[p1].level) << 5;
                if (m1 < 0x11) {
                    m1 = 0x10;
                }
            }
            if (AiPlayerSkill[p1].level > 0x18) {
                m1 += (AiPlayerSkill[p1].level - 0x18) << 6;
            }
        }
        if (GameInfo[p2 + 0x5C] != 0) {
            if (AiPlayerSkill[p2].level < 9) {
                m2 -= (9 - AiPlayerSkill[p2].level) << 5;
                if (m2 < 0x11) {
                    m2 = 0x10;
                }
            }
            if (AiPlayerSkill[p2].level > 0x18) {
                m2 += (AiPlayerSkill[p2].level - 0x18) << 6;
            }
        }
        if (Heroes[p1].hero_mode == 0x51 && Heroes[p1].hero_eng->unk2C.x > 0xFA0) {
            m1 += 0x220;
        }
        if (Heroes[p2].hero_mode == 0x51 && Heroes[p2].hero_eng->unk2C.x > 0xFA0) {
            m2 += 0x220;
        }
        v1 = move1->x_vel;
        v2 = move2->x_vel;
        d = -(k * (v1 - v2));
        a = d * m2 + ((m1 * v1 + m2 * v2) << 8);
        a /= m2 + m1;
        b = a - d;
        if (a < 0) {
            if (a < -0x9600) {
                a = -0x9600;
            }
        } else if (a > 0x9600) {
            a = 0x9600;
        }
        if (b < 0) {
            if (b < -0x9600) {
                b = -0x9600;
            }
        } else if (b > 0x9600) {
            b = 0x9600;
        }
        move1->x_vel = a >> 8;
        move2->x_vel = b >> 8;
        v1 = move1->z_vel;
        v2 = move2->z_vel;
        d = -(k * (v1 - v2));
        a = d * m2 + ((m1 * v1 + m2 * v2) << 8);
        a /= m2 + m1;
        b = a - d;
        if (a < 0) {
            if (a < -0x9600) {
                a = -0x9600;
            }
        } else if (a > 0x9600) {
            a = 0x9600;
        }
        if (b < 0) {
            if (b < -0x9600) {
                b = -0x9600;
            }
        } else if (b > 0x9600) {
            b = 0x9600;
        }
        move1->z_vel = a >> 8;
        move2->z_vel = b >> 8;
    }
}

void PolarHeroBounceOff(s32 player, EXTheromove *other) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = &base[player];
    EXTheromove *move = hero->hero_move;
    s32 m1 = move->mass;
    s32 m2 = other->mass;
    s32 k = 0x100;
    s32 v1;
    s32 v2;
    s32 d;
    s32 a;
    s32 b;

    if (m1 > 0 && m2 > 0) {
        if (hero->hero_mode == 0x51 && hero->hero_eng->unk2C.x > 0xFA0) {
            m1 += 0x220;
        }
        v1 = move->x_vel;
        v2 = other->x_vel;
        d = -(k * (v1 - v2));
        a = d * m2 + ((m1 * v1 + m2 * v2) << 8);
        a /= m2 + m1;
        b = a - d;
        if (a < 0) {
            if (a < -0x9600) {
                a = -0x9600;
            }
        } else if (a > 0x9600) {
            a = 0x9600;
        }
        if (b < 0) {
            if (b < -0x9600) {
                b = -0x9600;
            }
        } else if (b > 0x9600) {
            b = 0x9600;
        }
        move->x_vel = -(a >> 8);
        other->x_vel = -(b >> 8);
        v1 = move->z_vel;
        v2 = other->z_vel;
        d = -(k * (v1 - v2));
        a = d * m2 + ((m1 * v1 + m2 * v2) << 8);
        a /= m2 + m1;
        b = a - d;
        if (a < 0) {
            if (a < -0x9600) {
                a = -0x9600;
            }
        } else if (a > 0x9600) {
            a = 0x9600;
        }
        if (b < 0) {
            if (b < -0x9600) {
                b = -0x9600;
            }
        } else if (b > 0x9600) {
            b = 0x9600;
        }
        move->z_vel = -(a >> 8);
        other->z_vel = -(b >> 8);
    }
}

void PolarInitShadow(s32 player) {
    u8 *g = GameInfo;
    HeroInfo *hero;
    EXTengine *shadow;
    EXTengine *eng;
    s32 *p;

    if (*(s16 *)(g + 0x34) != 0x15 && g[player + 0x50] != 9) {
        HeroInfo *base = Heroes;
        s32 *tbl;

        hero = &base[player];
        shadow = hero->hero_shadow;
        if (shadow != NULL) {
            eng = hero->hero_eng;
            shadow->flags = 0x8000;
            shadow->unk5C = 1;
            if (eng != NULL) {
                shadow->sub->u6C.dbase = &HeroModelBank[DB_Polar];
                tbl = PolarShadowState;
                p = &tbl[player];
                *p = 0x4000;
                shadow->sub->entity = *p;
                shadow->sub->unk68 = 0x80;
                shadow->sub->flags |= 0x02000000;
                shadow->unk10 = eng->unk10;
                shadow->unk5D = 0;
                shadow->sub->flags = 0x8000;
                shadow->unk5E = 1;
            }
        }
    }
}

void PolarUpdateShadow(s32 player) {
    u8 *g;
    HeroInfo *h;
    s32 *ext;
    EXTengine *shadow;
    EXTengine *eng;
    EXTanimator *anim;
    PolarWanderFux *fux;
    s32 *p;
    s32 i;
    HeroInfo *base;
    HeroInfo *h2;
    HeroInfo *base2;
    HeroInfo *base3;
    HeroInfo *hh;
    HeroInfo *h3;
    HeroInfo *hb2;
    HeroInfo *hb6;
    HeroInfo *hb5;
    HeroInfo *hb4;

    base = Heroes;
    h = &base[player];
    ext = FindExtension(h->hero, 0x20000000);
    if (ext == NULL) {
        return;
    }
    g = GameInfo;
    if (*(s16 *)(g + 0x34) == 0x15) {
        return;
    }
    shadow = h->hero_shadow;
    eng = h->hero_eng;
    if (shadow == NULL) {
        return;
    }
    shadow->flags |= 0x200000;
    shadow->unk1E = eng->unk1E;
    shadow->unk5E = 1;
    if (g[player + 0x50] == 9 || h->hero_mode == 0x4E || h->hero_mode == 0x50) {
        shadow->sub->flags &= ~0x8000;
        shadow->flags &= ~0x8000;
        return;
    }
    shadow->sub->flags |= 0x8000;
    shadow->flags |= 0x8000;
    if (h->hero_mode == 0x58) {
        if (h->hero_name != 1 && h->hero_name != 7 && h->hero_name != 8 && h->hero_name != 2 &&
            h->hero_name != 6 && h->hero_name != 5 && h->hero_name != 4) {
            shadow->sub->entity = 0x4000;
            shadow->unk1E = eng->unk1E;
            shadow->unk10 = eng->unk10;
            shadow->unk5E = 1;
            return;
        }
    }
    fux = (PolarWanderFux *)Heroes[player].hero->FUX;
    if (fux != NULL && fux->unk50 != 0) {
        shadow->sub->entity = 0x4000;
        shadow->unk1E = eng->unk1E;
        shadow->unk10 = eng->unk10;
        shadow->unk5E = 1;
        return;
    }
    if (eng == NULL) {
        return;
    }
    base2 = Heroes;
    h2 = &base2[player];
    anim = h2->hero_anim;
    if (anim == NULL) {
        return;
    }
    if (h2->hero_mode != 0x51) {
        if (h2->hero_mode != 0x58) {
            shadow->sub->u6C.dbase = &HeroModelBank[DB_Polar];
            shadow->sub->flags = 0x8000;
            p = &PolarShadowState[player];
            shadow->sub->entity = *p + (anim->Aindex - 1);
            if (h2->hero_mode == 0x4B || h2->hero_mode == 0x4C) {
                shadow->sub->u6C.dbase = &HeroModelBank[DB_Polar];
                shadow->sub->entity = 0x4000;
                *p = 0x4000;
            }
            shadow->unk10 = eng->unk10;
            if (h2->hero_mode == 0x52) {
                shadow->sub->u6C.dbase = MapScene->dbase;
                shadow->unk10.y += 0x298;
                shadow->sub->entity = 0x2003;
                *p = 0x2003;
            }
            shadow->unk1E = eng->unk1E;
            shadow->unk5E = 1;
        }
        if (Heroes[player].hero_mode != 0x51) {
            goto mode58;
        }
    }
    {
        shadow->sub->u6C.dbase = &HeroModelBank[DB_Polar];
        shadow->unk10 = eng->unk10;
        shadow->sub->flags = 0x8000;
        switch (Heroes[player].hero_name) {
        case 4:
            shadow->sub->entity = PolarShadowFrames0[anim->Aindex - 1][0];
            break;
        case 3:
            shadow->sub->entity = PolarShadowFrames0[anim->Aindex - 1][0];
            break;
        case 7:
            shadow->sub->entity = PolarShadowFrames1[anim->Aindex - 1][0];
            break;
        case 8:
            shadow->sub->entity = PolarShadowFrames2[anim->Aindex - 1][0];
            break;
        case 5:
            shadow->sub->entity = PolarShadowFrames1[anim->Aindex - 1][0];
            break;
        case 6:
            shadow->sub->entity = PolarShadowFrames1[anim->Aindex - 1][0];
            break;
        default:
            shadow->sub->entity = PolarShadowFrames3[anim->Aindex - 1][0];
            break;
        }
        shadow->unk1E = eng->unk1E;
        shadow->unk5E = 1;
    }
mode58:
    base3 = Heroes;
    h3 = &base3[player];
    if (h3->hero_mode == 0x58) {
        shadow->sub->u6C.dbase = &HeroModelBank[DB_Polar];
        shadow->unk10 = eng->unk10;
        shadow->sub->flags = 0x8000;
        switch (h3->hero_name) {
        case 1:
            if (Heroes[player].hero_anim->Atable == CRA_seq_polarwin) {
                shadow->sub->entity = PolarShadowFrames4[anim->Aindex - 1][0];
            } else {
                shadow->sub->entity = PolarShadowFrames4[0][0];
            }
            break;
        case 7:
            if (Heroes[player].hero_anim->Atable == DNG_seq_polarwin) {
                shadow->sub->entity = PolarShadowFrames5[anim->Aindex - 1][0];
            } else {
                shadow->sub->entity = PolarShadowFrames5[0][0];
            }
            break;
        case 8:
            if (Heroes[player].hero_anim->Atable == KAN_seq_polarwin) {
                shadow->sub->entity = PolarShadowFrames6[anim->Aindex - 1][0];
            } else {
                shadow->sub->entity = PolarShadowFrames6[0][0];
            }
            break;
        case 2:
            hb2 = Heroes;
            hh = &hb2[player];
            if (hh->hero_anim->Atable == COC_seq_polarwin || ext[45] != 0) {
                i = hh->hero_eng->sub->entity - 0x4300;
                if (i < 0) {
                    i = 0;
                }
                ext[45] = 1;
                shadow->sub->entity = PolarShadowFrames7[i][0];
            } else {
                shadow->sub->entity = PolarShadowFrames7[0][0];
            }
            break;
        case 6:
            hb6 = Heroes;
            hh = &hb6[player];
            if (hh->hero_anim->Atable == KON_seq_polarwin || ext[45] != 0) {
                i = hh->hero_eng->sub->entity - 0x4280;
                if (i < 0) {
                    i = 0;
                }
                ext[45] = 1;
                shadow->sub->entity = PolarShadowFrames9[i][0];
            } else {
                shadow->sub->entity = PolarShadowFrames9[0][0];
            }
            break;
        case 5:
            hb5 = Heroes;
            hh = &hb5[player];
            if (hh->hero_anim->Atable == TIN_seq_polarwin || ext[45] != 0) {
                i = hh->hero_eng->sub->entity - 0x4300;
                if (i < 0) {
                    i = 0;
                }
                ext[45] = 1;
                shadow->sub->entity = PolarShadowFrames10[i][0];
            } else {
                shadow->sub->entity = PolarShadowFrames10[0][0];
            }
            break;
        case 4:
            hb4 = Heroes;
            hh = &hb4[player];
            if (hh->hero_anim->Atable == BRO_seq_polarwin || ext[45] != 0) {
                i = hh->hero_eng->sub->entity - 0x4300;
                if (i < 0) {
                    i = 0;
                }
                ext[45] = 1;
                shadow->sub->entity = PolarShadowFrames8[i][0];
            } else {
                shadow->sub->entity = PolarShadowFrames8[0][0];
            }
            break;
        }
        shadow->unk1E = eng->unk1E;
        shadow->unk5E = 1;
    }
}

void PolarPreMoveHero(s32 player) {
    HeroInfo *h;
    HeroInfo *base;
    EXTheromove *m;
    EXTheromove *m2;
    PolarPadMove *pad;
    PolarPadMove *p2;
    PolarModeView *g;
    s32 *spd;

    base = Heroes;
    h = base + player;
    if (h->hero_frozen != 0) {
        ((PolarPadMove *)Pads)[player].buttons = 0;
        ((PolarPadMove *)Pads)[player].unk54 = 0;
    }
    m = h->hero_move;
    if (m != NULL) {
        if (h->hero_mode != 0x51) {
            m->x_max = 0;
            m->z_max = 0;
        }
        pad = (PolarPadMove *)Pads;
        pad += player;
        p2 = pad;
        if ((p2->flags & 1) || h->hero_ai->AImode != 0) {
            if (p2->buttons & 0xF0) {
                m->forward_angle = (p2->angle + 0x800) & 0xFFF;
                spd = &PolarHeroMass[player];
                m->x_max = (*spd * rcossin_tbl[(m->forward_angle + 0x400) & 0xFFF].unk2) >> 12;
                m->z_max = (*spd * rcossin_tbl[(m->forward_angle + 0x800) & 0xFFF].unk2) >> 12;
            }
        }
        g = (PolarModeView *)GameInfo;
        if (g->mode != 0x16) {
            m->x_max += ((m->mass * 3) >> 3) * rcossin_tbl[(PolarTiltZ - 0x400) & 0xFFF].unk2 >> 12;
            m->z_max -= ((m->mass * 3) >> 3) * rcossin_tbl[(PolarTiltX - 0x400) & 0xFFF].unk2 >> 12;
            if (g->mode == 0x12) {
                if (g->flags & 0x80) {
                    if (GameInfo[player + 0x5C] != 0) {
                        m->x_max += ((m->mass * 3) >> 4) * rcossin_tbl[(MapSceneRot.vz - 0x400) & 0xFFF].unk2 >> 12;
                        m->z_max -= ((m->mass * 3) >> 4) * rcossin_tbl[(MapSceneRot.vx - 0x400) & 0xFFF].unk2 >> 12;
                    } else {
                        m->x_max += ((m->mass * 5) >> 3) * rcossin_tbl[(MapSceneRot.vz - 0x400) & 0xFFF].unk2 >> 12;
                        m->z_max -= ((m->mass * 5) >> 3) * rcossin_tbl[(MapSceneRot.vx - 0x400) & 0xFFF].unk2 >> 12;
                    }
                }
            }
        } else {
            m->x_max += ((m->mass * 25) >> 6) * rcossin_tbl[(PolarTiltZ - 0x400) & 0xFFF].unk2 >> 12;
            m->z_max -= ((m->mass * 25) >> 6) * rcossin_tbl[(PolarTiltX - 0x400) & 0xFFF].unk2 >> 12;
        }
    }
    if (Heroes[player].hero_status & 0x200) {
        m2 = Heroes[player].hero_move;
        if (m2 != NULL) {
            m2->z_dir = 0;
            m2->x_max = 0;
            m2->z_max = 0;
            m2->x_vel = 0;
            m2->z_vel = 0;
        }
    }
    if (abs(angle_diff(m->forward_angle, m->facing_angle)) > 0x110) {
        m->facing_angle = (m->forward_angle + 0x100) & 0xE00;
    }
}

s32 PolarGetLevelPoint(s32 *arg0, s32 arg1) {
    s32 v[3];
    u8 *base;
    s32 off;
    u8 *p;
    u8 *a2;
    u8 *a3;
    TrigEntry *t;
    s32 n;
    s32 i;
    s32 ang;

    base = ((u8 *)MapScene->map);
    off = *(s32 *)(base + 0x14);
    p = (u8 *)&((s32 *)(base + 0x14 + off))[arg1 + 1] + ((s32 *)(base + 0x14 + off))[arg1 + 1];
    if (p != NULL) {
        i = 0;
        n = *(s32 *)(p + 0x38);
        t = rcossin_tbl;
        a2 = p + 0x50;
        a3 = p;
        for (;;) {
            if (i >= n) {
                return 1;
            }
            ang = *(s32 *)(a3 + 0x64);
            v[0] = arg0[0] - *(s32 *)(a2 - 8);
            v[2] = arg0[2] - *(s32 *)a2;
            ang = -ang;
            a3 += 0x38;
            if (((v[0] * t[ang & 0xFFF].unk2) >> 12) - ((v[2] * t[(ang - 0x400) & 0xFFF].unk2) >> 12) < 0) {
                return 0;
            }
            i++;
            a2 += 0x38;
        }
    }
    return 1;
}

s32 TestPolarHeroMelted(s32 player) {
    HeroInfo *hb;
    HeroInfo *h;
    EXTengine *eng;
    Ext20 *ext;
    s32 out;
    u16 act;
    u16 mode;

    hb = Heroes;
    h = &hb[player];
    eng = h->hero_eng;
    ext = FindExtension(h->hero, 0x20000000);
    if (ext != NULL) {
    if (GameMode != 0x16) {
        act = 0x4E;
        if (eng != NULL && CheckMelted(&eng->unk10, 0)) {
            return 1;
        }
        mode = Heroes[player].hero_mode;
        if (mode == 0x4C) {
            if (!(Heroes[player].hero_status & 0x200)) {
                SetHeroAction(player, 0x4E);
            }
        } else if (!(Heroes[player].hero_status & 0x200)) {
            switch (mode) {
            case 0x4E:
                break;
            default:
                SetHeroAction(player, act);
                break;
            }
        }
        return 0;
    } else if (bossmapsub != NULL) {
        if (bossmapsub->entity == 0x2008
            && ((s32 (*)(s32 *, s32, s32))PolarGetLevelPoint)((s32 *)&eng->unk10, 1, player)
            && !(h->hero_status & 0x200)) {
            ext->unkA8 = 1;
            if (h->hero_mode != 0x4E) {
                SetHeroAction(player, 0x4E);
            }
        }
        if (bossmapsub->entity == 0x2009
            && ((s32 (*)(s32 *, s32, s32))PolarGetLevelPoint)((s32 *)&eng->unk10, 2, player)
            && !(Heroes[player].hero_status & 0x200)) {
            ext->unkA8 = 1;
            if (Heroes[player].hero_mode != 0x4E) {
                SetHeroAction(player, 0x4E);
            }
        }
        if (bossmapsub->entity == 0x200A) {
            out = ((s32 (*)(s32 *, s32, s32))PolarGetLevelPoint)((s32 *)&eng->unk10, 3, player) != 1;
            if (out && !(Heroes[player].hero_status & 0x200)) {
                ext->unkA8 = 1;
                if (Heroes[player].hero_mode != 0x4E) {
                    SetHeroAction(player, 0x4E);
                }
            }
        }
        if (bossmapsub->flags & 0x8000) {
            if (eng != NULL && ((s32 (*)(s32 *, s32, s32))PolarGetLevelPoint)((s32 *)&eng->unk10, 0, player)) {
                ext->unkA8 = 0;
                return 1;
            }
        } else {
            ext->unkA8 = 1;
        }
        return PolarEndAction(player);
    }
    }
    return 0;
}

s32 PolarClearHitBy(s32 arg0) {
    Heroes[arg0].hero_move->HitBy = -1;
    return 1;
}

void SetPolarHeroIdle(s32 player) {
    HeroInfo *base;
    HeroInfo *info;
    EXTheromove *move;
    Ext20 *ext;
    s32 rnd;
    s32 sfx;
    s32 speed;

    rnd = Random(6);
    sfx = 0;
    base = Heroes;
    info = &base[player];
    speed = 0;
    if (info->hero_eng->sfx != NULL) {
        StopSfx(info->hero_eng->sfx);
    }
    info->hero_eng->sub->flags &= 0xFFBFFFFF;
    if (TestPolarHeroWon(player) != 0) {
        return;
    }
    info->hero_eng->sub->unk68 = 0;
    ext = FindExtension(info->hero, 0x20000000);
    if (ext != NULL) {
        ext->unk14 = 0;
    }
    info->hero_eng->unk5D = 0;
    info->hero_eng->unk1C = 0;
    move = info->hero_move;
    if (move != NULL) {
        speed = csqrt(move->x_vel * move->x_vel + move->z_vel * move->z_vel) >> 6;
        move->z_dir = 0;
        move->x_acc = 2;
        move->x_dec = 1;
        move->z_acc = 2;
        move->z_dec = 1;
        if (((PolarModeView *)GameInfo)->mode == 0x16) {
            move->x_acc = 4;
            move->z_acc = 4;
        }
    }
    if (GameMode != 0x14) {
        if (Heroes[player].hero_name == 1) {
            Heroes[player].hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolar2Crash];
        }
        if (Heroes[player].hero_name == 2) {
            Heroes[player].hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolar2Coco];
        }
        if (Heroes[player].hero_name == 5) {
            Heroes[player].hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolar2Tiny];
        }
        if (Heroes[player].hero_name == 3) {
            Heroes[player].hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolar2Cortex];
        }
        if (Heroes[player].hero_name == 7) {
            Heroes[player].hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolar2Dingo];
        }
        if (Heroes[player].hero_name == 8) {
            Heroes[player].hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolar2Rilla];
        }
        if (Heroes[player].hero_name == 4) {
            Heroes[player].hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolar2Brio];
        }
        if (Heroes[player].hero_name == 6) {
            Heroes[player].hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolar2Kong];
        }
    } else {
        if (Heroes[player].hero_name == 1) {
            Heroes[player].hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolarCrash];
        }
        if (Heroes[player].hero_name == 2) {
            Heroes[player].hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolarCoco];
        }
        if (Heroes[player].hero_name == 5) {
            Heroes[player].hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolarTiny];
        }
        if (Heroes[player].hero_name == 3) {
            Heroes[player].hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolarCortex];
        }
        if (Heroes[player].hero_name == 7) {
            Heroes[player].hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolarDingo];
        }
        if (Heroes[player].hero_name == 8) {
            Heroes[player].hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolarRilla];
        }
        if (Heroes[player].hero_name == 4) {
            Heroes[player].hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolarBrio];
        }
        if (Heroes[player].hero_name == 6) {
            Heroes[player].hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolarKong];
        }
    }
    SetHeroSeq(player, 0x3A);
    PolarShadowState[player] = 0x4000;
    if (rnd == 0) {
        sfx = 0x456;
    }
    if (rnd == 1) {
        sfx = 0x457;
    }
    if (rnd == 2) {
        sfx = 0x458;
    }
    if (rnd == 3) {
        sfx = 0x459;
    }
    if (rnd == 4) {
        sfx = 0x45A;
    }
    if (rnd == 5) {
        sfx = 0x45B;
    }
    if (speed > 0x20) {
        PlaySfx(sfx, &Heroes[player].hero_eng->sfx, NULL, 0x1000, Random(0x100) + 0x1D80, 0);
    }
}

void PolarHeroIdle(s32 player) {
    Ext20 *ext;
    Ext20 *ext2;

    ext = FindExtension(Heroes[player].hero, 0x20000000);
    if (ext != NULL) {
        ext->val += 4;
        if (ext->val > 0x100) {
            ext->val = 0x100;
        }
    }
    PolarHeroCollisions(player);
    ProcessHeroAnim(player);
    ext2 = FindExtension(Heroes[player].hero, 0x20000000);
    if (ext2 != NULL && (((PolarPad *)Pads)[player].unk50 & 0xF0)) {
        if (ext2->state == 0) {
            SetHeroAction(player, 0x4A);
        }
        if (ext2->state == 1) {
            SetHeroAction(player, 0x55);
        }
    }
    PolarTestAttack(player);
    PolarTestSnowball(player);
    TestPolarHeroMelted(player);
    PolarIceHit(&Heroes[player].hero_eng->unk10, player);
    TestPolarHeroWon(player);
}

void PolarHeroRun(s32 player) {
    HeroInfo *h;
    HeroInfo *base;
    Ext20 *ext;
    Ext20 *ext1;

    ext1 = FindExtension(Heroes[player].hero, 0x20000000);
    if (ext1 != NULL) {
        ext1->val += 4;
        if (ext1->val > 0x100) {
            ext1->val = 0x100;
        }
    }
    PolarHeroCollisions(player);
    base = Heroes;
    h = &base[player];
    if (h->hero_anim->Aindex == 4) {
        if (h->hero_move->mass > 0x50) {
            if (player == 0) {
                PlaySfx(0x49C, &base[0].hero_eng->sfx, &base[0].hero_eng->unk10, 0x1000, 0x1E00, 0);
            }
            if (player == 1) {
                PlaySfx(0x49D, &base[1].hero_eng->sfx, &base[1].hero_eng->unk10, 0x1000, 0x1E00, 0);
            }
            if (player == 2) {
                PlaySfx(0x49E, &base[2].hero_eng->sfx, &base[2].hero_eng->unk10, 0x1000, 0x1E00, 0);
            }
            if (player == 3) {
                PlaySfx(0x49F, &base[3].hero_eng->sfx, &base[3].hero_eng->unk10, 0x1000, 0x1E00, 0);
            }
        } else {
            PlaySfx(0x4A0, &h->hero_eng->sfx, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
        }
    }
    ProcessHeroAnim(player);
    if (!(((PolarPad *)Pads)[player].unk50 & 0xF0)) {
        ext = FindExtension(Heroes[player].hero, 0x20000000);
        if (ext != NULL) {
            if (ext->state == 0) {
                SetHeroAction(player, 0x49);
            }
            if (ext->state == 1) {
                SetHeroAction(player, 0x54);
            }
        }
    }
    PolarTestAttack(player);
    PolarTestSnowball(player);
    TestPolarHeroMelted(player);
    PolarIceHit(&Heroes[player].hero_eng->unk10, player);
    if (TestPolarHeroWon(player) != 0) {
        if (Heroes[player].hero_eng->sfx != NULL) {
            StopSfx(Heroes[player].hero_eng->sfx);
        }
    }
}

void SetPolarHeroDazed(s32 player) {
    s32 *ext;
    NAS *nas;
    s32 *fux;
    EXTengine *eng;
    EXTheromove *move;
    s32 sfx;

    if (TestPolarHeroWon() == 0) {
        PlaySfx(0x467, NULL, NULL, 0x1000, 0x1E00, 0);
        switch (Heroes[player].hero_name) {
        case 1:
            sfx = 3;
            goto play;
        case 2:
            sfx = 0x35;
            goto play;
        case 3:
            sfx = 0x67;
            goto play;
        case 4:
            sfx = 0x99;
            goto play;
        case 5:
            sfx = 0x12F;
            goto play;
        case 7:
            sfx = 0xFD;
            goto play;
        case 8:
            sfx = 0xCB;
            goto play;
        case 6:
            sfx = 0x161;
        play:
            PlaySfx(sfx, NULL, NULL, 0x1000, 0x1E00, 0);
            break;
        }
        ext = FindExtension(Heroes[player].hero, 0x20000000);
        if (ext != NULL) {
            if (GameMode != 0x16) {
                eng = Heroes[player].hero_eng;
                ext[0x98 / 4] = 0;
                nas = CreateNAS(eng->unk10.x, eng->unk10.y, eng->unk10.z, 0x1605, 0x11);
                if (nas != NULL) {
                    nas->Engine->flags |= 0x200000;
                    fux = (s32 *)nas->FUX;
                    if (fux != NULL) {
                        fux[0x18 / 4] = 0x200;
                        fux[0xC / 4] = 0;
                        fux[0x20 / 4] = 5;
                        nas->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
                        nas->Engine->flags |= 0x200000;
                        fux[0x28 / 4] = 6;
                    }
                    nas->InitDelete(nas);
                }
            }
            ext[0x1C / 4] = 0;
            move = Heroes[player].hero_move;
            if (move != NULL) {
                move->z_dir = 0;
                move->x_max = 0;
                move->z_max = 0;
                move->x_vel = 0;
                move->z_vel = 0;
                move->mass = 0x200;
            }
            Heroes[player].hero_eng->sub->unk68 = 0;
            if (Heroes[player].hero_name == 1) {
                if (ext[0x24 / 4] == 0) {
                    SetHeroSeq(player, 0x44);
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                    SetHeroSeq(player, 0x3C);
                }
            }
            if (Heroes[player].hero_name == 5) {
                if (ext[0x24 / 4] == 0) {
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    SetHeroSeq(player, 0x44);
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                    SetHeroSeq(player, 0x3C);
                }
            }
            if (Heroes[player].hero_name == 6) {
                if (ext[0x24 / 4] == 0) {
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    SetHeroSeq(player, 0x44);
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                    SetHeroSeq(player, 0x3C);
                }
            }
            if (Heroes[player].hero_name == 3) {
                if (ext[0x24 / 4] == 0) {
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    SetHeroSeq(player, 0x44);
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                    SetHeroSeq(player, 0x3C);
                }
            }
            if (Heroes[player].hero_name == 2) {
                if (ext[0x24 / 4] == 0) {
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    SetHeroSeq(player, 0x44);
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                    SetHeroSeq(player, 0x3C);
                }
            }
            if (Heroes[player].hero_name == 4) {
                if (ext[0x24 / 4] == 0) {
                    Heroes[player].hero_eng->sub->unk8E = 0x2001;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    SetHeroSeq(player, 0x44);
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                    SetHeroSeq(player, 0x3C);
                }
            }
            if (Heroes[player].hero_name == 7) {
                if (ext[0x24 / 4] == 0) {
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    SetHeroSeq(player, 0x44);
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                    SetHeroSeq(player, 0x3C);
                }
            }
            if (Heroes[player].hero_name == 8) {
                if (ext[0x24 / 4] == 0) {
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    SetHeroSeq(player, 0x44);
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                    SetHeroSeq(player, 0x3C);
                }
            }
        }
    }
}

void PolarHeroDazed(s32 player) {
    EXTheromove *move;
    s32 *ext;
    s32 *ext2;
    s32 *p;
    NAS *nas;
    EXTengine *eng;
    s32 x;
    s32 y;
    s32 z;

    if (TestPolarHeroWon() == 0) {
        move = Heroes[player].hero_move;
        ext = FindExtension(Heroes[player].hero, 0x20000000);
        if (ext != NULL) {
            eng = Heroes[player].hero_eng;
            x = eng->unk10.x;
            y = eng->unk10.y;
            z = eng->unk10.z;
            if (Heroes[player].hero_name == 1) {
                ProcessHeroAnim(player);
            }
            if (Heroes[player].hero_name == 7) {
                ProcessHeroAnim(player);
            }
            if (Heroes[player].hero_name == 8) {
                ProcessHeroAnim(player);
            }
            if (Heroes[player].hero_name == 3) {
                ProcessHeroAnim(player);
            }
            if (Heroes[player].hero_name == 2) {
                ProcessHeroAnim(player);
            }
            if (Heroes[player].hero_name == 5) {
                ProcessHeroAnim(player);
            }
            if (Heroes[player].hero_name == 6) {
                ProcessHeroAnim(player);
            }
            if (Heroes[player].hero_name == 4) {
                ProcessHeroAnim(player);
            }
            if (ext[0x98 / 4]-- < 0) {
                ext[0x98 / 4] = Random(8) + 0x18;
                nas = CreateNAS(x + 0x60, y - 0x180, z, 0x1612, 0x11);
                if (nas != NULL) {
                    p = FindExtension(nas, 0x10000000);
                    if (p != NULL) {
                        p[0x1C / 4] = 0xA;
                        p[0xC / 4] = 0x100;
                        p[0x10 / 4] = 0x50;
                        p[0x14 / 4] = -0x40;
                        p[0x24 / 4] = 0x20;
                        p[0x28 / 4] = 1;
                        p[0x18 / 4] |= 1;
                    }
                }
                nas = CreateNAS(x - 0x60, y - 0x180, z, 0x1612, 0x11);
                if (nas != NULL) {
                    p = FindExtension(nas, 0x10000000);
                    if (p != NULL) {
                        p[0x1C / 4] = 0xA;
                        p[0xC / 4] = 0x100;
                        p[0x10 / 4] = 0x50;
                        p[0x14 / 4] = -0x40;
                        p[0x24 / 4] = 0x20;
                        p[0x28 / 4] = 1;
                        p[0x18 / 4] |= 1;
                    }
                }
            }
            if (move != NULL) {
                ((PolarPad *)Pads)[player].unk50 = 0;
                ((PolarPad *)Pads)[player].unk54 = 0;
                PolarHeroCollisions(player);
                TestPolarHeroMelted(player);
                PolarIceHit(&Heroes[player].hero_eng->unk10, player);
                Heroes[player].hero_frozen = 2;
                if (ext[0x24 / 4] == 0 && ext[0x1C / 4] >= 0x1F) {
                    Heroes[player].hero_eng->unk1E += 0x10;
                    if (PolarRoundState != 0 && !(Heroes[player].hero_status & 0x200)) {
                        Heroes[player].hero_eng->sub->flags &= 0xFFFF7FFF;
                        Heroes[player].hero_shadow->sub->flags &= 0xFFFF7FFF;
                        SetHeroAction(player, 0x50);
                        return;
                    }
                    if (Heroes[player].hero_name == 1 && ext[0x24 / 4] == 0) {
                        Heroes[player].hero_eng->sub->entity = 0x4100;
                    }
                    if (Heroes[player].hero_name == 7 && ext[0x24 / 4] == 0) {
                        Heroes[player].hero_eng->sub->entity = 0x4000;
                    }
                    if (Heroes[player].hero_name == 8 && ext[0x24 / 4] == 0) {
                        Heroes[player].hero_eng->sub->entity = 0x4000;
                    }
                    if (Heroes[player].hero_name == 3 && ext[0x24 / 4] == 0) {
                        Heroes[player].hero_eng->sub->entity = 0x4100;
                    }
                    if (Heroes[player].hero_name == 2 && ext[0x24 / 4] == 0) {
                        Heroes[player].hero_eng->sub->entity = 0x4000;
                    }
                    if (Heroes[player].hero_name == 5 && ext[0x24 / 4] == 0) {
                        Heroes[player].hero_eng->sub->entity = 0x4000;
                    }
                    if (Heroes[player].hero_name == 6 && ext[0x24 / 4] == 0) {
                        Heroes[player].hero_eng->sub->entity = 0x4000;
                    }
                    if (Heroes[player].hero_name == 4 && ext[0x24 / 4] == 0) {
                        Heroes[player].hero_eng->sub->entity = 0x4080;
                    }
                }
                Heroes[player].hero_eng->unk5E = 1;
            }
            if (ext[0x1C / 4]++ >= 0xB5) {
                ext2 = FindExtension(Heroes[player].hero, 0x20000000);
                if (ext2 != NULL) {
                    ResetMass(player);
                    Heroes[player].hero_eng->sub->flags &= 0xFFBFFFFF;
                    if (ext2[0x24 / 4] == 0) {
                        SetHeroAction(player, 0x49);
                    }
                    if (ext2[0x24 / 4] == 1) {
                        SetHeroAction(player, 0x54);
                    }
                }
            }
        }
    }
}

void SetPolarHeroChallenge(s32 player) {
    s32 *ext;
    s32 sfx;

    if (TestPolarHeroWon() == 0) {
        PlaySfx(0x467, NULL, NULL, 0x1000, 0x1E00, 0);
        PolarShadowState[player] = 0x4000;
        switch (Heroes[player].hero_name) {
        case 1:
            sfx = 3;
            goto play;
        case 2:
            sfx = 0x35;
            goto play;
        case 3:
            sfx = 0x67;
            goto play;
        case 4:
            sfx = 0x99;
            goto play;
        case 5:
            sfx = 0x12F;
            goto play;
        case 7:
            sfx = 0xFD;
            goto play;
        case 8:
            sfx = 0xCB;
            goto play;
        case 6:
            sfx = 0x161;
        play:
            PlaySfx(sfx, NULL, NULL, 0x1000, 0x1E00, 0);
            break;
        }
        ext = FindExtension(Heroes[player].hero, 0x20000000);
        if (ext != NULL) {
            ext[0x98 / 4] = 0;
            Heroes[player].hero_eng->sub->flags |= 0x400000;
            ext[0x1C / 4] = 0;
            if (Heroes[player].hero_name == 1) {
                if (ext[0x24 / 4] == 0) {
                    SetHeroSeq(player, 0x44);
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                }
            }
            if (Heroes[player].hero_name == 5) {
                if (ext[0x24 / 4] == 0) {
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                }
            }
            if (Heroes[player].hero_name == 6) {
                if (ext[0x24 / 4] == 0) {
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                }
            }
            if (Heroes[player].hero_name == 3) {
                if (ext[0x24 / 4] == 0) {
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                }
            }
            if (Heroes[player].hero_name == 2) {
                if (ext[0x24 / 4] == 0) {
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                }
            }
            if (Heroes[player].hero_name == 4) {
                if (ext[0x24 / 4] == 0) {
                    Heroes[player].hero_eng->sub->unk8E = 0x2001;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                }
            }
            if (Heroes[player].hero_name == 7) {
                if (ext[0x24 / 4] == 0) {
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                }
            }
            if (Heroes[player].hero_name == 8) {
                if (ext[0x24 / 4] == 0) {
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->unk8E = 0x2002;
                }
            }
            if (Heroes[player].hero_name == 1 && ext[0x24 / 4] == 0) {
                Heroes[player].hero_eng->sub->entity = 0x4100;
            }
            if (Heroes[player].hero_name == 7 && ext[0x24 / 4] == 0) {
                Heroes[player].hero_eng->sub->entity = 0x4000;
            }
            if (Heroes[player].hero_name == 8 && ext[0x24 / 4] == 0) {
                Heroes[player].hero_eng->sub->entity = 0x4000;
            }
            if (Heroes[player].hero_name == 3 && ext[0x24 / 4] == 0) {
                Heroes[player].hero_eng->sub->entity = 0x4100;
            }
            if (Heroes[player].hero_name == 2 && ext[0x24 / 4] == 0) {
                Heroes[player].hero_eng->sub->entity = 0x4000;
            }
            if (Heroes[player].hero_name == 5 && ext[0x24 / 4] == 0) {
                Heroes[player].hero_eng->sub->entity = 0x4000;
            }
            if (Heroes[player].hero_name == 6 && ext[0x24 / 4] == 0) {
                Heroes[player].hero_eng->sub->entity = 0x4000;
            }
            if (Heroes[player].hero_name == 4 && ext[0x24 / 4] == 0) {
                Heroes[player].hero_eng->sub->entity = 0x4080;
            }
            Heroes[player].hero_eng->sub->unk68 = 0;
        }
    }
}

void PolarHeroChallenge(s32 player) {
    EXTheromove *move;
    s32 *ext;
    s32 *ext2;
    s32 *p;
    NAS *nas;
    EXTengine *eng;
    s32 x;
    s32 y;
    s32 z;

    if (TestPolarHeroWon() == 0) {
        move = Heroes[player].hero_move;
        ext = FindExtension(Heroes[player].hero, 0x20000000);
        if (ext != NULL) {
            eng = Heroes[player].hero_eng;
            x = eng->unk10.x;
            y = eng->unk10.y;
            z = eng->unk10.z;
            if (ext[0x98 / 4]-- < 0) {
                ext[0x98 / 4] = Random(0x14) + 1;
                nas = CreateNAS(x + 0x60, y - 0x180, z, 0x1612, 0x11);
                if (nas != NULL) {
                    p = FindExtension(nas, 0x10000000);
                    if (p != NULL) {
                        p[0x1C / 4] = 0xA;
                        p[0xC / 4] = 0x100;
                        p[0x10 / 4] = 0x50;
                        p[0x14 / 4] = -0x40;
                        p[0x24 / 4] = 0x20;
                        p[0x28 / 4] = 1;
                        p[0x18 / 4] |= 1;
                    }
                }
                nas = CreateNAS(x - 0x60, y - 0x180, z, 0x1612, 0x11);
                if (nas != NULL) {
                    p = FindExtension(nas, 0x10000000);
                    if (p != NULL) {
                        p[0x1C / 4] = 0xA;
                        p[0xC / 4] = 0x100;
                        p[0x10 / 4] = 0x50;
                        p[0x14 / 4] = -0x40;
                        p[0x24 / 4] = 0x20;
                        p[0x28 / 4] = 1;
                        p[0x18 / 4] |= 1;
                    }
                }
            }
            if (move != NULL) {
                ((PolarPad *)Pads)[player].unk50 = 0;
                ((PolarPad *)Pads)[player].unk54 = 0;
                PolarHeroCollisions(player);
                TestPolarHeroMelted(player);
                PolarIceHit(&Heroes[player].hero_eng->unk10, player);
                Heroes[player].hero_frozen = 2;
                if (ext[0x24 / 4] == 0) {
                    Heroes[player].hero_eng->unk1E += 0x10;
                }
                Heroes[player].hero_eng->unk5E = 1;
            }
            if (ext[0x1C / 4]++ >= 0xB5) {
                ext2 = FindExtension(Heroes[player].hero, 0x20000000);
                if (ext2 != NULL) {
                    ResetMass(player);
                    Heroes[player].hero_eng->sub->flags &= 0xFFBFFFFF;
                    if (ext2[0x24 / 4] == 0) {
                        SetHeroAction(player, 0x49);
                    }
                    if (ext2[0x24 / 4] == 1) {
                        SetHeroAction(player, 0x54);
                    }
                }
            }
        }
    }
}

void SetPolarHeroTransmute(s32 player) {
    HeroInfo *base;
    HeroInfo *hero;

    if (TestPolarHeroWon() == 0) {
        base = Heroes;
        hero = base + player;
        if (hero->hero_move != NULL) {
            hero->hero_move->z_dir = 0;
        }
        hero->hero_eng->sub->unk68 = 0;
        if (hero->hero_name == 1) {
            hero->hero_eng->sub->entity = 0x2002;
        }
        if (hero->hero_name == 5) {
            hero->hero_eng->sub->entity = 0x2002;
        }
        if (hero->hero_name == 3) {
            hero->hero_eng->sub->entity = 0x2002;
        }
        if (hero->hero_name == 2) {
            hero->hero_eng->sub->entity = 0x2002;
        }
        if (hero->hero_name == 4) {
            hero->hero_eng->sub->entity = 0x2002;
        }
        if (hero->hero_name == 7) {
            hero->hero_eng->sub->entity = 0x2003;
        }
        if (hero->hero_name == 6) {
            hero->hero_eng->sub->entity = 0x2002;
        }
        if (hero->hero_name == 8) {
            hero->hero_eng->sub->entity = 0x2002;
        }
    }
}

void PolarHeroTransmute(s32 player) {
    HeroInfo *base;
    HeroInfo *hero;
    EXTheromove *move;

    if (TestPolarHeroWon() == 0) {
        base = Heroes;
        hero = &base[player];
        move = hero->hero_move;
        if (move != NULL) {
            ((PolarPad *)Pads)[player].unk50 = 0;
            ((PolarPad *)Pads)[player].unk54 = 0;
            PolarHeroCollisions(player);
            TestPolarHeroMelted(player);
            PolarIceHit(&hero->hero_eng->unk10, player);
            hero->hero_frozen = 2;
            hero->hero_eng->unk1E += 0x10;
            hero->hero_eng->unk5E = 1;
            move->x_max = 0;
            move->z_max = 0;
        }
    }
}

void SetPolarHeroOffice(s32 player) {
    HeroInfo *hero;
    PolarHitExt *ext;
    EXTheromove *move;
    u8 *g;
    u16 status;
    HeroInfo *base;
    HeroInfo *base2;
    HeroInfo *h;

    if (TestPolarHeroWon() == 0) {
        base = Heroes;
        hero = &base[player];
        ext = FindExtension(hero->hero, 0x20000000);
        if (ext != NULL) {
            if (hero->hero_last_mode == 0x52) {
                ext->unkAC = 1;
            }
            if (hero->hero_last_mode == 0x4C) {
                ext->unkAC = 1;
            }
            if (hero->hero_last_mode != 0x4C && hero->hero_last_mode != 0x52
                && hero->hero_last_mode != 0x4B) {
                move = hero->hero_move;
                if (move != NULL) {
                    move->z_dir = 0;
                }
                if (ext->unk24 == 0) {
                    SetHeroSeq(player, 0x3D);
                }
                hero->hero_eng->sub->unk68 = 0;
                ext->speed = csqrt(move->x_vel * move->x_vel + move->z_vel * move->z_vel) >> 6;
                if (ext->speed >= 0x4A || ext->unkA8 != 0) {
                    hero->hero_anim->Aindex = 0x21;
                    status = hero->hero_status;
                    if (!(status & 0x200)) {
                        hero->hero_status = status | 0x2000;
                    }
                } else if (ext->unk24 == 0) {
                    PlaySfx(0x45C, &hero->hero_eng->sfx, NULL, 0x1000, 0x1E00, 0);
                }
            }
            if (GameMode == 0x12) {
                if (PolarOfficeBox(player) != 0) {
                    Heroes[player].hero_anim->Aindex = 0x21;
                }
            }
            g = GameInfo;
            if (*(u16 *)(g + 0x32) & 0x80) {
                if (g[player + 0x5C] == 0 && *(s16 *)(g + 0x34) == 0x14) {
                    Heroes[player].hero_anim->Aindex = 0x21;
                }
            }
            base2 = Heroes;
            h = &base2[player];
            if (h->hero_eng->unk10.z >= -0x3E7) {
                h->hero_eng->sub->unk68 = 0x200;
            }
            if (GameMode == 0x16) {
                if (h->hero_eng->unk10.z >= -0x3E7) {
                    h->hero_eng->sub->unk68 = 0x320;
                }
            }
        }
    }
}

void PolarHeroSlideFromCentre(s32 player) {
    HeroInfo *hero;
    EXTheromove *move;
    HeroInfo *base;
    s32 angle;

    base = Heroes;
    hero = &base[player];
    move = hero->hero_move;
    if (move != NULL) {
        angle = (get_angle(hero->hero_eng->unk10.x - PolarCentre.vx,
                           hero->hero_eng->unk10.z - PolarCentre.vz) + 0x800) & 0xFFF;
        move->forward_angle = angle + 0x800;
        move->x_max = 0;
        move->z_max = 0;
        move->x_vel = (rcossin_tbl[(angle - 0x400) & 0xFFF].unk2 * 0x25) >> 11;
        move->z_vel = (rcossin_tbl[angle].unk2 * 0x25) >> 11;
        hero->hero_eng->unk10.x += move->x_vel;
        hero->hero_eng->unk10.z += move->z_vel;
    }
}

void PolarHeroOffice(s32 player) {
    HeroInfo *h;
    HeroInfo *h2;
    Ext20 *ext;
    EXTheromove *move;
    EXTengine *eng;
    s32 angle;
    u16 mode;

    if (TestPolarHeroWon() != 0) {
        return;
    }
    {
        HeroInfo *b = Heroes;
        h = &b[player];
    }
    ext = FindExtension(h->hero, 0x20000000);
    if (ext == NULL) {
        return;
    }
    if (GameMode == 0x16) {
        PolarHeroCollisions(player);
    }
    mode = h->hero_last_mode;
    if (mode == 0x4C || mode == 0x52 || ext->state == 1 || mode == 0x4B ||
        mode == 0x5A || ext->unkAC != 0) {
        {
            HeroInfo *b = Heroes;
            h2 = &b[player];
        }
        h2->hero_eng->unk10.y += 0x60;
        ext->unkAC = 1;
        RemoveCamera(h2->hero);
        if (!(h2->hero_status & 0x200)) {
            h2->hero_status |= 0x2000;
        }
        if (h2->hero_eng->unk10.y > 0x400) {
            SetHeroAction(player, 0x50);
        }
        return;
    }
    move = h->hero_move;
    if (move == NULL) {
        return;
    }
    eng = h->hero_eng;
    angle = (get_angle(eng->unk10.x - PolarCentre.vx, eng->unk10.z - PolarCentre.vz) + 0x800) & 0xFFF;
    if (ext->unk18 == 0) {
        ext->unk18 = csqrt(move->x_vel * move->x_vel + move->z_vel * move->z_vel) >> 6;
    }
    move->forward_angle = angle + 0x800;
    move->x_max = 0;
    move->z_max = 0;
    move->x_vel = 0;
    move->z_vel = 0;
    if (((PolarPad *)Pads)[player].unk54 & 0x8000) {
        ext->unk14++;
    }
    if (h->hero_anim->Aindex < 0x21) {
        if (ext->unk18 < 0x4A && ext->unk14 >= 5) {
            do {
                PolarHeroSlideFromCentre(player);
                ext->unk18 = 0;
                if (h->hero_eng->sfx != NULL) {
                    StopSfx(h->hero_eng->sfx);
                }
                SetHeroAction(player, 0x49);
            } while (0);
            return;
        }
    } else if (!(h->hero_status & 0x200)) {
        h->hero_status |= 0x2000;
    }
    ProcessHeroAnim(player);
}

void SetPolarHeroInBeam(s32 player) {
    Ext20 *ext;
    EXTheromove *m;

    Heroes[player].hero_eng->sub->flags &= 0xFFBFFFFF;
    ext = FindExtension(Heroes[player].hero, 0x20000000);
    if (ext != NULL) {
        if (ext->state == 0) {
            SetHeroAction(player, 0x49);
        }
        if (ext->state == 1) {
            SetHeroAction(player, 0x54);
        }
    }
    PolarShadowState[player] = 0x4000;
    Heroes[player].hero_eng->sub->unk68 = 0;
    m = Heroes[player].hero_move;
    if (m != NULL) {
        m->z_dir = 0;
        m->x_max = 0;
        m->z_max = 0;
        m->x_vel = 0;
        m->z_vel = 0;
    }
}

void SetPolarHeroPoll(s32 player) {
    HeroInfo *base;
    HeroInfo *h;

    base = Heroes;
    h = &base[player];
    if (h->hero_shadow != NULL) {
        h->hero_shadow->sub->flags &= ~0x8000;
    }
    if (PolarRoundState == 0) {
        PlaySfx(0x45D, NULL, NULL, 0x1000, 0x1E00, 0);
    }
    RemoveCamera(h->hero);
    h->hero_eng->sub->flags &= ~0x8000;
    if (!(h->hero_status & 0x200)) {
        h->hero_status |= 0x8000;
        if (!(h->hero_status & 0x200)) {
            h->hero_status |= 0x2000;
        }
    }
    if (GameMode == 0x15) {
        FindExtension(Heroes[player].hero, 0x20000000);
    }
}

s32 PolarHeroAttackPower(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *info = base + player;
    s32 *ext = FindExtension(info->hero, 0x20000000);

    if (ext != NULL) {
        if (ext[9] != 0) {
            return 0x80;
        }
        switch (info->hero_name) {
        case 3:
        case 4:
            return 0x100;
        case 5:
        case 6:
            return 0xB4;
        case 7:
        case 8:
        default:
            return 0x80;
        }
    }
    return 0x80;
}

s32 PolarTestAttack(s32 player) {
    HeroInfo *base;
    HeroInfo *h;
    Ext20 *ext;
    PolarPad *pads;
    PolarPad *pad;

    base = Heroes;
    h = base + player;
    ext = FindExtension(h->hero, 0x20000000);

    if (ext != NULL && h->hero_move != NULL && ext->val >= PolarHeroAttackPower(player)) {
        pads = (PolarPad *)Pads;
        pad = pads + player;
        if (pad->unk54 & 0x8000) {
            if (h->hero_mode != 0x51) {
                SetHeroAction(player, 0x51);
            }
            return 1;
        } else {
            pad->unk50 &= ~0x8000;
            return 0;
        }
    }
    return 0;
}

s32 PolarHeroThrowPower(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *info = base + player;
    s32 *ext = FindExtension(info->hero, 0x20000000);

    if (ext != NULL) {
        if (ext[9] != 0) {
            return 0x46;
        }
        switch (info->hero_name) {
        case 7:
        case 8:
            return 0x4B;
        case 5:
        case 6:
            return 0x69;
        case 3:
        case 4:
            return 0x82;
        default:
            return 0x5A;
        }
    }
    return 0;
}

void SetPolarHeroAttack(s32 player) {
    HeroInfo *h;
    HeroInfo *h2;
    EXTheromove *move;
    Ext20 *ext;
    HeroInfo *base;
    HeroInfo *base2;

    base = Heroes;
    h = &base[player];
    if (h->hero_eng->sfx != NULL) {
        StopSfx(h->hero_eng->sfx);
    }
    ext = FindExtension(h->hero, 0x20000000);
    if (ext != NULL && TestPolarHeroWon(player) == 0) {
        if (ext->state == 1) {
            switch (h->hero_name) {
            case 1:
                PlaySfx(0x1A, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 2:
                PlaySfx(0x4C, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 3:
                PlaySfx(0x80, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 4:
                PlaySfx(0xB0, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 7:
                PlaySfx(0x114, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 8:
                PlaySfx(0xE2, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 5:
                PlaySfx(0x146, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 6:
                PlaySfx(0x178, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            }
        } else {
            switch (h->hero_name) {
            case 1:
                PlaySfx(0x16, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 2:
                PlaySfx(0x48, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 3:
                PlaySfx(0x7A, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 4:
                PlaySfx(0xAC, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 7:
                PlaySfx(0x110, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 8:
                PlaySfx(0xDE, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 5:
                PlaySfx(0x142, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            case 6:
                PlaySfx(0x174, NULL, NULL, 0x1000, 0x1E00, 0);
                break;
            }
        }
        base2 = Heroes;
        h2 = base2 + player;
        move = h2->hero_move;
        if (move != NULL) {
            if (ext->state == 0) {
                SetHeroSeq(player, 0x3E);
            }
            if (ext->state == 1) {
                SetHeroSeq(player, 0x45);
            }
            PolarShadowState[player] = 0x4080;
            h2->hero_eng->sub->unk68 = 0;
            ext->val -= PolarHeroAttackPower(player);
            move->x_vel = 0;
            move->z_vel = 0;
            move->x_max = 0;
            move->z_max = 0;
        }
    }
}

void PolarHeroAttackThrow(s32 player) {
    HeroInfo *h;
    HeroInfo *hero;
    EXTheromove *move;
    Ext20 *ext;
    NAS *nas;

    {
        HeroInfo *base = Heroes;
        h = base + player;
    }
    move = h->hero_move;
    if (move == NULL) {
        return;
    }
    ext = FindExtension(h->hero, 0x20000000);
    if (ext == NULL) {
        return;
    }
    if (ext->state == 0) {
        switch (h->hero_name) {
        case 3:
        case 4:
            {
                HeroInfo *base = Heroes;
                hero = base + player;
            }
            hero->hero_frozen = 0;
            if (hero->hero_anim->Aindex == 0xA) {
                POLAR_AIM(move, player);
                move->x_max = 0;
                move->x_acc = 6;
                move->x_dec = 3;
                move->z_acc = 6;
                move->z_dec = 3;
                move->z_max = 0;
                {
                    EXTengine *eng = hero->hero_eng;
                    hero->hero_frozen = 2;
                    nas = CreateNAS(eng->unk10.x, eng->unk10.y - ((eng->unk2C.x * 0x1A4) >> 12), eng->unk10.z, 0x1616, 0x11);
                }
                ext->unkA4 = nas;
                if (nas != NULL) {
                    nas->UserInfo.user1 = (s32)hero;
                }
            }
            return;
        case 7:
            {
                HeroInfo *base = Heroes;
                hero = base + player;
            }
            if (hero->hero_anim->Aindex == 1) {
                POLAR_AIM(move, player);
            }
            if (hero->hero_anim->Aindex == 7) {
                {
                    EXTengine *eng = hero->hero_eng;
                    nas = CreateNAS(eng->unk10.x, eng->unk10.y - ((eng->unk2C.x * 0x1A4) >> 12), eng->unk10.z, 0x1615, 0x11);
                }
                ext->unkA4 = nas;
                if (nas != NULL) {
                    nas->UserInfo.user1 = (s32)hero;
                    if (GameMode == 0x14) {
                        ext->unkA4->Engine->sub->u6C.dbase = &HeroModelBank[DB_CharsPolarDingo];
                        ext->unkA4->Engine->sub->entity = 0x2002;
                    }
                }
            }
            return;
        case 8:
            {
                HeroInfo *base = Heroes;
                hero = base + player;
            }
            if (hero->hero_anim->Aindex == 1) {
                POLAR_AIM(move, player);
            }
            if (hero->hero_anim->Aindex == 7) {
                {
                    EXTengine *eng = hero->hero_eng;
                    nas = CreateNAS(eng->unk10.x, eng->unk10.y - ((eng->unk2C.x * 0xB4) >> 12), eng->unk10.z, 0x1615, 0x11);
                }
                ext->unkA4 = nas;
                if (nas != NULL) {
                    nas->UserInfo.user1 = (s32)hero;
                    if (GameMode == 0x14) {
                        ext->unkA4->Engine->sub->u6C.dbase = &HeroModelBank[DB_CharsPolarRilla];
                    } else {
                        ext->unkA4->Engine->sub->u6C.dbase = &HeroModelBank[DB_CharsPolar2Rilla];
                    }
                    ext->unkA4->Engine->sub->entity = 0x2003;
                }
            }
            return;
        case 5:
        case 6:
            {
                HeroInfo *base = Heroes;
                h = base + player;
            }
            if (h->hero_anim->Aindex == 6) {
                POLAR_AIM(move, player);
                move->x_max = 0;
                move->z_max = 0;
            }
            if (h->hero_anim->Aindex < 6) {
                move->x_vel = 0;
                move->z_vel = 0;
                move->x_max = 0;
                move->z_max = 0;
            }
            return;
        }
    }
    if (h->hero_anim->Aindex == 1) {
        POLAR_AIM(move, player);
    }
}

s32 PolarTestSnowball(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *info = base + player;
    s32 *ext = FindExtension(info->hero, 0x20000000);
    PolarPad *pads;
    PolarPad *pad;

    if (ext != NULL) {
        if (ext[10] == 0) {
            return 0;
        }
        if (info->hero_move != NULL) {
            pads = (PolarPad *)Pads;
            pad = pads + player;
            if (pad->unk54 & 0x2000) {
                if (info->hero_mode != 0x53) {
                    SetHeroAction(player, 0x53);
                }
                return 1;
            }
            pad->unk50 &= ~0x2000;
            return 0;
        }
    }
    return 0;
}

void SetPolarHeroThrowBomb(s32 player) {
    SLONG_XYZ pos;
    SLONG_XYZ to;
    HeroInfo *base;
    HeroInfo *base2;
    HeroInfo *h;
    HeroInfo *h2;
    EXTengine *eng;
    s32 *ext;
    NAS *nas;
    PolarThrown *t;
    s32 dist;

    base = Heroes;
    h = &base[player];
    ext = FindExtension(h->hero, 0x20000000);
    if (ext != NULL) {
    dist = 0;
    pos = h->hero_eng->unk10;
    pos.y -= 0x100;
    nas = CreateNAS(pos.x, pos.y, pos.z, 0x160D, 0x11);
    if (nas != NULL) {
    t = FindExtension(nas, 0x10000000);
    if (t != NULL) {
    t->start = pos;
    t->owner = player;
    ext[10]--;
    nas->Engine->unk1E = base[t->owner].hero_eng->unk1E;
    to.y = h->hero_eng->unk10.y;
    switch (h->hero_name) {
    case 1:
        PlaySfx(0x17, NULL, NULL, 0x1000, 0x1E00, 0);
        dist = 0x700;
        t->unk74 = 0x56;
        break;
    case 2:
        PlaySfx(0x49, NULL, NULL, 0x1000, 0x1E00, 0);
        dist = 0x700;
        t->unk74 = 0x56;
        break;
    case 3:
        PlaySfx(0x7B, NULL, NULL, 0x1000, 0x1E00, 0);
        dist = 0x700;
        t->unk74 = 0x56;
        break;
    case 4:
        PlaySfx(0xAD, NULL, NULL, 0x1000, 0x1E00, 0);
        dist = 0x700;
        t->unk74 = 0x56;
        break;
    case 5:
        PlaySfx(0x143, NULL, NULL, 0x1000, 0x1E00, 0);
        dist = 0x800;
        t->unk74 = 0x6A;
        break;
    case 6:
        PlaySfx(0x175, NULL, NULL, 0x1000, 0x1E00, 0);
        dist = 0x800;
        t->unk74 = 0x6A;
        break;
    case 7:
        PlaySfx(0x111, NULL, NULL, 0x1000, 0x1E00, 0);
        dist = 0x800;
        t->unk74 = 0x6A;
        break;
    case 8:
        PlaySfx(0xDF, NULL, NULL, 0x1000, 0x1E00, 0);
        dist = 0x800;
        t->unk74 = 0x6A;
        break;
    }
    base2 = Heroes;
    h2 = &base2[player];
    eng = h2->hero_eng;
    to.x = eng->unk10.x - (dist * rcossin_tbl[(eng->unk1E - 0x400) & 0xFFF].unk2 >> 12);
    eng = h2->hero_eng;
    to.z = eng->unk10.z - (dist * rcossin_tbl[eng->unk1E & 0xFFF].unk2 >> 12);
    t->pos = to;
    }
    }
    }
}

#define SET_ENTITY(v)                                              \
    *(s16 *)((u8 *)Heroes[player].hero_eng->sub + 0x74) = (v); \
    if (GameLevelFlags & 0x80) {                                  \
        Heroes[player].hero_eng->sub->entity = (v);            \
    }

void SetPolarHeroSnowball(s32 player) {
    HeroInfo *base;
    HeroInfo *h;

    base = Heroes;
    h = &base[player];
    if (h->hero_move != NULL) {
        h->hero_move->z_dir = 0;
    }
    if (TestPolarHeroWon(player) != 0) {
        return;
    }
    PolarShadowState[player] = 0x4100;
    h->hero_eng->sub->unk68 = 0;
    switch (h->hero_name) {
    case 1:
        SET_ENTITY(0x2004);
        break;
    case 2:
        SET_ENTITY(0x2006);
        break;
    case 3:
        SET_ENTITY(0x2007);
        break;
    case 5:
        SET_ENTITY(0x2009);
        break;
    case 6:
        SET_ENTITY(0x200A);
        break;
    case 4:
        SET_ENTITY(0x2005);
        break;
    case 7:
        SET_ENTITY(0x2008);
        break;
    case 8:
        SET_ENTITY(0x200B);
        break;
    default:
        *(s16 *)((u8 *)h->hero_eng->sub + 0x74) = 0x2004;
        if (GameLevelFlags & 0x80) {
            h->hero_eng->sub->entity = 0x2004;
        }
        break;
    }
    Heroes[player].hero_eng->sub->u6C.dbase = MapScene->dbase;
    Heroes[player].hero_eng->unk5D = 2;
}

void PolarHeroSnowball(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *info = base + player;
    EXTheromove *move;
    Ext20 *ext;
    s32 speed;

    move = info->hero_move;
    if (move != NULL && TestPolarHeroWon() == 0) {
        speed = csqrt(move->x_vel * move->x_vel + move->z_vel * move->z_vel) >> 6;
        ext = FindExtension(info->hero, 0x20000000);
        if (ext != NULL) {
            ext->val += 4;
            if (ext->val > 0x100) {
                ext->val = 0x100;
            }
        }
        PolarHeroCollisions(player);
        Heroes[player].hero_eng->unk1C += speed;
        TestPolarHeroMelted(player);
        PolarIceHit(&Heroes[player].hero_eng->unk10, player);
        Heroes[player].hero_eng->unk5E = 1;
    }
}

void SetPolarHeroFootIdle(s32 player) {
    HeroInfo *info;
    HeroInfo *h;
    EXTheromove *move;
    Ext20 *ext;
    s32 rnd;
    s32 sfx;
    s32 speed;
    s32 off;
    u8 *base;

    rnd = Random(6);
    off = player * sizeof(HeroInfo);
    info = (HeroInfo *)((u8 *)Heroes + off);
    sfx = 0;
    info->hero_eng->sub->unk68 = 0;
    speed = 0;
    if (info->hero_eng->sfx != NULL) {
        StopSfx(info->hero_eng->sfx);
    }
    ext = FindExtension(info->hero, 0x20000000);
    if (ext != NULL) {
        ext->unk14 = 0;
        ext->state = 1;
    }
    info->hero_eng->unk5D = 0;
    info->hero_eng->unk1C = 0;
    move = info->hero_move;
    if (move != NULL) {
        speed = csqrt(move->x_vel * move->x_vel + move->z_vel * move->z_vel) >> 6;
        move->z_dir = 0;
        move->x_acc = 4;
        move->x_dec = 2;
        move->z_acc = 4;
        move->z_dec = 2;
    }
    if (info->hero_name == 1) {
        info->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsCrate2Crash];
    }
    if (info->hero_name == 2) {
        info->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsCrate2Coco];
    }
    if (info->hero_name == 5) {
        info->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsCrate2Tiny];
    }
    if (info->hero_name == 3) {
        info->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsCrate2Cortex];
    }
    if (info->hero_name == 7) {
        info->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsCrate2Dingo];
    }
    if (info->hero_name == 4) {
        info->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsCrate2Brio];
    }
    base = (u8 *)Heroes;
    if (info->hero_name == 6) {
        info->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsCrate2Kong];
    }
    h = (HeroInfo *)(base + off);
    if (h->hero_name == 8) {
        h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsCrate2Rilla];
    }
    SetHeroSeq(player, 0x42);
    if (rnd == 0) {
        sfx = 0x456;
    }
    if (rnd == 1) {
        sfx = 0x457;
    }
    if (rnd == 2) {
        sfx = 0x458;
    }
    if (rnd == 3) {
        sfx = 0x459;
    }
    if (rnd == 4) {
        sfx = 0x45A;
    }
    if (rnd == 5) {
        sfx = 0x45B;
    }
    if (speed > 0x20) {
        PlaySfx(sfx, &h->hero_eng->sfx, NULL, 0x1000, Random(0x100) + 0x1D80, 0);
    }
}

void PolarHeroFootIdle(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *info = base + player;
    Ext20 *ext;
    Ext20 *ext1;
    Ext20 *ext2;

    ext1 = FindExtension(info->hero, 0x20000000);
    if (ext1 != NULL) {
        ext1->state = 1;
    }
    if (TestPolarHeroWon(player) == 0) {
        ext2 = FindExtension(info->hero, 0x20000000);
        if (ext2 != NULL) {
            ext2->val += 4;
            if (ext2->val > 0x100) {
                ext2->val = 0x100;
            }
        }
        PolarHeroCollisions(player);
        ProcessHeroAnim(player);
        ext = FindExtension(Heroes[player].hero, 0x20000000);
        if (ext != NULL && (((PolarPad *)Pads)[player].unk50 & 0xF0)) {
            if (ext->state == 0) {
                SetHeroAction(player, 0x4A);
            }
            if (ext->state == 1) {
                SetHeroAction(player, 0x55);
            }
        }
        PolarTestAttack(player);
        PolarTestSnowball(player);
        TestPolarHeroMelted(player);
        PolarIceHit(&Heroes[player].hero_eng->unk10, player);
    }
}

void PolarHeroFootRun(s32 player) {
    HeroInfo *h;
    HeroInfo *base;
    Ext20 *ext;
    Ext20 *ext1;

    if (TestPolarHeroWon() == 0) {
        ext1 = FindExtension(Heroes[player].hero, 0x20000000);
        if (ext1 != NULL) {
            ext1->val += 4;
            if (ext1->val > 0x100) {
                ext1->val = 0x100;
            }
        }
        PolarHeroCollisions(player);
        base = Heroes;
        h = &base[player];
        if (h->hero_anim->Aindex == 4) {
            if (h->hero_move->mass != 0x50) {
                if (player == 0) {
                    PlaySfx(0x49C, &base[0].hero_eng->sfx, &base[0].hero_eng->unk10, 0x1000, 0x1E00, 0);
                }
                if (player == 1) {
                    PlaySfx(0x49D, &base[1].hero_eng->sfx, &base[1].hero_eng->unk10, 0x1000, 0x1E00, 0);
                }
                if (player == 2) {
                    PlaySfx(0x49E, &base[2].hero_eng->sfx, &base[2].hero_eng->unk10, 0x1000, 0x1E00, 0);
                }
                if (player == 3) {
                    PlaySfx(0x49F, &base[3].hero_eng->sfx, &base[3].hero_eng->unk10, 0x1000, 0x1E00, 0);
                }
            } else {
                PlaySfx(0x4A0, &h->hero_eng->sfx, &h->hero_eng->unk10, 0x1000, 0x1E00, 0);
            }
        }
        ProcessHeroAnim(player);
        if (!(((PolarPad *)Pads)[player].unk50 & 0xF0)) {
            ext = FindExtension(Heroes[player].hero, 0x20000000);
            if (ext != NULL) {
                if (ext->state == 0) {
                    SetHeroAction(player, 0x49);
                }
                if (ext->state == 1) {
                    SetHeroAction(player, 0x54);
                }
            }
        }
        PolarTestAttack(player);
        PolarTestSnowball(player);
        TestPolarHeroMelted(player);
        PolarIceHit(&Heroes[player].hero_eng->unk10, player);
    }
}

void SetPolarHeroHit(s32 player) {
    HeroInfo *h;
    Ext20 *ext;
    HeroInfo *base = Heroes;

    h = &base[player];
    if (h->hero_name == 1) {
        h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsCrate2Crash];
    }
    if (h->hero_name == 2) {
        h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsCrate2Coco];
    }
    if (h->hero_name == 5) {
        h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsCrate2Tiny];
    }
    if (h->hero_name == 3) {
        h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsCrate2Cortex];
    }
    if (h->hero_name == 7) {
        h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsCrate2Dingo];
    }
    if (h->hero_name == 4) {
        h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsCrate2Brio];
    }
    if (h->hero_name == 6) {
        h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsCrate2Kong];
    }
    if (h->hero_name == 8) {
        h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsCrate2Rilla];
    }
    h->hero_eng->sub->flags &= 0xFFBFFFFF;
    ext = FindExtension(h->hero, 0x20000000);
    if (ext != NULL) {
        if (ext->state == 0) {
            ext->state = 1;
        } else if (!(h->hero_status & 0x200)) {
            h->hero_status |= 0x2000;
        }
        SetHeroSeq(player, 0x3F);
        pad_rumble_small(player, 0xA);
        ((void (*)(s32, s32, s32))pad_rumble_big)(player, 0xA, 0xFF);
    }
}

void SetPolarHeroDead(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;
    s32 *ext;
    u16 status;
    EXTengine *eng;

    ext = FindExtension(h->hero, 0x20000000);
    if (ext != NULL) {
        ext[0x9C / 4] = 0;
        ext[0x98 / 4] = 0;
        h->hero_eng->unk5D = 2;
        status = h->hero_status;
        if (!(status & 0x200)) {
            h->hero_status = status | 0x2000;
        }
        h->hero_eng->sub->flags |= 0x8000;
        h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
        h->hero_eng->sub->entity = 0x202D;
        eng = h->hero_eng;
        CreateNAS(eng->unk10.x + 0x80, eng->unk10.y - 0x200, eng->unk10.z, 0x1613, 0x11);
    }
}

void PolarHeroDead(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + player;
    s32 *ext;
    s32 *p;
    EXTheromove *move;
    EXTengine *eng;
    NAS *nas;
    s32 z;
    s32 y;
    s32 x;
    s32 t;

    ext = FindExtension(h->hero, 0x20000000);
    if (ext != NULL) {
        move = h->hero_move;
        if (move != NULL) {
            move->z_vel = 0;
            move->x_vel = 0;
            move->z_max = 0;
        }
        eng = h->hero_eng;
        t = ext[0x9C / 4] + 1;
        x = eng->unk10.x;
        y = eng->unk10.y;
        z = eng->unk10.z;
        ext[0x9C / 4] = t;
        if (t == 4) {
            CreateNAS(x - 0x80, y - 0x100, z, 0x1613, 0x11);
            CreateNAS(x, y - 0x100, z, 0x1614, 0x11);
        }
        if (ext[0x9C / 4] == 2) {
            CreateNAS(x + 0x80, y - 0x180, z, 0x1613, 0x11);
        }
        if (ext[0x9C / 4] == 0x3C) {
            SetHeroAction(player, 0x50);
        }
        if (--ext[0x98 / 4] < 0) {
            ext[0x98 / 4] = Random(7) + 1;
            nas = CreateNAS(x + 0x40, y - 0x40, z, 0x1612, 0x11);
            if (nas != NULL) {
                p = FindExtension(nas, 0x10000000);
                if (p != NULL) {
                    p[0x1C / 4] = 0xA;
                    p[0xC / 4] = 0x400;
                    p[0x10 / 4] = 0x50;
                    p[0x14 / 4] = -0x40;
                    p[0x24 / 4] = 0x20;
                    p[0x28 / 4] = 1;
                    p[0x18 / 4] |= 1;
                }
            }
            nas = CreateNAS(x - 0x40, y - 0x40, z, 0x1612, 0x11);
            if (nas != NULL) {
                p = FindExtension(nas, 0x10000000);
                if (p != NULL) {
                    p[0x1C / 4] = 0xA;
                    p[0xC / 4] = 0x258;
                    p[0x10 / 4] = 0x50;
                    p[0x14 / 4] = -0x40;
                    p[0x24 / 4] = 0x20;
                    p[0x28 / 4] = 1;
                    p[0x18 / 4] |= 1;
                }
            }
        }
    }
}

void SetPolarHeroWinner(s32 player) {
    HeroInfo *tab;
    HeroInfo *first;
    HeroInfo *h;
    HeroInfo *base;
    EXTengine *shadow;
    PolarGameExt *game;
    Ext20 *ext;
    s32 i;
    s32 lost;

    tab = Heroes;
    first = &tab[player];
    first->hero_eng->unk1C = 0;
    first->hero_eng->unk5E = 1;
    first->hero_eng->unk5E = 1;
    first->hero_eng->unk2C.x = 0x1000;
    first->hero_eng->unk2C.y = 0x1000;
    first->hero_eng->unk2C.z = 0x1000;
    shadow = first->hero_shadow;
    if (shadow != NULL) {
        shadow->unk2C.x = 0x1000;
        shadow->unk2C.y = 0x1000;
        shadow->unk2C.z = 0x1000;
        shadow->unk5E = 1;
    }
    first->hero_eng->sub->flags &= 0xFFBFFFFF;
    if (PolarRoundState != 0 && PolarBeamOwner != NULL) {
        game = FindExtension((NAS *)PolarBeamOwner, 0x40000000);
        i = 0;
        if (game != NULL) {
            for (; i < 4; i++) {
                game->unk78[i]->flags &= 0xFFFF7FFF;
            }
            game->unk74->flags &= 0xFFFF7FFF;
        }
    }
    base = Heroes;
    h = &base[player];
    ext = FindExtension(h->hero, 0x20000000);
    if (ext == NULL) {
        return;
    }
    if (ext->state == 0) {
        if (GameMode != 0x14) {
            if (h->hero_name == 1) {
                h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolar2Crash];
            }
            if (h->hero_name == 2) {
                h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolar2Coco];
            }
            if (h->hero_name == 5) {
                h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolar2Tiny];
            }
            if (h->hero_name == 3) {
                h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolar2Cortex];
            }
            if (h->hero_name == 7) {
                h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolar2Dingo];
            }
            if (h->hero_name == 8) {
                h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolar2Rilla];
            }
            if (h->hero_name == 4) {
                h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolar2Brio];
            }
            if (h->hero_name == 6) {
                h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolar2Kong];
            }
        } else {
            if (h->hero_name == 1) {
                h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolarCrash];
            }
            if (h->hero_name == 2) {
                h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolarCoco];
            }
            if (h->hero_name == 5) {
                h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolarTiny];
            }
            if (h->hero_name == 3) {
                h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolarCortex];
            }
            if (h->hero_name == 7) {
                h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolarDingo];
            }
            if (h->hero_name == 8) {
                h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolarRilla];
            }
            if (h->hero_name == 4) {
                h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolarBrio];
            }
            if (h->hero_name == 6) {
                h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsPolarKong];
            }
        }
    } else {
        if (h->hero_name == 1) {
            h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsCrate2Crash];
        }
        if (h->hero_name == 2) {
            h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsCrate2Coco];
        }
        if (h->hero_name == 5) {
            h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsCrate2Tiny];
        }
        if (h->hero_name == 3) {
            h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsCrate2Cortex];
        }
        if (h->hero_name == 7) {
            h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsCrate2Dingo];
        }
        if (h->hero_name == 4) {
            h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsCrate2Brio];
        }
        if (h->hero_name == 6) {
            h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsCrate2Kong];
        }
        if (h->hero_name == 8) {
            h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_CharsCrate2Rilla];
        }
    }
    lost = TestPolarHeroMelted(player) ^ 1;
    if (lost) {
        RemoveCamera(Heroes[player].hero);
    }
    if (Heroes[player].hero_eng->unk10.y > 0x60) {
        RemoveCamera(Heroes[player].hero);
    }
    if (ext->state == 0) {
        SetHeroSeq(player, 0x3A);
        RequestHeroAnim(player, 0);
    } else {
        SetHeroSeq(player, 0x42);
        RequestHeroAnim(player, 1);
    }
    StartHeroVictory(player);
}

void PolarHeroWinner(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = base + player;
    EXTheromove *move;

    hero->hero_eng->unk5E = 1;
    hero->hero_eng->unk2C.x = 0x1000;
    hero->hero_eng->unk2C.y = 0x1000;
    hero->hero_eng->unk2C.z = 0x1000;
    move = hero->hero_move;
    if (move != NULL) {
        hero->hero_eng->unk1C = 0;
        hero->hero_eng->unk5E = 1;
        CheckHeroAnim(player);
        move = hero->hero_move;
        hero->hero_frozen = 0x14;
        if (move != NULL) {
            move->x_max = 0;
            move->z_max = 0;
            move->x_vel = 0;
            move->z_vel = 0;
        }
        if (UpdateHeroVictory(player) != 0) {
            FinishLevel();
        }
        ProcessHeroAnim(player);
    }
}

void PolarHeroSquashed(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *hero = base + player;
    EXTheromove *move;

    if (FindExtension(hero->hero, 0x20000000) != NULL) {
        move = hero->hero_move;
        if (move != NULL) {
            move->z_vel = 0;
            move->x_vel = 0;
            move->z_max = 0;
        }
        SetHeroAction(player, 0x50);
    }
}

void SetPolarHeroSquashed(s32 arg0) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + arg0;
    u16 status;

    if (FindExtension(h->hero, 0x20000000) != NULL) {
        h->hero_eng->unk5D = 2;
        status = h->hero_status;
        if (!(status & 0x200)) {
            h->hero_status = status | 0x2000;
        }
    }
}

s32 TestPolarHeroWon(arg0)
s32 arg0;
{
    HeroInfo *base = Heroes;
    HeroInfo *hero = &base[arg0];

    if (hero->hero_status & 0x200) {
        if (hero->hero_mode != 0x58) {
            SetHeroAction(arg0, 0x58);
        }
        return 1;
    }
    return 0;
}

void PolarHeroHit(s32 player) {
    ProcessHeroAnim(player);
    TestPolarHeroMelted(player);
    PolarHeroCollisions(player);
    PolarIceHit(&Heroes[player].hero_eng->unk10, player);
}

void SetPolarHeroFootRun(s32 player) {
    HeroInfo *base;
    HeroInfo *hero;

    base = Heroes;
    hero = base + player;
    if (hero->hero_eng->sfx != NULL) {
        StopSfx(hero->hero_eng->sfx);
    }
    if (hero->hero_move != NULL) {
        hero->hero_move->z_dir = 0;
    }
    SetHeroSeq(player, 0x43);
    PolarShadowState[player] = 0x4100;
    hero->hero_eng->sub->unk68 = 0;
}

void PolarHeroThrowBomb(s32 player) {
    s32 *ext = FindExtension(Heroes[player].hero, 0x20000000);

    if (ext != NULL) {
        if (ext[9] == 0) {
            SetHeroAction(player, 0x49);
        }
        if (ext[9] == 1) {
            SetHeroAction(player, 0x54);
        }
        ProcessHeroAnim(player);
    }
}

void PolarHeroAttack(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *info = base + player;
    s32 *ext = FindExtension(info->hero, 0x20000000);

    if (ext != NULL && TestPolarHeroWon(player) == 0 && info->hero_move != NULL) {
        PolarHeroCollisions(player);
        PolarHeroAttackThrow(player);
        ext[4] -= 0xF;
        if (ext[4] < 0) {
            ext[4] = 0;
        }
        ProcessHeroAnim(player);
        TestPolarHeroMelted(player);
        PolarIceHit(&info->hero_eng->unk10, player);
    }
}

void PolarHeroPoll(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *info = base + player;
    EXTheromove *move = info->hero_move;

    if (move != NULL) {
        move->x_max = 0;
        move->z_max = 0;
        move->x_vel = 0;
        move->z_vel = 0;
    }
    info->hero_frozen = 4;
    TestPolarHeroWon();
}

void PolarHeroInBeam(s32 player) {
    PadState *base;
    PolarPad *pad;

    base = Pads;
    pad = (PolarPad *)(base + player);
    pad->unk50 = 0;
    pad->unk54 = 0;
    ProcessHeroAnim(player);
}

void PolarHeroWeight(s32 player) {
    u8 *g = GameInfo;
    EXTheromove *mv;

    if (*(s16 *)(g + 0x34) != 0x16) {
        PadState *b = Pads;
        PadState *p = &b[player];
        *(s32 *)((u8 *)p + 0x50) = 0;
        *(s32 *)((u8 *)p + 0x54) = 0;
    }
    mv = Heroes[player].hero_move;
    if (mv != NULL) {
        mv->z_dir = 0;
        mv->x_max = 0;
        mv->z_max = 0;
        mv->x_vel = 0;
        mv->z_vel = 0;
    }
    if (*(s16 *)(g + 0x34) != 0x16) {
        TestPolarHeroWon(player);
    }
}

void SetPolarHeroWeight(void) {
    if (GameMode != 0x16) {
        TestPolarHeroWon();
    }
}

void SetPolarHeroRun(s32 player) {
    HeroInfo *base;
    HeroInfo *hero;

    if (TestPolarHeroWon() == 0) {
        base = Heroes;
        hero = base + player;
        if (hero->hero_move != NULL) {
            hero->hero_move->z_dir = 0;
        }
        if (hero->hero_eng->sfx != NULL) {
            StopSfx(hero->hero_eng->sfx);
        }
        SetHeroSeq(player, 0x3B);
        PolarShadowState[player] = 0x4100;
        hero->hero_eng->sub->unk68 = 0;
    }
}

void PolarHeroAddWeight(s32 arg0) {
    Ext20 *ext = FindExtension(Heroes[arg0].hero, 0x20000000);

    if (ext != NULL) {
        ext->val += 4;
        if (ext->val > 0x100) {
            ext->val = 0x100;
        }
    }
}

s32 TestPolarHeroStop(s32 player) {
    Ext20 *ext;

    if (((PolarPad *)Pads)[player].unk50 & 0xF0) {
        return 0;
    }
    ext = FindExtension(Heroes[player].hero, 0x20000000);
    if (ext != NULL) {
        if (ext->state == 0) {
            SetHeroAction(player, 0x49);
        }
        if (ext->state == 1) {
            SetHeroAction(player, 0x54);
        }
    }
    return 1;
}

s32 TestPolarHeroRun(s32 player) {
    Ext20 *ext;

    ext = FindExtension(Heroes[player].hero, 0x20000000);
    if (ext != NULL) {
        if (!(((PolarPad *)Pads)[player].unk50 & 0xF0)) {
            return 0;
        }
        if (ext->state == 0) {
            SetHeroAction(player, 0x4A);
        }
        if (ext->state == 1) {
            SetHeroAction(player, 0x55);
        }
        return 1;
    }
    return 0;
}

void PolarInitHeroMove(s32 arg0) {
    HeroInfo *base = Heroes;
    s16 two;
    HeroInfo *h;
    EXTheromove *m;

    h = base + arg0;
    if ((m = h->hero_move) != NULL) {
        do {
            two = 2;
        } while (0);
        m->x_dec = 1;
        m->z_dec = 1;
        m->HitBy = -1;
        m->x_acc = two;
        m->z_acc = two;
        m->velocity_calc = 3;
        PolarHeroMass[arg0] = HeroHooks.RunSpeeds[h->hero_name * 3];
    }
}

void PolarInitHero(void) {
}

void PolarEmptyStub0(void) {
}

void PolarEmptyStub1(void) {
}

void PolarEmptyStub2(void) {
}
