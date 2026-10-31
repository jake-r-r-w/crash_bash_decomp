#include "common.h"
#include "bash/engine.h"
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
GroupFile PolarEndFiles[5] = {
    &HeroModelBank[DB_Font], &DatIndex[445], &DatIndex[444], &HeroModelBank[DB_Loading], &DatIndex[441],
    &DatIndex[440], &HeroModelBank[DB_Interface], &DatIndex[443], &DatIndex[442], &HeroModelBank[DB_PolarObjects],
    &DatIndex[439], &DatIndex[438], 0, 0, 0,
};
GroupFile PolarFiles[6] = {
    &HeroModelBank[DB_Font], &DatIndex[437], &DatIndex[436], &HeroModelBank[DB_Loading], &DatIndex[433],
    &DatIndex[432], &HeroModelBank[DB_Interface], &DatIndex[435], &DatIndex[434], &HeroModelBank[DB_PolarObjects],
    &DatIndex[431], &DatIndex[430], &HeroModelBank[DB_Polar], &DatIndex[429], &DatIndex[428],
    0, 0, 0,
};
GroupFile PolarBossFiles[8] = {
    &HeroModelBank[DB_Font], &DatIndex[427], &DatIndex[426], &HeroModelBank[DB_Loading], &DatIndex[423],
    &DatIndex[422], &HeroModelBank[DB_Interface], &DatIndex[425], &DatIndex[424], &HeroModelBank[DB_PolarObjects],
    &DatIndex[421], &DatIndex[420], &HeroModelBank[DB_Polar], &DatIndex[419], &DatIndex[418],
    &HeroModelBank[DB_RoboPolar], &DatIndex[417], &DatIndex[416], &HeroModelBank[DB_Powerups], &DatIndex[415],
    &DatIndex[414], 0, 0,
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
subengine *PolarSceneA = 0;
subengine *PolarSceneB = 0;
s32 PolarSceneWord0 = 0;
s32 PolarSceneWord1 = 0;
s32 PolarSceneWord2 = 0x100;
s32 PolarSceneWord3 = 0x10;
NAS *BombMaster = 0;
VictorySeq PolarVictorySeqs[4] = {
    { &PolarAnimWin, 0x46 },
    { &PolarAnimWin, 0x47 },
    { &PolarAnimWin, 0xD },
    { &PolarAnimWin, 0xD },
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
void *WalrusAnimList[3] = { &WalrusFrames, 0, 0 };
ScriptInit WalrusScriptInit = { 0, (void *)&WalrusAnimList, 0x402 };
s32 WalrusState = 0x240;
s32 WalrusFishSteps[17] = {
    0x4180, 0x4181, 0x4182, 0x4183, 0x4184, 0x4185, 0x4186, 0x4187, 0x4188, 0x4189, 0x418A,
    0x418B, 0x418C, 0x418D, 0x418E, 0x418F, 0x8001,
};
void *WalrusFishAnimList[3] = { &WalrusFishSteps, 0, 0 };
ScriptInit WalrusFishScriptInit = { 0, (void *)&WalrusFishAnimList, 0x402 };
s32 BearDeathSteps0[8] = { 0x4000, 0x4001, 0x4002, 0x4003, 0x4004, 0x4005, 0x4006, 0x8001 };
s32 BearDeathSteps1[2] = { 0x4006, 0x8000 };
void *BearDeathAnimList[4] = { &BearDeathSteps0, &BearDeathSteps1, 0, 0 };
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
void *PolarBossAnimList[13] = {
    &PolarBossSteps0, &PolarBossSteps1, &PolarBossSteps0, &PolarBossSteps3, &PolarBossSteps0,
    &PolarBossSteps4, &PolarBossSteps0, &PolarBossSteps5, &PolarBossSteps0, &PolarBossSteps2,
    &PolarBossSteps0, 0, 0,
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
void *BearBossAnimList[12] = {
    &BearBossSteps0, &BearBossSteps1, &BearBossSteps0, &BearBossSteps2, &BearBossSteps0,
    &BearBossSteps3, &BearBossSteps4, &BearBossSteps5, &BearBossSteps6, &BearBossSteps0,
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
void *BearminatorAnimList[7] = {
    &BearminatorSteps0, &BearminatorSteps1, &BearminatorSteps4, &BearminatorSteps3, &BearminatorSteps2,
    0, 0,
};
ScriptInit BearminatorScriptInit = { 0, (void *)&BearminatorAnimList, 0x402 };
s32 PolarAIPace[4] = {0};
s32 PolarInitWord = 0;
s32 PolarSceneCount = 0;
s32 PolarSceneTimer = 0;
MATRIX PolarSceneMatrix = {0};
s32 PolarAIFlags[7] = {0}; /* a word a hero, then the beam's mode and two more */
s32 BeamCount = 0;
s32 BeamDelay = 0;
subengine *PolarIceObjects[80] = {0};
PolarRiserObj *UkaMaskRisers[16] = {0};
subengine *UkaMaskObjects[16] = {0};
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
    subengine *obj;

    i = 0;
    PolarTiltZ = 0;
    PolarTiltX = 0;
    bossmapsub = NULL;
    PolarSceneA = NULL;
    PolarSceneTimer = 0;
    obj =((subengine *)MapScene->subs);
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
            if (obj->u6C.dbase->data != NULL) {
                entity_address(obj->entity, obj->u6C.dbase->data);
                if (((GameState *)GameInfo)->mode == 0x12) {
                    if (!(((GameState *)GameInfo)->levelFlags & 0x80)) {
                        if (obj->entity == 0x5001 || obj->entity == 0x5004) {
                            obj->flags |= 0x200000;
                        }
                    } else {
                        if (obj->entity == 0x5001 || obj->entity == 0x5004) {
                            obj->flags |= 0x200000;
                        }
                        if (obj->entity == 0x5005) {
                            PolarSceneA = obj;
                        }
                        if (obj->entity == 0x5003) {
                            PolarSceneB = obj;
                            PolarSceneCount = obj->XYZ.y;
                        }
                    }
                }
                if (((GameState *)GameInfo)->mode == 0x16 &&obj->entity == 0x5003) {
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
  NAS *p;
  EXTengine *eng;
  PolarMissileExt *ext;
  subengine *o;
  o = PolarSceneA;
  if (o != 0)
  {
    o->xform.dirty = 1;
    o->xform.order = 0;
    o->xform.rot.vy += 6;
  }
  if (PolarSceneB != 0)
  {
    PolarSceneTimer = (PolarSceneTimer + 0x20) & 0xFFF;
  }
  if (((GameState *) GameInfo)->pause != 0)
  {
    return;
  }
  if ((((GameState *) GameInfo)->mode == 0x12) || (((GameState *) GameInfo)->mode == 0x16))
  {
    sx = 0;
    sz = 0;
    if (((GameState *) GameInfo)->mode == 0x12)
    {
      i = 0;
      if (!(((GameState *) GameInfo)->levelFlags & 0x80))
      {

        for (i = 0; i < 4; i++)
        {
          if (!(Heroes[i].hero_status & 0xFA00))
          {
            k = Heroes[i].hero_move->mass;
            sx += Heroes[i].hero_eng->XYZ.x * k;
            sz += Heroes[i].hero_eng->XYZ.z * k;
          }
        }


        if ((PolarProximity != 0) && (PolarProximity->flag != 0))
        {
          r = PolarProximity->flag->mode;
          if (((r == 1) || (r == 5)) || (r == 7))
          {
            eng = PolarProximity->eng;
            sx += eng->XYZ.x << 10;
            sx >>= 8;
            sx /= 5;
            sz += eng->XYZ.z << 10;
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
      if (((GameState *) GameInfo)->teams != 0)
      {
        n = 2;
      }
      count = 0;
      for (i = 0; i < n; i++)
      {
        if (!(Heroes[i].hero_status & 0xFA00))
        {
          sx += Heroes[i].hero_eng->XYZ.x;
          sz += Heroes[i].hero_eng->XYZ.z;
        }
      }

      p = PolarBears;
      loop:
      if (p != 0)
      {
        if ((((NAS *) p)->Engine != 0) && (((NAS *) p)->Engine->XYZ.y == 0))
        {
          ext = FindExtension((NAS *) p, 0x10000000);
          if (ext != 0)
          {
            sx += ((NAS *) p)->Engine->XYZ.x;
            sz += ((NAS *) p)->Engine->XYZ.z;
            if (ext->flag != 0)
            {
              sx *= ext->flag;
              sz *= ext->flag;
              sx >>= 6;
              sz >>= 6;
              ext->flag = (ext->flag * 200) >> 8;
            }
          }
          count++;
        }
        p = p->Group.Child;
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
    if ((((GameState *) GameInfo)->levelFlags & 0x80) && (((GameState *) GameInfo)->mode == 0x12))
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
    off = *(s32 *)&((map_map *)base)->contour_grids;
    p = SELF_REL(&((s32 *)(base + 0x14 + off))[arg1 + 1]);
    if (p != NULL) {
        i = 0;
        cnt = 0;
        vp = &v;
        a2 = p + 0x50;
        a3 = p;
        ry = ((((get_angle(arg0->x, arg0->z) - 0x7D) & 0xFFF) >> 8) + 1 & 0xF) << 8;
        t = rcossin_tbl;
        n = ((MapContour *)p)->nEdges;
        for (;;) {
            s32 ang;

            if (i >= n) {
                break;
            }
            ang = ((MapContour *)a3)->edges[0].angle - ry;
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
        l = PolarBeamOwner->link;
        if (l != NULL) {
            sec = (u32)((get_angle(arg0->x, arg0->z) - 0x7D) & 0xFFF) >> 8;
            if (l->sectors[sec].hitcount == 1) {
                arg1 = 1;
            }
            if (l->sectors[sec].hitcount == 3 && l->sectors[sec].status >= 0x72 && !IsPtInALLZone(arg0, 2)) {
                return 0;
            }
            if (l->sectors[sec].hitcount == 2 && l->sectors[sec].status >= 0x72 && !IsPtInALLZone(arg0, 3)) {
                return 0;
            }
            if (l->sectors[sec].hitcount == 4 && l->sectors[sec].status >= 0x72) {
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
    off = *(s32 *)&((map_map *)base)->contour_grids;
    p = SELF_REL(&((s32 *)(base + 0x14 + off))[arg1 + 1]);
    i = 0;
    if (p != NULL) {
        n = ((MapContour *)p)->nEdges;
        t = rcossin_tbl;
        a2 = p + 0x50;
        a3 = p;
        for (;;) {
            if (i >= n) {
                return 1;
            }
            ang = ((MapContour *)a3)->edges[0].angle;  /* a3 steps an edge at a time */
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
    PolarAi *ext;
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
        dx = nas->Engine->XYZ.x - h->hero_eng->XYZ.x;
        dz = nas->Engine->XYZ.z - h->hero_eng->XYZ.z;
        if (dist <= dx * dx + dz * dz) {
            goto next;
        }
        if (ext->flag == 1) {
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
            h->hero_move->forward_angle = nas->Engine->xform.rot.vy;
            h->hero_eng->xform.order = 2;
            h->hero_move->forward_angle = 0x800;
            h->hero_eng->xform.rot.vy = 0x800;
            h->hero_eng->xform.dirty = 1;
            if (h->hero_mode != 0x57) {
                SetHeroAction(i, 0x57);
            }
        }
        if (ext->flag == 0) {
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
                obj = CreateNAS(h->hero_eng->XYZ.x, h->hero_eng->XYZ.y, h->hero_eng->XYZ.z, 0x1611, 0x11);
                if (obj != NULL) {
                    b = FindExtension(obj, 0x10000000);
                    if (b != NULL) {
                        b->angle = nas->Engine->xform.rot.vy;
                        obj->Engine->xform.rot.vy = nas->Engine->xform.rot.vy;
                        h->hero_move->forward_angle = nas->Engine->xform.rot.vy + 0x800;
                        b->start = h->hero_eng->XYZ;
                        b->pos.x = h->hero_eng->XYZ.x - ((t[(nas->Engine->xform.rot.vy - 0x400) & 0xFFF].unk2 * spd) >> 12);
                        b->pos.z = h->hero_eng->XYZ.z - ((t[nas->Engine->xform.rot.vy & 0xFFF].unk2 * spd) >> 12);
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
 do { if (!(hero->hero_status & 0x2000)) { s32 dx; s32 dz; EXTengine *a; EXTengine *b; a = nas->Engine; b = hero->hero_eng; dx = a->XYZ.x - b->XYZ.x; dz = a->XYZ.z - b->XYZ.z; if (((dx * dx) + (dz * dz)) > 0x15F8F) { goto next; } if (FindExtension(hero->hero, 0x20000000) != 0) { EXT_W(ext, 5) = i; } return 1; } next: hero++; i++; } while (0);
    goto loop;
  }

  return 0;
}

void InitPolarBomb(NAS *m) {
    EXTpolarbomb *ext;
    EXTengine *e;

    ext = AddExt(m, sizeof(EXTpolarbomb), 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->entity = 0x201A;
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
        m->Engine->xform.order = 2;
        m->Engine->sub->flags |= 0x02000000;
        ext->grav.gravity = 2;
        ext->grav.magvelocity = 0x56;
        ext->grav.initflag = 0;
        ext->grav.floorheight = 0;
        ext->timer = 0;
        ext->NasStatus = 0;
        m->FUX = NULL;
        e = AddExtEngine(m, 2);
        if (e != NULL) {
            e->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
            e->sub->entity = 0x202F;
            e->Engine->xform.hasScale = 1;
            e->Engine->xform.order = 0;
            e->Engine->xform.scale.vx = 0xC00;
            e->Engine->xform.scale.vy = 0xC00;
            e->Engine->xform.scale.vz = 0xC00;
            e->XYZ = m->Engine->XYZ;
            e->flags |= 0x8041;
            e->sub->scale = 0x200;
            e->sub->flags |= 0x40000000;
            m->FUX = (EXT *)e;
        }
    }
}

void PlotPolarBomb(NAS *m) {
    EXTpolarbomb *ext;
    NAS *nas;
    NAS *smoke;
    ExtSpawn5910 *fx;
    EXTscriptdata *bx;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    switch (ext->NasStatus) {
    case 0:
        if ((CheckMelted(&m->Engine->XYZ, 0) ^ 1) != 0) {
            ext->grav.floorheight = 0x100;
            m->Engine->sub->otz = 0x200;
            if (m->Engine->XYZ.y >= ext->grav.floorheight) {
                PlaySfx(0x494, NULL, NULL, 0x1000, 0x1E00, 0);
                KillNAS(m);
                break;
            }
        }
        if (ArcUpdate(&ext->startpoint.x, &ext->aimpoint.x, &ext->grav.gravity, &m->Engine->XYZ.x) != 0) {
            PlaySfx((CheckMelted(&m->Engine->XYZ, 0) ^ 1) != 0 ? 0x494 : 0x48F, NULL, NULL, 0x1000, 0x1E00, 0);
            nas = CreateNAS(m->Engine->XYZ.x, m->Engine->XYZ.y, m->Engine->XYZ.z, 0x1605, 0x11);
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
            ext->timer = 0;
            ext->NasStatus = 1;
            break;
        }
        if (ext->timer-- < 0) {
            smoke = CreateNAS(m->Engine->XYZ.x, m->Engine->XYZ.y, m->Engine->XYZ.z, 0x1612, 0x11);
            if (smoke != NULL) {
                smoke->Engine->xform.scale.vx = 0x5DC;
                smoke->Engine->xform.scale.vy = 0x5DC;
                smoke->Engine->xform.scale.vz = 0x5DC;
                bx = FindExtension(smoke, 0x10000000);
                if (bx != NULL) {
                    EXT_W(bx, 3) = 0x400;
                    EXT_W(bx, 4) = 0xC8;
                    EXT_W(bx, 5) = 0x10;
                }
            }
            ext->timer = Random(2);
        }
        if (m->FUX != NULL) {
            ((EXTengine *)m->FUX)->XYZ = m->Engine->XYZ;
            ((EXTengine *)m->FUX)->XYZ.y = 0;
            ((EXTengine *)m->FUX)->xform.scale.vx = (0x100 - abs(m->Engine->XYZ.y)) * 8 + 0xC00;
            ((EXTengine *)m->FUX)->xform.scale.vy = (0x100 - abs(m->Engine->XYZ.y)) * 8 + 0xC00;
            ((EXTengine *)m->FUX)->xform.scale.vz = (0x100 - abs(m->Engine->XYZ.y)) * 8 + 0xC00;
            ((EXTengine *)m->FUX)->xform.dirty = 1;
        }
        m->Engine->xform.rot.vx += 0x100;
        m->Engine->xform.dirty = 1;
        if (PolarBombHitHeroes(m, 0x15F90) != 0) {
            ext->NasStatus = 1;
            ext->timer = 0;
        }
        break;
    case 1:
        m->Engine->sub->flags &= ~0x8000;
        if (m->FUX != NULL) {
            ((EXTengine *)m->FUX)->sub->flags &= ~0x8000;
        }
        if (ext->timer == 0) {
            CreateNAS(m->Engine->XYZ.x, m->Engine->XYZ.y, m->Engine->XYZ.z, 0x1614, 0x11);
            PolarBombHitHeroes(m, 0x35EE4);
        }
        ext->timer++;
        if (ext->timer >= 0x1F) {
            KillNAS(m);
        }
        break;
    }
}

void PlotBombGFX(NAS *m) {
    s32 *ext;
    PolarAi *hx;
    PolarAi *hero;
    PolarGame *gx;
    PolarGame *game;
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
    switch (EXT_W(ext, 3)) {
    case 0:
        m->Engine->xform.rot.vy += 0x20;
        m->Engine->xform.dirty = 1;
        if (PolarBombOwnerEngine(m) == 0) {
            return;
        }
        hx = FindExtension(Heroes[EXT_W(ext, 5)].hero, 0x20000000);
        if (hx == NULL) {
            return;
        }
        ok = 1;
        gx = FindExtension((NAS *)PolarBeamOwner, 0x40000000);
        if (gx != NULL) {
            v = gx->unkC;
            if (v == 5) {
                ok = gx->itPlayer != EXT_W(ext, 5);
            }
            if (v == 6 && gx->itPlayer < 4 && gx->itPlayer == EXT_W(ext, 5)) {
                ok = 0;
            }
        }
        PlaySfx(0x48E, NULL, NULL, 0x1000, 0x1E00, 0);
        if (hx->unk28 == 0 && ok) {
            hx->unk28 = 1;
            EXT_W(ext, 3) = 1;
            hx->unkA0 = m;
            return;
        }
        n = BombMaster;
        if (n == NULL) {
            return;
        }
        goto dec;
    case 1:
        hero = FindExtension(Heroes[EXT_W(ext, 5)].hero, 0x20000000);
        if (hero == NULL) {
            return;
        }
        if (m->FUX != NULL) {
            ((EXTengine *)m->FUX)->sub->flags &= ~0x8000;
        }
        game = FindExtension((NAS *)PolarBeamOwner, 0x40000000);
        if (game != NULL) {
            p = EXT_W(ext, 5);
            st = Heroes[p].hero_status;
            if (st & 0x2000) {
                n = BombMaster;
                if (n != NULL) {
                dec:
                    bx = FindExtension(n, 0x10000000);
                    if (bx != NULL) {
                        EXT_W(bx, 6)--;
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
            if ((game->unkC == 5 && game->itPlayer == p && (n = BombMaster) != NULL)
                || (game->unkC == 6 && game->itPlayer < 4 && game->itPlayer == EXT_W(ext, 5) && (n = BombMaster) != NULL)) {
            dec2:
                bx = FindExtension(n, 0x10000000);
                if (bx != NULL) {
                    EXT_W(bx, 6)--;
                    hero->unk28 = 0;
                }
                goto kill;
            }
        }
        hero = FindExtension(Heroes[EXT_W(ext, 5)].hero, 0x20000000);
        if (hero == NULL) {
            return;
        }
        m->Engine->XYZ = Heroes[EXT_W(ext, 5)].hero_eng->XYZ;
        h = &Heroes[EXT_W(ext, 5)];
        if (h->hero_name == 1) {
            m->Engine->XYZ.y -= (h->hero_eng->xform.scale.vx * 0x258) >> 12;
        } else {
            m->Engine->XYZ.y -= (h->hero_eng->xform.scale.vx * 0x300) >> 12;
        }
        m->Engine->xform.rot.vy += 0x20;
        e = m->Engine;
        e->xform.scale.vz = 0x800;
        e->xform.scale.vy = 0x800;
        e->xform.scale.vx = 0x800;
        m->Engine->xform.dirty = 1;
        if (hero->unk28 != 0) {
            return;
        }
        if (BombMaster == NULL) {
            return;
        }
        bx = FindExtension(BombMaster, 0x10000000);
        if (bx != NULL) {
            EXT_W(bx, 6)--;
        }
        if (hero->unkA0 != 0) {
            KillNAS(hero->unkA0);
            hero->unkA0 = NULL;
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
                h->hero_eng->xform.rot.vy += angle_diff(h->hero_eng->xform.rot.vy, h->hero_move->forward_angle) >> 2;
            } else {
                h->hero_eng->xform.rot.vy += angle_diff(h->hero_eng->xform.rot.vy, h->hero_move->forward_angle) >> 5;
            }
        }
        Heroes[player].hero_ctrl &= ~1;
        Heroes[player].hero_eng->xform.dirty = 1;
    }
}

void ResetMass(s32 player) {
    EXTheromove *move = Heroes[player].hero_move;
    s32 *ext;

    if (move != NULL) {
        switch (GAME->heroes[player]) {
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
        if (ext != NULL && EXT_W(ext, 9) == 1) {
            move->mass -= 0x46;
        }
    }
}

void PolarInitHeroEngine(s32 arg0) {
    EXTengine *e = Heroes[arg0].hero_eng;
    EXTheromove *m = Heroes[arg0].hero_move;
    s32 step;

    HeroHooks.HeroFacing(arg0);
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
        e->XYZ.z += m->z_vel;
        e->XYZ.x += m->x_vel;
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
            pos.x = hero->hero_eng->XYZ.x;
            pos.y = hero->hero_eng->XYZ.y - 0x80;
            pos.z = hero->hero_eng->XYZ.z;
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
    EngineHooks *h;
    GameState *g;
    void **p;
    void **p2;
    void **p3;
    void **p4;
    void **p5;
    void *t;
    void *t4;
    GameState *g2;
    GameState *g3;

    h = &HeroHooks;
    h->flag = NULL;
    h->HeroModels = PolarHeroModelsB;
    PolarBeamOwner = NULL;
    BombMaster = NULL;
    PolarFirstFlag = 0;
    PolarArenaRadius2 = 0x400000;
    if (((GameState *)GameInfo)->mode != 0x15) {
        h->CreateShadow = PolarInitShadow;
        h->UpdateShadow = PolarUpdateShadow;
    } else {
        h->CreateShadow = ModelCreateShadow;
        h->UpdateShadow = ModelUpdateShadow;
        h->flag = PolarHeroModelsB;
        h->HeroModels = PolarHeroModels;
    }
    g = (GameState *)GameInfo;
    if (g->mode == 0x14) {
        HeroHooks.HeroModels = PolarHeroModelsCut;
    }
    HeroHooks.KeepInBounds = DoNowt;
    HeroHooks.InitHero = PolarInitHero;
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
        g2 = (GameState *)GameInfo;
        g2->levelFlags |= 0x100;
        if (g2->teams == 0) {
            g2->heroes[1] = 9;
        }
        g2->heroes[2] = 9;
        g2->heroes[3] = 9;
        return;
    case 0x2C:
        g3 = (GameState *)GameInfo;
        g3->levelFlags |= 0x100;
        g3->heroes[1] = 9;
        g3->heroes[2] = 9;
        g3->heroes[3] = 9;
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

void PolarAI_SteerToTarget(s32 player) {
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
        ((PadState *)Pads)[player].dir = h->hero_ai->AIpad;
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
        h2 = (HeroInfo *)((u8 *)base + off);
        h2->hero_ai->AIang = ang;
        h2->hero_ai->AItimer1 = fux->aiDelay;
    }
    if (Heroes[player].hero_frozen == 0) {
        ((PadState *)Pads)[player].angle.w = Heroes[player].hero_ai->AIang;
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
    DistXZSq((SLONG_XYZ *)&base[fux->target].hero_eng->XYZ, (SLONG_XYZ *)&h->hero_eng->XYZ);
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
        ext->count = 8;
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
    fux->aiDelay = 1;
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
        if (((GameState *)GameInfo)->teams != 0 && n >= 3) {
            fux->state = 5;
            fux->func = PolarEmptyStub6;
            return;
        }
    }
    if (!(fux->flags2 & 2)) {
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
                pos = h->hero_eng->XYZ;
                if (DistXZSq((SLONG_XYZ *)&pos, (SLONG_XYZ *)&me->hero_eng->XYZ) < min) {
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
    PolarAi *ext;
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
    fux->aiDelay = 1;
    if (fux->flags2 & 2) {
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
                    pos = Heroes[i].hero_eng->XYZ;
                    d = DistXZSq((SLONG_XYZ *)&pos, (SLONG_XYZ *)&Heroes[player].hero_eng->XYZ);
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
        if (ext->flag == 0) {
            skip = 0;
        }
        if (ext->flag == 1 && Heroes[fux->target].hero_mode == 0x4E) {
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
        if (fux->flags2 & 2) {
            for (i = 0; ; i++) {
                s32 d;

                if (i >= 4) {
                    break;
                }
                if (i != player && !(Heroes[i].hero_status & 0x2000) && !skip
                    && Heroes[i].hero_mode != 0x50) {
                    pos = Heroes[i].hero_eng->XYZ;
                    d = DistXZSq((SLONG_XYZ *)&pos, (SLONG_XYZ *)&Heroes[player].hero_eng->XYZ);
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
                        pos = Heroes[i].hero_eng->XYZ;
                        d = DistXZSq((SLONG_XYZ *)&pos, (SLONG_XYZ *)&Heroes[player].hero_eng->XYZ);
                        if (d < dist) {
                            dist = d;
                            best = i;
                        }
                    }
                }
                fux->state = 1;
                fux->target = best;
                fux->func = PolarAI_DoAttack;
                fux->flags2 |= 2;
            } else {
                fux->target = r;
            }
        }
    }
    if (fux->timer2-- <= 0) {
        fux->state = 5;
        fux->func = PolarEmptyStub6;
        fux->flags2 &= ~2;
    }
    if (skip || TestForTeamMate(player, fux->target) != 0) {
        fux->state = 5;
        fux->func = PolarEmptyStub6;
        fux->flags2 &= ~2;
        return;
    }
    PolarAI_TryAttack(player);
    fux->vel = Heroes[fux->target].hero_eng->XYZ;
    fux->pos = Heroes[player].hero_eng->XYZ;
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
        fux->aiDelay = 1;
        if (PolarBeamOwner != NULL) {
            fux->vel = PolarBeamOwner->eng->XYZ;
        }
        fux->pos = h->hero_eng->XYZ;
        if (fux->timer2-- < 0) {
            fux->state = 5;
            fux->func = PolarEmptyStub6;
            fux->flags2 &= ~1;
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
        dx = a->XYZ.x - b->XYZ.x;
        dz = a->XYZ.z - b->XYZ.z;
        if (dx * dx + dz * dz > 0x3D08FF) {
            return 0;
        }
        if (fux->count < 0x1F) {
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
        fux->timer2 = Random(0x78) + 0x3C;
        r = PolarAI_PickVictim(player);
        if (r == -1) {
            fux->flags2 |= 2;
        } else {
            fux->target = r;
            fux->flags2 &= ~2;
        }
        fux->state = 1;
        fux->func = PolarAI_DoAttack;
        return;
    }
    nas = PolarFreeBomb();
    if (nas != NULL) {
        fux->aiDelay = 1;
        fux->vel = nas->Engine->XYZ;
        fux->pos = Heroes[player].hero_eng->XYZ;
        if (fux->timer2-- < 0) {
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
                pos = o->hero_eng->XYZ;
                d = DistXZSq((SLONG_XYZ *)&pos, (SLONG_XYZ *)&me->hero_eng->XYZ);
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
        move->forward_angle = get_angle(a->XYZ.x - b->XYZ.x, a->XYZ.z - b->XYZ.z) & 0xFFF;
    }
    if (dist <= 0xFFFFF) {
        fux->player = best;
        fux->timer2 = 0x5A;
        PolarAI_TryAttack(player);
        fux->state = 6;
        fux->func = PolarAI_AvoidMelt;
        fux->flags2 |= 1;
    }
    if (fux->timer2-- < 0) {
        fux->state = 5;
        fux->func = PolarEmptyStub6;
        fux->flags2 &= ~1;
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
        fux->pos = h->hero_eng->XYZ;
        dx = fux->vel.x - fux->pos.x;
        dz = fux->vel.z - fux->pos.z;
        if (dx * dx + dz * dz > 0x24000) {
            PolarAI_SteerToTarget(player);
        } else {
            fux->timer2 = Random(0x78);
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
    if (fux->timer2-- < 0) {
        fux->state = 5;
        fux->func = PolarEmptyStub6;
    }
    h = &Heroes[fux->target];
    if (h->hero_mode == 0x4E) {
        fux->state = 5;
        fux->func = PolarEmptyStub6;
        return;
    }
    fux->vel = h->hero_eng->XYZ;
    fux->pos = Heroes[player].hero_eng->XYZ;
    PolarAI_SteerToTarget(player);
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
    fux->aiDelay = 1;
    if (PolarBeamOwner != NULL) {
        fux->vel = PolarBeamOwner->eng->XYZ;
    }
    fux->pos = Heroes[player].hero_eng->XYZ;
    e = PolarBeamOwner->eng;
    dx = eng->XYZ.x - e->XYZ.x;
    dz = eng->XYZ.z - e->XYZ.z;
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
    ang = a->xform.rot.vy;
    b = base[target].hero_eng;
    vel.vx = PolarIceRadius * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2 >> 12;
    vel.vz = PolarIceRadius * rcossin_tbl[ang & 0xFFF].unk2 >> 12;
    pos.vx = 0;
    pos.vz = 0;
    d.vx = b->XYZ.x - a->XYZ.x;
    d.vz = b->XYZ.z - a->XYZ.z;
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
            pos = h->hero_eng->XYZ;
            d = DistXZSq((SLONG_XYZ *)&pos, (SLONG_XYZ *)&me->hero_eng->XYZ);
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
                pos = h->hero_eng->XYZ;
                d = DistXZSq((SLONG_XYZ *)&pos, (SLONG_XYZ *)&me->hero_eng->XYZ);
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

void PolarAI_AvoidMelt(s32 player) {
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
  if (((GameState *) GameInfo)->mode == 0x12)
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
  fux->timer2--;
  if (((GameState *) GameInfo)->mode == 0x15)
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
            pos.v = o->hero_eng->XYZ;
            dd = DistXZSq((SLONG_XYZ *)(&pos.v), (SLONG_XYZ *)(&me->hero_eng->XYZ));
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
  if (fux->flags2 & 1)
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
          pos.v = o->hero_eng->XYZ;
          dd = DistXZSq((SLONG_XYZ *)(&pos.v), (SLONG_XYZ *)(&me1->hero_eng->XYZ));
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

    fux->player = best;
  }
  if (mv != 0)
  {
    mv->forward_angle = get_angle(Heroes[player].hero_eng->XYZ.x - Heroes[fux->player].hero_eng->XYZ.x, Heroes[player].hero_eng->XYZ.z - Heroes[fux->player].hero_eng->XYZ.z) & 0xFFF;
  }
  my = Heroes[player].hero_eng;
  ve = Heroes[fux->player].hero_eng;
  d[0].vx = (*((u16 *) (&ve->XYZ.x))) - (*((u16 *) (&my->XYZ.x)));
  d[0].vz = (*((u16 *) (&ve->XYZ.z))) - (*((u16 *) (&my->XYZ.z)));
  d[0].vy = 0;
  d[0].pad = 0xFA;
  dx = ve->XYZ.x - my->XYZ.x;
  dz = ve->XYZ.z - my->XYZ.z;
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
        if (((j != player) && (j != fux->player)) && (!(hp->hero_status & 0x2000)))
        {
          d2[0].vx = (*((u16 *) (&e2->XYZ.x))) - (*((u16 *) (&my->XYZ.x)));
          d2[0].vz = (*((u16 *) (&e2->XYZ.z))) - (*((u16 *) (&my->XYZ.z)));
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
      pos.v.x = me2->hero_eng->XYZ.x - v1.vx;
      pos.v.z = me2->hero_eng->XYZ.z - v1.vz;
      if (GameMode != 0x14)
      {
        ok = CheckMelted(&pos.v, 0);
        goto join;
        here:
        fux->vel = pos.v;

        fux->pos = Heroes[player].hero_eng->XYZ;
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
        fux->pos = me2->hero_eng->XYZ;
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
      pos.v.x = Heroes[player].hero_eng->XYZ.x - v1.vx;
      pos.v.z = Heroes[player].hero_eng->XYZ.z - v1.vz;
      for (;;)
      {
        EXTengine *e3;
        if (k >= 4)
        {
          break;
        }
        e3 = Heroes[k].hero_eng;
        if (((k != player) && (k != fux->player)) && (!(Heroes[k].hero_status & 0x2000)))
        {
          d2[0].vx = (*((u16 *) (&e3->XYZ.x))) - (*((u16 *) (&my->XYZ.x)));
          d2[0].vz = (*((u16 *) (&e3->XYZ.z))) - (*((u16 *) (&my->XYZ.z)));
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
            fux->pos = Heroes[player].hero_eng->XYZ;
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

  if (fux->timer2 <= 0)
  {
    fux->state = 5;
    tail:
    fux->func = PolarEmptyStub6;

    fux->flags2 &= ~1;
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
    fux->timer2--;
    if (fux->flags2 & 1) {
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
                    pos.v = o->hero_eng->XYZ;
                    dist = DistXZSq((SLONG_XYZ *)&pos.v, (SLONG_XYZ *)&me->hero_eng->XYZ);
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
        fux->player = best;
        if (((GameState *)GameInfo)->mode == 0x12 && PolarProximity != NULL) {
            a = PolarProximity->eng;
            b = Heroes[player].hero_eng;
            dx = a->XYZ.x - b->XYZ.x;
            dz = a->XYZ.z - b->XYZ.z;
            dx = dx * dx + dz * dz;
            if (dx < bestd) {
                prox = 1;
                fux->player = 5;
            }
        }
    }
    if (mv != NULL) {
        if (prox == 0) {
            ax = Heroes[player].hero_eng->XYZ.x - Heroes[fux->player].hero_eng->XYZ.x;
            az = Heroes[player].hero_eng->XYZ.z - Heroes[fux->player].hero_eng->XYZ.z;
        } else {
            ax = Heroes[player].hero_eng->XYZ.x - PolarProximity->eng->XYZ.x;
            az = Heroes[player].hero_eng->XYZ.z - PolarProximity->eng->XYZ.z;
        }
        mv->forward_angle = get_angle(ax, az) & 0xFFF;
    }
    my = Heroes[player].hero_eng;
    if (prox == 0) {
        ve = Heroes[fux->player].hero_eng;
        goto aim;
escape:
        fux->vel = pos.v;
        fux->pos = Heroes[player].hero_eng->XYZ;
        PolarAI_SteerToTarget(player);
        fux->angle = ang;
        goto end;
    } else if (PolarProximity != NULL) {
        ve = PolarProximity->eng;
    }
aim:
    d[0].vx = *(u16 *)&ve->XYZ.x - *(u16 *)&my->XYZ.x;
    d[0].vz = *(u16 *)&ve->XYZ.z - *(u16 *)&my->XYZ.z;
    d[0].vy = 0;
    d[0].pad = 0x10E;
    dx = ve->XYZ.x - my->XYZ.x;
    dz = ve->XYZ.z - my->XYZ.z;
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
                if (j != player && j != fux->player && !(hp->hero_status & 0x2000)) {
                    d2[0].vx = *(u16 *)&e2->XYZ.x - *(u16 *)&my->XYZ.x;
                    d2[0].vz = *(u16 *)&e2->XYZ.z - *(u16 *)&my->XYZ.z;
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
            pos.v.x = me2->hero_eng->XYZ.x - v1.vx;
            pos.v.z = me2->hero_eng->XYZ.z - v1.vz;
            if (CheckMelted(&pos.v, 0) != 0) {
                fux->vel = pos.v;
                fux->pos = me2->hero_eng->XYZ;
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
            pos.v.x = Heroes[player].hero_eng->XYZ.x - v1.vx;
            q = Heroes;
            pos.v.z = Heroes[player].hero_eng->XYZ.z - v1.vz;
        loop3:
            if (i < 4) {
                do {
                    EXTengine *e3;
                    e3 = q->hero_eng;
                    if (i != player && i != fux->player && !(q->hero_status & 0x2000)) {
                        d3[0].vx = *(u16 *)&e3->XYZ.x - *(u16 *)&my->XYZ.x;
                        d3[0].vz = *(u16 *)&e3->XYZ.z - *(u16 *)&my->XYZ.z;
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
    if (fux->timer2 <= 0) {
        fux->state = 5;
        fux->func = PolarEmptyStub6;
        fux->flags2 &= ~1;
    }
}

void PolarAI_DoChaseBomber(s32 player) {
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
    fux->aiDelay = 1;
    fux->vel = *(SLONG_XYZ *)&PolarCentre;
    fux->pos = h->hero_eng->XYZ;
    dx = eng->XYZ.x - PolarCentre.vx;
    dz = eng->XYZ.z - PolarCentre.vz;
    if (dx * dx + dz * dz < 0x19000) {
        fux->state = 5;
        fux->func = PolarEmptyStub6;
    }
    if (fux->timer2-- <= 0) {
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
            l = PolarBeamOwner->link;
            if (l == NULL) {
                goto fail;
            }
            if (l->aiFlag == player) {
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
    if (ext != NULL && ext->timer2 == 1) {
        return 0;
    }
    if (r >= PolarAIChances[Heroes[player].hero_name - 1]) {
        return 0;
    }
    PolarAI_StartAttack(player);
    fux->timer2 = Random(0x78) + 0x3C;
    t = PolarAI_PickVictim(player);
    if (t == -1) {
        fux->flags2 |= 2;
    } else {
        fux->target = t;
        fux->flags2 &= ~2;
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
                    f->timer2 = Random(0x168) + 0x5A;
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
    if (ext != NULL && ext->timer2 == 1) {
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
        f->pos = h->hero_eng->XYZ;
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
                f->timer2 = Random(0x168) + 0x5A;
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
    lead = ((GameState *)GameInfo)->teamLeader[0];
    if (hero == lead || (mate = Heroes[lead].hero_teammate, hero == mate)) {
        if (r < 0x800) {
            other = ((GameState *)GameInfo)->teamLeader[1];
            if (!(Heroes[other].hero_status & 0x2000)) {
                return other;
            }
            mate = Heroes[other].hero_teammate;
            if (Heroes[mate].hero_status & 0x2000) {
                return -1;
            }
            return mate;
        } else {
            lead = ((GameState *)GameInfo)->teamLeader[1];
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
    GameState *g;
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
        if (((GameState *)GameInfo)->mode == 0x12) {
            PolarArenaRadius2 = 1000000;
        }
        h->hero_ai->AImode = 1;
        eng = h->hero_eng;
        if (h->hero_mode != 0x49) {
            fux->flag = 0;
        }
        i = 0;
        if (fux->state != 1) {
            fux->flag2 = 0;
        }
        g = (GameState *)GameInfo;
        one = 1;
        for (;; i++) {
            NAS *nas;
            EXTbombmaker *b;

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

                if (p != NULL && (m = p->flag) != NULL) {
                    dx = p->eng->XYZ.x - Heroes[player].hero_eng->XYZ.x;
                    dz = p->eng->XYZ.z - Heroes[player].hero_eng->XYZ.z;
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
                    if (b != NULL && b->NasStatus == one && b->owner_id == i) {
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
                    o->count++;
                } else {
                    o->count = 0;
                    if (PolarFreeBomb() != NULL && o->func == PolarEmptyStub6 && ext->state == one) {
                        f = (PolarWanderFux *)Heroes[i].hero->FUX;
                        if (f != NULL) {
                            f->timer2 = Random(0x168) + 0x5A;
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
            if (PolarArenaRadius2 < DistXZSq((SLONG_XYZ *)&PolarCentre, (SLONG_XYZ *)&eng->XYZ)) {
                PolarAIFlags[player] |= 1;
            }
            if (PolarBeamOwner != NULL && (l = PolarBeamOwner->link) != NULL) {
                if (l->beamMode == 2) {
                    PolarBeamMode = l->beamMode;
                } else {
                    PolarBeamMode = 1;
                    fux->flags &= ~1;
                }
                if (l->aiFlag != 5) {
                    if (l->state == 5) {
                        PolarAIFlags[l->aiFlag] |= 2;
                    }
                    if (l->state == 6) {
                        PolarAIFlags[l->aiFlag] |= 0x40;
                    }
                }
                if (l->player != -1) {
                    switch (l->modes[l->player]) {
                    case 1:
                        PolarAIFlags[l->player] |= 4;
                        break;
                    case 0:
                        PolarAIFlags[l->player] |= 8;
                        break;
                    case 2:
                        PolarAIFlags[l->player] |= 0x20;
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

                dx = a->XYZ.x - c->XYZ.x;
                dz = a->XYZ.z - c->XYZ.z;
                if (dx * dx + dz * dz <= 0x4D1C3F) {
                    all[6] = -1;
                    POLAR_ATTACK(fux, hp, player, i, f, j);
                }
            }
            if (*fl & 4) {
                all[6] = -1;
                if (fux->state != 6 || fux->timer2 == 0) {
                    fux->timer2 = 0xD2;
                }
                fux->state = 6;
                fux->func = PolarAI_AvoidMelt;
                fux->player = i;
                fux->aiDelay = 1;
                fux->flags2 |= 1;
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
                if (fux->timer2 <= 0 && (fux->state != 6 || fux->timer2 == 0)) {
                    fux->timer2 = 0x168;
                }
                fux->state = 6;
                fux->func = PolarAI_AvoidMelt;
                fux->player = i;
                fux->aiDelay = 0;
                fux->flags2 |= 1;
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
            fux->aiDelay = 2;
            if (fux->state != 1 || fux->timer2 == 0) {
                fux->timer2 = 0xD2;
            }
            if (fux->flag2 == 0) {
                s32 t = PolarAI_PickVictim(player);

                fux->flag2 = 1;
                if (t == -1) {
                    fux->state = 1;
                    fux->target = i;
                    fux->func = PolarAI_DoAttack;
                    fux->flags2 |= 2;
                } else {
                    fux->func = PolarAI_DoAttack;
                    fux->target = t;
                    fux->state = 1;
                    fux->flags2 &= ~2;
                }
            }
        }
        if (PolarAIFlags[player] & 8) {
            fux->flags2 |= 1;
            if (fux->state != 6 || fux->timer2 == 0) {
                fux->timer2 = 0xD2;
            }
            fux->state = 6;
            fux->aiDelay = 2;
            fux->player = i;
            fux->func = PolarAI_AvoidMelt;
        }
        if (PolarAIFlags[player] & 2) {
            fux->aiDelay = 0;
            if (fux->state != 1 || fux->timer2 == 0) {
                fux->timer2 = 0x168;
            }
            if (fux->flag2 == 0) {
                s32 t = PolarAI_PickVictim(player);

                fux->flag2 = 1;
                if (t == -1) {
                    fux->state = 1;
                    fux->target = i;
                    fux->func = PolarAI_DoAttack;
                    fux->flags2 |= 2;
                } else {
                    fux->func = PolarAI_DoAttack;
                    fux->target = t;
                    fux->state = 1;
                    fux->flags2 &= ~2;
                }
            }
        }
        if (PolarAIFlags[player] & 0x40) {
            fux->flags2 |= 1;
            if (fux->state != 6 || fux->timer2 == 0) {
                fux->timer2 = 0xD2;
            }
            fux->state = 6;
            fux->aiDelay = 2;
            fux->player = i;
            fux->func = PolarAI_AvoidMelt;
        }
        if (PolarAIFlags[player] & 0x100) {
            fux->flags2 |= 1;
            if (fux->state != 6 || fux->timer2 == 0) {
                fux->timer2 = 0xD2;
            }
            fux->state = 0xC;
            fux->aiDelay = 1;
            fux->player = i;
            fux->func = PolarAI_DoIceWalk;
        }
        if (PolarAIFlags[player] & 0x200) {
            fux->flags2 |= 1;
            if (fux->state != 6 || fux->timer2 == 0) {
                fux->timer2 = 0xD2;
            }
            fux->state = 6;
            fux->aiDelay = 2;
            fux->player = i;
            fux->func = PolarAI_AvoidMelt;
        }
        if (PolarAIFlags[player] & 0x800) {
            fux->flags2 |= 1;
            if (fux->state != 6 || fux->timer2 == 0) {
                fux->timer2 = 0xD2;
            }
            fux->state = 6;
            fux->aiDelay = 2;
            fux->player = i;
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
    subengine *obj;
    camera_info_tag *cam;
    s32 i;
    s32 *speed;
    HeroInfo *h;
    GameState *v;
    u8 *g;
    EXTheromove *move;
    EXTpolarAI *fux;
    PolarAi *ai;
    subengine *sub;
    EXTengine *eng;
    EXTengine *eng2;
    NAS *nas;
    u16 status;
    u16 ang;
    s32 fast;
    s32 slow;
    GameState *w;
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
    obj = ((subengine *)MapScene->subs);
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
    if (((GameState *)GameInfo)->mode != 0x16) {
        if (((GameState *)GameInfo)->levelFlags & 0x40) {
            Set_Clock(0x45, 0x81C0);
        } else {
            Set_Clock(0x130, 0x81C0);
        }
        if (((GameState *)GameInfo)->mode == 0x13) {
            if (((GameState *)GameInfo)->levelFlags & 0x40) {
                Set_Clock(0x100, 0x81C0);
            } else {
                Set_Clock(0x130, 0x81C0);
            }
            if (((GameState *)GameInfo)->levelFlags & 0x40) {
                if (((GameState *)GameInfo)->teams != 0) {
                    Set_Clock(0x45, 0x81C0);
                }
            }
        }
        if (((GameState *)GameInfo)->mode == 0x12) {
            if (((GameState *)GameInfo)->levelFlags & 0x40) {
                Set_Clock(0x30, 0x81C0);
            } else {
                Set_Clock(0x130, 0x81C0);
            }
            if (((GameState *)GameInfo)->teams != 0) {
                if (((GameState *)GameInfo)->levelFlags & 0x40) {
                    Set_Clock(0x25, 0x81C0);
                }
            }
        }
        if (((GameState *)GameInfo)->mode == 0x14) {
            if (((GameState *)GameInfo)->levelFlags & 0x40) {
                Set_Clock(0x25, 0x81C0);
            } else {
                Set_Clock(0x130, 0x81C0);
            }
            if (((GameState *)GameInfo)->teams != 0) {
                if (((GameState *)GameInfo)->levelFlags & 0x40) {
                    Set_Clock(0x20, 0x81C0);
                }
            }
        }
        if (((GameState *)GameInfo)->mode == 0x15) {
            if (((GameState *)GameInfo)->levelFlags & 0x40) {
                Set_Clock(0x30, 0x81C0);
            } else {
                Set_Clock(0x130, 0x81C0);
            }
            if (((GameState *)GameInfo)->teams != 0
                && (((GameState *)GameInfo)->levelFlags & 0x40)) {
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
    v = (GameState *)GameInfo;
    fn = PolarEmptyStub6;
    h = Heroes;
    g = (u8 *)v;
loop:
    if (i < 4) {
        move = h->hero_move;
        if (move != NULL) {
            if (((GameState *)g)->heroes[0] == 9) {
                status = h->hero_status;
                h->hero_status = status | 0x2000;
                if (((GameState *)g)->heroes[0] == 9) {
                    h->hero_status = status | 0xA000;
                }
            }
            PolarInitHeroMove(i);
            SetHeroAction(i, 0x49);
            if (v->mode == 0x15 && (v->levelFlags & 0x80) && ((GameState *)g)->heroCpu[0] == 0) {
                SetHeroAction(i, 0x54);
            }
            h->hero_eng->xform.hasScale = 1;
            h->hero_eng->xform.order = 0;
            h->hero_eng->XYZ.y = 0;
            eng = h->hero_eng;
            eng->xform.scale.vz = 0x1000;
            eng->xform.scale.vy = 0x1000;
            eng->xform.scale.vx = 0x1000;
            if (v->mode == 0x12) {
                dbase_load_all_mesh_anims((s32 *)&HeroModelBank[DB_ArenaPolarTiltSeal]);
            }
            if (v->mode == 0x15) {
                dbase_load_all_mesh_anims((s32 *)&HeroModelBank[DB_PolarObjects]);
            }
            ResetMass(i);
            eng2 = h->hero_eng;
            ang = ((get_angle(eng2->XYZ.x - PolarCentre.vx, eng2->XYZ.z - PolarCentre.vz) + 0x800) & 0xFFF) + 0x800;
            move->forward_angle = ang;
            h->hero_eng->xform.rot.vy = ang;
            h->hero_eng->xform.dirty = 1;
            h->hero_shadow->xform.rot.vy = h->hero_eng->xform.rot.vy;
            h->hero_ai->AItimer1 = 0;
            fux = (EXTpolarAI *)h->hero->FUX;
            if (fux == NULL) {
                fux = AddExt(h->hero, sizeof(EXTpolarAI), 0x10000000);
                if (fux != NULL) {
                    h->hero->FUX = (EXT *)fux;
                    fux->choice_timer = 0;
                    fux->beamflag = 0;
                    fux->hit_count = 0;
                    fux->AI_action = 5;
                    fux->DoFunction = fn;
                    h->hero_frozen = 0;
                    fux->AI_sub_action = 0;
                    fux->ai_status = 0;
                    fux->tauntflag = 0;
                    fux->fire_delay_timer = 0;
                    fux->weightflag = 0;
                    sub = h->hero_eng->sub;
                    sub->flags |= 0x2000000;
                    h->hero_eng->sub->otz = 0;
                }
                ai = AddExt(h->hero, sizeof(PolarAi), 0x20000000);
            } else {
                fux->choice_timer = 0;
                fux->beamflag = 0;
                fux->hit_count = 0;
                fux->AI_action = 5;
                fux->DoFunction = fn;
                h->hero_frozen = 0;
                fux->AI_sub_action = 0;
                fux->ai_status = 0;
                fux->tauntflag = 0;
                fux->fire_delay_timer = 0;
                fux->weightflag = 0;
                fux->attack_delay = 0;
                sub = h->hero_eng->sub;
                sub->flags |= 0x2000000;
                h->hero_eng->sub->otz = 0;
                ai = FindExtension(h->hero, 0x20000000);
            }
            if (ai != NULL) {
                ai->unk10 = 0x1E;
                ai->unkC = 0;
                ai->unk14 = 0;
                ai->unk18 = 0;
                ai->unk1C = 0;
                ai->unk20 = 0x100;
                ai->flag = 0;
                ai->unk28 = 0;
                ai->unk98 = 0;
                ai->unkA0 = 0;
                ai->unkA4 = 0;
                ai->unkA8 = 0;
                ai->unkAC = 0;
                ai->eng = NULL;
                ai->eng = AddExtEngine(h->hero, 0x14);
                ai->flag2 = 0;
            }
            PolarAIFlags[i] = 0;
            PolarAIPace[2] = 0;
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
    w = (GameState *)GameInfo;
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
        if (!(w->levelFlags & 0x80)) {
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
    if (((GameState *)GameInfo)->mode == 0x12
        && (((GameState *)GameInfo)->levelFlags & 0x80)
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
    if (GameMode == 0x2C && (Pads[0].dirPressed & 0x2000)) {
        ScriptFxOn ^= 1;
    }
    if (PolarBeamOwner != NULL) {
        l = PolarBeamOwner->link;
        if (l != NULL) {
            if (PolarAIPace[2] == 0 && l->player != -1) {
                PolarAIPace[2] = 1;
            }
            if (PolarAIPace[2] == -1 && l->player == PolarAIPace[2]) {
                PolarAIPace[2] = 0;
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
                if (GAME->heroCpu[i] == 0) {
                    HeroesAllOut = 0;
                }
                if (((GameState *)GameInfo)->teams != 0
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
        if (Heroes[i].hero_eng->XYZ.y > 0) {
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
    NAS *p;
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
                dx = e->XYZ.x - eng->XYZ.x;
                dz = e->XYZ.z - eng->XYZ.z;
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
                            ((PolarAi *)e)->eng->XYZ = hp->hero_eng->XYZ;
                            if (((PolarAi *)e)->eng != NULL) {
                            switch (hp->hero_name) {
                            case 1:
                                r = Random(4);
                                if (r == 0) {
                                    PlaySfx(3, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (r == 1) {
                                    PlaySfx(4, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (r == 2) {
                                    PlaySfx(5, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (r == 3) {
                                    PlaySfx(6, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                break;
                            case 2:
                                r = Random(4);
                                if (r == 0) {
                                    PlaySfx(0x35, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (r == 1) {
                                    PlaySfx(0x36, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (r == 2) {
                                    PlaySfx(0x37, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (r == 3) {
                                    PlaySfx(0x38, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                break;
                            case 3:
                                r = Random(4);
                                if (r == 0) {
                                    PlaySfx(0x67, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (r == 1) {
                                    PlaySfx(0x68, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (r == 2) {
                                    PlaySfx(0x69, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (r == 3) {
                                    PlaySfx(0x6A, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                break;
                            case 4:
                                r = Random(4);
                                if (r == 0) {
                                    PlaySfx(0x99, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (r == 1) {
                                    PlaySfx(0x9A, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (r == 2) {
                                    PlaySfx(0x9B, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (r == 3) {
                                    PlaySfx(0x9C, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                break;
                            case 5:
                                pick = Random(4);
                                if (pick == 0) {
                                    PlaySfx(0x12F, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (pick == 1) {
                                    PlaySfx(0x130, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (pick == 2) {
                                    PlaySfx(0x131, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (pick == 3) {
                                    PlaySfx(0x132, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                break;
                            case 7:
                                pick = Random(4);
                                if (pick == 0) {
                                    PlaySfx(0xFD, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (pick == 1) {
                                    PlaySfx(0xFE, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (pick == 2) {
                                    PlaySfx(0xFF, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (pick == 3) {
                                    PlaySfx(0x100, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                break;
                            case 8:
                                pick = Random(4);
                                if (pick == 0) {
                                    PlaySfx(0xCB, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (pick == 1) {
                                    PlaySfx(0xCC, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (pick == 2) {
                                    PlaySfx(0xCD, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (pick == 3) {
                                    PlaySfx(0xCE, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                break;
                            case 6:
                                pick = Random(4);
                                if (pick == 0) {
                                    PlaySfx(0x161, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (pick == 1) {
                                    PlaySfx(0x162, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (pick == 2) {
                                    PlaySfx(0x163, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
                                }
                                if (pick == 3) {
                                    PlaySfx(0x164, &((PolarAi *)e)->eng->sfx, NULL, 0x1000, 0x1E00, 0);
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
                            if (player == ((PolarGame *)e)->itPlayer && ((PolarGame *)e)->itTime > 0) {
                                if (((PolarGame *)e)->out[i] == 0) {
                                    ((PolarGame *)e)->itPlayer = i;
                                    ((PolarGame *)e)->out[player] = 0xF;
                                    PlaySfx(0x454, NULL, NULL, 0x1000, 0x1E00, 0);
                                    swapped = 1;
                                    ((PolarGame *)e)->out[i] = 0;
                                }
                            }
                            if (i == ((PolarGame *)e)->itPlayer && swapped == 0 && ((PolarGame *)e)->itTime > 0) {
                                if (((PolarGame *)e)->out[player] == 0) {
                                    ((PolarGame *)e)->itPlayer = player;
                                    ((PolarGame *)e)->out[i] = 0xF;
                                    PlaySfx(0x454, NULL, NULL, 0x1000, 0x1E00, 0);
                                    ((PolarGame *)e)->out[player] = 0;
                                }
                            }
                        }
                    }
                    eng->XYZ.x -= ((d + 2) * rcossin_tbl[(ang - 0x400) & 0xFFF].unk2) >> 12;
                    eng->XYZ.z -= ((d + 2) * rcossin_tbl[ang].unk2) >> 12;
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
                e = p->Engine;
                if (e != NULL && e != eng && PolarBoxOverlap(e, 0, hq->hero_eng, 0) != 0) {
                    mv = FindExtension((NAS *)p, 0x20000000);
                    if (mv != NULL) {
                        hit = 1;
                        PolarMovesSwap(hq->hero_move, mv);
                    }
                }
            }
            p = p->Group.Child;
        }
    } else {
        range = 0x1F4;
        p = PolarBears;
        t = rcossin_tbl;
        while (1) {
            if (p == NULL) {
                break;
            }
            e = p->Engine;
            if (e != NULL && e != eng) {
                ai = FindExtension((NAS *)p, 0x10000000);
                if (ai != NULL && ai->flag2 == 0 && abs(e->XYZ.y - eng->XYZ.y) <= range) {
                    dx = e->XYZ.x - eng->XYZ.x;
                    dz = e->XYZ.z - eng->XYZ.z;
                    d = csqrt(dx * dx + dz * dz) >> 6;
                    if (d < range) {
                        e = FindExtension((NAS *)p, 0x20000000);
                        if (e != NULL) {
                            ang = get_angle(dx, dz) & 0xFFF;
                            d = range - d + 2;
                            eng->XYZ.x -= (d * t[(ang - 0x400) & 0xFFF].unk2) >> 12;
                            eng->XYZ.z -= (d * t[ang].unk2) >> 12;
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
            p = p->Group.Child;
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
    if (ext->timer2 != 0) {
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
    if (fux->flag != 0) {
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
    fux->flag = 1;
    return 1;
fail:
    return 0;
}

bool TestForTeamMate(u32 arg0, u32 arg1) {
    GameState *g = GAME;

    if (g->teams != 0) {
        if (Heroes[arg1].hero_status & 0x2000) {
            return 0;
        }
        if (arg0 == g->teamLeader[0] && arg1 == Heroes[arg0].hero_teammate) {
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
                eng->xform.scale.vx = 0x1000;
                eng->xform.scale.vy = 0x1000;
                eng->xform.scale.vz = 0x1000;
                shadow->xform.scale.vx = 0x1000;
                shadow->xform.scale.vy = 0x1000;
                shadow->xform.scale.vz = 0x1000;
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
    EXTtransmuter *ext;
    UkaBitExt *bx;
    EXTengine *e;
    subengine *sub;
    subengine *s2;
    NAS *n;
    s32 i;

    ext = AddExt(m, sizeof(EXTtransmuter), 0x40000000);
    if (ext == NULL) {
        return;
    }
    BeamCount = 0;
    BeamDelay = 0;
    BeamState = 0;
    BeamTarget = 0;
    BeamTimer = 0;
    ext->beamtype = 5;
    PolarBeamOwner = (PolarOwner *)m;
    m->FUX = (EXT *)ext;
    for (i = 0; i < 4; i++) {
        ext->hero_action[i] = -1;
        ext->hero_last_action[i] = -1;
        ext->hero_action_timer[i] = 0;
        ext->hero_immune_timer[i] = 0;
        ext->light_eng[i] = 0;
    }
    ext->effect_player = -1;
    ext->itplayer = 5;
    ext->onoff_timer = 0;
    ext->beamindex = 0;
    ext->ontime = 0x78;
    ext->offtime = Random(0x5A) + 0xD2;
    ext->NasStatus = 1;
    ext->ittimer = -1;
    ext->aimpoint = *(SLONG_XYZ *)&m->Engine->XYZ;
    ext->rad = 0x5DC;
    ext->pause = 0;
    ext->addang = 8;
    ext->yang = 0;
    ext->pause = Random(0x3C);
    m->UserInfo.user1 = 0;
    ext->chasetimer = 0x1E0;
    ext->glow = n = CreateNAS(m->Engine->XYZ.x, m->Engine->XYZ.y, m->Engine->XYZ.z, 0x1605, 0x11);
    if (n != NULL) {
        bx = FindExtension(n, 0x10000000);
        if (bx != NULL) {
            if (((GameState *)GameInfo)->mode == 0x12) {
                bx->unkC = 1;
                bx->flags = 4;
                goto tex1;
            }
            if (((GameState *)GameInfo)->levelFlags & 0x80) {
                if (((GameState *)GameInfo)->mode == 0x13) {
                    bx->unkC = 1;
                }
                if (((GameState *)GameInfo)->mode != 0x15) {
                    goto skip1;
                }
            }
            bx->flags = 4;
            bx->unkC = 4;
        tex1:
            ext->glow->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
        skip1:
            bx->unk18 = 0x100;
            bx->flags |= 8;
            ext->glow->Engine->flags |= 0x200000;
            ext->glow->InitDelete(ext->glow);
        }
    }
    ext->shadow = n = CreateNAS(m->Engine->XYZ.x, m->Engine->XYZ.y, m->Engine->XYZ.z, 0x1605, 0x11);
    if (n != NULL) {
        n->Engine->flags |= 0x200000;
        bx = FindExtension(ext->shadow, 0x10000000);
        if (bx != NULL) {
            if (((GameState *)GameInfo)->mode == 0x12) {
                bx->unkC = 2;
                bx->flags = 4;
                goto tex2;
            }
            if (((GameState *)GameInfo)->levelFlags & 0x80) {
                if (((GameState *)GameInfo)->mode == 0x13) {
                    bx->unkC = 2;
                }
                if (((GameState *)GameInfo)->mode != 0x15) {
                    goto skip2;
                }
            }
            bx->flags = 4;
            bx->unkC = 3;
        tex2:
            ext->shadow->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
        skip2:
            bx->unk18 = 0x100;
            ext->shadow->InitDelete(ext->shadow);
            ext->beam = 0;
        }
    }
    m->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
    m->Engine->sub->entity = 0x2013;
    m->Engine->xform.order = 2;
    m->Engine->flags |= 0x200000;
    ext->it_eng = NULL;
    e = AddExtEngine(m, 2);
    if (e != NULL) {
        ext->it_eng = e;
        e->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
        e->sub->entity = 0x2003;
        e->sub->xform.order = 0;
        s2 = e->sub;
        s2->xform.rot.vz = 0;
        s2->xform.rot.vy = 0;
        s2->xform.rot.vx = 0;
        e->sub->xform.hasScale = 1;
        sub = e->sub;
        sub->xform.scale.vz = 0x200;
        sub->xform.scale.vy = 0x200;
        sub->xform.scale.vx = 0x200;
        e->sub->flags &= ~0x8000;
        e->flags |= 0x200000;
    }
    for (i = 0; i < 4; i++) {
        e = AddExtEngine(m, i + 3);
        if (e != NULL) {
            ext->light_eng[i] = e;
            e->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
            e->sub->entity = 0x2019;
            e->sub->xform.order = 0;
            s2 = e->sub;
            s2->xform.rot.vz = 0;
            s2->xform.rot.vy = 0;
            s2->xform.rot.vx = 0;
            e->sub->xform.hasScale = 1;
            s2 = e->sub;
            s2->xform.scale.vz = 0x800;
            s2->xform.scale.vy = 0x800;
            s2->xform.scale.vx = 0x800;
            e->sub->flags &= ~0x8000;
            e->flags |= 0x200000;
        }
    }
}

/* The hit switch's cases 0-2 in PlotTransmuterBeam: hero i's timer, then its
   effect kind in both slots. As an inline call, case 1's kind 1 gets its own
   argument pseudo (copy_to_mode_reg), which loop.c needs (MATCHING.md, Diff
   patterns). */
static inline void TBSetHeroEffect(EXTtransmuter *e, s32 i, s32 k) {
    e->hero_action_timer[i] = 0xB4;
    e->hero_action[i] = k;
    e->hero_last_action[i] = k;
}

/* The spawn's `if (TB_MODE13()) c++;` is dead (c is never read): flow empties
   it and jump2 deletes the test, but until then its read keeps the mode test's
   view base live, which is why retail holds that base in a1 for a single lh
   (MATCHING.md, Diff patterns). */
void PlotTransmuterBeam(NAS *m) {
    EXTtransmuter *ext;
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
    EXTbeardeath *wx;
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
        if (ext->beam != NULL) {
            KillNAS(ext->beam);
            ext->beam = NULL;
        }
        ext->shadow->Engine->flags &= ~0x8000;
    }
    if (TB_MODE13()) {
        if (TBV->teams != 0 && ext->chasetimer == 0) {
            t = BeamTarget;
            BeamTarget = t ^ 1;
            if (Heroes[t ^ 1].hero_status & 0x2000) {
                BeamTarget = t;
            }
        }
        if (ext->chasetimer < 0) {
            mode = Heroes[BeamTarget].hero_mode;
            if (mode != 0x4B && mode != 0x5A) {
                BeamCount = 1;
            }
        } else {
            ext->chasetimer--;
        }
    }
    if (TB_MODE13() && TBV->pause == 0 && PolarClock == 0 && ext->NasStatus == 1) {
        if (++m->UserInfo.user1 == 0x3C) {
            e1 = m->Engine;
            n = CreateNAS(e1->XYZ.x, 0x80, e1->XYZ.z, 0x1605, 0x11);
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
                m->UserInfo.user2 = (iptr)n;
                m->UserInfo.user1 = 0;
                PlaySfx(0x46D, NULL, NULL, 0x1000, 0x1E00, 0);
            } else {
                m->UserInfo.user2 = 0;
                m->UserInfo.user1 = 0;
            }
        }
        if (m->UserInfo.user1 < 0x20 && m->UserInfo.user2 != 0) {
            if (TB_MODE13()) {
                if (ext->glow != NULL) {
                    ext->glow->Engine->sub->flags |= 0x8000;
                }
                if (ext->shadow != NULL) {
                    ext->shadow->Engine->sub->flags |= 0x8000;
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
                if (TBV->heroCpu[i] != 0) {
                    goto next1;
                }
                if (i == 1 && TBV->teams == 0) {
                    goto next1;
                }
                lx = m->Engine->XYZ.x - Heroes[i].hero_eng->XYZ.x;
                lz = m->Engine->XYZ.z - Heroes[i].hero_eng->XYZ.z;
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
            ((NAS *)m->UserInfo.user2)->Engine->XYZ.x = m->Engine->XYZ.x;
            ((NAS *)m->UserInfo.user2)->Engine->XYZ.z = m->Engine->XYZ.z;
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
            lx = m->Engine->XYZ.x - Heroes[i].hero_eng->XYZ.x;
            lz = m->Engine->XYZ.z - Heroes[i].hero_eng->XYZ.z;
            ld = lx * lx + lz * lz;
            if (ld < best) {
                closest = i;
                best = ld;
            }
            if (ext->hero_immune_timer[i] > 0) {
                ext->hero_immune_timer[i]--;
            }
        }
    }
    if (ext->NasStatus == 1) {
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
                ox = me->XYZ.x - (ext->aimpoint.x + (((ext->rad << 8) >> 8) * tr[(tr[(a - 0x400) & 0xFFF].unk2 - 0x400) & 0xFFF].unk2 >> 12));
                sa = tr[a & 0xFFF].unk2;
                oz = me->XYZ.z - (ext->aimpoint.z + (ext->rad * tr[sa & 0xFFF].unk2 >> 12));
                me->XYZ.x -= ox >> 4;
                m->Engine->XYZ.z -= oz >> 4;
                if (ox * ox + oz * oz < 0x1000) {
                    BeamCount = 0;
                    BeamDelay = 0;
                    BeamTimer = 0;
                    ext->chasetimer = (Random(4) + 0x12) * 0x1E;
                }
            } else {
                EXTengine *me;
                EXTengine *te;
                s32 ddx;
                s32 ddz;

                me = m->Engine;
                te = h3->hero_eng;
                BeamDelay++;
                ddx = me->XYZ.x - te->XYZ.x;
                ddz = me->XYZ.z - te->XYZ.z;
                me->XYZ.x -= ddx >> 5;
                m->Engine->XYZ.z -= ddz >> 5;
            }
        } else {
            TrigEntry *tr;

            tr = rcossin_tbl;
            t2 = BeamState >> 1;
            m->Engine->XYZ.x = ext->aimpoint.x + (((ext->rad << 8) >> 8) * tr[(tr[(t2 - 0x400) & 0xFFF].unk2 - 0x400) & 0xFFF].unk2 >> 12);
            BeamTimer = 0;
            t5 = tr[t2 & 0xFFF].unk2;
            m->Engine->XYZ.z = ext->aimpoint.z + (ext->rad * tr[t5 & 0xFFF].unk2 >> 12);
        }
        if (PolarClock == 0) {
            m->Engine->XYZ.y = ext->aimpoint.y + (rcossin_tbl[((ext->yang << 4) - 0x400) & 0xFFF].unk2 * 64 >> 12);
        } else {
            m->Engine->XYZ.y -= 0x10;
        }
        ext->yang += ext->addang;
        if (BeamCount == 0) {
            BeamState += ext->addang;
        }
        if (ext->glow != NULL) {
            ext->glow->Engine->XYZ = m->Engine->XYZ;
            ext->glow->Engine->sub->otz = 0x200;
            if (GameMode == 0x12) {
                ext->glow->Engine->XYZ.y -= 0x40;
            } else {
                ext->glow->Engine->XYZ.y -= 0x60;
            }
        }
    }
    m->Engine->xform.dirty = 1;
    if (ext->shadow != NULL) {
        ext->shadow->Engine->XYZ = m->Engine->XYZ;
        ext->shadow->Engine->sub->otz = 0x100;
        ext->shadow->Engine->XYZ.y = 0x5E;
    }
    me = m->Engine;
    ce = Heroes[closest].hero_eng;
    dx = me->XYZ.x - ce->XYZ.x;
    dz = me->XYZ.z - ce->XYZ.z;
    dy = (me->XYZ.y - ce->XYZ.y) + 0x1C2;
    d = csqrt(dx * dx + dz * dz) >> 6;
    yaw = get_angle(dx, dz) & 0xFFF;
    pitch = get_angle(-dy, d);
    if (ext->NasStatus == 1 && BeamCount == 0 && PolarBeamTimer == 0) {
        ext->onoff_timer++;
        if (!TB_MODE13() || m->UserInfo.user1 >= 0x1F) {
            if (abs(ext->onoff_timer - ext->offtime) == 0xF) {
                PlaySfx(0x463, NULL, NULL, 0x1000, 0x1E00, 0);
            }
            if (ext->onoff_timer >= ext->offtime) {
                ext->beamindex = Random(4);
                ext->onoff_timer = 0;
                ext->beamtype = BeamFrames[ext->beamindex];
                if (HeroesAllOut != 0) {
                    if (ext->beamtype == 0) {
                        ext->beamtype = 6;
                    }
                    if (ext->beamtype == 1) {
                        ext->beamtype = 5;
                    }
                }
                e2 = m->Engine;
                n = CreateNAS(e2->XYZ.x, 0x5E, e2->XYZ.z, 0x1605, 0x11);
                if (n != NULL) {
                    ext->beam = n;
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
                ext->NasStatus = 2;
                PlaySfx(0x44C, &m->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
            }
        }
    }
    if (ext->NasStatus == 2) {
        if (PolarClock != 0) {
            ext->NasStatus = 1;
            if (m->Engine->sfx != NULL) {
                StopSfx(m->Engine->sfx);
            }
        }
        if (++ext->onoff_timer >= ext->ontime) {
            ext->NasStatus = 1;
            PlaySfx(0x44D, NULL, NULL, 0x1000, 0x1E00, 0);
            if (m->Engine->sfx != NULL) {
                StopSfx(m->Engine->sfx);
            }
            ext->onoff_timer = 0;
        }
        pitch = 0x400;
    }
    m->Engine->xform.rot.vx -= ((m->Engine->xform.rot.vx - pitch) << 20) >> 22;
    m->Engine->xform.rot.vy -= ((m->Engine->xform.rot.vy - yaw) << 20) >> 22;
    m->Engine->xform.rot.vz = 0;
    if (TB_MODE13()) {
        if (ext->glow != NULL) {
            ext->glow->Engine->sub->entity = 0x2008;
        }
        if (ext->shadow != NULL) {
            ext->shadow->Engine->sub->entity = 0x2008;
        }
    }
    if (ext->NasStatus & 2) {
        if (TB_MODE13()) {
            if (ext->glow != NULL) {
                ext->glow->Engine->sub->entity = 0x200C;
            }
            if (ext->shadow != NULL) {
                ext->shadow->Engine->sub->entity = 0x200C;
            }
        }
        for (i = 0;; i++) {
            s32 lx;
            s32 lz;
            s32 ld;

            if (i >= 4) {
                break;
            }
            lx = Heroes[i].hero_eng->XYZ.x - m->Engine->XYZ.x;
            lz = Heroes[i].hero_eng->XYZ.z - m->Engine->XYZ.z;
            ld = lx * lx + lz * lz;
            if (Heroes[i].hero_status & 0x2000 || ext->effect_player != -1 || ld > 0x18FFF) {
                continue;
            }
            ext->effect_player = i;
            ext->NasStatus = 1;
            ext->onoff_timer = 0;
            if (m->Engine->sfx != NULL) {
                StopSfx(m->Engine->sfx);
            }
            if (ext->beam != NULL) {
                KillNAS(ext->beam);
                ext->beam = NULL;
            }
            e3 = m->Engine;
            n = CreateNAS(e3->XYZ.x, 0x5E, e3->XYZ.z, 0x1605, 0x11);
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
                    if (TBV->mode == 0x12 && (TBV->levelFlags & 0x80)) {
                        bx->unkC = 7;
                    }
                    bx->unk18 = 0x80;
                    bx->unk20 = 1;
                    bx->unk28 = 0;
                }
                n->InitDelete(n);
            }
            switch (ext->beamtype) {
            case 0:
                PolarBeamRelease(i, ext->hero_last_action[i], 0);
                ext->ontime = 0x78;
                ext->offtime = Random(0x3C) + 0xD2;
                TBSetHeroEffect(ext, i, 0);
                Heroes[i].hero_move->mass = 0x50;
                PolarHeroMass[i] = 0x40;
                ext->hero_in_beam_timer = 0;
                PlaySfx(0x44E, NULL, NULL, 0x1000, 0x1E00, 0);
                SetHeroAction(i, 0x4F);
                break;
            case 1:
                PolarBeamRelease(i, ext->hero_last_action[i], 1);
                ext->ontime = 0x78;
                ext->offtime = Random(0x3C) + 0xD2;
                TBSetHeroEffect(ext, i, 1);
                Heroes[i].hero_move->mass = 0x258;
                PolarHeroMass[i] = 0x20;
                ext->hero_in_beam_timer = 0;
                PlaySfx(0x44F, NULL, NULL, 0x1000, 0x1E00, 0);
                SetHeroAction(i, 0x4F);
                break;
            case 2:
                PolarBeamRelease(i, ext->hero_last_action[i], 2);
                ext->ontime = 0x78;
                ext->offtime = Random(0x3C) + 0xD2;
                TBSetHeroEffect(ext, i, 2);
                ext->hero_in_beam_timer = 0xF;
                PlaySfx(0x450, NULL, NULL, 0x1000, 0x1E00, 0);
                SetHeroAction(i, 0x4F);
                break;
            case 5:
                PolarBeamRelease(i, ext->hero_last_action[i], 5);
                ext->ontime = 0x78;
                ext->offtime = 0x168;
                ext->hero_action_timer[i] = 0;
                ext->hero_action[i] = 5;
                ext->hero_in_beam_timer = 0xF;
                ext->hero_last_action[i] = 5;
                PlaySfx(0x451, NULL, NULL, 0x1000, 0x1E00, 0);
                PlaySfx(0x452, &ext->it_eng->sfx, NULL, 0x1000, 0x1E00, 0);
                SetHeroAction(i, 0x4F);
                a1x = FindExtension(Heroes[ext->effect_player].hero, 0x20000000);
                if (a1x != NULL) {
                    a1x->unk28 = 0;
                }
                break;
            case 6:
                ext->ontime = 0x78;
                ext->offtime = 0x168;
                ext->hero_action_timer[i] = 0;
                ext->hero_action[i] = 6;
                ext->hero_in_beam_timer = 0xF;
                ext->hero_last_action[i] = 6;
                PlaySfx(0x464, NULL, NULL, 0x1000, 0x1E00, 0);
                e = m->Engine;
                PlaySfx(0x465, &e->sfx, &e->XYZ, 0x1000, 0x1E00, 0);
                e = ext->it_eng;
                PlaySfx(0x452, &e->sfx, &e->XYZ, 0x1000, 0x1E00, 0);
                SetHeroAction(i, 0x4F);
                a1x = FindExtension(Heroes[ext->effect_player].hero, 0x20000000);
                if (a1x != NULL) {
                    a1x->unk28 = 0;
                }
                break;
            }
        }
    }
    if (ext->effect_player != -1) {
        hb1 = Heroes;
        h5 = &hb1[ext->effect_player];
        he = h5->hero_eng;
        hs = h5->hero_shadow;
        if (PolarRoundState != 0) {
            if (m->Engine->sfx != NULL) {
                StopSfx(m->Engine->sfx);
            }
            if (ext->it_eng->sfx != NULL) {
                StopSfx(ext->it_eng->sfx);
            }
            if (Heroes[ext->effect_player].hero_status & 0x2000) {
                return;
            }
        }
        if (ext->hero_action_timer[ext->effect_player] > 0) {
            ext->hero_action_timer[ext->effect_player]--;
        }
        hs->xform.hasScale = 1;
        if (ext->hero_in_beam_timer > 0) {
            ext->hero_in_beam_timer--;
        }
        switch (ext->hero_action[ext->effect_player]) {
        case 0:
            if (ext->hero_in_beam_timer <= 0) {
                if (he->xform.scale.vx > 0xA00) {
                    he->xform.scale.vx -= 0x100;
                    he->xform.scale.vy -= 0x100;
                    he->xform.scale.vz -= 0x100;
                    hs->xform.scale.vx -= 0x100;
                    hs->xform.scale.vy -= 0x100;
                    hs->xform.scale.vz -= 0x100;
                } else {
                    ai = FindExtension(Heroes[ext->effect_player].hero, 0x20000000);
                    if (ai != NULL) {
                        if (ai->flag == 0) {
                            SetHeroAction(ext->effect_player, 0x49);
                        }
                        if (ai->flag == 1) {
                            SetHeroAction(ext->effect_player, 0x54);
                        }
                    }
                    ext->hero_action[ext->effect_player] = -1;
                }
            }
            break;
        case 1:
            if (ext->hero_in_beam_timer <= 0) {
                if (he->xform.scale.vx < 0x1771) {
                    he->xform.scale.vx += 0x100;
                    he->xform.scale.vy += 0x100;
                    he->xform.scale.vz += 0x100;
                    hs->xform.scale.vx += 0x100;
                    hs->xform.scale.vy += 0x100;
                    hs->xform.scale.vz += 0x100;
                } else {
                    ext->hero_action[ext->effect_player] = -1;
                    ai = FindExtension(Heroes[ext->effect_player].hero, 0x20000000);
                    if (ai != NULL) {
                        if (ai->flag == 0) {
                            SetHeroAction(ext->effect_player, 0x49);
                        }
                        if (ai->flag == 1) {
                            SetHeroAction(ext->effect_player, 0x54);
                        }
                    }
                }
            }
            break;
        case 2:
            if (ext->hero_in_beam_timer <= 0 && Heroes[ext->effect_player].hero_mode != 0x4C) {
                SetHeroAction(ext->effect_player, 0x4C);
                mv = Heroes[ext->effect_player].hero_move;
                if (mv != NULL) {
                    mv->x_max = 0;
                    mv->z_max = 0;
                    mv->x_vel = 0;
                    mv->z_vel = 0;
                }
                ext->hero_action[ext->effect_player] = -1;
            }
            break;
        case 5:
            if (ext->hero_in_beam_timer <= 0) {
                ext->itplayer = ext->effect_player;
                if (ext->it_eng != NULL) {
                    ext->it_eng->XYZ = Heroes[ext->itplayer].hero_eng->XYZ;
                    ext->it_eng->sub->flags |= 0x8000;
                }
                ext->ittimer = 0x12C;
                ext->hero_action[ext->effect_player] = -1;
                ai = FindExtension(Heroes[ext->effect_player].hero, 0x20000000);
                if (ai != NULL) {
                    if (ai->flag == 0) {
                        SetHeroAction(ext->effect_player, 0x49);
                    }
                    if (ai->flag == 1) {
                        SetHeroAction(ext->effect_player, 0x54);
                    }
                }
            }
            break;
        case 6:
            k2 = 0;
            if (ext->hero_in_beam_timer <= 0) {
                ext->itplayer = ext->effect_player;
                do {
                    if (k2 == ext->itplayer && !(Heroes[k2].hero_status & 0x2000) && ext->light_eng[k2] != NULL) {
                        ext->light_eng[k2]->XYZ = Heroes[k2].hero_eng->XYZ;
                        ext->light_eng[k2]->sub->flags |= 0x8000;
                    }
                    k2++;
                } while (k2 < 4);
                ext->ittimer = 0x12C;
                ext->hero_action[ext->effect_player] = -1;
                ai = FindExtension(Heroes[ext->effect_player].hero, 0x20000000);
                if (ai != NULL) {
                    if (ai->flag == 0) {
                        SetHeroAction(ext->effect_player, 0x49);
                    }
                    if (ai->flag == 1) {
                        SetHeroAction(ext->effect_player, 0x54);
                    }
                }
            }
            break;
        }
        if (ext->hero_action_timer[ext->effect_player] == 0) {
            switch (ext->hero_last_action[ext->effect_player]) {
            case 0:
                if (he->xform.scale.vx < 0x1000) {
                    he->xform.scale.vx += 0x100;
                    he->xform.scale.vy += 0x100;
                    he->xform.scale.vz += 0x100;
                    hs->xform.scale.vx += 0x100;
                    hs->xform.scale.vy += 0x100;
                    hs->xform.scale.vz += 0x100;
                } else {
                    ext->hero_last_action[ext->effect_player] = -1;
                    PolarHeroMass[ext->effect_player] = HeroHooks.RunSpeeds[Heroes[ext->effect_player].hero_name * 3];
                    ext->onoff_timer = 0;
                    ResetMass(ext->effect_player);
                    ext->effect_player = -1;
                    he->xform.scale.vx = 0x1000;
                }
                break;
            case 1:
                if (he->xform.scale.vx > 0x1000) {
                    he->xform.scale.vx -= 0x100;
                    he->xform.scale.vy -= 0x100;
                    he->xform.scale.vz -= 0x100;
                    hs->xform.scale.vx -= 0x100;
                    hs->xform.scale.vy -= 0x100;
                    hs->xform.scale.vz -= 0x100;
                } else {
                    ext->hero_last_action[ext->effect_player] = -1;
                    ResetMass(ext->effect_player);
                    PolarHeroMass[ext->effect_player] = HeroHooks.RunSpeeds[Heroes[ext->effect_player].hero_name * 3];
                    ext->onoff_timer = 0;
                    ext->effect_player = -1;
                    he->xform.scale.vx = 0x1000;
                }
                break;
            case 2:
                a1x = FindExtension(Heroes[ext->effect_player].hero, 0x20000000);
                if (a1x != NULL) {
                    if (a1x->flag == 0) {
                        if (Heroes[ext->effect_player].hero_mode != 0x49) {
                            SetHeroAction(ext->effect_player, 0x49);
                            ext->offtime = 0xD2;
                        } else {
                            ext->hero_last_action[ext->effect_player] = -1;
                            ext->onoff_timer = 0;
                        }
                    } else {
                        if (Heroes[ext->effect_player].hero_mode != 0x54) {
                            SetHeroAction(ext->effect_player, 0x54);
                            ext->offtime = 0xD2;
                        } else {
                            ext->hero_last_action[ext->effect_player] = -1;
                            ext->onoff_timer = 0;
                        }
                    }
                    ext->effect_player = -1;
                }
                break;
            case 5:
                if (ext->itplayer != 5) {
                    if (ext->ittimer > 0) {
                        t3 = --ext->ittimer;
                        if (ext->it_eng->sfx != NULL) {
                            sfx_set_pitch(ext->it_eng->sfx, (0x12C - t3) * 0xF00 / 300 + 0x1E00);
                        }
                        if (ext->it_eng != NULL) {
                            ext->it_eng->XYZ = Heroes[ext->itplayer].hero_eng->XYZ;
                            if (PolarRoundState == 0) {
                                ext->it_eng->sub->flags |= 0x8000;
                            } else {
                                ext->it_eng->sub->flags &= ~0x8000;
                            }
                            ext->it_eng->xform.rot.vy += 0x80;
                            ext->it_eng->xform.dirty = 1;
                            switch (Heroes[ext->itplayer].hero_name) {
                            case 1:
                                ext->it_eng->XYZ.y -= 0x258;
                                break;
                            case 2:
                                ext->it_eng->XYZ.y -= 0x300;
                                break;
                            case 3:
                                ext->it_eng->XYZ.y -= 0x300;
                                break;
                            case 4:
                                ext->it_eng->XYZ.y -= 0x300;
                                break;
                            case 5:
                                ext->it_eng->XYZ.y -= 0x300;
                                break;
                            case 6:
                                ext->it_eng->XYZ.y -= 0x300;
                                break;
                            case 7:
                                ext->it_eng->XYZ.y -= 0x364;
                                break;
                            case 8:
                                ext->it_eng->XYZ.y -= 0x364;
                                break;
                            default:
                                ext->it_eng->XYZ.y -= 0x300;
                                break;
                            }
                        }
                    }
                    if (ext->ittimer == 0) {
                        hb2 = Heroes;
                        h = &hb2[ext->itplayer];
                        if (h->hero_mode != 0x4D) {
                            h->hero_status |= 0x2000;
                            hq = Heroes[ext->itplayer].hero_eng;
                            if (hq != NULL) {
                                NAS *dn;

                                dn = CreateNAS(hq->XYZ.x, hq->XYZ.y - 0x898, hq->XYZ.z, 0x1602, 0x11);
                                if (dn != NULL) {
                                    wx = FindExtension(dn, 0x10000000);
                                    if (wx != NULL) {
                                        wx->NasStatus = hq->XYZ.y;
                                        wx->aimpoint.z = ext->itplayer;
                                    }
                                }
                            }
                            SetHeroAction(ext->itplayer, 0x4D);
                            ext->it_eng->sub->flags &= ~0x8000;
                            ext->offtime = 0xD2;
                            if (ext->it_eng->sfx != NULL) {
                                StopSfx(ext->it_eng->sfx);
                            }
                            ext->hero_last_action[ext->effect_player] = -1;
                            ext->onoff_timer = 0;
                            ext->effect_player = -1;
                            ext->itplayer = 5;
                        }
                    }
                    if (PolarClock != 0 || (hb3 = Heroes, h = &hb3[ext->itplayer], h->hero_mode == 0x50) || (h->hero_status & 0x2000)) {
                        if (ext->it_eng->sfx != NULL) {
                            StopSfx(ext->it_eng->sfx);
                        }
                        ext->it_eng->sub->flags &= ~0x8000;
                        ext->offtime = 0xD2;
                        if (ext->effect_player != -1) {
                            ext->hero_last_action[ext->effect_player] = -1;
                        }
                        ext->onoff_timer = 0;
                        ext->effect_player = -1;
                        ext->itplayer = 5;
                    }
                }
                break;
            case 6:
                if (ext->itplayer != 5) {
                    if (ext->ittimer > 0) {
                        t4 = --ext->ittimer;
                        if (ext->it_eng->sfx != NULL) {
                            sfx_set_pitch(ext->it_eng->sfx, (0x12C - t4) * 0xF00 / 300 + 0x1E00);
                        }
                        if (ext->it_eng != NULL) {
                            k = 0;
                            do {
                                if (k != ext->itplayer || (Heroes[k].hero_status & 0x2000)) {
                                    ext->light_eng[k]->sub->flags &= ~0x8000;
                                } else if (ext->light_eng[k] != NULL) {
                                    ext->light_eng[k]->XYZ = Heroes[k].hero_eng->XYZ;
                                    if (PolarRoundState == 0) {
                                        ext->light_eng[k]->sub->flags |= 0x8000;
                                    } else {
                                        ext->light_eng[k]->sub->flags &= ~0x8000;
                                    }
                                    ext->light_eng[k]->xform.rot.vy += 0x80;
                                    ext->light_eng[k]->xform.dirty = 1;
                                    switch (Heroes[k].hero_name) {
                                    case 1:
                                        ext->light_eng[k]->XYZ.y -= 0x258;
                                        break;
                                    case 2:
                                        ext->light_eng[k]->XYZ.y -= 0x300;
                                        break;
                                    case 3:
                                        ext->light_eng[k]->XYZ.y -= 0x300;
                                        break;
                                    case 4:
                                        ext->light_eng[k]->XYZ.y -= 0x300;
                                        break;
                                    case 5:
                                        ext->light_eng[k]->XYZ.y -= 0x300;
                                        break;
                                    case 6:
                                        ext->light_eng[k]->XYZ.y -= 0x300;
                                        break;
                                    case 7:
                                        ext->light_eng[k]->XYZ.y -= 0x300;
                                        break;
                                    case 8:
                                        ext->light_eng[k]->XYZ.y -= 0x300;
                                        break;
                                    default:
                                        ext->light_eng[k]->XYZ.y -= 0x300;
                                        break;
                                    }
                                }
                                k++;
                            } while (k < 4);
                        }
                    }
                    if (ext->ittimer == 0) {
                        for (j = 0; j < 4; j++) {
                            h = &Heroes[j];
                            if (j != ext->itplayer && !(h->hero_status & 0x2000) && h->hero_mode != 0x50) {
                                if (TBV->teams == 0 || TestForTeamMate(ext->itplayer, j) == 0) {
                                    if (h->hero_mode != 0x4B) {
                                        SetHeroAction(j, 0x4B);
                                        ext->light_eng[ext->itplayer]->sub->flags &= ~0x8000;
                                    }
                                }
                            }
                        }
                        ext->offtime = 0x14A;
                        ext->hero_last_action[ext->effect_player] = -1;
                        if (m->Engine->sfx != NULL) {
                            StopSfx(m->Engine->sfx);
                        }
                        PlaySfx(0x466, NULL, NULL, 0x1000, 0x1E00, 0);
                        if (ext->it_eng->sfx != NULL) {
                            StopSfx(ext->it_eng->sfx);
                        }
                        ext->onoff_timer = 0;
                        ext->effect_player = -1;
                        ext->itplayer = 5;
                    }
                    hb4 = Heroes;
                    h = &hb4[ext->itplayer];
                    mode = h->hero_mode;
                    if (mode == 0x56 || mode == 0x50 || (h->hero_status & 0x2000) || PolarClock != 0) {
                        k3 = 0;
                        do {
                            if (k3 == ext->itplayer) {
                                ext->light_eng[k3]->sub->flags &= ~0x8000;
                            }
                            k3++;
                        } while (k3 < 4);
                        ext->offtime = 0xD2;
                        ext->hero_last_action[ext->effect_player] = -1;
                        ext->onoff_timer = 0;
                        ext->effect_player = -1;
                        ext->itplayer = 5;
                        if (m->Engine->sfx != NULL) {
                            StopSfx(m->Engine->sfx);
                        }
                        if (ext->it_eng->sfx != NULL) {
                            StopSfx(ext->it_eng->sfx);
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
            ext->light_eng[n2]->flags &= ~0x8000;
        }
        ext->it_eng->flags &= ~0x8000;
    }
}

void PlotWeightDeath(NAS *m) {
    EXTbeardeath *ext;
    HeroInfo *h;
    HeroInfo *base;
    HeroInfo *p;
    HeroInfo *q;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    if (ext->startpoint.y-- >= 0) {
        return;
    }
    m->Engine->XYZ.y += ext->ydir;
    ext->ydir += ext->aimpoint.x;
    if (ext->NasStatus < m->Engine->XYZ.y) {
        m->Engine->XYZ.y -= abs(m->Engine->XYZ.y - ext->NasStatus);
        if (++ext->aimpoint.y < 5) {
            ext->ydir = -ext->ydir >> 3;
            if (ext->aimpoint.y == 1) {
                PlaySfx(0x455, NULL, NULL, 0x1000, 0x1E00, 0);
            }
        }
    }
    if (ext->startpoint.x > 0x3C) {
        HeroInfo *b = Heroes;

        q = &b[ext->aimpoint.z];
        if (!(q->hero_status & 0x200)) {
            q->hero_status |= 0x2000;
        }
        m->Engine->sub->flags &= ~0x8000;
    }
    if (ext->startpoint.x > 0x78) {
        base = Heroes;
        p = &base[ext->aimpoint.z];
        if (!(p->hero_status & 0x200)) {
            p->hero_status |= 0x2000;
            base[ext->aimpoint.z].hero_status |= 0x8000;
        }
    } else {
        ext->startpoint.x++;
    }
    if (ext->startpoint.x > 0xF) {
        base = Heroes;
        h = &base[ext->aimpoint.z];
        if (!(h->hero_status & 0x200)) {
            h->hero_eng->flags &= ~0x8000;
            base[ext->aimpoint.z].hero_shadow->flags &= ~0x8000;
        }
    }
}

void GravityMotionUpdate(NAS *nas, EXTmotion *m) {
    EXTengine *eng;
    s32 y;
    s32 floor;

    eng = nas->Engine;
    if (eng != NULL) {
        eng->XYZ.x += m->Xvel;
        eng->XYZ.y += m->Yvel;
        eng->XYZ.z += m->Zvel;
        m->Xvel += m->Xacc;
        m->Yvel += m->Yacc;
        m->Zvel += m->Zacc;
        if (m->flags & 4) {
            nas->Engine->xform.rot.vx += m->Xa;
        }
        if (m->flags & 8) {
            nas->Engine->xform.rot.vy += m->Ya;
        }
        if (m->flags & 0x10) {
            nas->Engine->xform.rot.vz += m->Za;
        }
        nas->Engine->xform.dirty = 1;
        floor = m->floorheight;
        y = eng->XYZ.y;
        if (y >= floor) {
            eng->XYZ.y -= abs(y - floor);
            if (m->flags & 1) {
                m->Yvel = -m->Yvel >> 1;
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
        if ((e->XYZ.x - e->OldXYZ.x) * (e->XYZ.x - e->OldXYZ.x)
            + (e->XYZ.z - e->OldXYZ.z) * (e->XYZ.z - e->OldXYZ.z) > 0 && s != NULL) {
            if (Heroes[m->UserInfo.user1].hero_mode == 0x49) {
                if (!(m->UserInfo.user2++ & 1) && !(s->flags & 0x8000)) {
                    *(SLONG_XYZ *)&s->XYZ.x = Heroes[m->UserInfo.user1].hero_eng->XYZ;
                    if (Heroes[m->UserInfo.user1].hero_eng->xform.scale.vx > 0x1000) {
                        s->xform.hasScale = 1;
                        dx = Heroes[m->UserInfo.user1].hero_eng->xform.scale.vx;
                        s->xform.scale.vz = dx;
                        s->xform.scale.vy = dx;
                        s->xform.scale.vx = dx;
                    }
                    if (Heroes[m->UserInfo.user1].hero_eng->xform.scale.vx < 0x1000) {
                        s->xform.hasScale = 1;
                        v = Heroes[m->UserInfo.user1].hero_eng->xform.scale.vx;
                        s->xform.scale.vz = v;
                        s->xform.scale.vy = v;
                        s->xform.scale.vx = v;
                    }
                    if (GameMode == 0x15) {
                        s->flags = 0x40008021;
                        s->entity = 0x2030;
                    } else {
                        s->flags = 0x40008061;
                    }
                    ang = Heroes[m->UserInfo.user1].hero_eng->xform.rot.vy + 0x800;
                    s->xform.dirty = 1;
                    s->scale = 0x400;
                    s->xform.rot.vy = ang;
                }
            }
            if (Heroes[m->UserInfo.user1].hero_mode == 0x4A
                && Heroes[m->UserInfo.user1].hero_anim->Aindex == 9
                && !(s->flags & 0x8000)) {
                if (Heroes[m->UserInfo.user1].hero_eng->xform.scale.vx > 0x1000) {
                    s->xform.hasScale = 1;
                    w = Heroes[m->UserInfo.user1].hero_eng->xform.scale.vx;
                    s->xform.scale.vz = w;
                    s->xform.scale.vy = w;
                    s->xform.scale.vx = w;
                }
                if (Heroes[m->UserInfo.user1].hero_eng->xform.scale.vx < 0x1000) {
                    s->xform.hasScale = 1;
                    x = Heroes[m->UserInfo.user1].hero_eng->xform.scale.vx;
                    s->xform.scale.vz = x;
                    s->xform.scale.vy = x;
                    s->xform.scale.vx = x;
                }
                *(SLONG_XYZ *)&s->XYZ.x = Heroes[m->UserInfo.user1].hero_eng->XYZ;
                if (GameMode == 0x15) {
                    s->flags = 0x40008021;
                    s->entity = 0x2030;
                } else {
                    s->flags = 0x40008061;
                }
                ang = Heroes[m->UserInfo.user1].hero_eng->xform.rot.vy + 0x800;
                s->scale = 0;
                s->xform.dirty = 1;
                s->xform.rot.vy = ang;
            }
        }
    }
    for (s = m->Engine->sub; s != NULL; s = s->next) {
        if (s->flags & 0x8000) {
            s->scale += 0x60;
            if ((s16)s->scale > 0xC00) {
                s->flags = 0;
                s->xform.scale.vz = 0x1000;
                s->xform.scale.vy = 0x1000;
                s->xform.scale.vx = 0x1000;
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
        m->Engine->sub->xform.order = 0;
        if (m->UserInfo.user1 == 1) {
            m->Engine->sub->xform.rot.vy = 0x800;
            m->Engine->sub->otz = 0x200;
        }
        m->Engine->sub->otz = 0x200;
    }
}

s32 PolarIceHit(SLONG_XYZ *pos, s32 player) {
    volatile s32 t0;
    volatile s32 t1;
    subengine *o;
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
                dx = o->XYZ.x - pos->x;
                dz = o->XYZ.z - pos->z;
                if (abs(dx) < 0x14B && abs(dz) < 0x14B) {
                    d = dx * dx + dz * dz;
                    if (d <= 0x1A963) {
                        ang = get_angle(dx, dz) & 0xFFF;
                        eng = Heroes[player].hero_eng;
                        d = csqrt(d) >> 6;
                        d = 0x14A - d;
                        d += 2;
                        eng->XYZ.x -= d * t[(ang - 0x400) & 0xFFF].unk2 >> 12;
                        eng->XYZ.z -= d * t[ang].unk2 >> 12;
                        ang = get_angle(eng->XYZ.x, eng->XYZ.z) & 0xFFF;
                        mv = Heroes[player].hero_move;
                        if (mv != NULL) {
                            sp = csqrt(mv->x_vel * mv->x_vel + mv->z_vel * mv->z_vel) >> 6;
                            mv->x_vel = -(sp * t[(-ang + 0x400) & 0xFFF].unk2 >> 13);
                            mv->z_vel = -(sp * t[-ang & 0xFFF].unk2 >> 13);
                            sp *= Heroes[player].hero_move->mass;
                            if ((HeroHooks.RunSpeeds[Heroes[player].hero_name * 3] * 0xC000 >> 8) < sp) {
                                nas = CreateNAS(o->XYZ.x, o->XYZ.y - 0x80, o->XYZ.z, 0x1604, 0x11);
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
                                        nas->Engine->sub->otz = 0x200;
                                    }
                                }
                                nas = CreateNAS(o->XYZ.x, o->XYZ.y - 0x80, o->XYZ.z, 0x1604, 0x11);
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
                                        nas->Engine->sub->otz = 0x200;
                                    }
                                }
                                nas = CreateNAS(o->XYZ.x, o->XYZ.y - 0x80, o->XYZ.z, 0x1604, 0x11);
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
                                        nas->Engine->sub->otz = 0x200;
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
    subengine *obj;
    subengine *scan;
    s32 n;
    VECTOR v;
    VECTOR *vp;

    scan = ((subengine *)MapScene->subs);
    for (i = 15; i >= 0; i--) {
        UkaMaskObjects[i] = NULL;
    }
    n = 0;
    while (scan != NULL) {
        if ((scan->flags & 0x8000) && scan->entity == 0x4000) {
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
        ApplyMatrixLV(&((subengine *)obj)->xform.mtx, vp, vp);
        i++;
        UkaMaskRisers[(((get_angle(v.vx, vp->vz) + 1) & 0xFFF) >> 8) - 1 & 0xF] = (PolarRiserObj *)obj;
    }
}

#define UKA_SEG_SET(j, st, a) \
    { m->seg[j].status = (st); m->seg[j].baseframe = (a); m->seg[j].secframe = 0; m->seg[j].hitcount++; }

void UkaMaskSetSegments(s32 i, UkaMaskSet *m) {
    s32 next;
    s32 prev;

    next = (i + 1) & 0xF;
    prev = (i - 1) & 0xF;
    if (m->seg[i].hitcount == 0) {
        UKA_SEG_SET(i, 1, 0x4000);
        switch (m->seg[next].status) {
        case 0:
            UKA_SEG_SET(next, 2, 0x4300);
            break;
        case 3:
            UKA_SEG_SET(next, 4, 0x4200);
            break;
        }
        switch (m->seg[prev].status) {
        case 0:
            UKA_SEG_SET(prev, 3, 0x4380);
            break;
        case 2:
            UKA_SEG_SET(prev, 4, 0x4100);
            break;
        }
    } else if (m->seg[i].hitcount == 1) {
        switch (m->seg[i].status) {
        case 3:
            UKA_SEG_SET(i, 1, 0x4280);
            break;
        case 2:
            UKA_SEG_SET(i, 1, 0x4180);
            break;
        }
        switch (m->seg[prev].status) {
        case 0:
            UKA_SEG_SET(prev, 3, 0x4380);
            break;
        case 2:
            UKA_SEG_SET(prev, 4, 0x4100);
            break;
        }
        switch (m->seg[next].status) {
        case 0:
            UKA_SEG_SET(next, 2, 0x4300);
            break;
        case 3:
            UKA_SEG_SET(next, 4, 0x4200);
            break;
        }
    } else if (m->seg[i].hitcount == 2) {
        if (m->seg[i].status == 4) {
            UKA_SEG_SET(i, 1, 0x4080);
        }
    }
}

void InitUkaPolarMask(NAS *m) {
    EXTtransmuter *ext;
    UkaBitExt *bx;
    EXTengine *e;
    subengine *sub;
    subengine *s2;
    NAS *n;
    s32 r;
    s32 i;

    ext = AddExt(m, sizeof(EXTtransmuter), 0x40000000);
    if (ext == NULL) {
        return;
    }
    UkaMaskInitRisers();
    ext->NasStatus = 4;
    ext->timer = 0x12C;
    ext->angular_speed = 0xC;
    ext->angular_max = 0xC;
    m->FUX = (EXT *)ext;
    m->Engine->sub->u6C.dbase = MapScene->dbase;
    m->Engine->sub->entity = 0x200C;
    PolarBeamOwner = (PolarOwner *)m;
    if (GameLevelFlags & 0x80) {
        m->Engine->sub->entity = 0x200C;
    }
    m->UserInfo.user1 = 0;
    m->Engine->XYZ.x = 0;
    m->Engine->XYZ.z = 0;
    ext->effect_player = -1;
    r = Random(4);
    if (r == 0) {
        ext->target_angle = -0x100;
    }
    if (r == 1) {
        ext->target_angle = 0x300;
    }
    if (r == 2) {
        ext->target_angle = 0x700;
    }
    if (r == 3) {
        ext->target_angle = 0xB00;
    }
    ext->target_pos.x = (rcossin_tbl[(ext->target_angle - 0x400) & 0xFFF].unk2 * 0x115) >> 9;
    ext->target_pos.z = (rcossin_tbl[ext->target_angle & 0xFFF].unk2 * 0x115) >> 9;
    ext->angular_acc = 1;
    ext->angular_dec = 1;
    ext->ittimer = -1;
    ext->aimpoint = *(SLONG_XYZ *)&m->Engine->XYZ;
    ext->offtime = 0x5A;
    ext->itplayer = 5;
    for (i = 0; i < 16; i++) {
        ext->sectordata[i].status = 0;
        ext->sectordata[i].hitcount = 0;
        ext->sectordata[i].secframe = 0;
        ext->sectordata[i].baseframe = 0;
    }
    ext->cam = n = CreateNAS(0, 0, 0, 0x160B, 0x11);
    if (n != NULL) {
        AddCamera(n);
    }
    ext->shadow = n = CreateNAS(m->Engine->XYZ.x, m->Engine->XYZ.y, m->Engine->XYZ.z, 0x1605, 0x11);
    if (n != NULL) {
        n->Engine->flags |= 0x200000;
        bx = FindExtension(ext->shadow, 0x10000000);
        if (bx != NULL) {
            bx->unkC = 3;
            bx->unk18 = 0x100;
            bx->flags = 4;
            ext->shadow->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
            ext->shadow->InitDelete(ext->shadow);
            ext->beam = 0;
        }
    }
    e = AddExtEngine(m, 2);
    if (e != NULL) {
        ext->it_eng = e;
        e->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
        e->sub->entity = 0x2003;
        e->sub->xform.order = 0;
        s2 = e->sub;
        s2->xform.rot.vz = 0;
        s2->xform.rot.vy = 0;
        s2->xform.rot.vx = 0;
        e->sub->xform.hasScale = 1;
        sub = e->sub;
        sub->xform.scale.vz = 0x200;
        sub->xform.scale.vy = 0x200;
        sub->xform.scale.vx = 0x200;
        e->sub->flags &= ~0x8000;
    }
}

void PlotUkaPolarMask(NAS *m) {
    EXTtransmuter *ext;
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
        RemoveCamera(ext->cam);
    }
    if (PolarClock != 0) {
        if (ext->beam != NULL) {
            KillNAS(ext->beam);
            ext->beam = NULL;
        }
    }
    if (PolarClock != 0) {
        if (m->Engine->sfx != NULL) {
            StopSfx(m->Engine->sfx);
        }
        if (ext->it_eng->sfx != NULL) {
            StopSfx(ext->it_eng->sfx);
        }
        ext->shadow->Engine->flags &= ~0x8000;
    }
    seg = ((get_angle(m->Engine->XYZ.x, m->Engine->XYZ.z) + 1) & 0xFFF) >> 8;
    cs = rcossin_tbl[(ext->yang - 0x400) & 0xFFF].unk2;
    sn = rcossin_tbl[ext->yang & 0xFFF].unk2;
    ty = (cs * 3) >> 5;
    tx = tz = sn * 128 >> 12;
    if (ext->NasStatus != 2) {
        m->Engine->XYZ.y = ext->aimpoint.y + (cs * 64 >> 12);
    }
    m->Engine->xform.dirty = 1;
    if (ext->angular_max < 0) {
        ext->yang += 0x40;
    } else {
        ext->yang -= 0x40;
    }
    for (j = 0; j < 16; j++) {
        if (ext->sectordata[j].hitcount != 0 && ext->sectordata[j].secframe < 0x74) {
            ext->sectordata[j].secframe += 4;
            UkaMaskRisers[j]->y = ext->sectordata[j].baseframe + ext->sectordata[j].secframe;
        }
    }
    for (i = 0; i < 4; i++) {
        if (ext->hero_immune_timer[i] > 0) {
            ext->hero_immune_timer[i]--;
        }
    }
    if (ext->offtime < 0) {
        for (i = r; ; i++) {
            HeroInfo *h;
            s32 s;
            s32 kind;

            if (i >= r + 4) {
                break;
            }
            if (ext->NasStatus == 1 || PolarClock != 0 || ext->effect_player != -1) {
                continue;
            }
            s = i & 3;
            h = &Heroes[s];
            if (h->hero_status & 0x2000) {
                continue;
            }
            if (DistXZSq((SLONG_XYZ *)&m->Engine->XYZ, (SLONG_XYZ *)&h->hero_eng->XYZ) > 0x18FFF) {
                continue;
            }
            {
                EXTheromove *hm = h->hero_move;

                hm->x_vel = 0;
                hm->z_vel = 0;
                hm->x_max = 0;
                hm->z_max = 0;
            }
            ext->beamindex = Random(4);
            kind = UkaMaskFrames[ext->beamindex];
            ext->effect_player = s;
            ext->NasStatus = 3;
            ext->beamtype = kind;
            PlaySfx(0x44C, &m->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
            ext->beam = CreateNAS(m->Engine->XYZ.x, 0x5E, m->Engine->XYZ.z, 0x1605, 0x11);
            if (ext->beam != NULL) {
                bx = FindExtension(ext->beam, 0x10000000);
                if (bx != NULL) {
                    bx->unkC = 0;
                    bx->unk18 = 0x100;
                    bx->flags = 1;
                    bx->unk28 = 0;
                    ext->beam->InitDelete(ext->beam);
                }
            }
            switch (kind) {
            case 5:
                ext->offtime = 0x14A;
                ext->hero_action_timer[i & 3] = 0;
                ext->hero_action[i & 3] = kind;
                ext->hero_in_beam_timer = 0xF;
                ext->hero_last_action[i & 3] = kind;
                PlaySfx(0x451, NULL, NULL, 0x1000, 0x1E00, 0);
                SetHeroAction(i & 3, 0x4F);
                ext->timer = 0x3C;
                PlaySfx(0x452, &ext->it_eng->sfx, &ext->it_eng->XYZ, 0x1000, 0x1E00, 0);
                break;
            case 2:
                ext->offtime = Random(0x3C) + 0x5A;
                ext->hero_action_timer[i & 3] = 0;
                ext->hero_action[i & 3] = kind;
                ext->hero_last_action[i & 3] = kind;
                ext->hero_in_beam_timer = 0xF;
                PlaySfx(0x450, NULL, NULL, 0x1000, 0x1E00, 0);
                SetHeroAction(i & 3, 0x4F);
                ext->timer = 0x3C;
                break;
            case 0:
                ext->offtime = Random(0x3C) + 0x96;
                ext->hero_action_timer[i & 3] = 0;
                ext->hero_action[i & 3] = 0;
                ext->hero_last_action[i & 3] = 0;
                Heroes[i & 3].hero_move->mass = 0x50;
                PolarHeroMass[i & 3] = 0x40;
                ext->hero_in_beam_timer = 0xF;
                PlaySfx(0x44E, NULL, NULL, 0x1000, 0x1E00, 0);
                SetHeroAction(i & 3, 0x4F);
                ext->timer = 0x3C;
                break;
            case 8:
                ext->offtime = Random(0x3C) + 0x12C;
                ext->hero_action_timer[i & 3] = 0;
                ext->hero_action[i & 3] = kind;
                ext->hero_last_action[i & 3] = kind;
                Heroes[i & 3].hero_move->mass = 0x190;
                ext->hero_in_beam_timer = 0xF;
                PolarHeroMass[i & 3] = 0x10;
                PlaySfx(0x48A, NULL, NULL, 0x1000, 0x1E00, 0);
                SetHeroAction(i & 3, 0x4F);
                ext->timer = 0x3C;
                break;
            }
        }
    }
    switch (ext->NasStatus) {
    case 2:
        m->Engine->XYZ.y -= 0x10;
        ext->shadow->Engine->flags &= ~0x8000;
        break;
    case 4:
        if (ext->offtime-- < 0) {
            ext->NasStatus = 0;
        }
        break;
    case 0:
        if (PolarClock != 0) {
            ext->NasStatus = 2;
            if (m->Engine->sfx != NULL) {
                StopSfx(m->Engine->sfx);
            }
            return;
        }
        if (m->Engine->XYZ.x < ext->target_pos.x) {
            m->Engine->XYZ.x += 0x20;
        }
        if (ext->target_pos.x < m->Engine->XYZ.x) {
            m->Engine->XYZ.x -= 0x20;
        }
        if (m->Engine->XYZ.z < ext->target_pos.z) {
            m->Engine->XYZ.z += 0x20;
        }
        if (ext->target_pos.z < m->Engine->XYZ.z) {
            m->Engine->XYZ.z -= 0x20;
        }
        ext->cam->Engine->XYZ = m->Engine->XYZ;
        ext->cam->Engine->XYZ.y = 0x100;
        if (ext->angular_speed < ext->angular_max) {
            ext->angular_speed += ext->angular_acc;
            if (ext->angular_speed > ext->angular_max) {
                ext->angular_speed = ext->angular_max;
            }
        }
        if (ext->angular_speed > ext->angular_max) {
            ext->angular_speed -= ext->angular_acc;
            if (ext->angular_speed < ext->angular_max) {
                ext->angular_speed = ext->angular_max;
            }
        }
        m->UserInfo.user1 += ext->angular_speed;
        ga = get_angle(m->Engine->XYZ.x, m->Engine->XYZ.z) & 0xFFF;
        d = ((ga - ext->target_angle) << 20) >> 20;
        abs(d);
        if (ext->angular_speed == ext->angular_max) {
            s32 brake = abs(d) <= abs(ext->angular_speed) * 8;

            if (brake) {
                ext->angular_max = 0;
            }
        }
        {
            s32 dx = m->Engine->XYZ.x - ext->target_pos.x;
            s32 dz = m->Engine->XYZ.z - ext->target_pos.z;

            p = dx * dx + dz * dz;
        }
        if (p < 0x1000) {
            v = Random(0x1000);
            ext->NasStatus = 1;
            ext->timer = 0x3C;
            m->UserInfo.user1 -= ext->angular_speed;
            PlaySfx(0x489, NULL, NULL, 0x1000, 0x1E00, 0);
            PlaySfx(0x44C, &m->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
            UkaMaskSetSegments(seg, (UkaMaskSet *)ext);
            n = CreateNAS(m->Engine->XYZ.x, 0x5E, m->Engine->XYZ.z, 0x1605, 0x11);
            if (n != NULL) {
                bx = FindExtension(n, 0x10000000);
                if (bx != NULL) {
                    bx->unkC = 0;
                    bx->unk18 = 0x100;
                    bx->flags = 1;
                    bx->unk28 = 0;
                    n->InitDelete(n);
                    ext->beam = n;
                }
            }
            for (cnt = 0; cnt < 16; cnt++) {
                if (ext->sectordata[v++ & 0xF].status != 1) {
                    break;
                }
                if (cnt == 0xF) {
                    ext->NasStatus = 2;
                    return;
                }
            }
            v &= 0xF;
            ga = get_angle(m->Engine->XYZ.x, m->Engine->XYZ.z) & 0xFFF;
            ext->target_angle = ((v << 8) - 0x40) & 0xFFF;
            ext->target_pos.x = (rcossin_tbl[(ext->target_angle - 0x400) & 0xFFF].unk2 * 0x115) >> 9;
            ext->target_pos.z = (rcossin_tbl[ext->target_angle & 0xFFF].unk2 * 0x115) >> 9;
            UkaMaskTimer++;
            ext->angular_speed = 0;
            if (((ga - ext->target_angle) << 20) < 0) {
                ext->angular_max = 0xC;
            } else {
                ext->angular_max = -0xC;
            }
        }
        break;
    case 1:
        tx = 0;
        ty = 0;
        tz = 0;
        if (--ext->timer == -1) {
            ext->NasStatus = 0;
            ext->timer = 0xF0;
            if (m->Engine->sfx != NULL) {
                StopSfx(m->Engine->sfx);
            }
        }
        break;
    case 3:
        switch (ext->hero_action[ext->effect_player]) {
        case 5:
            if (ext->hero_in_beam_timer-- <= 0) {
                ext->itplayer = ext->effect_player;
                if (ext->it_eng != NULL) {
                    ext->it_eng->XYZ = Heroes[ext->itplayer].hero_eng->XYZ;
                    ext->it_eng->sub->flags |= 0x8000;
                }
                ext->ittimer = 0x12C;
                ext->hero_action[ext->effect_player] = -1;
                SetHeroAction(ext->effect_player, 0x49);
                if (m->Engine->sfx != NULL) {
                    StopSfx(m->Engine->sfx);
                }
            }
            break;
        case 2:
            if (ext->hero_in_beam_timer-- <= 0) {
                if (Heroes[ext->effect_player].hero_mode != 0x4C) {
                    SetHeroAction(ext->effect_player, 0x4C);
                    mv = Heroes[ext->effect_player].hero_move;
                    if (mv != NULL) {
                        mv->x_max = 0;
                        mv->z_max = 0;
                        mv->x_vel = 0;
                        mv->z_vel = 0;
                    }
                    ext->hero_action[ext->effect_player] = -1;
                    if (m->Engine->sfx != NULL) {
                        StopSfx(m->Engine->sfx);
                    }
                }
            }
            break;
        case 0: {
            EXTengine *e = Heroes[ext->effect_player].hero_eng;
            EXTengine *sh = Heroes[ext->effect_player].hero_shadow;

            if (ext->hero_in_beam_timer-- <= 0) {
                if (e->xform.scale.vx > 0xA00) {
                    e->xform.scale.vx -= 0x100;
                    e->xform.scale.vy -= 0x100;
                    e->xform.scale.vz -= 0x100;
                    sh->xform.scale.vx -= 0x100;
                    sh->xform.scale.vy -= 0x100;
                    sh->xform.scale.vz -= 0x100;
                } else {
                    ext->hero_action[ext->effect_player] = -1;
                    SetHeroAction(ext->effect_player, 0x49);
                }
                if (m->Engine->sfx != NULL) {
                    StopSfx(m->Engine->sfx);
                }
            }
            break;
        }
        case 8:
            if (ext->hero_in_beam_timer-- <= 0) {
                if (Heroes[ext->effect_player].hero_mode != 0x52) {
                    SetHeroAction(ext->effect_player, 0x52);
                    Heroes[ext->effect_player].hero_eng->XYZ.y -= 0x14C;
                    {
                        HeroInfo *h8 = &Heroes[ext->effect_player];
                        EXTheromove *m8 = h8->hero_move;

                        if (m8 != NULL) {
                            ang = get_angle(h8->hero_eng->XYZ.x, h8->hero_eng->XYZ.z) & 0xFFF;
                            Heroes[ext->effect_player].hero_eng->xform.rot.vy = ang;
                            Heroes[ext->effect_player].hero_move->forward_angle = ang;
                            m8->x_max = -(rcossin_tbl[(ang - 0x400) & 0xFFF].unk2 * 64 >> 12);
                            m8->z_max = -(rcossin_tbl[ang].unk2 * 64 >> 12);
                            m8->x_vel = m8->x_max;
                            m8->z_vel = m8->z_max;
                        }
                    }
                    ext->hero_action[ext->effect_player] = -1;
                    if (m->Engine->sfx != NULL) {
                        StopSfx(m->Engine->sfx);
                    }
                }
            }
            break;
        }
        if (ext->hero_action[ext->effect_player] != -1) {
            break;
        }
        switch (ext->hero_last_action[ext->effect_player]) {
        case 0: {
            EXTengine *e = Heroes[ext->effect_player].hero_eng;
            EXTengine *sh = Heroes[ext->effect_player].hero_shadow;

            if (ext->timer-- < 0) {
                if (m->Engine->XYZ.x < 0) {
                    m->Engine->XYZ.x += 0x18;
                }
                if (m->Engine->XYZ.x > 0) {
                    m->Engine->XYZ.x -= 0x18;
                }
                if (m->Engine->XYZ.z < 0) {
                    m->Engine->XYZ.z += 0x18;
                }
                if (m->Engine->XYZ.z > 0) {
                    m->Engine->XYZ.z -= 0x18;
                }
            }
            if (ext->offtime-- < 0) {
                if (e->xform.scale.vx < 0x1000) {
                    e->xform.scale.vx += 0x100;
                    e->xform.scale.vy += 0x100;
                    e->xform.scale.vz += 0x100;
                    sh->xform.scale.vx += 0x100;
                    sh->xform.scale.vy += 0x100;
                    sh->xform.scale.vz += 0x100;
                } else {
                    ext->hero_last_action[ext->effect_player] = -1;
                    PolarHeroMass[ext->effect_player] = HeroHooks.RunSpeeds[Heroes[ext->effect_player].hero_name * 3];
                    ResetMass(ext->effect_player);
                    ext->effect_player = -1;
                    ext->NasStatus = 0;
                    e->xform.scale.vx = 0x1000;
                    e->xform.scale.vy = 0x1000;
                    e->xform.scale.vz = 0x1000;
                }
            }
            break;
        }
        case 2:
            if (ext->timer-- < 0) {
                if (m->Engine->XYZ.x < 0) {
                    m->Engine->XYZ.x += 0x18;
                }
                if (m->Engine->XYZ.x > 0) {
                    m->Engine->XYZ.x -= 0x18;
                }
                if (m->Engine->XYZ.z < 0) {
                    m->Engine->XYZ.z += 0x18;
                }
                if (m->Engine->XYZ.z > 0) {
                    m->Engine->XYZ.z -= 0x18;
                }
            }
            if (ext->offtime-- < 0) {
                ext->NasStatus = 0;
                ext->hero_last_action[ext->effect_player] = -1;
                if (Heroes[ext->effect_player].hero_mode != 0x49) {
                    SetHeroAction(ext->effect_player, 0x49);
                    ext->effect_player = -1;
                }
            }
            break;
        case 8:
            if (ext->timer-- < 0) {
                if (m->Engine->XYZ.x < 0) {
                    m->Engine->XYZ.x += 0x18;
                }
                if (m->Engine->XYZ.x > 0) {
                    m->Engine->XYZ.x -= 0x18;
                }
                if (m->Engine->XYZ.z < 0) {
                    m->Engine->XYZ.z += 0x18;
                }
                if (m->Engine->XYZ.z > 0) {
                    m->Engine->XYZ.z -= 0x18;
                }
            }
            if (ext->offtime-- < 0) {
                ResetMass(ext->effect_player);
                ext->NasStatus = 0;
                PolarHeroMass[ext->effect_player] = HeroHooks.RunSpeeds[Heroes[ext->effect_player].hero_name * 3];
                ext->hero_last_action[ext->effect_player] = -1;
                if (Heroes[ext->effect_player].hero_mode != 0x49) {
                    SetHeroAction(ext->effect_player, 0x49);
                    Heroes[ext->effect_player].hero_eng->XYZ.y += 0x14C;
                    ext->effect_player = -1;
                }
            }
            break;
        case 5:
            if (ext->timer-- < 0) {
                if (m->Engine->XYZ.x < 0) {
                    m->Engine->XYZ.x += 0x18;
                }
                if (m->Engine->XYZ.x > 0) {
                    m->Engine->XYZ.x -= 0x18;
                }
                if (m->Engine->XYZ.z < 0) {
                    m->Engine->XYZ.z += 0x18;
                }
                if (m->Engine->XYZ.z > 0) {
                    m->Engine->XYZ.z -= 0x18;
                }
            }
            ext->offtime--;
            if (ext->itplayer == 5) {
                break;
            }
            if (ext->beam != NULL) {
                KillNAS(ext->beam);
                ext->beam = NULL;
            }
            if (ext->it_eng->sfx != NULL) {
                sfx_set_pitch(ext->it_eng->sfx, (0x12C - ext->ittimer) * 0xF00 / 300 + 0x1E00);
            }
            if (ext->ittimer > 0) {
                ext->ittimer--;
                if (ext->it_eng != NULL) {
                    ext->it_eng->XYZ = Heroes[ext->itplayer].hero_eng->XYZ;
                    ext->it_eng->sub->flags |= 0x8000;
                    ext->it_eng->xform.rot.vy += 0x80;
                    ext->it_eng->xform.dirty = 1;
                    switch (Heroes[ext->itplayer].hero_name) {
                    case 1:
                        ext->it_eng->XYZ.y -= 0x258;
                        break;
                    case 7:
                        ext->it_eng->XYZ.y -= 0x364;
                        break;
                    case 8:
                        ext->it_eng->XYZ.y -= 0x364;
                        break;
                    case 2:
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    default:
                        ext->it_eng->XYZ.y -= 0x300;
                        break;
                    }
                }
            }
            if (ext->ittimer == 0) {
                if (Heroes[ext->itplayer].hero_mode != 0x4D) {
                    EXTengine *he;

                    Heroes[ext->itplayer].hero_status |= 0x2000;
                    he = Heroes[ext->itplayer].hero_eng;
                    if (he != NULL) {
                        NAS *sp = CreateNAS(he->XYZ.x, he->XYZ.y - 0x898, he->XYZ.z, 0x1602, 0x11);

                        if (sp != NULL) {
                            UkaBitExt *sx = FindExtension(sp, 0x10000000);

                            if (sx != NULL) {
                                sx->unkC = he->XYZ.y;
                                sx->unk1C = ext->itplayer;
                            }
                        }
                    }
                    if (ext->it_eng->sfx != NULL) {
                        StopSfx(ext->it_eng->sfx);
                    }
                    SetHeroAction(ext->itplayer, 0x4D);
                    ext->it_eng->sub->flags &= ~0x8000;
                    ext->offtime = 0xD2;
                    ext->hero_last_action[ext->effect_player] = -1;
                    ext->onoff_timer = 0;
                    ext->effect_player = -1;
                    ext->itplayer = 5;
                    ext->NasStatus = 0;
                    return;
                }
            }
            if (PolarClock != 0 || Heroes[ext->itplayer].hero_mode == 0x50
                || (Heroes[ext->itplayer].hero_status & 0x2000)) {
                ext->it_eng->sub->flags &= ~0x8000;
                ext->offtime = 0xD2;
                if (ext->it_eng->sfx != NULL) {
                    StopSfx(ext->it_eng->sfx);
                }
                if (ext->effect_player != -1) {
                    ext->hero_last_action[ext->effect_player] = -1;
                }
                ext->onoff_timer = 0;
                ext->effect_player = -1;
                ext->itplayer = 5;
                ext->NasStatus = 0;
            }
            break;
        }
        break;
    }
    m->Engine->xform.rot.vx += (tx - m->Engine->xform.rot.vx) >> 2;
    m->Engine->xform.rot.vy += (ty - m->Engine->xform.rot.vy) >> 2;
    m->Engine->xform.rot.vz += (tz - m->Engine->xform.rot.vz) >> 2;
    if (ext->shadow != NULL) {
        ext->shadow->Engine->XYZ = m->Engine->XYZ;
        ext->shadow->Engine->sub->otz = 0x100;
        ext->shadow->Engine->XYZ.y = 0x5E;
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
  v.vx = c->XYZ.x - ((unsigned long long) a->XYZ.x);
  v.vz = c->XYZ.z - a->XYZ.z;
  rot.vx = -a->xform.rot.vx;
  new_var4 = (rot.vy = -a->xform.rot.vy);
  rot.vz = -a->xform.rot.vz;
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
    rot.vx = a->xform.rot.vx;
    rot.vy = a->xform.rot.vy;
    rot.vz = a->xform.rot.vz;
    RotMatrix(&rot, &m);
    ApplyMatrixLV(&m, &dir, &dir);
    c->XYZ.x += (push * dir.vx) >> 12;
    c->XYZ.z += (push * dir.vz) >> 12;
    return 1;
  }
  return 0;
}

void InitPolarWalrus(NAS *m) {
    PolarWalrusExt *ext;
    EXTheromove *e2;

    PolarProximity = (PolarProx *)m;
    m->Engine->flags |= 0x200000;
    ext = AddExt(m, sizeof(PolarWalrusExt), 0x10000000);
    if (ext != NULL) {
        ext->cursor = ScriptCursorFirst(m, &WalrusScriptInit);
        m->FUX = (EXT *)ext;
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_ArenaPolarTiltSeal];
        m->Engine->sub->entity = 0x4000;
        m->UserInfo.status = 0;
        AddToGroup((NAS **)&PolarBears, (NAS *)m);
        m->Engine->XYZ.x = (rcossin_tbl[(WalrusState - 0x400) & 0xFFF].unk2 * 0x307) >> 10;
        m->Engine->XYZ.z = (rcossin_tbl[WalrusState & 0xFFF].unk2 * 0x307) >> 10;
        m->Engine->XYZ.y = 0x400;
        m->Engine->xform.rot.vy = get_angle(m->Engine->XYZ.x, m->Engine->XYZ.z) & 0xFFF;
        m->Engine->sub->flags &= ~0x8000;
        ext->state = 0;
        m->Engine->xform.hasScale = 1;
        m->Engine->xform.order = 0;
        m->Engine->xform.scale.vx = 0x1F40;
        m->Engine->xform.scale.vy = 0x1F40;
        m->Engine->xform.scale.vz = 0x1F40;
        m->Engine->xform.dirty = 1;
        m->Engine->sub->otz = 0x200;
        ext->arc[0] = 0x10;
        ext->unk58 = 0x28;
        ext->arc[1] = 0;
        ext->arc[6] = 0;
        ext->from = *(SLONG_XYZ *)&m->Engine->XYZ;
        ext->to.x = (rcossin_tbl[(WalrusState - 0x400) & 0xFFF].unk2 * 0x7D) >> 8;
        ext->to.y = 0;
        ext->to.z = (rcossin_tbl[WalrusState & 0xFFF].unk2 * 0x7D) >> 8;
        m->Engine->sub->flags |= 0x02000000;
        ext->timer = Random(8) * 0x1E + 0x10E;
        ext->angle = 0;
        m->UserInfo.user1 = 0;
    }
    e2 = AddExt(m, sizeof(EXTheromove), 0x20000000);
    if (e2 != NULL) {
        e2->mass = 0x400;
        e2->x_vel = 0;
        e2->z_vel = 0;
        e2->z_dir = 0;
    }
}

void PlotPolarWalrus(NAS *m) {
    PolarWalrusExt *ext;
    EXTheromove *e2;
    PolarGame *gx;
    s32 v;
    s32 r;
    s32 ang;
    SLONG_XYZ pos;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    m->Engine->xform.dirty = 1;
    if (PolarClock != 0) {
        RemoveCamera(m);
    }
    e2 = FindExtension(m, 0x20000000);
    if (e2 == NULL) {
        return;
    }
    e2->x_vel = ext->xvel;
    e2->z_vel = ext->zvel;
    switch (ext->state) {
    case 0:
        if (ext->timer-- < 0 && PolarBeamOwner != NULL) {
            gx = FindExtension((NAS *)PolarBeamOwner, 0x40000000);
            if (gx != NULL && PolarClock == 0) {
                v = gx->unk14;
                if (v == 1 && gx->unk10 == -1) {
                    ext->nas = CreateNAS(0, 0, 0, 0x1610, 0x11);
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
            m->Engine->sub->otz = 0x200;
        } else {
            m->Engine->sub->otz = 0;
        }
        RunNasAnim(m);
        ArcUpdate(&ext->from.x, &ext->to.x, ext->arc, &m->Engine->XYZ.x);
        e2->x_vel = ext->xvel;
        e2->z_vel = ext->zvel;
        if (ext->cursor->frame == 0x18) {
            ext->state = 4;
            ext->cursor->frame = 0;
            ext->cursor->unk1C = 0;
            ext->cursor->cur = WalrusSteps0;
        }
        break;
    case 4:
        m->Engine->sub->otz = 0;
        e2->x_vel = ext->xvel;
        e2->z_vel = ext->zvel;
        RunNasAnim(m);
        if (ArcUpdate(&ext->from.x, &ext->to.x, ext->arc, &m->Engine->XYZ.x) != 0) {
            r = Random(4);
            ext->state = 1;
            ext->timer = 0x96;
            ext->cursor->frame = 0;
            ext->cursor->unk1C = 0;
            ext->cursor->cur = WalrusSteps1;
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
            ext->cursor->frame = 0;
            ext->cursor->unk1C = 0;
            ext->cursor->cur = WalrusSteps2;
            ext->timer = 0;
            ext->maxSpeed = 0x10;
            ext->speed = 0;
            ext->angle = m->Engine->xform.rot.vy + 0x800;
            if (ext->nas != NULL) {
                KillNAS(ext->nas);
                ext->nas = NULL;
            }
        }
        break;
    case 5:
        RunNasAnim(m);
        if (ext->cursor->frame == 7) {
            PlaySfx(0x476, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (ext->cursor->frame == 0xD) {
            PlaySfx(0x477, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        m->Engine->xform.rot.vy -= (s32)((m->Engine->xform.rot.vy - ext->angle) << 20) >> 24;
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
        m->Engine->XYZ.x -= (ext->speed * rcossin_tbl[(m->Engine->xform.rot.vy - 0x400) & 0xFFF].unk2) >> 12;
        m->Engine->XYZ.z -= (ext->speed * rcossin_tbl[m->Engine->xform.rot.vy & 0xFFF].unk2) >> 12;
        pos.x = m->Engine->XYZ.x + ((rcossin_tbl[(m->Engine->xform.rot.vy - 0x400) & 0xFFF].unk2 * 0x80) >> 12);
        pos.z = m->Engine->XYZ.z + ((rcossin_tbl[m->Engine->xform.rot.vy & 0xFFF].unk2 * 0x80) >> 12);
        e2->x_vel = 0;
        e2->z_vel = 0;
        if ((CheckMelted(&pos, 0) ^ 1) != 0) {
            ext->state = 7;
            ext->cursor->frame = 0;
            ext->cursor->unk1C = 0;
            ext->cursor->cur = WalrusSteps3;
            m->Engine->sub->otz = 0x19C;
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
        m->Engine->sub->otz = 0x200;
        ang = WalrusState;
        m->Engine->XYZ.x = (rcossin_tbl[(ang + 0x400) & 0xFFF].unk2 * 0x307) >> 10;
        m->Engine->XYZ.z = (rcossin_tbl[(ang + 0x800) & 0xFFF].unk2 * 0x307) >> 10;
        m->Engine->XYZ.y = 0x400;
        WalrusState += 0x800;
        ext->from = *(SLONG_XYZ *)&m->Engine->XYZ;
        ext->to.x = (rcossin_tbl[(WalrusState - 0x400) & 0xFFF].unk2 * 0x7D) >> 8;
        ext->to.y = 0;
        ext->to.z = (rcossin_tbl[WalrusState & 0xFFF].unk2 * 0x7D) >> 8;
        m->Engine->xform.rot.vy = get_angle(m->Engine->XYZ.x, m->Engine->XYZ.z) & 0xFFF;
        ext->cursor->frame = 0;
        ext->cursor->unk1C = 0;
        ext->cursor->cur = WalrusFrames;
        ext->arc[0] = 0x10;
        ext->unk58 = 0x28;
        ext->arc[1] = 0;
        RemoveCamera(m);
        break;
    }
}

void InitWalrusFish(NAS *m) {
    WalrusFishExt *ext;

    ext = AddExt(m, sizeof(WalrusFishExt), 0x10000000);
    if (ext != NULL) {
        ext->cursor = ScriptCursorFirst(m, &WalrusFishScriptInit);
        m->Engine->flags |= 0x200000;
        m->Engine->XYZ.x = (rcossin_tbl[(WalrusState - 0x400) & 0xFFF].unk2 * 0x307) >> 10;
        m->Engine->XYZ.z = (rcossin_tbl[WalrusState & 0xFFF].unk2 * 0x307) >> 10;
        m->Engine->XYZ.y = 0x400;
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_ArenaPolarTiltSeal];
        m->Engine->sub->entity = 0x2002;
        m->Engine->xform.hasScale = 1;
        m->Engine->xform.order = 0;
        m->Engine->xform.scale.vx = 0x1F40;
        m->Engine->xform.scale.vy = 0x1F40;
        m->Engine->xform.scale.vz = 0x1F40;
        ext->state = 0;
        m->Engine->sub->otz = 0x200;
        ext->arc[0] = 0x10;
        ext->unk54 = 0x28;
        ext->arc[1] = 0;
        ext->arc[6] = 0;
        ext->from = *(SLONG_XYZ *)&m->Engine->XYZ;
        ext->to.x = (rcossin_tbl[(WalrusState - 0x400) & 0xFFF].unk2 * 0x7D) >> 8;
        ext->to.y = 0;
        ext->to.z = (rcossin_tbl[WalrusState & 0xFFF].unk2 * 0x7D) >> 8;
        m->Engine->sub->flags |= 0x02000000;
        ext->spin = 0x80;
        ext->timer = 0;
        PlaySfx(0x478, NULL, NULL, 0x1000, 0x1E00, 0);
        m->Engine->xform.dirty = 1;
    }
}

void PlotWalrusFish(NAS *m) {
    WalrusFishExt *ext;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    m->Engine->xform.rot.vy += ext->spin;
    m->Engine->xform.rot.vx += 0x100;
    m->Engine->xform.dirty = 1;
    switch (ext->state) {
    case 0:
        if (++ext->timer == 0xF) {
            m->Engine->sub->otz = 0;
        }
        if (ArcUpdate(&ext->from.x, &ext->to.x, ext->arc, &m->Engine->XYZ.x) != 0) {
            ext->state = 2;
            PlaySfx(0x479, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        break;
    case 2:
        RunNasAnim(m);
        m->Engine->XYZ.x += ext->vx;
        m->Engine->XYZ.z += ext->vz;
        ext->spin >>= 1;
        m->Engine->xform.rot.vx >>= 1;
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
    EXTsmoketrail *ext;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    if (ext->flag & 1) {
        m->Engine->XYZ.y -= ext->yvel;
        m->Engine->XYZ.x = ext->startx + ((ext->amp * rcossin_tbl[(ext->angle - 0x400) & 0xFFF].unk2) >> 12);
        ext->amp -= ext->ampdec;
        if (ext->amp < 0) {
            ext->amp = 0;
        }
        m->Engine->sub->entity = 0x202E;
        ext->angle += 0x80;
    }
    switch (m->UserInfo.status) {
    case 0:
        m->Engine->sub->scale -= ext->fadeuprate;
        if ((s16)m->Engine->sub->scale < 0) {
            m->Engine->sub->scale = 0;
            m->UserInfo.status = 1;
        }
        break;
    case 1:
        m->Engine->sub->scale += ext->fadedownrate;
        m->Engine->xform.scale.vx -= ext->scalerate;
        m->Engine->xform.scale.vy -= ext->scalerate;
        m->Engine->xform.scale.vz -= ext->scalerate;
        if ((s16)m->Engine->sub->scale >= 0xDAD) {
            KillNAS(m);
            return;
        }
        break;
    }
    m->Engine->xform.dirty = 1;
}

void InitSimpleExplosion(NAS *m) {
    ExtSimpleExplosion *ext;
    subengine *sub;
    s32 i;

    ext = AddExt(m, sizeof(ExtSimpleExplosion), 0x10000000);
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
                sub->xform.hasScale = 1;
                sub->xform.order = 0;
                sub->xform.scale.vz = 0x1000;
                sub->xform.scale.vy = 0x1000;
                sub->xform.scale.vx = 0x1000;
                ext->count = 0;
                sub->scale = 0;
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
        owner = (BrioOwner *)m->UserInfo.user1;
        if (owner->type != 0x51) {
            KillNAS(m);
            return;
        }
        m->Engine->xform.rot.vx += 0x200;
        m->Engine->xform.dirty = 1;
        m->Engine->XYZ = owner->engine->XYZ;
        m->Engine->xform.rot.vy = owner->engine->xform.rot.vy - 0x400;
        m->Engine->xform.scale.vx = owner->engine->xform.scale.vx;
        m->Engine->xform.scale.vy = owner->engine->xform.scale.vy;
        m->Engine->xform.scale.vz = owner->engine->xform.scale.vz;
        if (owner->subtype == 7) {
            m->Engine->XYZ.y = owner->engine->XYZ.y - ((owner->engine->xform.scale.vx * 0x208) >> 12);
        } else {
            m->Engine->XYZ.y = owner->engine->XYZ.y - ((owner->engine->xform.scale.vx * 0x1A4) >> 12);
        }
        if (owner->anim->frame >= 0xD) {
            m->Engine->sub->scale += 0x400;
        }
    }
}

void PlotBrioPack(NAS *m) {
    BrioOwner *owner;
    NAS *nas;
    s32 user;

    user = m->UserInfo.user1;
    if (user != 0) {
        owner = (BrioOwner *)nas->UserInfo.user1;
        if (owner->type != 0x51) {
            KillNAS(m);
            return;
        }
        if (owner->anim->frame == 0xC) {
            nas = CreateNAS(owner->engine->XYZ.x,
                               owner->engine->XYZ.y - ((owner->engine->xform.scale.vx * 0x1A4) >> 12),
                               owner->engine->XYZ.z + 0x40, 0x1613, 0x11);
            if (nas != NULL) {
                nas->UserInfo.user1 = (iptr)owner;
            }
        }
        if (owner->anim->frame >= 0xA) {
            m->Engine->XYZ = owner->engine->XYZ;
            m->Engine->xform.rot.vy = owner->engine->xform.rot.vy;
            m->Engine->xform.scale.vx = owner->engine->xform.scale.vx;
            m->Engine->xform.scale.vy = owner->engine->xform.scale.vy;
            m->Engine->xform.scale.vz = owner->engine->xform.scale.vz;
            m->Engine->XYZ.y = owner->engine->XYZ.y - ((owner->engine->xform.scale.vx * 0x1A4) >> 12);
        }
        if (owner->anim->frame >= 0x18) {
            m->Engine->sub->scale += 0x400;
        }
    }
    m->Engine->xform.dirty = 1;
}

void InitPackEffect(NAS *m) {
    ExtSimpleExplosion *ext;
    subengine *sub;
    s32 i;

    ext = AddExt(m, sizeof(ExtSimpleExplosion), 0x10000000);
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
                sub->xform.hasScale = 1;
                sub->xform.order = 0;
                sub->xform.scale.vz = 0x1000;
                sub->xform.scale.vy = 0x1000;
                sub->xform.scale.vx = 0x1000;
                ext->count = 0;
                sub->scale = 0;
                sub = sub->next;
                i++;
            } while (sub != NULL);
        }
    }
}

void InitPolarBoss(NAS *nas) {
    PolarBossExt *ext = AddExt(nas, sizeof(PolarBossExt), 0x10000000);
    NAS *s;
    Ext20 *e;

    if (ext != NULL) {
        AddCamera(nas);
        ResetHeroGlobals();
        BearminatorNas = nas;
        ext->cursor = ScriptCursorFirst(nas, &PolarBossScriptInit);
        GotoLinkAnim((EXTanimator *)ext->cursor, 0);
        RunNasAnim(nas);
        ext->animNas = CreateNAS(nas->Engine->XYZ.x, nas->Engine->XYZ.y, nas->Engine->XYZ.z, 0x1622, 0x11);
        s = CreateNAS(nas->Engine->XYZ.x, 0, nas->Engine->XYZ.z, 0x1620, 0x11);
        if (s != NULL) {
            ext->nas = s;
            AddCamera(s);
        }
        ext->count = 0;
        nas->Engine->sub->u6C.dbase = MapScene->dbase;
        nas->Engine->sub->otz = 0x300;
        nas->Engine->sub->flags |= 0x02000000;
        nas->Engine->xform.order = 0;
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
        ext->timer = 0;
        ext->ang = 0;
        ext->flag = 1;
        ext->unk74 = 0;
        ext->phase = 0;
        ext->flag2 = 0;
        ext->spin = 0xC;
        PlaySfx(0x226, &nas->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
        nas->Engine->OldXYZ.x = (rcossin_tbl[(((ext->ang + 0x20) & 0xFFF) - 0x400) & 0xFFF].unk2 * 3) >> 2;
        nas->Engine->OldXYZ.z = ((rcossin_tbl[(ext->ang + 0x20) & 0xFFF].unk2 * 0x400) >> 12) + 0x2000;
        get_angle(nas->Engine->OldXYZ.x - nas->Engine->XYZ.x, nas->Engine->OldXYZ.z - nas->Engine->XYZ.z);
        nas->Engine->xform.rot.vy = 0x800;
        nas->Engine->xform.dirty = 1;
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
    if (((GameState *)GameInfo)->teams != 0) {
        h = Heroes;
        if ((h[((GameState *)GameInfo)->teamLeader[0]].hero_status & 0x2000)
            && (h[h[((GameState *)GameInfo)->teamLeader[0]].hero_teammate].hero_status & 0x2000)) {
            CameraMode = 0;
        }
    } else if (Heroes[0].hero_status & 0x2000) {
        CameraMode = 0;
    }
    polarbossnas = m;
    m->UserInfo.user1 = RunNasAnim(m);
    if (!(Heroes[0].hero_mode & 0x2000)) {
        ang = get_angle(m->Engine->XYZ.x - Heroes[0].hero_eng->XYZ.x,
                        m->Engine->XYZ.z - Heroes[0].hero_eng->XYZ.z) & 0xFFF;
    } else {
        ang = get_angle(m->Engine->XYZ.x - Heroes[1].hero_eng->XYZ.x,
                        m->Engine->XYZ.z - Heroes[1].hero_eng->XYZ.z) & 0xFFF;
    }
    if (ext->state != 8) {
        ext->animNas->Engine->xform.rot.vy = ang;
    }
    mate = FindExtension(ext->animNas, 0x10000000);
    if (mate != NULL) {
        if (((GameState *)GameInfo)->teams != 0) {
            h = Heroes;
            if ((h[((GameState *)GameInfo)->teamLeader[0]].hero_status & 0x2000)
                && (h[h[((GameState *)GameInfo)->teamLeader[0]].hero_teammate].hero_status & 0x2000)
                && ext->state != 9 && ext->count == 0) {
                ext->state = 9;
                GotoLinkAnim(mate->anim, 7);
                return;
            }
        } else if ((Heroes[0].hero_status & 0x2000) && ext->count == 0 && ext->state != 9) {
            PlaySfx(0x23B, NULL, NULL, 0x1000, 0x1E00, 0);
            ext->state = 9;
            GotoLinkAnim(mate->anim, 7);
            return;
        }
    }
    if (ext->timer-- < 0) {
        for (k = 0;; k++) {
            s32 d;

            if (k >= 4) {
                break;
            }
            d = 0x700;
            pos.x = m->Engine->XYZ.x + PolarSinMul((u16)m->Engine->xform.rot.vy, d);
            pos.z = m->Engine->XYZ.z + PolarSinMul(m->Engine->xform.rot.vy + 0x400, d);
            pos.x += PolarRandSpread(0x400);
            pos.z += PolarRandSpread(0x400);
            d = 0x500;
            pos.x = m->Engine->XYZ.x - PolarSinMul((u16)m->Engine->xform.rot.vy, d);
            pos.z = m->Engine->XYZ.z - PolarSinMul(m->Engine->xform.rot.vy + 0x400, d);
            pos.x += PolarRandSpread(0x5F4);
            pos.z += PolarRandSpread(0x5F4);
        }
        ext->timer = Random(4) + 5;
    }
    switch (ext->state) {
    case 5:
        if (ext->animNas->Engine->sfx != NULL) {
            StopSfx(ext->animNas->Engine->sfx);
        }
        m->Engine->OldXYZ.x = (rcossin_tbl[(((ext->ang + 0x20) & 0xFFF) - 0x400) & 0xFFF].unk2 * 3) >> 2;
        m->Engine->OldXYZ.z = ((rcossin_tbl[(ext->ang + 0x20) & 0xFFF].unk2 * 0x400) >> 12) + 0x2000;
        ext->animNas->Engine->XYZ = m->Engine->XYZ;
        ext->nas->Engine->XYZ = m->Engine->XYZ;
        ext->nas->Engine->XYZ.y -= 0x400;
        rot = (get_angle(m->Engine->OldXYZ.x - m->Engine->XYZ.x,
                         m->Engine->OldXYZ.z - m->Engine->XYZ.z) & 0xFFF) + 0x400;
        m->Engine->xform.rot.vy -= ((m->Engine->xform.rot.vy - rot) << 20) >> 22;
        m->Engine->XYZ.x = (rcossin_tbl[(ext->ang - 0x400) & 0xFFF].unk2 * 3) >> 2;
        m->Engine->XYZ.z = ((rcossin_tbl[ext->ang & 0xFFF].unk2 * 0x400) >> 12) + 0x2000;
        ext->ang = (ext->ang + ext->spin) & 0xFFF;
        if (ext->flag != 0) {
            if (ext->phase == 0 || ext->phase == 2 || ext->phase == 4 || ext->phase == 6) {
                if ((u32)(ext->ang - 0x709) < 0xC7) {
                    ext->state = 6;
                    ext->flag = 0;
                    ext->spin = 4;
                }
            }
            if (ext->phase == 1 || ext->phase == 3 || ext->phase == 5 || ext->phase == 7) {
                if (ext->phase == 7) {
                    FindExtension(Heroes[0].hero, 0x20000000);
                    FindExtension(Heroes[1].hero, 0x20000000);
                }
                if ((u32)(ext->ang - 0x12C) >= 0xE11) {
                    if (ext->flag2 == 0 && ext->phase < 4) {
                        ext->phase--;
                        return;
                    }
                    ext->state = 6;
                    ext->flag = 0;
                    ext->flag2 = 0;
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
            pe = FindExtension(ext->animNas, 0x10000000);
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
                nas = CreateNAS(m->Engine->XYZ.x + dx, m->Engine->XYZ.y - 0x5DC,
                                   m->Engine->XYZ.z + dz, 0x161B, 0x11);
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
            PlaySfx(0x227, &ext->animNas->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
            return;
        }
        if (ext->phase == 1 || ext->phase == 3 || ext->phase == 5 || ext->phase == 7) {
            NAS *nas;

            pe = FindExtension(ext->animNas, 0x10000000);
            if (pe != NULL) {
                GotoLinkAnim(pe->anim, 1);
            }
            GotoLinkAnim((EXTanimator *)ext->cursor, 7);
            nas = CreateNAS(m->Engine->XYZ.x, m->Engine->XYZ.y, m->Engine->XYZ.z, 0x1621, 0x11);
            if (nas != NULL) {
                be = FindExtension(nas, 0x10000000);
                if (be != NULL) {
                    be->flag = 0;
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
            ext->flag = 1;
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
            if (++ext->count == 4) {
                GotoLinkAnim(mate->anim, 0);
                ext->state = 10;
                return;
            }
            PlaySfx(0x23B, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        break;
    }
    m->Engine->xform.dirty = 1;
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
    off = *(s32 *)&((map_map *)base)->contour_grids;
    p = SELF_REL(&((s32 *)(base + 0x14 + off))[arg1 + 1]);
    if (p != NULL) {
        i = 0;
        n = ((MapContour *)p)->nEdges;
        t = rcossin_tbl;
        a2 = p + 0x50;
        a3 = p;
        for (;;) {
            if (i >= n) {
                return 1;
            }
            ang = ((MapContour *)a3)->edges[0].angle;  /* a3 steps an edge at a time */
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
        if (BearminatorFindPoint((s32 *)&m->Engine->XYZ, 1)) {
            return 1;
        }
        if (BearminatorFindPoint((s32 *)&m->Engine->XYZ, 0)) {
            return 0;
        }
    }
    if (bossmapsub->entity == 0x2009) {
        if (BearminatorFindPoint((s32 *)&m->Engine->XYZ, 2)) {
            return 1;
        }
        if (BearminatorFindPoint((s32 *)&m->Engine->XYZ, 0)) {
            return 0;
        }
    }
    if (bossmapsub->entity == 0x200A) {
        if (BearminatorFindPoint((s32 *)&m->Engine->XYZ, 3)) {
            return 0;
        }
    }
    if (bossmapsub->entity == 0x5003) {
        if (BearminatorFindPoint((s32 *)&m->Engine->XYZ, 0)) {
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
    EXTheromove *e2;
    NAS *n;
    EXTengine *e;
    EXTengine *f;
    s32 r;
    s16 v;

    ext = AddExt(m, sizeof(BearminatorExt), 0x10000000);
    if (ext != NULL) {
        r = Random(2);
        if (r == 0) {
            PlaySfx(0x22B, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (r == 1) {
            PlaySfx(0x22C, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        PlaySfx(0x22E, NULL, NULL, 0x1000, 0x1E00, 0);
        e2 = AddExt(m, sizeof(EXTheromove), 0x20000000);
        if (e2 != NULL) {
            m->Engine->sub->u6C.dbase = &HeroModelBank[DB_RoboPolar];
            n = CreateNAS(m->Engine->XYZ.x, 0, m->Engine->XYZ.z, 0x1620, 0x11);
            if (n != NULL) {
                ext->follower = n;
                AddCamera(n);
            }
            ext->easeCam = e2;
            ext->flag = 0;
            m->Engine->xform.hasScale = 1;
            m->Engine->xform.scale.vx = 0x1F40;
            m->Engine->xform.scale.vy = 0x1F40;
            m->Engine->xform.scale.vz = 0x1F40;
            m->Engine->xform.dirty = 1;
            m->Engine->flags |= 0x200000;
            AddToGroup((NAS **)&PolarBears, (NAS *)m);
            ext->cursor = ScriptCursorFirst(m, &BearminatorScriptInit);
            ext->state = 3;
            e2->x_vel = 0;
            e2->mass = v = ((u16)AiPlayerSkill[0].level * 8) - 0x30;
            if (v < 0x10) {
                e2->mass = 0x10;
            }
            e2->z_vel = 0;
            e2->x_acc = 4;
            e2->x_dec = 1;
            e2->z_acc = 4;
            e2->z_dec = 1;
            ext->target = 0;
            ext->timer = 0;
            ext->timer3 = 0;
            ext->targetAngle = m->Engine->xform.rot.vy;
            ext->arc = 0xB;
            ext->unk7C = 0x96;
            ext->unk3C = 0;
            ext->unk50 = Heroes[0].hero_eng->XYZ.y;
            ext->unk8C = 0;
            ext->timer2 = 0x5A;
            ext->count = 0;
            ext->timer22 = 0;
            m->FUX = NULL;
            e = AddExtEngine(m, 2);
            if (e != NULL) {
                e->sub->u6C.dbase = &HeroModelBank[DB_RoboPolar];
                e->sub->entity = 0x4000;
                e->sub->otz = 0x80;
                e->sub->flags |= 0x02000000;
                e->XYZ = m->Engine->XYZ;
                e->xform.order = 0;
                e->flags |= 0x200000;
                e->sub->flags = 0x40008021;
                e->sub->flags &= ~0x8000;
                e->xform.rot.vy = m->Engine->xform.rot.vy;
                e->xform.rot.vz = -0x800;
                m->FUX = (EXT *)e;
                e->xform.scale.vx = 0x1F40;
                e->xform.scale.vy = 0x1F40;
                e->xform.scale.vz = 0x1F40;
                e->sub->scale = 0xC00;
                e->xform.dirty = 1;
            }
            ext->fxEng = NULL;
            f = AddExtEngine(m, 3);
            if (f != NULL) {
                f->sub->u6C.dbase = &HeroModelBank[DB_RoboPolar];
                f->sub->entity = 0x2006;
                f->XYZ = m->Engine->XYZ;
                f->xform.order = 0;
                f->flags |= 0x200000;
                f->sub->flags &= ~0x8000;
                f->xform.rot.vy = m->Engine->xform.rot.vy;
                ext->fxEng = f;
                f->xform.dirty = 1;
            }
        }
    }
}

void PlotBearminator(NAS *m) {
    BearminatorExt *ext;
    PolarEaseCam *cam;
    EXTengine *fux;
    NAS *p;
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
                dx = n->XYZ.x - e->XYZ.x;
                dz = n->XYZ.z - e->XYZ.z;
                d = dx * dx + dz * dz;
                if (d <= 0x3D08F) {
                    s = 0x7D00 - csqrt(d);
                    a = get_angle(dx, dz) & 0xFFF;
                    m->Engine->XYZ.x -= ((s + 2) * rcossin_tbl[(a - 0x400) & 0xFFF].unk2) >> 18;
                    m->Engine->XYZ.z -= ((s + 2) * rcossin_tbl[a].unk2) >> 18;
                }
            }
        }
        p = p->Group.Child;
    }
    switch (ext->state) {
    case 8:
        CameraMode = 0;
        break;
    case 3:
        m->Engine->sub->flags &= ~0x8000;
        if (BearminatorNas->UserInfo.user1 != 0) {
            s32 r;

            r = Random(0x29C) + 0x400;
            ext->vel[0] = (r * rcossin_tbl[(ext->angle - 0x400) & 0xFFF].unk2) >> 12;
            ext->vel[1] = 0;
            ext->vel[2] = (r * rcossin_tbl[ext->angle & 0xFFF].unk2) >> 12;
            *(SLONG_XYZ *)ext->from = m->Engine->XYZ;
            ArcUpdate(ext->from, ext->vel, &ext->arc, &m->Engine->XYZ.x);
            m->Engine->sub->flags |= 0x8000;
            ext->state = 4;
        }
        break;
    case 9:
        if (--ext->timer22 == -1) {
            CreateNAS(m->Engine->XYZ.x, m->Engine->XYZ.y, m->Engine->XYZ.z, 0x1612, 0x11);
            ext->timer22 = 1;
        }
        m->Engine->sub->entity = 0x4100;
        m->Engine->xform.rot.vx += 0x200;
        if (ext->yvel < 0) {
            RemoveCamera(ext->follower);
        }
        if (ArcUpdate(ext->from, ext->vel, &ext->arc, &m->Engine->XYZ.x) == 0) {
            break;
        }
        goto kill;
    case 4:
        m->Engine->sub->entity = 0x4100;
        m->Engine->xform.rot.vx += 0x200;
        if (ArcUpdate(ext->from, ext->vel, &ext->arc, &m->Engine->XYZ.x) != 0) {
            s32 r;

            if (ext->flag != 0) {
                m->Engine->XYZ.y = 0;
                ext->state = 9;
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
                ext->yvel = -((ext->yvel * 0xAA) >> 8);
                return;
            }
            GotoLinkAnim((EXTanimator *)ext->cursor, 2);
            ext->state = 5;
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
                ext->target = r;
            } else {
                ext->target = 0;
            }
            m->Engine->xform.rot.vx = 0;
            fux->sub->flags |= 0x8000;
            m->Engine->XYZ.y = 0;
            return;
        }
        break;
    case 5: {
        HeroInfo *hi;
        EXTengine *h;
        NAS *n;

        ((PolarEaseCam *)ext->easeCam)->x.cur = 0;
        ((PolarEaseCam *)ext->easeCam)->z.cur = 0;
        ((PolarEaseCam *)ext->easeCam)->x.target = 0;
        ((PolarEaseCam *)ext->easeCam)->z.target = 0;
        if (ext->timer2 >= 7) {
            if (ext->timer3-- < 0) {
                PlaySfx(0x23C, NULL, NULL, 0x1000, 0x1E00, 0);
                ext->timer3 = ((0xF - (0x5A - ext->timer2) * 3) / 90 + 3) * 2;
            }
        }
        ext->count++;
        if (ext->timer2 == 6) {
            PlaySfx(0x23D, NULL, NULL, 0x1000, 0x1E00, 0);
        }
        if (((GameState *)GameInfo)->teams != 0) {
            HeroInfo *hb = Heroes;

            hi = &hb[((GameState *)GameInfo)->teamLeader[0]];
            if (hi->hero_status & 0x2000) {
                ext->target = hi->hero_teammate;
            }
            if (hb[hb[((GameState *)GameInfo)->teamLeader[0]].hero_teammate].hero_status & 0x2000) {
                ext->target = ((GameState *)GameInfo)->teamLeader[0];
            }
            hi = &hb[((GameState *)GameInfo)->teamLeader[0]];
            if ((hi->hero_status & 0x2000) && (hb[hi->hero_teammate].hero_status & 0x2000)) {
                ext->state = 8;
                GotoLinkAnim((EXTanimator *)ext->cursor, 3);
                return;
            }
        } else if (Heroes[0].hero_status & 0x2000) {
            ext->state = 8;
            GotoLinkAnim((EXTanimator *)ext->cursor, 3);
        }
        h = Heroes[ext->target].hero_eng;
        ext->targetAngle = (get_angle(h->XYZ.x - m->Engine->XYZ.x, h->XYZ.z - m->Engine->XYZ.z) + 0x800) & 0xFFF;
        ext->fxEng->sub->flags |= 0x8000;
        ext->fxEng->XYZ = m->Engine->XYZ;
        ext->fxEng->XYZ.y -= 0x19A;
        ext->fxEng->xform.dirty = 1;
        m->Engine->xform.rot.vy += ((ext->targetAngle - m->Engine->xform.rot.vy) << 20) >> 22;
        ext->fxEng->xform.rot.vy = m->Engine->xform.rot.vy;
        if (ext->timer2-- < 0) {
            ext->state = 2;
            n = CreateNAS(m->Engine->XYZ.x, m->Engine->XYZ.y - 0x19A, m->Engine->XYZ.z, 0x161C, 0x11);
            if (n != NULL) {
                n->Engine->xform.rot.vy = m->Engine->xform.rot.vy;
                ext->fxEng->sub->flags &= ~0x8000;
            }
            cam->x.target = 0;
            cam->z.target = 0;
            ext->timer2 = 0x5A;
            GotoLinkAnim((EXTanimator *)ext->cursor, 0);
        }
        break;
    }
    case 7: {
        EXTengine *e;

        e = m->Engine;
        if (e->XYZ.z > 0) {
            e->sub->otz = 0x400;
        } else {
            e->sub->otz = 0x200;
        }
        m->Engine->sub->flags |= 0x02000000;
        fux->sub->flags &= ~0x8000;
        if (done != 0) {
        kill:
            PlaySfx(0x45D, NULL, NULL, 0x1000, 0x1E00, 0);
            KillNAS(ext->follower);
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
        if (ext->timer-- < 0) {
            if (((GameState *)GameInfo)->teams != 0) {
                HeroInfo *hb = Heroes;

                hi = &hb[((GameState *)GameInfo)->teamLeader[0]];
                if (hi->hero_status & 0x2000) {
                    ext->target = hi->hero_teammate;
                }
                if (hb[hb[((GameState *)GameInfo)->teamLeader[0]].hero_teammate].hero_status & 0x2000) {
                    ext->target = ((GameState *)GameInfo)->teamLeader[0];
                }
                hi = &hb[((GameState *)GameInfo)->teamLeader[0]];
                if ((hi->hero_status & 0x2000) && (hb[hi->hero_teammate].hero_status & 0x2000)) {
                    ext->state = 8;
                    GotoLinkAnim((EXTanimator *)ext->cursor, 3);
                    return;
                }
            } else if (Heroes[0].hero_status & 0x2000) {
                GotoLinkAnim((EXTanimator *)ext->cursor, 3);
                ext->state = 8;
            }
            h = Heroes[ext->target].hero_eng;
            ext->targetAngle = (get_angle(h->XYZ.x - m->Engine->XYZ.x, h->XYZ.z - m->Engine->XYZ.z) + 0x800) & 0xFFF;
            ext->timer = 1;
        }
        m->Engine->xform.rot.vy += ((ext->targetAngle - m->Engine->xform.rot.vy) << 20) >> 21;
        {
            s32 k;

            k = AiPlayerSkill[0].level + 0x20;
            cam->x.target = (k * rcossin_tbl[(m->Engine->xform.rot.vy - 0x400) & 0xFFF].unk2) >> 13;
            cam->z.target = (k * rcossin_tbl[m->Engine->xform.rot.vy & 0xFFF].unk2) >> 13;
        }
        if (Heroes[ext->target].hero_mode == 0x4E) {
            ext->state = 6;
            return;
        }
        BearminatorEaseCam(cam, &m->Engine->XYZ.x);
        if (ext->timer2-- < 0) {
            hi = &Heroes[ext->target];
            if (hi->hero_mode != 0x4E) {
                s32 dx;
                s32 dz;

                dx = m->Engine->XYZ.x - hi->hero_eng->XYZ.x;
                dz = m->Engine->XYZ.z - hi->hero_eng->XYZ.z;
                near = dx * dx + dz * dz < 0x40000;
                if (near != 0)
                if (near == 1) {
                    ext->state = near;
                    cam->x.target = cam->x.cur = (rcossin_tbl[(m->Engine->xform.rot.vy - 0x400) & 0xFFF].unk2 * 0x4B) >> 12;
                    cam->z.target = cam->z.cur = (rcossin_tbl[m->Engine->xform.rot.vy & 0xFFF].unk2 * 0x4B) >> 12;
                    GotoLinkAnim((EXTanimator *)ext->cursor, 1);
                    ext->timer2 = 0x5A - (AiPlayerSkill[0].level - 0x10) * 4;
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
            ext->state = 7;
            PlaySfx(0x23E, NULL, NULL, 0x1000, 0x1E00, 0);
            GotoLinkAnim((EXTanimator *)ext->cursor, 4);
            return;
        }
        if (Heroes[0].hero_mode != 0x4E) {
            ext->state = 0;
            return;
        }
        if (ext->timer-- < 0) {
            e = m->Engine;
            x = e->XYZ.x;
            z = e->XYZ.z;
            dx = -x;
            dz = -z;
            if (dx * dx + dz * dz > 0x400000) {
                a = (get_angle(dx, dz) + 0x800) & 0xFFF;
                ext->targetAngle = a;
                m->Engine->xform.rot.vy += ((a - m->Engine->xform.rot.vy) << 20) >> 21;
                cam->x.target = (rcossin_tbl[(m->Engine->xform.rot.vy - 0x400) & 0xFFF].unk2 * 3) >> 9;
                cam->z.target = (rcossin_tbl[m->Engine->xform.rot.vy & 0xFFF].unk2 * 3) >> 9;
            } else {
                h = Heroes[ext->target].hero_eng;
                dx = h->XYZ.x;
                dz = h->XYZ.z;
                cam->x.target = 0;
                cam->z.target = 0;
                ext->targetAngle = (get_angle(dx - x, dz - z) + 0x800) & 0xFFF;
            }
            ext->timer = 1;
        }
        BearminatorEaseCam(cam, &m->Engine->XYZ.x);
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
            ext->state = 7;
            GotoLinkAnim((EXTanimator *)ext->cursor, 4);
            return;
        }
        if (ext->timer-- < 0) {
            if (((GameState *)GameInfo)->teams != 0) {
                HeroInfo *hb = Heroes;

                hi = &hb[((GameState *)GameInfo)->teamLeader[0]];
                if (hi->hero_status & 0x2000) {
                    ext->target = hi->hero_teammate;
                }
                if (hb[hb[((GameState *)GameInfo)->teamLeader[0]].hero_teammate].hero_status & 0x2000) {
                    ext->target = ((GameState *)GameInfo)->teamLeader[0];
                }
                hi = &hb[((GameState *)GameInfo)->teamLeader[0]];
                if ((hi->hero_status & 0x2000) && (hb[hi->hero_teammate].hero_status & 0x2000)) {
                    ext->state = 8;
                    GotoLinkAnim((EXTanimator *)ext->cursor, 3);
                    return;
                }
            } else if (Heroes[0].hero_status & 0x2000) {
                GotoLinkAnim((EXTanimator *)ext->cursor, 3);
                ext->state = 8;
            }
            h = Heroes[ext->target].hero_eng;
            ext->targetAngle = (get_angle(h->XYZ.x - m->Engine->XYZ.x, h->XYZ.z - m->Engine->XYZ.z) + 0x800) & 0xFFF;
            ext->timer = 1;
        }
        m->Engine->xform.rot.vy += ((ext->targetAngle - m->Engine->xform.rot.vy) << 20) >> 21;
        cam->x.target = (rcossin_tbl[(m->Engine->xform.rot.vy - 0x400) & 0xFFF].unk2 * 0x4B) >> 12;
        cam->z.target = (rcossin_tbl[m->Engine->xform.rot.vy & 0xFFF].unk2 * 0x4B) >> 12;
        BearminatorEaseCam(cam, &m->Engine->XYZ.x);
        if (done != 0) {
            ext->state = 0;
            GotoLinkAnim((EXTanimator *)ext->cursor, 0);
            return;
        }
        break;
    }
    case 2:
        ext->state = 0;
        break;
    }
    fux->sub->entity = m->Engine->sub->entity;
    fux->xform.rot.vy = m->Engine->xform.rot.vy;
    fux->XYZ = m->Engine->XYZ;
    if (ext->follower != NULL) {
        ext->follower->Engine->XYZ = m->Engine->XYZ;
        ext->follower->Engine->XYZ.y = 0;
    }
    fux->xform.dirty = 1;
    m->Engine->xform.dirty = 1;
}

void PlotPolarMissile(NAS *m) {
    PolarMissileExt *ext;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    switch (ext->state) {
    case 0:
        ext->state = 1;
        ext->pos = m->Engine->XYZ;
        break;
    case 1:
        if (--ext->timer == -1) {
            PolarMissileBlast(m, 0);
            ext->timer = 1;
        }
        break;
    }
    ext->pos.x -= (ext->speed * rcossin_tbl[(m->Engine->xform.rot.vy - 0x400) & 0xFFF].unk2) >> 12;
    ext->pos.z -= (ext->speed * rcossin_tbl[(u16)m->Engine->xform.rot.vy & 0xFFF].unk2) >> 12;
    if (PolarMissileHitHero(&ext->pos) != -1) {
    blast:
        PolarMissileBlast(m, 1);
    } else {
        m->Engine->xform.dirty = 1;
        m->Engine->XYZ = ext->pos;
        if (ext->bounces >= 3) {
            goto blast;
        }
        if (csqrt(m->Engine->XYZ.x * m->Engine->XYZ.x
                  + m->Engine->XYZ.z * m->Engine->XYZ.z) >> 6 <= 0xFA0) {
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
    if (ArcUpdate(&ext->from.x, &ext->to.x, ext->arc, &m->Engine->XYZ.x) == 0) {
        return;
    }
    m->Engine->XYZ.y = 0x20;
    pos = &m->Engine->XYZ;
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
        boss->flag = 1;
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

    ext = AddExt(m, sizeof(SniperExt), 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->entity = 0x2006;
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_RoboPolar];
        ext->gravity = 8;
        ext->yvel = 0;
        ext->bounces = 0;
        ext->state = 0;
        ext->timer = 0;
        ext->timer2 = 0;
        ext->aimX = 0;
        ext->aimY = 0;
        ext->unk54 = 0;
        ext->unk74 = 0;
        ext->xmove = 0;
        ext->ymove = 0;
        ext->unk58 = 0xC;
        ext->unk78 = 0xC;
        ext->unk5A = 1;
        ext->unk7A = 1;
        ext->fired = 0;
        ext->player = 0;
        m->Engine->xform.scale.vx = 0x1830;
        m->Engine->xform.scale.vy = 0x1830;
        m->Engine->xform.scale.vz = 0x1830;
        m->Engine->xform.dirty = 1;
        m->Engine->flags |= 0x200000;
        ext->aimEng = NULL;
        ext->aimEng2 = NULL;
        e = AddExtEngine(m, 2);
        if (e != NULL) {
            ext->aimEng = e;
            e->sub->u6C.dbase = MapScene->dbase;
            e->sub->entity = 0x2018;
            e->sub->flags = 0x22000000;
            e->sub->flags &= ~0x8000;
            e->XYZ.x = 0;
            e->XYZ.y = 0;
            e->XYZ.z = 0;
            e->XYZ.x = 0;
            e->XYZ.y = 0;
            e->XYZ.z = 0;
            e->sub->otz = -0x600;
        }
        e = AddExtEngine(m, 3);
        if (e != NULL) {
            ext->aimEng2 = e;
            e->sub->u6C.dbase = MapScene->dbase;
            e->sub->entity = 0x2019;
            e->sub->flags = 0x22000000;
            e->sub->flags &= ~0x8000;
            e->XYZ.x = 0;
            e->XYZ.y = 0;
            e->XYZ.z = 0;
            e->XYZ.x = 0;
            e->XYZ.y = 0;
            e->XYZ.z = 0;
            e->sub->otz = -0x3F4;
        }
        f = AddExtEngine(m, 4);
        if (f != NULL) {
            ext->shadow = f;
            f->sub->u6C.dbase = &HeroModelBank[DB_RoboPolar];
            f->sub->entity = 0x2006;
            f->sub->flags |= 0x02000000;
            f->XYZ = m->Engine->XYZ;
            f->xform.order = 0;
            f->flags |= 0x200000;
            f->sub->flags = 0x40008021;
            f->xform.rot.vy = m->Engine->xform.rot.vy;
            f->xform.rot.vz = -0x800;
            f->xform.scale.vx = 0x1830;
            f->xform.scale.vy = 0x1830;
            f->xform.scale.vz = 0x1830;
            f->sub->scale = 0xC00;
            f->xform.dirty = 1;
        }
    }
}

/* The sniper missile that's up, for a test run's player (bossbot.c). */
NAS *PolarSniperNas;
s32 PolarSniperSeen;

void PlotSniper(NAS *m) {
    PolarSniperNas = m;
    PolarSniperSeen = FrameCount;
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
        m->Engine->xform.dirty = 1;
        switch (ext->state) {
        case 0:
            if (ext->bounces < 3) {
                m->Engine->XYZ.y += ext->yvel;
                ext->yvel += ext->gravity;
                y = m->Engine->XYZ.y;
                if (y >= -0x1F) {
                    m->Engine->XYZ.y -= abs(y - 0x20);
                    ext->bounces++;
                    ext->yvel = -(ext->yvel >> 1);
                }
            }
            m->Engine->xform.rot.vy += 0x40;
            if (++ext->timer2 == 0x96) {
                boss->spin = 0xC;
                PolarMissileBlast(m, 1);
                PlaySfx(0x43D, NULL, NULL, 0x1000, 0x1E00, 0);
                KillNAS(m);
                return;
            }
            if (ext->bounces > 0) {
                for (i = 0;; i++) {
                    EXTengine *e;
                    EXTengine *he;

                    if (i >= 2) {
                        break;
                    }
                    e = m->Engine;
                    he = Heroes[i].hero_eng;
                    if ((he->XYZ.x - e->XYZ.x) * (he->XYZ.x - e->XYZ.x)
                        + (he->XYZ.z - e->XYZ.z) * (he->XYZ.z - e->XYZ.z) <= 0x23FFF) {
                        SNIPER_CAM->at.x = polarbossnas->Engine->XYZ.x;
                        SNIPER_CAM->at.y = polarbossnas->Engine->XYZ.y + 0x100;
                        SNIPER_CAM->at.z = polarbossnas->Engine->XYZ.z;
                        SNIPER_CAM->eye.x = 0;
                        SNIPER_CAM->eye.y = 0;
                        ext->savedEyeZ = SNIPER_CAM->eye.z;
                        SNIPER_CAM->eye.z = 0x1A80;
                        SNIPER_CAM->xform.angle.vx = 0xDC;
                        SNIPER_CAM->xform.angle.vy = 0;
                        SNIPER_CAM->xform.angle.vz = 0;
                        m->Engine->sub->flags &= ~0x8000;
                        ext->aimEng->sub->flags |= 0x8000;
                        ext->state = 1;
                        CameraMode = 0;
                        ext->aimEng->sub->XYZ.x = SNIPER_CAM->at.x;
                        ext->aimEng->sub->XYZ.y = SNIPER_CAM->at.y - 0x600;
                        ext->aimEng->sub->XYZ.z = SNIPER_CAM->at.z;
                        ext->aimEng2->sub->flags |= 0x8000;
                        ext->state = 1;
                        ext->aimEng2->sub->XYZ.x = SNIPER_CAM->at.x;
                        ext->aimEng2->sub->XYZ.y = SNIPER_CAM->at.y - 0x600;
                        ext->aimEng2->sub->XYZ.z = SNIPER_CAM->at.z;
                        ext->aimX = -0x5DC;
                        ext->aimY = 0x400;
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
                        ext->player = i;
                        return;
                    }
                }
            }
            break;
        case 1:
            ext->aimEng->sub->XYZ.x = SNIPER_CAM->at.x;
            ext->unk54 = 0;
            ext->unk74 = 0;
            ext->timer++;
            if (SNIPER_PAD[ext->player].dir & 0x80) {
                ext->unk54 = 0xC4;
            }
            if (SNIPER_PAD[ext->player].dir & 0x20) {
                ext->unk54 = -0xC4;
            }
            if (SNIPER_PAD[ext->player].dir & 0x10) {
                ext->unk74 = 0xC4;
            }
            if (SNIPER_PAD[ext->player].dir & 0x40) {
                ext->unk74 = -0xC4;
            }
            if (ext->fired == 0) {
                SniperAimUpdate((SniperAim *)&ext->pad2C[4], &ext->aimX);
                sub = ext->aimEng2->sub;
                dx = SNIPER_CAM->at.x - sub->XYZ.x;
                dy = SNIPER_CAM->at.y - sub->XYZ.y;
                if (dx < -0x700) {
                    SNIPER_CAM->at.x += 0x41;
                    i = abs(ext->xmove);
                    ext->xmove = 0;
                    ext->aimX -= i;
                }
                if (dx > 0x700) {
                    SNIPER_CAM->at.x -= 0x41;
                    i = abs(ext->xmove);
                    ext->xmove = 0;
                    ext->aimX += i;
                }
                if (dy > 0xAF0) {
                    i = abs(ext->ymove);
                    ext->ymove = 0;
                    ext->aimY += i;
                }
                if (dy < 0x190) {
                    i = abs(ext->ymove);
                    ext->ymove = 0;
                    ext->aimY -= i;
                }
                if (SNIPER_CAM->at.x < -0x1000) {
                    SNIPER_CAM->at.x = -0x1000;
                }
                if (SNIPER_CAM->at.x > 0x1000) {
                    SNIPER_CAM->at.x = 0x1000;
                }
                ext->aimEng2->sub->XYZ.x = SNIPER_CAM->at.x + ext->aimX;
                {
                    s32 ay = ext->aimY - 0x600;
                    ext->aimEng2->sub->XYZ.y = SNIPER_CAM->at.y + ay;
                }
                if (ext->fired == 0 && (SNIPER_PAD[ext->player].dirPressed & 0x8000)) {
                    ext->fired = 1;
                    ext->timer = 0xD2;
                    nas = CreateNAS(Heroes[ext->player].hero_eng->XYZ.x,
                                       Heroes[ext->player].hero_eng->XYZ.y,
                                       Heroes[ext->player].hero_eng->XYZ.z, 0x1624, 0x11);
                    if (nas != NULL) {
                        mext = FindExtension(nas, 0x10000000);
                        if (mext != NULL) {
                            mext->from = *(SLONG_XYZ *)&ext->aimEng2->sub->XYZ.x;
                            mext->to = Heroes[ext->player].hero_eng->XYZ;
                            mext->to.x = SNIPER_CAM->at.x;
                            mext->to.y = SNIPER_CAM->at.y - 0x700;
                            mext->to.z = SNIPER_CAM->at.z - SNIPER_CAM->eye.z + 0x300;
                            nas->Engine->XYZ = mext->to;
                        }
                    }
                }
            }
            if (ext->timer == 0x10E) {
                SNIPER_CAM->at.x = 0;
                SNIPER_CAM->at.y = 0;
                SNIPER_CAM->at.z = 0;
                SNIPER_CAM->eye.z = ext->savedEyeZ;
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
                boss->spin = 0xC;
                KillNAS(m);
                return;
            }
            break;
        }
    }
    if (ext->shadow != NULL) {
        ext->shadow->XYZ = m->Engine->XYZ;
        ext->shadow->XYZ.y = -m->Engine->XYZ.y + 0x40;
        ext->shadow->xform.rot.vy = m->Engine->xform.rot.vy;
        ext->shadow->xform.dirty = 1;
    }
}

void PlotSniperMissile(NAS *m) {
    SniperMissileExt *ext;
    PolarBossExt *boss;
    s32 *anim;
    PolarAi *gun;
    s32 dx, dz, dy, dist;

    ext = FindExtension(m, 0x10000000);
    if (ext == NULL) {
        return;
    }
    boss = FindExtension(polarbossnas, 0x10000000);
    if (boss == NULL) {
        return;
    }
    m->Engine->xform.dirty = 1;
    switch (ext->state) {
    case 0:
        dx = ext->to.x - ext->from.x;
        dz = ext->to.z - ext->from.z;
        dy = ext->to.y - ext->from.y;
        dist = csqrt(dx * dx + dz * dz) >> 6;
        ext->yaw = get_angle(dx, dz) & 0xFFF;
        ext->pitch = get_angle(dy, dist) & 0xFFF;
        ext->state = 1;
        m->Engine->xform.rot.vy = ext->yaw;
        m->Engine->xform.rot.vx = -ext->pitch;
        break;
    case 1:
        ext->count++;
        m->Engine->XYZ.x -= rcossin_tbl[(ext->yaw - 0x400) & 0xFFF].unk2 * 0x100 >> 12;
        m->Engine->XYZ.z -= rcossin_tbl[ext->yaw & 0xFFF].unk2 * 0x100 >> 12;
        m->Engine->XYZ.y -= rcossin_tbl[(ext->pitch - 0x400) & 0xFFF].unk2 * 0x100 >> 12;
        if (--ext->timer == -1) {
            PolarMissileBlast(m, 0);
            ext->timer = 1;
        }
        if (PolarXZBoxToSphere(polarbossnas->Engine, m->Engine, 0) != 0 || ext->count == 0x3C) {
            PolarMissileBlast(m, 1);
            PlaySfx(0x22F, NULL, NULL, 0x1000, 0x1E00, 0);
            boss->spin = 0xC;
            anim = FindExtension(boss->animNas, 0x10000000);
            if (anim != NULL) {
                GotoLinkAnim(EXT_P(anim, 3), 8);
                PlaySfx(0x23B, NULL, NULL, 0x1000, 0x1E00, 0);
            }
        } else {
            if (PolarXZBoxToSphere(boss->animNas->Engine, m->Engine, 1) == 0) {
                return;
            }
            PolarMissileBlast(m, 1);
            anim = FindExtension(boss->animNas, 0x10000000);
            if (anim != NULL) {
                GotoLinkAnim(EXT_P(anim, 3), 3);
            }
            boss->flag2 = 1;
            boss->spin = 0xC;
            GotoLinkAnim((EXTanimator *)boss->cursor, 9);
            PlaySfx(0x22F, NULL, NULL, 0x1000, 0x1E00, 0);
            PlaySfx(0x23A, NULL, NULL, 0x1000, 0x1E00, 0);
            BossState.health -= 7;
            gun = FindExtension(Heroes[3].hero, 0x20000000);
            if (gun != NULL) {
                gun->unk20 -= 0x55;
            }
            if (BossState.health <= 0) {
                BossState.health = 0;
                BossState.flags |= 0x8000;
                anim = FindExtension(boss->animNas, 0x10000000);
                if (anim != NULL) {
                    GotoLinkAnim(EXT_P(anim, 3), 5);
                    boss->animNas->Engine->sub->otz = 0x238;
                    boss->state = 8;
                }
            }
        }
        KillNAS(m);
        break;
    }
}

s32 PolarXZBoxToSphere(EXTengine *a, EXTengine *b, s32 c) {
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
        printf("No Coll box on subengine in PolarXZBoxToSphere\n");
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
    if (a->xform.hasScale == 1) {
        lo.v[0] = (lo.v[0] * a->xform.scale.vx) >> 12;
        lo.v[1] = (lo.v[1] * a->xform.scale.vy) >> 12;
        lo.v[2] = (lo.v[2] * a->xform.scale.vz) >> 12;
        hi.v[0] = (hi.v[0] * a->xform.scale.vx) >> 12;
        hi.v[1] = (hi.v[1] * a->xform.scale.vy) >> 12;
        hi.v[2] = (hi.v[2] * a->xform.scale.vz) >> 12;
    }
    if (a->sub->xform.hasScale == 1) {
        lo.v[0] = (lo.v[0] * a->sub->xform.scale.vx) >> 12;
        lo.v[1] = (lo.v[1] * a->sub->xform.scale.vy) >> 12;
        lo.v[2] = (lo.v[2] * a->sub->xform.scale.vz) >> 12;
        hi.v[0] = (hi.v[0] * a->sub->xform.scale.vx) >> 12;
        hi.v[1] = (hi.v[1] * a->sub->xform.scale.vy) >> 12;
        hi.v[2] = (hi.v[2] * a->sub->xform.scale.vz) >> 12;
    }
    d.vx = b->XYZ.x - a->XYZ.x;
    d.vy = b->XYZ.y - a->XYZ.y;
    d.vz = b->XYZ.z - a->XYZ.z;
    ang.vx = -a->xform.rot.vx;
    ang.vy = -a->xform.rot.vy;
    ang.vz = -a->xform.rot.vz;
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
        ang.vx = a->xform.rot.vx;
        ang.vy = a->xform.rot.vy;
        ang.vz = a->xform.rot.vz;
        RotMatrix(&ang, &m);
        ApplyMatrixLV(&m, &n, &n);
        if (c == 0) {
            b->XYZ.x += (push * n.vx) >> 12;
            b->XYZ.z += (push * n.vz) >> 12;
        }
        return 1;
    }
    return 0;
}

void InitSniperMissile(NAS *m) {
    SniperMissileExt *ext;

    ext = AddExt(m, sizeof(SniperMissileExt), 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->entity = 0x2006;
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_RoboPolar];
        ext->state = 0;
        m->Engine->xform.hasScale = 1;
        m->Engine->xform.scale.vx = 0x2AF8;
        m->Engine->xform.scale.vy = 0x2AF8;
        m->Engine->xform.scale.vz = 0x2AF8;
        m->Engine->xform.order = 2;
        ext->timer = 0;
        ext->count = 0;
        PlaySfx(0x522, &m->Engine->sfx, NULL, 0x1000, 0x1E00, 0);
    }
}

void InitBigMissile(NAS *m) {
    BigMissileExt *ext;

    ext = AddExt(m, sizeof(BigMissileExt), 0x10000000);
    if (ext != NULL) {
        m->Engine->xform.order = 2;
        m->Engine->xform.hasScale = 1;
        m->Engine->sub->entity = 0x201A;
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
        m->Engine->xform.scale.vx = 0x2000;
        m->Engine->xform.scale.vy = 0x2000;
        m->Engine->xform.scale.vz = 0x2000;
        m->Engine->xform.dirty = 1;
        ext->unkC = 0x10;
        ext->unk50 = 0x96;
        ext->unk10 = 0;
        ext->unk24 = Heroes[0].hero_eng->XYZ.y;
        ext->unk78 = 0;
        ext->pos = m->Engine->XYZ;
        PlaySfx(0x22D, NULL, NULL, 0x1000, 0x1E00, 0);
        PlaySfx(0x22B, NULL, NULL, 0x1000, 0x1E00, 0);
    }
}

void PlotBossTransmuter(NAS *m) {
}

void InitBossTransmuter(NAS *m) {
}

/* For a test run's player (bossbot.c): the hero's charge meter, full at 0x100. */
s32 PolarHeroMeter(s32 player) {
    Ext20 *ext = FindExtension(Heroes[player].hero, 0x20000000);

    return ext != NULL ? ext->val : 0;
}

NAS *PolarFirstBear(void) {
    return (NAS *)PolarBears;
}

s32 CountBears(void) {
    NAS *p;
    s32 n;

    n = 0;
    for (p = (NAS *)PolarBears; p != NULL; p = p->Group.Child) {
        n++;
    }
    return n;
}

void InitPolarMissile(NAS *m) {
    PolarMissileExt *ext;

    ext = AddExt(m, sizeof(PolarMissileExt), 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->entity = 0x2006;
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_RoboPolar];
        m->Engine->xform.order = 2;
        m->Engine->xform.hasScale = 1;
        m->Engine->sub->xform.order = 2;
        m->Engine->sub->flags |= 0x02000000;
        ext->speed = 0x8C;
        ext->speedMax = 0x3000;
        ext->unk38 = 0x700;
        ext->flag = 4;
        ext->state = 0;
        ext->unk18 = 0;
        ext->bounces = 0;
        ext->unk44 = 0;
        ext->unk48 = 0x100;
        ext->unk54 = 0;
        ext->timer = 0;
        ext->unk50 = ext->speedMax;
        PlaySfx(0x522, &m->Engine->sfx, &m->Engine->XYZ, 0x1000, 0x1E00, 0);
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
            DistXZSq(arg0, &hero->hero_eng->XYZ) < 0x7BC4) {
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
    NAS *p;
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
        e = p->Engine;
        if (e != NULL && e != m->Engine) {
            dx = e->XYZ.x - m->Engine->XYZ.x;
            dz = e->XYZ.z - m->Engine->XYZ.z;
            if (dx * dx + dz * dz < 0x3D090) {
                d = 0x7D00 - csqrt(dx * dx + dz * dz);
                ang = get_angle(dx, dz) & 0xFFF;
                m->Engine->XYZ.x -= ((d + 2) * t[(ang - 0x400) & 0xFFF].unk2) >> 18;
                m->Engine->XYZ.z -= ((d + 2) * t[ang].unk2) >> 18;
            }
        }
        p = p->Group.Child;
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
        dx = eng->XYZ.x - e->XYZ.x;
        dz = eng->XYZ.z - e->XYZ.z;
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
    GameState *g = GAME;
    s32 k;

    if (g->adventure.modeSkill[22] < 10) {
        return 0;
    }
    k = 0xB8;
    if (g->adventure.modeSkill[22] >= 30) {
        return 0xE66;
    }
    return (g->adventure.modeSkill[22] - 9) * k;
}

void PlotBearBoss(NAS *m) {
    if (FindExtension(m, 0x10000000) != NULL) {
        m->Engine->xform.dirty = 1;
        RunNasAnim(m);
    }
}

void InitBearBoss(NAS *m) {
    Ext10000000 *ext;

    ext = AddExt(m, sizeof(Ext10000000), 0x10000000);
    if (ext != NULL) {
        m->Engine->xform.order = 0;
        m->Engine->sub->otz = 0x300;
        m->Engine->sub->flags |= 0x02000000;
        m->Engine->sub->u6C.dbase = MapScene->dbase;
        ext->flag = ScriptCursorFirst(m, &BearBossScriptInit);
        GotoLinkAnim(ext->flag, 0);
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
        ext->count++;
        if (sub != NULL) {
            i = 0;
            do {
                sub->XYZ.x += ext->vel[i].x;
                sub->XYZ.y += ext->vel[i].z;
                sub->XYZ.z += ext->vel[i].y;
                sub->scale += 0x100;
                sub->xform.scale.vx -= 0x80;
                sub->xform.scale.vy -= 0x80;
                sub->xform.scale.vz -= 0x80;
                sub = sub->next;
                i++;
            } while (sub != NULL);
        }
        if (ext->count == 0x2D) {
            KillNAS(m);
        }
    }
}

void PlotCortexPack(NAS *m) {
    m->Engine->xform.dirty = 1;
}

void InitCortexPack(NAS *m) {
    m->Engine->sub->u6C.dbase = &HeroModelBank[DB_CharsPolar2Dingo];
    m->Engine->sub->entity = 0x2003;
    m->Engine->xform.order = 2;
    m->Engine->xform.hasScale = 1;
    m->Engine->sub->flags |= 0x40000021;
    m->Engine->sub->scale = 0;
}

void InitBrioPack(NAS *m) {
    m->Engine->flags |= 0x200000;
    m->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
    m->Engine->sub->entity = 0x2031;
    m->Engine->xform.order = 2;
    m->Engine->xform.hasScale = 1;
    m->Engine->sub->flags |= 0x40000021;
    m->Engine->sub->scale = 0;
    m->UserInfo.user2 = 0;
    m->UserInfo.user1 = 0;
}

void InitDingoTail(NAS *m) {
    m->Engine->flags |= 0x200000;
    m->Engine->sub->u6C.dbase = &HeroModelBank[DB_CharsPolar2Dingo];
    m->Engine->sub->entity = 0x2003;
    m->Engine->xform.order = 2;
    m->Engine->xform.hasScale = 1;
    m->Engine->sub->flags |= 0x40000021;
    m->Engine->sub->scale = 0;
}

void PlotSimpleExplosion(NAS *m) {
    ExtSimpleExplosion *ext;
    subengine *sub;
    s32 i;

    ext = FindExtension(m, 0x10000000);
    if (ext != NULL) {
        sub = m->Engine->sub;
        ext->count++;
        if (sub != NULL) {
            i = 0;
            do {
                sub->XYZ.x += ext->vel[i].x;
                sub->XYZ.y += ext->vel[i].z;
                sub->XYZ.z += ext->vel[i].y;
                sub->scale += 0x100;
                sub->xform.scale.vx -= 0x80;
                sub->xform.scale.vy -= 0x80;
                sub->xform.scale.vz -= 0x80;
                sub = sub->next;
                i++;
            } while (sub != NULL);
        }
        if (ext->count == 0x2D) {
            KillNAS(m);
        }
    }
}

void InitSmokeTrail(NAS *m) {
    EXTsmoketrail *ext;

    ext = AddExt(m, sizeof(EXTsmoketrail), 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
        m->Engine->sub->entity = 0x2020;
        m->Engine->sub->flags |= 0x40000041;
        m->Engine->sub->scale = 0x1000;
        m->UserInfo.status = 0;
        ext->fadeuprate = 0x400;
        ext->fadedownrate = 0xC8;
        ext->scalerate = 0x60;
        ext->flag = 0;
        ext->startx = m->Engine->XYZ.x;
        m->Engine->xform.hasScale = 1;
        m->Engine->xform.order = 0;
        ext->angle = Random(0x1000);
    }
}

void PlotBearDeath(NAS *m) {
    BearDeathExt *ext;

    ext = FindExtension(m, 0x10000000);
    if (ext != NULL) {
        RunNasAnim(m);
        m->Engine->xform.rot.vx += 0x100;
        m->Engine->xform.dirty = 1;
        if (ext->yvel > 0) {
            m->Engine->sub->otz = 0x200;
        }
        if (++ext->count == 0x1F) {
            RemoveCamera(m);
        }
        if (--ext->timer < 0) {
            CreateNAS(m->Engine->XYZ.x, m->Engine->XYZ.y, m->Engine->XYZ.z, 0x1612, 0x11);
            ext->timer = Random(7) + 1;
        }
        if (ArcUpdate(&ext->from.x, &ext->to.x, &ext->arc, &m->Engine->XYZ.x) != 0) {
            KillNAS(m);
        }
    }
}

void InitBearDeath(NAS *m) {
    BearDeathExt *ext;

    ext = AddExt(m, sizeof(BearDeathExt), 0x10000000);
    if (ext != NULL) {
        ScriptCursorFirst(m, &BearDeathScriptInit);
        m->Engine->sub->u6C.dbase = MapScene->dbase;
        m->Engine->xform.order = 2;
        m->Engine->sub->flags |= 0x02000000;
        ext->arc = 2;
        ext->unk70 = 0x32;
        ext->unk44 = 0x100;
        ext->unk30 = 0;
        ext->count = 0;
        ext->timer = 5;
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
    subengine *obj;
    subengine **out;

    obj = ((subengine *)MapScene->subs);
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
            if (obj->u6C.dbase->data != NULL) {
                u8 *p = entity_address(obj->entity, obj->u6C.dbase->data);
                if (p != NULL && ((MeshHeader *)p)->normals != 0) {
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
    EXTengine *temp_a1;
    EXTengine *temp_v1;

    temp_a1 = (arg0->Engine);
    (temp_a1->XYZ.x) = (s32) ((temp_a1->XYZ.x) + ((s32) (arg0->UserInfo.user2) >> 8));
    temp_v1 = (arg0->Engine);
    if ((temp_v1->XYZ.x) >= 0x1AF5) {
        (temp_v1->XYZ.x) = -0x1C84;
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
            sub->xform.order = 2;
            sub->xform.rot.vz = 0;
            sub->xform.rot.vy = 0;
            sub->xform.rot.vx = 0;
            sub->scale = 0x800;
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
    EXTmotion *target = obj->target;

    if (target != 0) {
        GravityMotionUpdate(obj, target);
    }
}

void InitGravityObject(NAS *m) {
    EXTmotion *g;
    s32 t;

    g = AddExt(m, sizeof(EXTmotion), 0x10000000);
    if (g != NULL) {
        m->Engine->sub->entity = 0x200C;
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
        m->Engine->sub->flags |= 0x02000000;
        g->Xvel = (rcossin_tbl[0xE00].unk2 * 27) >> 11;
        t = rcossin_tbl[0x200].unk2;
        g->Yvel = -0x54;
        g->Xacc = 0;
        g->Zacc = 0;
        g->Yacc = 2;
        g->flags = 0;
        g->Zvel = (t * 27) >> 11;
        g->floorheight = *(u16 *)&m->Engine->XYZ.y + 0x400;
        m->FUX = (EXT *)g;
    }
}

void InitWeightDeath(NAS *m) {
    EXTbeardeath *ext;

    ext = AddExt(m, sizeof(EXTbeardeath), 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
        m->Engine->sub->entity = 0x2003;
        ext->ydir = 0x40;
        ext->aimpoint.x = 8;
        ext->aimpoint.y = 0;
        ext->startpoint.x = 0;
        ext->startpoint.y = 0;
        PlaySfx(0x453, NULL, NULL, 0x1000, 0x1E00, 0);
    }
}

s32 PolarOfficeBox(s32 i) {
    NAS *p;
    EXTengine *eng;
    EXTengine *e;
    HeroInfo *base;

    p = PolarBears;
    base = Heroes;
    eng = base[i].hero_eng;
    for (; p != NULL; p = p->Group.Child) {
        e = p->Engine;
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
    subengine *var_v1;

    var_v1 = MapScene->subs;
    if (var_v1 != NULL) {
        do {
            (var_v1->flags) = (s32) ((var_v1->flags) | 0x01000000);
            var_v1 = (var_v1->next);
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
            m = p->flag;
            if (m != NULL) {
                a = p->eng;
                b = Heroes[player].hero_eng;
                dx = a->XYZ.x - b->XYZ.x;
                dz = a->XYZ.z - b->XYZ.z;
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
        l = PolarBeamOwner->link;
        if (l != NULL) {
            if (PolarAIPace[2] == 0 && l->player != -1) {
                PolarAIPace[2] = 1;
            }
            if (PolarAIPace[2] == -1 && l->player == PolarAIPace[2]) {
                PolarAIPace[2] = 0;
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
        f->timer2 = 0x5A;
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
            pos = h->hero_eng->XYZ;
            d = DistXZSq((SLONG_XYZ *)&pos, (SLONG_XYZ *)&me->hero_eng->XYZ);
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
        fux->player = 0;
        fux->aiDelay = 0;
        fux->flags2 |= 1;
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
    f->aiDelay = 1;
    f->timer2 = Random(0x3C) + 5;
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
        f->pos = h->hero_eng->XYZ;
    }
}

void PolarAI_Timer2(s32 player) {
    PolarWanderFux *fux;

    fux = (PolarWanderFux *)Heroes[player].hero->FUX;
    if (fux != NULL) {
        fux->timer2 = Random(0x168) + 0x5A;
    }
}

bool DoesPlayerHaveBomb(u32 arg0) {
    NAS *nas;
    EXTbombmaker *ext;
    s32 one;

    nas = PolarBomb;
    one = 1;
loop:
    if (nas != NULL) {
        ext = FindExtension(nas, 0x10000000);
        if (ext != NULL && ext->NasStatus == one && ext->owner_id == arg0) {
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
        if (ext != NULL && ext->flag == 0) {
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
        fux->timer2 = Random(0x168) + 0x5A;
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

s32 PolarCompareXZ(SLONG_XYZ *arg0, SLONG_XYZ *arg1) {
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a2;
    s32 temp_v0;
    s32 var_v1;

    temp_v0 = (arg0->x);
    temp_a0 = (arg1->x);
    var_v1 = (temp_a0 < temp_v0) << 7;
    if (temp_v0 < temp_a0) {
        var_v1 |= 0x20;
    }
    temp_a2 = (arg0->z);
    temp_a0_2 = (arg1->z);
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

    ext = AddExt(m, sizeof(BombGFXExt), 0x10000000);
    if (ext != NULL) {
        m->Engine->sub->entity = 0x2018;
        m->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
        ext->unkC = 0;
        m->Engine->xform.hasScale = 1;
        AddToGroup((NAS **)&PolarBomb, m);
        m->FUX = NULL;
        e = AddExtEngine(m, 2);
        if (e != NULL) {
            e->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
            e->sub->entity = 0x202F;
            e->xform.order = 0;
            e->XYZ = m->Engine->XYZ;
            e->flags |= 0x8041;
            e->sub->scale = 0x200;
            e->sub->flags |= 0x40000000;
            m->FUX = (EXT *)e;
        }
    }
}

void PlotBombMaker(NAS *m) {
    EXTbombmaker *ext;
    s32 x;
    s32 z;

    ext = FindExtension(m, 0x10000000);
    if (ext != NULL) {
        if (ext->count < 0) {
            ext->count = 0;
        }
        switch (ext->NasStatus) {
        case 0:
            if (ext->count <= 0) {
                if (ext->timer-- < 0 && PolarClock == 0) {
                    ext->NasStatus = 2;
                    ext->timer = 0x5A;
                    ext->count++;
                }
            }
            break;
        case 2:
            x = Random(0x800) - Random(0x800);
            z = Random(0x800);
            CreateNAS(x, 0, z - Random(0x800), 0x160F, 0x11);
            PlaySfx(0x3FA, NULL, NULL, 0x1000, 0x1E00, 0);
            ext->NasStatus = 0;
            break;
        }
    }
}

void InitBombMaker(NAS *m) {
    EXTbombmaker *ext;

    ext = AddExt(m, sizeof(EXTbombmaker), 0x10000000);
    if (ext != NULL) {
        ext->timer = 0xB4;
        ext->NasStatus = 0;
        ext->count = 0;
        BombMaster = m;
    }
}

u32 PolarAngleOf(SLONG_XYZ *arg0) {
    return (u32) ((get_angle((arg0->x), (arg0->z)) - 0x7D) & 0xFFF) >> 8;
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
        if (GAME->heroCpu[p1] != 0) {
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
        if (GAME->heroCpu[p2] != 0) {
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
        if (Heroes[p1].hero_mode == 0x51 && Heroes[p1].hero_eng->xform.scale.vx > 0xFA0) {
            m1 += 0x220;
        }
        if (Heroes[p2].hero_mode == 0x51 && Heroes[p2].hero_eng->xform.scale.vx > 0xFA0) {
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
        if (hero->hero_mode == 0x51 && hero->hero_eng->xform.scale.vx > 0xFA0) {
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
    GameState *g = GAME;
    HeroInfo *hero;
    EXTengine *shadow;
    EXTengine *eng;
    s32 *p;

    if (g->mode != 0x15 && g->heroes[player] != 9) {
        HeroInfo *base = Heroes;
        s32 *tbl;

        hero = &base[player];
        shadow = hero->hero_shadow;
        if (shadow != NULL) {
            eng = hero->hero_eng;
            shadow->flags = 0x8000;
            shadow->xform.hasScale = 1;
            if (eng != NULL) {
                shadow->sub->u6C.dbase = &HeroModelBank[DB_Polar];
                tbl = PolarShadowState;
                p = &tbl[player];
                *p = 0x4000;
                shadow->sub->entity = *p;
                shadow->sub->otz = 0x80;
                shadow->sub->flags |= 0x02000000;
                shadow->XYZ = eng->XYZ;
                shadow->xform.order = 0;
                shadow->sub->flags = 0x8000;
                shadow->xform.dirty = 1;
            }
        }
    }
}

void PolarUpdateShadow(s32 player) {
    GameState *g;
    HeroInfo *h;
    PolarAi *ext;
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
    g = GAME;
    if (g->mode == 0x15) {
        return;
    }
    shadow = h->hero_shadow;
    eng = h->hero_eng;
    if (shadow == NULL) {
        return;
    }
    shadow->flags |= 0x200000;
    shadow->xform.rot.vy = eng->xform.rot.vy;
    shadow->xform.dirty = 1;
    if (g->heroes[player] == 9 || h->hero_mode == 0x4E || h->hero_mode == 0x50) {
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
            shadow->xform.rot.vy = eng->xform.rot.vy;
            shadow->XYZ = eng->XYZ;
            shadow->xform.dirty = 1;
            return;
        }
    }
    fux = (PolarWanderFux *)Heroes[player].hero->FUX;
    if (fux != NULL && fux->flag != 0) {
        shadow->sub->entity = 0x4000;
        shadow->xform.rot.vy = eng->xform.rot.vy;
        shadow->XYZ = eng->XYZ;
        shadow->xform.dirty = 1;
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
            shadow->XYZ = eng->XYZ;
            if (h2->hero_mode == 0x52) {
                shadow->sub->u6C.dbase = MapScene->dbase;
                shadow->XYZ.y += 0x298;
                shadow->sub->entity = 0x2003;
                *p = 0x2003;
            }
            shadow->xform.rot.vy = eng->xform.rot.vy;
            shadow->xform.dirty = 1;
        }
        if (Heroes[player].hero_mode != 0x51) {
            goto mode58;
        }
    }
    {
        shadow->sub->u6C.dbase = &HeroModelBank[DB_Polar];
        shadow->XYZ = eng->XYZ;
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
        shadow->xform.rot.vy = eng->xform.rot.vy;
        shadow->xform.dirty = 1;
    }
mode58:
    base3 = Heroes;
    h3 = &base3[player];
    if (h3->hero_mode == 0x58) {
        shadow->sub->u6C.dbase = &HeroModelBank[DB_Polar];
        shadow->XYZ = eng->XYZ;
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
            if (hh->hero_anim->Atable == COC_seq_polarwin || ext->flag2 != 0) {
                i = hh->hero_eng->sub->entity - 0x4300;
                if (i < 0) {
                    i = 0;
                }
                ext->flag2 = 1;
                shadow->sub->entity = PolarShadowFrames7[i][0];
            } else {
                shadow->sub->entity = PolarShadowFrames7[0][0];
            }
            break;
        case 6:
            hb6 = Heroes;
            hh = &hb6[player];
            if (hh->hero_anim->Atable == KON_seq_polarwin || ext->flag2 != 0) {
                i = hh->hero_eng->sub->entity - 0x4280;
                if (i < 0) {
                    i = 0;
                }
                ext->flag2 = 1;
                shadow->sub->entity = PolarShadowFrames9[i][0];
            } else {
                shadow->sub->entity = PolarShadowFrames9[0][0];
            }
            break;
        case 5:
            hb5 = Heroes;
            hh = &hb5[player];
            if (hh->hero_anim->Atable == TIN_seq_polarwin || ext->flag2 != 0) {
                i = hh->hero_eng->sub->entity - 0x4300;
                if (i < 0) {
                    i = 0;
                }
                ext->flag2 = 1;
                shadow->sub->entity = PolarShadowFrames10[i][0];
            } else {
                shadow->sub->entity = PolarShadowFrames10[0][0];
            }
            break;
        case 4:
            hb4 = Heroes;
            hh = &hb4[player];
            if (hh->hero_anim->Atable == BRO_seq_polarwin || ext->flag2 != 0) {
                i = hh->hero_eng->sub->entity - 0x4300;
                if (i < 0) {
                    i = 0;
                }
                ext->flag2 = 1;
                shadow->sub->entity = PolarShadowFrames8[i][0];
            } else {
                shadow->sub->entity = PolarShadowFrames8[0][0];
            }
            break;
        }
        shadow->xform.rot.vy = eng->xform.rot.vy;
        shadow->xform.dirty = 1;
    }
}

void PolarPreMoveHero(s32 player) {
    HeroInfo *h;
    HeroInfo *base;
    EXTheromove *m;
    EXTheromove *m2;
    PadState *pad;
    PadState *p2;
    GameState *g;
    s32 *spd;

    base = Heroes;
    h = base + player;
    if (h->hero_frozen != 0) {
        ((PadState *)Pads)[player].dir = 0;
        ((PadState *)Pads)[player].dirPressed = 0;
    }
    m = h->hero_move;
    if (m != NULL) {
        if (h->hero_mode != 0x51) {
            m->x_max = 0;
            m->z_max = 0;
        }
        pad = (PadState *)Pads;
        pad += player;
        p2 = pad;
        if ((p2->flags & 1) || h->hero_ai->AImode != 0) {
            if (p2->dir & 0xF0) {
                m->forward_angle = (p2->angle.lo + 0x800) & 0xFFF;
                spd = &PolarHeroMass[player];
                m->x_max = (*spd * rcossin_tbl[(m->forward_angle + 0x400) & 0xFFF].unk2) >> 12;
                m->z_max = (*spd * rcossin_tbl[(m->forward_angle + 0x800) & 0xFFF].unk2) >> 12;
            }
        }
        g = (GameState *)GameInfo;
        if (g->mode != 0x16) {
            m->x_max += ((m->mass * 3) >> 3) * rcossin_tbl[(PolarTiltZ - 0x400) & 0xFFF].unk2 >> 12;
            m->z_max -= ((m->mass * 3) >> 3) * rcossin_tbl[(PolarTiltX - 0x400) & 0xFFF].unk2 >> 12;
            if (g->mode == 0x12) {
                if (g->levelFlags & 0x80) {
                    if (GAME->heroCpu[player] != 0) {
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
    off = *(s32 *)&((map_map *)base)->contour_grids;
    p = SELF_REL(&((s32 *)(base + 0x14 + off))[arg1 + 1]);
    if (p != NULL) {
        i = 0;
        n = ((MapContour *)p)->nEdges;
        t = rcossin_tbl;
        a2 = p + 0x50;
        a3 = p;
        for (;;) {
            if (i >= n) {
                return 1;
            }
            ang = ((MapContour *)a3)->edges[0].angle;  /* a3 steps an edge at a time */
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
        if (eng != NULL && CheckMelted(&eng->XYZ, 0)) {
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
            && ((s32 (*)(s32 *, s32, s32))PolarGetLevelPoint)((s32 *)&eng->XYZ, 1, player)
            && !(h->hero_status & 0x200)) {
            ext->unkA8 = 1;
            if (h->hero_mode != 0x4E) {
                SetHeroAction(player, 0x4E);
            }
        }
        if (bossmapsub->entity == 0x2009
            && ((s32 (*)(s32 *, s32, s32))PolarGetLevelPoint)((s32 *)&eng->XYZ, 2, player)
            && !(Heroes[player].hero_status & 0x200)) {
            ext->unkA8 = 1;
            if (Heroes[player].hero_mode != 0x4E) {
                SetHeroAction(player, 0x4E);
            }
        }
        if (bossmapsub->entity == 0x200A) {
            out = ((s32 (*)(s32 *, s32, s32))PolarGetLevelPoint)((s32 *)&eng->XYZ, 3, player) != 1;
            if (out && !(Heroes[player].hero_status & 0x200)) {
                ext->unkA8 = 1;
                if (Heroes[player].hero_mode != 0x4E) {
                    SetHeroAction(player, 0x4E);
                }
            }
        }
        if (bossmapsub->flags & 0x8000) {
            if (eng != NULL && ((s32 (*)(s32 *, s32, s32))PolarGetLevelPoint)((s32 *)&eng->XYZ, 0, player)) {
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
    info->hero_eng->sub->otz = 0;
    ext = FindExtension(info->hero, 0x20000000);
    if (ext != NULL) {
        ext->count = 0;
    }
    info->hero_eng->xform.order = 0;
    info->hero_eng->xform.rot.vx = 0;
    move = info->hero_move;
    if (move != NULL) {
        speed = csqrt(move->x_vel * move->x_vel + move->z_vel * move->z_vel) >> 6;
        move->z_dir = 0;
        move->x_acc = 2;
        move->x_dec = 1;
        move->z_acc = 2;
        move->z_dec = 1;
        if (((GameState *)GameInfo)->mode == 0x16) {
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
    if (ext2 != NULL && (((PadState *)Pads)[player].dir & 0xF0)) {
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
    PolarIceHit(&Heroes[player].hero_eng->XYZ, player);
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
                PlaySfx(0x49C, &base[0].hero_eng->sfx, &base[0].hero_eng->XYZ, 0x1000, 0x1E00, 0);
            }
            if (player == 1) {
                PlaySfx(0x49D, &base[1].hero_eng->sfx, &base[1].hero_eng->XYZ, 0x1000, 0x1E00, 0);
            }
            if (player == 2) {
                PlaySfx(0x49E, &base[2].hero_eng->sfx, &base[2].hero_eng->XYZ, 0x1000, 0x1E00, 0);
            }
            if (player == 3) {
                PlaySfx(0x49F, &base[3].hero_eng->sfx, &base[3].hero_eng->XYZ, 0x1000, 0x1E00, 0);
            }
        } else {
            PlaySfx(0x4A0, &h->hero_eng->sfx, &h->hero_eng->XYZ, 0x1000, 0x1E00, 0);
        }
    }
    ProcessHeroAnim(player);
    if (!(((PadState *)Pads)[player].dir & 0xF0)) {
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
    PolarIceHit(&Heroes[player].hero_eng->XYZ, player);
    if (TestPolarHeroWon(player) != 0) {
        if (Heroes[player].hero_eng->sfx != NULL) {
            StopSfx(Heroes[player].hero_eng->sfx);
        }
    }
}

void SetPolarHeroDazed(s32 player) {
    PolarAi *ext;
    NAS *nas;
    EXTscriptdata *fux;
    EXTengine *eng;
    EXTheromove *move;
    s32 sfx;

    if (TestPolarHeroWon(player) == 0) {
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
                ext->unk98 = 0;
                nas = CreateNAS(eng->XYZ.x, eng->XYZ.y, eng->XYZ.z, 0x1605, 0x11);
                if (nas != NULL) {
                    nas->Engine->flags |= 0x200000;
                    fux = (EXTscriptdata *)nas->FUX;
                    if (fux != NULL) {
                        fux->speed = 0x200;
                        fux->scriptnumber = 0;
                        fux->flags = 5;
                        nas->Engine->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
                        nas->Engine->flags |= 0x200000;
                        fux->maxloops = 6;
                    }
                    nas->InitDelete(nas);
                }
            }
            ext->unk1C = 0;
            move = Heroes[player].hero_move;
            if (move != NULL) {
                move->z_dir = 0;
                move->x_max = 0;
                move->z_max = 0;
                move->x_vel = 0;
                move->z_vel = 0;
                move->mass = 0x200;
            }
            Heroes[player].hero_eng->sub->otz = 0;
            if (Heroes[player].hero_name == 1) {
                if (ext->flag == 0) {
                    SetHeroSeq(player, 0x44);
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                    SetHeroSeq(player, 0x3C);
                }
            }
            if (Heroes[player].hero_name == 5) {
                if (ext->flag == 0) {
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    SetHeroSeq(player, 0x44);
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                    SetHeroSeq(player, 0x3C);
                }
            }
            if (Heroes[player].hero_name == 6) {
                if (ext->flag == 0) {
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    SetHeroSeq(player, 0x44);
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                    SetHeroSeq(player, 0x3C);
                }
            }
            if (Heroes[player].hero_name == 3) {
                if (ext->flag == 0) {
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    SetHeroSeq(player, 0x44);
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                    SetHeroSeq(player, 0x3C);
                }
            }
            if (Heroes[player].hero_name == 2) {
                if (ext->flag == 0) {
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    SetHeroSeq(player, 0x44);
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                    SetHeroSeq(player, 0x3C);
                }
            }
            if (Heroes[player].hero_name == 4) {
                if (ext->flag == 0) {
                    Heroes[player].hero_eng->sub->flags2 = 0x2001;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    SetHeroSeq(player, 0x44);
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                    SetHeroSeq(player, 0x3C);
                }
            }
            if (Heroes[player].hero_name == 7) {
                if (ext->flag == 0) {
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    SetHeroSeq(player, 0x44);
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                    SetHeroSeq(player, 0x3C);
                }
            }
            if (Heroes[player].hero_name == 8) {
                if (ext->flag == 0) {
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    SetHeroSeq(player, 0x44);
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                    SetHeroSeq(player, 0x3C);
                }
            }
        }
    }
}

void PolarHeroDazed(s32 player) {
    EXTheromove *move;
    PolarAi *ext;
    PolarAi *ext2;
    EXTsmoketrail *p;
    NAS *nas;
    EXTengine *eng;
    s32 x;
    s32 y;
    s32 z;

    if (TestPolarHeroWon(player) == 0) {
        move = Heroes[player].hero_move;
        ext = FindExtension(Heroes[player].hero, 0x20000000);
        if (ext != NULL) {
            eng = Heroes[player].hero_eng;
            x = eng->XYZ.x;
            y = eng->XYZ.y;
            z = eng->XYZ.z;
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
            if (ext->unk98-- < 0) {
                ext->unk98 = Random(8) + 0x18;
                nas = CreateNAS(x + 0x60, y - 0x180, z, 0x1612, 0x11);
                if (nas != NULL) {
                    p = FindExtension(nas, 0x10000000);
                    if (p != NULL) {
                        p->yvel = 0xA;
                        p->fadeuprate = 0x100;
                        p->fadedownrate = 0x50;
                        p->scalerate = -0x40;
                        p->amp = 0x20;
                        p->ampdec = 1;
                        p->flag |= 1;
                    }
                }
                nas = CreateNAS(x - 0x60, y - 0x180, z, 0x1612, 0x11);
                if (nas != NULL) {
                    p = FindExtension(nas, 0x10000000);
                    if (p != NULL) {
                        p->yvel = 0xA;
                        p->fadeuprate = 0x100;
                        p->fadedownrate = 0x50;
                        p->scalerate = -0x40;
                        p->amp = 0x20;
                        p->ampdec = 1;
                        p->flag |= 1;
                    }
                }
            }
            if (move != NULL) {
                ((PadState *)Pads)[player].dir = 0;
                ((PadState *)Pads)[player].dirPressed = 0;
                PolarHeroCollisions(player);
                TestPolarHeroMelted(player);
                PolarIceHit(&Heroes[player].hero_eng->XYZ, player);
                Heroes[player].hero_frozen = 2;
                if (ext->flag == 0 && ext->unk1C >= 0x1F) {
                    Heroes[player].hero_eng->xform.rot.vy += 0x10;
                    if (PolarRoundState != 0 && !(Heroes[player].hero_status & 0x200)) {
                        Heroes[player].hero_eng->sub->flags &= 0xFFFF7FFF;
                        Heroes[player].hero_shadow->sub->flags &= 0xFFFF7FFF;
                        SetHeroAction(player, 0x50);
                        return;
                    }
                    if (Heroes[player].hero_name == 1 && ext->flag == 0) {
                        Heroes[player].hero_eng->sub->entity = 0x4100;
                    }
                    if (Heroes[player].hero_name == 7 && ext->flag == 0) {
                        Heroes[player].hero_eng->sub->entity = 0x4000;
                    }
                    if (Heroes[player].hero_name == 8 && ext->flag == 0) {
                        Heroes[player].hero_eng->sub->entity = 0x4000;
                    }
                    if (Heroes[player].hero_name == 3 && ext->flag == 0) {
                        Heroes[player].hero_eng->sub->entity = 0x4100;
                    }
                    if (Heroes[player].hero_name == 2 && ext->flag == 0) {
                        Heroes[player].hero_eng->sub->entity = 0x4000;
                    }
                    if (Heroes[player].hero_name == 5 && ext->flag == 0) {
                        Heroes[player].hero_eng->sub->entity = 0x4000;
                    }
                    if (Heroes[player].hero_name == 6 && ext->flag == 0) {
                        Heroes[player].hero_eng->sub->entity = 0x4000;
                    }
                    if (Heroes[player].hero_name == 4 && ext->flag == 0) {
                        Heroes[player].hero_eng->sub->entity = 0x4080;
                    }
                }
                Heroes[player].hero_eng->xform.dirty = 1;
            }
            if (ext->unk1C++ >= 0xB5) {
                ext2 = FindExtension(Heroes[player].hero, 0x20000000);
                if (ext2 != NULL) {
                    ResetMass(player);
                    Heroes[player].hero_eng->sub->flags &= 0xFFBFFFFF;
                    if (ext2->flag == 0) {
                        SetHeroAction(player, 0x49);
                    }
                    if (ext2->flag == 1) {
                        SetHeroAction(player, 0x54);
                    }
                }
            }
        }
    }
}

void SetPolarHeroChallenge(s32 player) {
    PolarAi *ext;
    s32 sfx;

    if (TestPolarHeroWon(player) == 0) {
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
            ext->unk98 = 0;
            Heroes[player].hero_eng->sub->flags |= 0x400000;
            ext->unk1C = 0;
            if (Heroes[player].hero_name == 1) {
                if (ext->flag == 0) {
                    SetHeroSeq(player, 0x44);
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                }
            }
            if (Heroes[player].hero_name == 5) {
                if (ext->flag == 0) {
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                }
            }
            if (Heroes[player].hero_name == 6) {
                if (ext->flag == 0) {
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                }
            }
            if (Heroes[player].hero_name == 3) {
                if (ext->flag == 0) {
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                }
            }
            if (Heroes[player].hero_name == 2) {
                if (ext->flag == 0) {
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                }
            }
            if (Heroes[player].hero_name == 4) {
                if (ext->flag == 0) {
                    Heroes[player].hero_eng->sub->flags2 = 0x2001;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                }
            }
            if (Heroes[player].hero_name == 7) {
                if (ext->flag == 0) {
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                }
            }
            if (Heroes[player].hero_name == 8) {
                if (ext->flag == 0) {
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                } else {
                    Heroes[player].hero_eng->sub->flags |= 0x400000;
                    Heroes[player].hero_eng->sub->flags2 = 0x2002;
                }
            }
            if (Heroes[player].hero_name == 1 && ext->flag == 0) {
                Heroes[player].hero_eng->sub->entity = 0x4100;
            }
            if (Heroes[player].hero_name == 7 && ext->flag == 0) {
                Heroes[player].hero_eng->sub->entity = 0x4000;
            }
            if (Heroes[player].hero_name == 8 && ext->flag == 0) {
                Heroes[player].hero_eng->sub->entity = 0x4000;
            }
            if (Heroes[player].hero_name == 3 && ext->flag == 0) {
                Heroes[player].hero_eng->sub->entity = 0x4100;
            }
            if (Heroes[player].hero_name == 2 && ext->flag == 0) {
                Heroes[player].hero_eng->sub->entity = 0x4000;
            }
            if (Heroes[player].hero_name == 5 && ext->flag == 0) {
                Heroes[player].hero_eng->sub->entity = 0x4000;
            }
            if (Heroes[player].hero_name == 6 && ext->flag == 0) {
                Heroes[player].hero_eng->sub->entity = 0x4000;
            }
            if (Heroes[player].hero_name == 4 && ext->flag == 0) {
                Heroes[player].hero_eng->sub->entity = 0x4080;
            }
            Heroes[player].hero_eng->sub->otz = 0;
        }
    }
}

void PolarHeroChallenge(s32 player) {
    EXTheromove *move;
    PolarAi *ext;
    PolarAi *ext2;
    EXTsmoketrail *p;
    NAS *nas;
    EXTengine *eng;
    s32 x;
    s32 y;
    s32 z;

    if (TestPolarHeroWon(player) == 0) {
        move = Heroes[player].hero_move;
        ext = FindExtension(Heroes[player].hero, 0x20000000);
        if (ext != NULL) {
            eng = Heroes[player].hero_eng;
            x = eng->XYZ.x;
            y = eng->XYZ.y;
            z = eng->XYZ.z;
            if (ext->unk98-- < 0) {
                ext->unk98 = Random(0x14) + 1;
                nas = CreateNAS(x + 0x60, y - 0x180, z, 0x1612, 0x11);
                if (nas != NULL) {
                    p = FindExtension(nas, 0x10000000);
                    if (p != NULL) {
                        p->yvel = 0xA;
                        p->fadeuprate = 0x100;
                        p->fadedownrate = 0x50;
                        p->scalerate = -0x40;
                        p->amp = 0x20;
                        p->ampdec = 1;
                        p->flag |= 1;
                    }
                }
                nas = CreateNAS(x - 0x60, y - 0x180, z, 0x1612, 0x11);
                if (nas != NULL) {
                    p = FindExtension(nas, 0x10000000);
                    if (p != NULL) {
                        p->yvel = 0xA;
                        p->fadeuprate = 0x100;
                        p->fadedownrate = 0x50;
                        p->scalerate = -0x40;
                        p->amp = 0x20;
                        p->ampdec = 1;
                        p->flag |= 1;
                    }
                }
            }
            if (move != NULL) {
                ((PadState *)Pads)[player].dir = 0;
                ((PadState *)Pads)[player].dirPressed = 0;
                PolarHeroCollisions(player);
                TestPolarHeroMelted(player);
                PolarIceHit(&Heroes[player].hero_eng->XYZ, player);
                Heroes[player].hero_frozen = 2;
                if (ext->flag == 0) {
                    Heroes[player].hero_eng->xform.rot.vy += 0x10;
                }
                Heroes[player].hero_eng->xform.dirty = 1;
            }
            if (ext->unk1C++ >= 0xB5) {
                ext2 = FindExtension(Heroes[player].hero, 0x20000000);
                if (ext2 != NULL) {
                    ResetMass(player);
                    Heroes[player].hero_eng->sub->flags &= 0xFFBFFFFF;
                    if (ext2->flag == 0) {
                        SetHeroAction(player, 0x49);
                    }
                    if (ext2->flag == 1) {
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

    if (TestPolarHeroWon(player) == 0) {
        base = Heroes;
        hero = base + player;
        if (hero->hero_move != NULL) {
            hero->hero_move->z_dir = 0;
        }
        hero->hero_eng->sub->otz = 0;
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

    if (TestPolarHeroWon(player) == 0) {
        base = Heroes;
        hero = &base[player];
        move = hero->hero_move;
        if (move != NULL) {
            ((PadState *)Pads)[player].dir = 0;
            ((PadState *)Pads)[player].dirPressed = 0;
            PolarHeroCollisions(player);
            TestPolarHeroMelted(player);
            PolarIceHit(&hero->hero_eng->XYZ, player);
            hero->hero_frozen = 2;
            hero->hero_eng->xform.rot.vy += 0x10;
            hero->hero_eng->xform.dirty = 1;
            move->x_max = 0;
            move->z_max = 0;
        }
    }
}

void SetPolarHeroOffice(s32 player) {
    HeroInfo *hero;
    PolarHitExt *ext;
    EXTheromove *move;
    GameState *g;
    u16 status;
    HeroInfo *base;
    HeroInfo *base2;
    HeroInfo *h;

    if (TestPolarHeroWon(player) == 0) {
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
                if (ext->flag == 0) {
                    SetHeroSeq(player, 0x3D);
                }
                hero->hero_eng->sub->otz = 0;
                ext->speed = csqrt(move->x_vel * move->x_vel + move->z_vel * move->z_vel) >> 6;
                if (ext->speed >= 0x4A || ext->flag2 != 0) {
                    hero->hero_anim->Aindex = 0x21;
                    status = hero->hero_status;
                    if (!(status & 0x200)) {
                        hero->hero_status = status | 0x2000;
                    }
                } else if (ext->flag == 0) {
                    PlaySfx(0x45C, &hero->hero_eng->sfx, NULL, 0x1000, 0x1E00, 0);
                }
            }
            if (GameMode == 0x12) {
                if (PolarOfficeBox(player) != 0) {
                    Heroes[player].hero_anim->Aindex = 0x21;
                }
            }
            g = GAME;
            if (g->levelFlags & 0x80) {
                if (g->heroCpu[player] == 0 && g->mode == 0x14) {
                    Heroes[player].hero_anim->Aindex = 0x21;
                }
            }
            base2 = Heroes;
            h = &base2[player];
            if (h->hero_eng->XYZ.z >= -0x3E7) {
                h->hero_eng->sub->otz = 0x200;
            }
            if (GameMode == 0x16) {
                if (h->hero_eng->XYZ.z >= -0x3E7) {
                    h->hero_eng->sub->otz = 0x320;
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
        angle = (get_angle(hero->hero_eng->XYZ.x - PolarCentre.vx,
                           hero->hero_eng->XYZ.z - PolarCentre.vz) + 0x800) & 0xFFF;
        move->forward_angle = angle + 0x800;
        move->x_max = 0;
        move->z_max = 0;
        move->x_vel = (rcossin_tbl[(angle - 0x400) & 0xFFF].unk2 * 0x25) >> 11;
        move->z_vel = (rcossin_tbl[angle].unk2 * 0x25) >> 11;
        hero->hero_eng->XYZ.x += move->x_vel;
        hero->hero_eng->XYZ.z += move->z_vel;
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

    if (TestPolarHeroWon(player) != 0) {
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
        mode == 0x5A || ext->flag2 != 0) {
        {
            HeroInfo *b = Heroes;
            h2 = &b[player];
        }
        h2->hero_eng->XYZ.y += 0x60;
        ext->flag2 = 1;
        RemoveCamera(h2->hero);
        if (!(h2->hero_status & 0x200)) {
            h2->hero_status |= 0x2000;
        }
        if (h2->hero_eng->XYZ.y > 0x400) {
            SetHeroAction(player, 0x50);
        }
        return;
    }
    move = h->hero_move;
    if (move == NULL) {
        return;
    }
    eng = h->hero_eng;
    angle = (get_angle(eng->XYZ.x - PolarCentre.vx, eng->XYZ.z - PolarCentre.vz) + 0x800) & 0xFFF;
    if (ext->flag == 0) {
        ext->flag = csqrt(move->x_vel * move->x_vel + move->z_vel * move->z_vel) >> 6;
    }
    move->forward_angle = angle + 0x800;
    move->x_max = 0;
    move->z_max = 0;
    move->x_vel = 0;
    move->z_vel = 0;
    if (((PadState *)Pads)[player].dirPressed & 0x8000) {
        ext->count++;
    }
    if (h->hero_anim->Aindex < 0x21) {
        if (ext->flag < 0x4A && ext->count >= 5) {
            do {
                PolarHeroSlideFromCentre(player);
                ext->flag = 0;
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
    Heroes[player].hero_eng->sub->otz = 0;
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
    PolarAi *ext = FindExtension(info->hero, 0x20000000);

    if (ext != NULL) {
        if (ext->flag != 0) {
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
    PadState *pads;
    PadState *pad;

    base = Heroes;
    h = base + player;
    ext = FindExtension(h->hero, 0x20000000);

    if (ext != NULL && h->hero_move != NULL && ext->val >= PolarHeroAttackPower(player)) {
        pads = (PadState *)Pads;
        pad = pads + player;
        if (pad->dirPressed & 0x8000) {
            if (h->hero_mode != 0x51) {
                SetHeroAction(player, 0x51);
            }
            return 1;
        } else {
            pad->dir &= ~0x8000;
            return 0;
        }
    }
    return 0;
}

s32 PolarHeroThrowPower(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *info = base + player;
    PolarAi *ext = FindExtension(info->hero, 0x20000000);

    if (ext != NULL) {
        if (ext->flag != 0) {
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
            h2->hero_eng->sub->otz = 0;
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
                    nas = CreateNAS(eng->XYZ.x, eng->XYZ.y - ((eng->xform.scale.vx * 0x1A4) >> 12), eng->XYZ.z, 0x1616, 0x11);
                }
                ext->nas = nas;
                if (nas != NULL) {
                    nas->UserInfo.user1 = (iptr)hero;
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
                    nas = CreateNAS(eng->XYZ.x, eng->XYZ.y - ((eng->xform.scale.vx * 0x1A4) >> 12), eng->XYZ.z, 0x1615, 0x11);
                }
                ext->nas = nas;
                if (nas != NULL) {
                    nas->UserInfo.user1 = (iptr)hero;
                    if (GameMode == 0x14) {
                        ext->nas->Engine->sub->u6C.dbase = &HeroModelBank[DB_CharsPolarDingo];
                        ext->nas->Engine->sub->entity = 0x2002;
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
                    nas = CreateNAS(eng->XYZ.x, eng->XYZ.y - ((eng->xform.scale.vx * 0xB4) >> 12), eng->XYZ.z, 0x1615, 0x11);
                }
                ext->nas = nas;
                if (nas != NULL) {
                    nas->UserInfo.user1 = (iptr)hero;
                    if (GameMode == 0x14) {
                        ext->nas->Engine->sub->u6C.dbase = &HeroModelBank[DB_CharsPolarRilla];
                    } else {
                        ext->nas->Engine->sub->u6C.dbase = &HeroModelBank[DB_CharsPolar2Rilla];
                    }
                    ext->nas->Engine->sub->entity = 0x2003;
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
    PolarAi *ext = FindExtension(info->hero, 0x20000000);
    PadState *pads;
    PadState *pad;

    if (ext != NULL) {
        if (ext->unk28 == 0) {
            return 0;
        }
        if (info->hero_move != NULL) {
            pads = (PadState *)Pads;
            pad = pads + player;
            if (pad->dirPressed & 0x2000) {
                if (info->hero_mode != 0x53) {
                    SetHeroAction(player, 0x53);
                }
                return 1;
            }
            pad->dir &= ~0x2000;
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
    PolarAi *ext;
    NAS *nas;
    PolarThrown *t;
    s32 dist;

    base = Heroes;
    h = &base[player];
    ext = FindExtension(h->hero, 0x20000000);
    if (ext != NULL) {
    dist = 0;
    pos = h->hero_eng->XYZ;
    pos.y -= 0x100;
    nas = CreateNAS(pos.x, pos.y, pos.z, 0x160D, 0x11);
    if (nas != NULL) {
    t = FindExtension(nas, 0x10000000);
    if (t != NULL) {
    t->start = pos;
    t->owner = player;
    ext->unk28--;
    nas->Engine->xform.rot.vy = base[t->owner].hero_eng->xform.rot.vy;
    to.y = h->hero_eng->XYZ.y;
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
    to.x = eng->XYZ.x - (dist * rcossin_tbl[(eng->xform.rot.vy - 0x400) & 0xFFF].unk2 >> 12);
    eng = h2->hero_eng;
    to.z = eng->XYZ.z - (dist * rcossin_tbl[eng->xform.rot.vy & 0xFFF].unk2 >> 12);
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
    h->hero_eng->sub->otz = 0;
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
        h->hero_eng->sub->entity = 0x2004;
        if (GameLevelFlags & 0x80) {
            h->hero_eng->sub->entity = 0x2004;
        }
        break;
    }
    Heroes[player].hero_eng->sub->u6C.dbase = MapScene->dbase;
    Heroes[player].hero_eng->xform.order = 2;
}

void PolarHeroSnowball(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *info = base + player;
    EXTheromove *move;
    Ext20 *ext;
    s32 speed;

    move = info->hero_move;
    if (move != NULL && TestPolarHeroWon(player) == 0) {
        speed = csqrt(move->x_vel * move->x_vel + move->z_vel * move->z_vel) >> 6;
        ext = FindExtension(info->hero, 0x20000000);
        if (ext != NULL) {
            ext->val += 4;
            if (ext->val > 0x100) {
                ext->val = 0x100;
            }
        }
        PolarHeroCollisions(player);
        Heroes[player].hero_eng->xform.rot.vx += speed;
        TestPolarHeroMelted(player);
        PolarIceHit(&Heroes[player].hero_eng->XYZ, player);
        Heroes[player].hero_eng->xform.dirty = 1;
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
    info = (HeroInfo *)FILE_AT(Heroes, off);
    sfx = 0;
    info->hero_eng->sub->otz = 0;
    speed = 0;
    if (info->hero_eng->sfx != NULL) {
        StopSfx(info->hero_eng->sfx);
    }
    ext = FindExtension(info->hero, 0x20000000);
    if (ext != NULL) {
        ext->count = 0;
        ext->state = 1;
    }
    info->hero_eng->xform.order = 0;
    info->hero_eng->xform.rot.vx = 0;
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
        if (ext != NULL && (((PadState *)Pads)[player].dir & 0xF0)) {
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
        PolarIceHit(&Heroes[player].hero_eng->XYZ, player);
    }
}

void PolarHeroFootRun(s32 player) {
    HeroInfo *h;
    HeroInfo *base;
    Ext20 *ext;
    Ext20 *ext1;

    if (TestPolarHeroWon(player) == 0) {
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
                    PlaySfx(0x49C, &base[0].hero_eng->sfx, &base[0].hero_eng->XYZ, 0x1000, 0x1E00, 0);
                }
                if (player == 1) {
                    PlaySfx(0x49D, &base[1].hero_eng->sfx, &base[1].hero_eng->XYZ, 0x1000, 0x1E00, 0);
                }
                if (player == 2) {
                    PlaySfx(0x49E, &base[2].hero_eng->sfx, &base[2].hero_eng->XYZ, 0x1000, 0x1E00, 0);
                }
                if (player == 3) {
                    PlaySfx(0x49F, &base[3].hero_eng->sfx, &base[3].hero_eng->XYZ, 0x1000, 0x1E00, 0);
                }
            } else {
                PlaySfx(0x4A0, &h->hero_eng->sfx, &h->hero_eng->XYZ, 0x1000, 0x1E00, 0);
            }
        }
        ProcessHeroAnim(player);
        if (!(((PadState *)Pads)[player].dir & 0xF0)) {
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
        PolarIceHit(&Heroes[player].hero_eng->XYZ, player);
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
    PolarAi *ext;
    u16 status;
    EXTengine *eng;

    ext = FindExtension(h->hero, 0x20000000);
    if (ext != NULL) {
        ext->deadTimer = 0;
        ext->unk98 = 0;
        h->hero_eng->xform.order = 2;
        status = h->hero_status;
        if (!(status & 0x200)) {
            h->hero_status = status | 0x2000;
        }
        h->hero_eng->sub->flags |= 0x8000;
        h->hero_eng->sub->u6C.dbase = &HeroModelBank[DB_PolarObjects];
        h->hero_eng->sub->entity = 0x202D;
        eng = h->hero_eng;
        CreateNAS(eng->XYZ.x + 0x80, eng->XYZ.y - 0x200, eng->XYZ.z, 0x1613, 0x11);
    }
}

void PolarHeroDead(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *h = base + player;
    PolarAi *ext;
    EXTsmoketrail *p;
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
        t = ext->deadTimer + 1;
        x = eng->XYZ.x;
        y = eng->XYZ.y;
        z = eng->XYZ.z;
        ext->deadTimer = t;
        if (t == 4) {
            CreateNAS(x - 0x80, y - 0x100, z, 0x1613, 0x11);
            CreateNAS(x, y - 0x100, z, 0x1614, 0x11);
        }
        if (ext->deadTimer == 2) {
            CreateNAS(x + 0x80, y - 0x180, z, 0x1613, 0x11);
        }
        if (ext->deadTimer == 0x3C) {
            SetHeroAction(player, 0x50);
        }
        if (--ext->unk98 < 0) {
            ext->unk98 = Random(7) + 1;
            nas = CreateNAS(x + 0x40, y - 0x40, z, 0x1612, 0x11);
            if (nas != NULL) {
                p = FindExtension(nas, 0x10000000);
                if (p != NULL) {
                    p->yvel = 0xA;
                    p->fadeuprate = 0x400;
                    p->fadedownrate = 0x50;
                    p->scalerate = -0x40;
                    p->amp = 0x20;
                    p->ampdec = 1;
                    p->flag |= 1;
                }
            }
            nas = CreateNAS(x - 0x40, y - 0x40, z, 0x1612, 0x11);
            if (nas != NULL) {
                p = FindExtension(nas, 0x10000000);
                if (p != NULL) {
                    p->yvel = 0xA;
                    p->fadeuprate = 0x258;
                    p->fadedownrate = 0x50;
                    p->scalerate = -0x40;
                    p->amp = 0x20;
                    p->ampdec = 1;
                    p->flag |= 1;
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
    first->hero_eng->xform.rot.vx = 0;
    first->hero_eng->xform.dirty = 1;
    first->hero_eng->xform.dirty = 1;
    first->hero_eng->xform.scale.vx = 0x1000;
    first->hero_eng->xform.scale.vy = 0x1000;
    first->hero_eng->xform.scale.vz = 0x1000;
    shadow = first->hero_shadow;
    if (shadow != NULL) {
        shadow->xform.scale.vx = 0x1000;
        shadow->xform.scale.vy = 0x1000;
        shadow->xform.scale.vz = 0x1000;
        shadow->xform.dirty = 1;
    }
    first->hero_eng->sub->flags &= 0xFFBFFFFF;
    if (PolarRoundState != 0 && PolarBeamOwner != NULL) {
        game = FindExtension((NAS *)PolarBeamOwner, 0x40000000);
        i = 0;
        if (game != NULL) {
            for (; i < 4; i++) {
                game->eng2[i]->flags &= 0xFFFF7FFF;
            }
            game->eng->flags &= 0xFFFF7FFF;
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
    if (Heroes[player].hero_eng->XYZ.y > 0x60) {
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

    hero->hero_eng->xform.dirty = 1;
    hero->hero_eng->xform.scale.vx = 0x1000;
    hero->hero_eng->xform.scale.vy = 0x1000;
    hero->hero_eng->xform.scale.vz = 0x1000;
    move = hero->hero_move;
    if (move != NULL) {
        hero->hero_eng->xform.rot.vx = 0;
        hero->hero_eng->xform.dirty = 1;
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
        h->hero_eng->xform.order = 2;
        status = h->hero_status;
        if (!(status & 0x200)) {
            h->hero_status = status | 0x2000;
        }
    }
}

s32 TestPolarHeroWon(s32 arg0) {
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
    PolarIceHit(&Heroes[player].hero_eng->XYZ, player);
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
    hero->hero_eng->sub->otz = 0;
}

void PolarHeroThrowBomb(s32 player) {
    PolarAi *ext = FindExtension(Heroes[player].hero, 0x20000000);

    if (ext != NULL) {
        if (ext->flag == 0) {
            SetHeroAction(player, 0x49);
        }
        if (ext->flag == 1) {
            SetHeroAction(player, 0x54);
        }
        ProcessHeroAnim(player);
    }
}

void PolarHeroAttack(s32 player) {
    HeroInfo *base = Heroes;
    HeroInfo *info = base + player;
    PolarAi *ext = FindExtension(info->hero, 0x20000000);

    if (ext != NULL && TestPolarHeroWon(player) == 0 && info->hero_move != NULL) {
        PolarHeroCollisions(player);
        PolarHeroAttackThrow(player);
        EXT_W(ext, 4) -= 0xF;
        if (EXT_W(ext, 4) < 0) {
            EXT_W(ext, 4) = 0;
        }
        ProcessHeroAnim(player);
        TestPolarHeroMelted(player);
        PolarIceHit(&info->hero_eng->XYZ, player);
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
    TestPolarHeroWon(player);
}

void PolarHeroInBeam(s32 player) {
    PadState *base;
    PadState *pad;

    base = Pads;
    pad = (PadState *)(base + player);
    pad->dir = 0;
    pad->dirPressed = 0;
    ProcessHeroAnim(player);
}

void PolarHeroWeight(s32 player) {
    GameState *g = GAME;
    EXTheromove *mv;

    if (g->mode != 0x16) {
        PadState *b = Pads;
        PadState *p = &b[player];
        p->dir = 0;
        p->dirPressed = 0;
    }
    mv = Heroes[player].hero_move;
    if (mv != NULL) {
        mv->z_dir = 0;
        mv->x_max = 0;
        mv->z_max = 0;
        mv->x_vel = 0;
        mv->z_vel = 0;
    }
    if (g->mode != 0x16) {
        TestPolarHeroWon(player);
    }
}

void SetPolarHeroWeight(s32 player) {
    if (GameMode != 0x16) {
        TestPolarHeroWon(player);
    }
}

void SetPolarHeroRun(s32 player) {
    HeroInfo *base;
    HeroInfo *hero;

    if (TestPolarHeroWon(player) == 0) {
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
        hero->hero_eng->sub->otz = 0;
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

    if (((PadState *)Pads)[player].dir & 0xF0) {
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
        if (!(((PadState *)Pads)[player].dir & 0xF0)) {
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
