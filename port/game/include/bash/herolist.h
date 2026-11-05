#ifndef BASH_HEROLIST_H
#define BASH_HEROLIST_H

/* The extra heroes (PC only; docs/EXTRA_HEROES.md), a line each: its EXTRA_ name and the HeroDef its file in main/heroes/ defines. Their
 * order is their cards' in the character select's third row, which has room for EXTRA_CARDS_MAX. A hero's line is all it needs outside
 * its own file: from it come its number (extraheroes.h's EXTRA_), its place in HeroDefs (bonusheroes.c), its card, and for each kind of
 * model an archive entry for its textures and one for its model (main.c's DatIndex), a file group of the two (FileGroups), a bank
 * (modelbank.h's DB_Bonus, dbase.c's HeroModelBank) and its loading (levelload.c's ModeDbaseLists), all by the numbers below. */
#define EXTRA_HERO_LIST(X) \
    X(ROO, HeroRoo) \
    X(PINSTRIPE, HeroPinstripe)

#define EXTRA_COUNT_ONE(name, def) +1
enum { EXTRA_HERO_COUNT = 0 EXTRA_HERO_LIST(EXTRA_COUNT_ONE) };
#define EXTRA_CARDS_MAX 4

/* Each one's kinds of model (bash/herodef.h's KIND_, as many as HERO_KINDS): M(x, kind) for each, as the tables list them */
#define HERO_KINDS 10
#define HERO_KIND_EACH(M, x) M(x, 0) M(x, 1) M(x, 2) M(x, 3) M(x, 4) M(x, 5) M(x, 6) M(x, 7) M(x, 8) M(x, 9)

/* Where they go: extra hero x's (from 1, its EXTRA_ number) kind k is the BONUS_SLOT'th of all theirs. Its bank is DB_Bonus plus that,
 * its archive entries two from BONUS_FILE_FIRST, its file group BONUS_GROUP_FIRST plus it, and its loading BONUS_LOAD_FIRST plus it. */
#define BONUS_SLOT(x, k) (((x) - 1) * HERO_KINDS + (k))
#define BONUS_SLOTS (EXTRA_HERO_COUNT * HERO_KINDS)
#define BONUS_FILE_FIRST 992
#define BONUS_TEX(x, k) (BONUS_FILE_FIRST + BONUS_SLOT(x, k) * 2)
#define BONUS_MDL(x, k) (BONUS_TEX(x, k) + 1)
#define BONUS_GROUP_FIRST 130
#define BONUS_LOAD_FIRST 96

#endif
