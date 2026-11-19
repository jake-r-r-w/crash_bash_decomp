/* Recomp Achievements (achievements.c; docs/PORT.md, Achievements): what the game reports to RecompLauncher. Outside RecompLauncher, and
 * on Android, every call does nothing. */
#ifndef ACHIEVEMENTS_H
#define ACHIEVEMENTS_H

/* Once, as the program starts: connects when RecompLauncher started it (RL_SESSION) */
void achievements_start(void);
/* Whether it's connected: the game's observer (game/main/achievements.c) only looks when it is */
int achievements_on(void);
/* Once a game step: what the launcher sent */
void achievements_frame(void);
/* An event the list maps to an achievement (port/release/achievements.json) */
void achievements_event(const char *name);
/* A save was loaded: unlocks just after it count as Casual */
void achievements_save_loaded(void);
/* Unlocks from now on are Casual: the cheat menu, a replayed online game */
void achievements_casual(const char *reason);
/* What the player's doing, for their profile */
void achievements_presence(const char *text);

#endif
