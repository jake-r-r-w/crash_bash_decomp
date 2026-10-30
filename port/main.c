/* The PC port's entry point (docs/PORT.md). The game doesn't run yet: for now this checks the build. */
#include <stdio.h>
#include <string.h>

#define SDL_MAIN_HANDLED
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

/* Starts SDL with its dummy drivers, so it works with no display or sound card. */
static int check_sdl(void) {
    SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "dummy");
    SDL_SetHint(SDL_HINT_AUDIO_DRIVER, "dummy");
    SDL_SetMainReady();
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    printf("SDL %d\n", SDL_GetVersion());
    SDL_Quit();
    return 0;
}

int main(int argc, char **argv) {
    if (argc == 2 && strcmp(argv[1], "--version") == 0) {
        printf("crashbash %s\n", BASH_VERSION);
        return 0;
    }
    if (argc == 2 && strcmp(argv[1], "--check-sdl") == 0) {
        return check_sdl();
    }
    puts("The PC port doesn't run the game yet (docs/ROADMAP-B.md).");
    return 0;
}
