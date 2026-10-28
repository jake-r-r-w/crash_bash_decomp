#ifndef BASH_AISKILL_H
#define BASH_AISKILL_H

/* A player's AI skill (AiPlayerSkill, by player; [3] is a boss's in a boss arena): the level
 * the games' odds tables are indexed by, and a quality the arena games sum. Its own header
 * for the units that don't include bash/main.h. owner: src/overlays/engine/heroes.c */
typedef struct AiSkill {
    s16 level;
    s16 unk2;
    s16 quality;
} AiSkill;
extern AiSkill AiPlayerSkill[4];

#endif /* BASH_AISKILL_H */
