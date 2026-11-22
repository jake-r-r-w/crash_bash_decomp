/* The hero tool's sounds (sound.c): a short, soft cue for each kind of action. */
#ifndef HEROTOOL_SOUND_H
#define HEROTOOL_SOUND_H

#ifdef __cplusplus
extern "C" {
#endif

enum {
    CUE_TAP,   /* a button */
    CUE_NAV,   /* another page or game */
    CUE_ON,    /* a toggle on (two notes up) */
    CUE_OFF,   /* a toggle off (two notes down) */
    CUE_PICK,  /* a bone or a key picked */
    CUE_DROP,  /* let go of, deleted, or gone back */
    CUE_OPEN,  /* a menu or card opened */
    CUE_HIDE,  /* a bone hidden (a fall) */
    CUE_SHOW,  /* a bone shown again (a rise) */
    CUE_PLAY,  /* playing */
    CUE_STOP,  /* stopped */
    CUE_SAVE,  /* saved (four notes up) */
    CUE_MADE,  /* the games made it again */
    CUE_VIEW,  /* looked from another side */
    CUE_UP,    /* a number up a step */
    CUE_DOWN,  /* a number down a step */
    CUE_GUESS, /* joints guessed (a run up) */
    CUE_N
};

void sound_init(void);
void sound_quit(void);
void sound_cue(int cue);
/* a WAV played as it is (a hero's own sound, to hear it) */
void sound_wav(const char *path);
void sound_mute(int muted);
int sound_muted(void);

#ifdef __cplusplus
}
#endif
#endif
