#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "render.h"
#include "game.h"

#define TARGET_FPS 20
#define FRAME_DELAY (1000 / TARGET_FPS)

int main(int argc, char *argv[]) {
    (void)argc; (void)argv;

    srand(time(NULL));

    if (render_init() != 0) {
        fprintf(stderr, "Failed to initialize renderer\n");
        return 1;
    }

    game_init();

    int running = 1;
    Uint32 frame_start;

    while (running) {
        frame_start = SDL_GetTicks();

        // Process SDL events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Run one frame of game logic
        running = game->mode();

        // Frame timing
        Uint32 frame_time = SDL_GetTicks() - frame_start;
        if (frame_time < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frame_time);
        }
    }

    game_cleanup();
    render_cleanup();

    return 0;
}
