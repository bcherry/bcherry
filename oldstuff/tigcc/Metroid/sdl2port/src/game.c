/*
 * game.c - Core game initialization and input handling for Metroid TI-89 SDL2 port
 * Ported from original TIGCC all.h / main game logic
 */
#include <stdlib.h>
#include <string.h>
#include <SDL.h>

#include "game.h"
#include "sprites.h"
#include "tiles.h"
#include "map_data.h"
#include "gfx_data.h"
#include "render.h"
#include "samus.h"
#include "enemies.h"
#include "objects.h"
#include "maps.h"

/* Global pointers */
struct samus* samus = NULL;
struct cam* cam = NULL;
struct game* game = NULL;
struct map* map = NULL;
struct graphics* gfx = NULL;
struct tile* tiles = NULL;
struct enemy* enemyKey = NULL;
struct sprites* sprites = NULL;
struct templates* templates = NULL;
struct enemyList* enemies = NULL;
struct objlist* objects = NULL;

/* Previous key state for edge detection */
static uint8_t prev_keys[SDL_NUM_SCANCODES];

/*
 * Update a single key's edge-detected state.
 * 0 = not pressed
 * 1 = just pressed this frame
 * 2 = held (was already pressed)
 */
static char update_key(SDL_Scancode sc) {
    const uint8_t *state = SDL_GetKeyboardState(NULL);
    char result;

    if (state[sc]) {
        if (prev_keys[sc])
            result = 2;  /* held */
        else
            result = 1;  /* just pressed */
        prev_keys[sc] = 1;
    } else {
        result = 0;
        prev_keys[sc] = 0;
    }
    return result;
}

void getKeys(void) {
    SDL_PumpEvents();

    game->keys.up    = update_key(SDL_SCANCODE_UP);
    game->keys.down  = update_key(SDL_SCANCODE_DOWN);
    game->keys.left  = update_key(SDL_SCANCODE_LEFT);
    game->keys.right = update_key(SDL_SCANCODE_RIGHT);
    game->keys.jump  = update_key(SDL_SCANCODE_Z);
    game->keys.fire  = update_key(SDL_SCANCODE_X);
    game->keys.sprint = update_key(SDL_SCANCODE_C);
    game->keys.aim   = update_key(SDL_SCANCODE_A);
    game->keys.f5    = update_key(SDL_SCANCODE_D);
    game->keys.f1    = update_key(SDL_SCANCODE_1);
    game->keys.f2    = update_key(SDL_SCANCODE_2);
    game->keys.f3    = update_key(SDL_SCANCODE_3);
    game->keys.f4    = update_key(SDL_SCANCODE_4);
}

int game_init(void) {
    /* Allocate game struct */
    game = calloc(1, sizeof(struct game));
    if (!game) return -1;

    game->frame = 0;
    /* game->mode will be set to playingMode by the caller after all systems init */

    /* Point gfx to the global gfx_data struct */
    gfx = &gfx_data;

    /* Allocate camera */
    cam = calloc(1, sizeof(struct cam));
    if (!cam) return -1;

    /* Initialize sprites */
    makeSprites();
    if (!sprites) return -1;

    /* Initialize tiles */
    makeTiles();
    if (!tiles) return -1;

    /* Initialize enemies system */
    makeEnemies();

    /* Initialize objects/templates system */
    makeObjects();

    /* Initialize maps and camera */
    makeMaps();

    /* Initialize Samus */
    makeSamus();

    /* Load starting map (map 0, door 0) */
    loadMap(0, 0);

    /* Set game mode to playing */
    game->mode = playingMode;

    /* Clear previous key state */
    memset(prev_keys, 0, sizeof(prev_keys));

    return 0;
}

void game_cleanup(void) {
    if (sprites) {
        free(sprites);
        sprites = NULL;
    }
    if (tiles) {
        free(tiles);
        tiles = NULL;
    }
    if (cam) {
        free(cam);
        cam = NULL;
    }
    if (samus) {
        free(samus);
        samus = NULL;
    }
    if (enemies) {
        /* TODO: free enemy linked list nodes */
        free(enemies);
        enemies = NULL;
    }
    if (objects) {
        /* TODO: free object linked list nodes */
        free(objects);
        objects = NULL;
    }
    if (game) {
        free(game);
        game = NULL;
    }
}
