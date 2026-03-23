/*
 * maps.c - Map loading, drawing, camera, doors, and game mode logic
 * Ported from original TIGCC Maps.c and Game Modes.c
 */
#include <stdlib.h>
#include <string.h>
#include <SDL.h>

#include "maps.h"
#include "game.h"
#include "render.h"
#include "tiles.h"
#include "sprites.h"
#include "samus.h"
#include "enemies.h"
#include "objects.h"
#include "gfx_data.h"
#include "hud.h"

/* Helper macro: get tile index at map position (c, r) using the original
 * scrolling map indirection: map->array[1 - map->array[r*map->w] + r*map->w + c]
 */
#define TILE_AT(c, r) (map->array[1 - map->array[(r) * map->w] + (r) * map->w + (c)])

/*
 * makeMaps - Allocate map and camera structs.
 * Ported from original Maps.c makeMaps().
 */
void makeMaps(void) {
    if ((map = malloc(sizeof(struct map))) == NULL)
        exit(1);
    if ((cam = malloc(sizeof(struct cam))) == NULL)
        exit(1);
    struct cam c = {0, 0, 1};
    memcpy(cam, &c, sizeof(c));
}

/*
 * loadMap - Load map m from the maps[] array. Set camera based on door d.
 * Replaces original fread-from-file approach with memcpy from compiled map data.
 */
void loadMap(int m, int d) {
    (void)d;
    freeObjects();
    freeEnemies();
    memcpy(map, &maps[m], sizeof(struct map));
}

/*
 * clearDoor - Open door i by setting its 4 tiles to 0.
 * Ported from original Maps.c clearDoor().
 */
void clearDoor(int i) {
    int x = map->doors[i].x;
    int y = map->doors[i].y;
    char dir = map->doors[i].dir;

    switch (dir) {
        case 'r':
        case 'l':
            map->array[1 + y * map->w + x] = 0;
            map->array[1 + (y + 1) * map->w + x] = 0;
            map->array[1 + (y + 2) * map->w + x] = 0;
            map->array[1 + (y + 3) * map->w + x] = 0;
            break;
        case 'u':
        case 'd':
            map->array[1 + y * map->w + x] = 0;
            map->array[1 + y * map->w + x + 1] = 0;
            map->array[1 + y * map->w + x + 2] = 0;
            map->array[1 + y * map->w + x + 3] = 0;
            break;
    }
}

/*
 * fillDoor - Close door i by setting its 4 tiles to the appropriate door tile indices.
 * Ported from original Maps.c fillDoor().
 */
void fillDoor(int i) {
    int x = map->doors[i].x;
    int y = map->doors[i].y;
    char dir = map->doors[i].dir;

    switch (dir) {
        case 'r':
            map->array[1 + y * map->w + x] = 19;
            map->array[1 + (y + 1) * map->w + x] = 20;
            map->array[1 + (y + 2) * map->w + x] = 21;
            map->array[1 + (y + 3) * map->w + x] = 22;
            break;
        case 'l':
            map->array[1 + y * map->w + x] = 11;
            map->array[1 + (y + 1) * map->w + x] = 12;
            map->array[1 + (y + 2) * map->w + x] = 13;
            map->array[1 + (y + 3) * map->w + x] = 14;
            break;
        case 'u':
            map->array[1 + y * map->w + x] = 32;
            map->array[1 + y * map->w + x + 1] = 33;
            map->array[1 + y * map->w + x + 2] = 34;
            map->array[1 + y * map->w + x + 3] = 35;
            break;
        case 'd':
            map->array[1 + y * map->w + x] = 40;
            map->array[1 + y * map->w + x + 1] = 41;
            map->array[1 + y * map->w + x + 2] = 42;
            map->array[1 + y * map->w + x + 3] = 43;
            break;
    }
}

/*
 * drawMap - Render visible tiles with camera offset.
 * Ported from original Maps.c drawMap().
 * Enemy spawns (tile >= 200) are converted to pushEnemy calls.
 */
void drawMap(void) {
    int i;

    /* Update door tile states */
    for (i = 0; i < map->d; i++) {
        if (map->doors[i].state == 0) {
            fillDoor(i);
        } else if (map->doors[i].state == 1) {
            clearDoor(i);
        }
    }

    int r, c;
    for (r = cam->y / 8 - BUFFER; r < (int)(cam->y / 8 + LCD_HEIGHT / 8 + BUFFER); r++) {
        for (c = cam->x / 8 - BUFFER; c < (int)(cam->x / 8 + LCD_WIDTH / 8 + BUFFER); c++) {
            if ((r < 0 || r > map->h) ||
                (c < map->array[r * map->w]) ||
                (c >= map->array[r * map->w] + (map->w - 1))) {
                /* Out of bounds: fill with black */
                blit_sprite_8(c * 8 - cam->x, r * 8 - cam->y, 8,
                              gfx->tiles.black, gfx->tiles.black, NULL);
            } else {
                int tile_idx = map->array[1 + r * map->w + c - map->array[r * map->w]];

                if (tile_idx >= 200) {
                    /* Enemy spawn tile */
                    pushEnemy(c * 8, (r + 1) * 8, tile_idx - 200);
                    map->array[1 + r * map->w + c - map->array[r * map->w]] = 0;
                    continue;
                }

                /* Render tile using tile sprite data
                 * tiles[idx].s is a [3][8] array: [0]=plane0, [1]=plane1, [2]=mask
                 */
                const uint8_t *tile_data = (const uint8_t *)tiles[tile_idx].s;
                blit_sprite_8(c * 8 - cam->x, r * 8 - cam->y, 8,
                              tile_data,          /* plane 0 */
                              tile_data + 8,      /* plane 1 */
                              tile_data + 16);    /* mask */

                onDraw(c, r);
            }
        }
    }
}

/*
 * adjustCamera - Track samus position with CAMERA_MOVE deadzone. Clamp to map bounds.
 * Ported from original Game Modes.c adjustCamera().
 */
void adjustCamera(void) {
    if (samus->x < (cam->x + CAMERA_MOVE)) {
        cam->x = samus->x - CAMERA_MOVE;
        if (cam->x < map->l)
            cam->x = map->l;
    } else if ((int)(samus->x) > (int)(cam->x + LCD_WIDTH - CAMERA_MOVE)) {
        cam->x = samus->x + CAMERA_MOVE - LCD_WIDTH;
        if ((int)(cam->x + LCD_WIDTH) >= (int)(map->r * 8))
            cam->x = map->r * 8 - LCD_WIDTH;
    }
    if ((int)samus->y > (int)(cam->y + LCD_HEIGHT - CAMERA_MOVE)) {
        cam->y = samus->y + CAMERA_MOVE - LCD_HEIGHT;
        if ((int)cam->y > (int)(map->b * 8 - LCD_HEIGHT))
            cam->y = map->b * 8 - LCD_HEIGHT;
    } else if ((int)(samus->y) < (int)(cam->y + CAMERA_MOVE)) {
        cam->y = samus->y - CAMERA_MOVE;
        if (cam->y < map->t)
            cam->y = map->t;
    }
}

/*
 * checkDoors - Check if samus is at a door boundary and initiate map transition.
 * Ported from original Game Modes.c checkDoors().
 * Returns 1 if transition started, 0 otherwise.
 */
int checkDoors(void) {
    if (SAMUS_LEFT < 3 || SAMUS_RIGHT > (map->w) * 8 - 3 ||
        samus->y - samus->sprt->h < 3 || samus->y > (map->h) * 8 - 3) {
        int i;
        for (i = 0; i < map->d; i++) {
            if (map->doors[i].state == 0)
                continue;
            else if (((map->doors[i].dir == 'l' || map->doors[i].dir == 'r') &&
                      (samus->y - samus->sprt->h >= map->doors[i].y * 8 &&
                       samus->y <= (map->doors[i].y + 4) * 8)) ||
                     ((map->doors[i].dir == 'u' || map->doors[i].dir == 'd') &&
                      (SAMUS_LEFT >= map->doors[i].x * 8 &&
                       SAMUS_RIGHT <= (map->doors[i].x + 4) * 8))) {
                int m = map->doors[i].map_link;
                int d = map->doors[i].link_i;
                cam->y = map->doors[i].y * 8 - cam->y;
                cam->x = map->doors[i].x * 8 - cam->x;
                loadMap(m, d);
                switch (map->doors[d].dir) {
                    case 'l':
                        samus->x = map->doors[d].x * 8 - 16;
                        samus->y = map->doors[d].y * 8 + 32;
                        cam->x = map->w * 8;
                        cam->y = map->doors[d].y * 8 - cam->y;
                        break;
                    case 'r':
                        samus->x = map->doors[d].x * 8 + 8;
                        samus->y = map->doors[d].y * 8 + 32;
                        cam->x = -LCD_WIDTH;
                        cam->y = map->doors[d].y * 8 - cam->y;
                        break;
                    case 'u':
                        samus->x = map->doors[d].x * 8 + 8;
                        samus->y = map->doors[d].y * 8;
                        cam->x = map->doors[d].x * 8 - cam->x;
                        cam->y = map->h * 8;
                        break;
                    case 'd':
                        samus->x = map->doors[d].x * 8 + 8;
                        samus->y = map->doors[d].y * 8 + 32;
                        cam->x = map->doors[d].x * 8 - cam->x;
                        cam->y = -LCD_HEIGHT;
                        break;
                }
                game->mode = mapTransition;
                game->frame = d;
                return 1;
            }
        }
    }
    return 0;
}

/*
 * drawDoors - Render door tiles during map transitions.
 * Ported from original Game Modes.c drawDoors().
 */
void drawDoors(void) {
    int x = 0, y = 0, d = game->frame, n = 0;
    switch (map->doors[d].dir) {
        case 'l':
            x = map->doors[d].x;
            y = map->doors[d].y;
            n = 11;
            break;
        case 'r':
            x = map->doors[d].x - 2;
            y = map->doors[d].y;
            n = 11;
            break;
        case 'u':
            x = map->doors[d].x;
            y = map->doors[d].y;
            n = 32;
            break;
        case 'd':
            x = map->doors[d].x;
            y = map->doors[d].y - 2;
            n = 32;
            break;
    }
    int i, j;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 4; j++) {
            int x2, y2;
            if (n == 11) {
                x2 = x + i;
                y2 = y + j;
            } else {
                x2 = x + j;
                y2 = y + i;
            }
            int tile_idx = n + i * 4 + j;
            const uint8_t *tile_data = (const uint8_t *)tiles[tile_idx].s;
            blit_sprite_8(x2 * 8 - cam->x, y2 * 8 - cam->y, 8,
                          tile_data,          /* plane 0 */
                          tile_data + 8,      /* plane 1 */
                          tile_data + 16);    /* mask */
        }
    }
}

/* drawHud is defined in hud.c */

#if 0 /* REMOVED — drawHud is in hud.c */
/*
 * drawHud - Draw the heads-up display (energy, tanks, weapons, ammo).
 * Ported from original HUD.c drawHud().
 */
void drawHud(void) {
    int x, y = 0, i;

    if (cam->hud > 1) {
        y = 1 - cam->hud;
        cam->hud--;
    } else if (cam->hud < 1) {
        y = -10 - cam->hud;
        if (++cam->hud > 0)
            cam->hud = 0;
    }

    #define HUD_LEFT 32
    #define HUD_RIGHT (LCD_WIDTH - 32 - HUD_LEFT)

    /* Draw HUD background panels */
    blit_sprite_32_tranw(HUD_LEFT, y, 12,
                         gfx->HUD.left_C0, gfx->HUD.left_C1);
    blit_sprite_32_tranw(HUD_LEFT + 32, y, 12,
                         gfx->HUD.middle_C0, gfx->HUD.middle_C1);
    blit_sprite_32_tranw(HUD_RIGHT, y, 12,
                         gfx->HUD.right_C0, gfx->HUD.right_C1);

    /* Draw energy tanks */
    for (i = 0; i < samus->items.tanks_owned; i++) {
        int tx = HUD_LEFT + 4 + (i % 6) * 5;
        int ty = (i < 6) ? y + 1 : y + 5;
        if (i < samus->items.tanks_full)
            blit_sprite_8(tx, ty, 3,
                          gfx->HUD.tankFull_C0, gfx->HUD.tankFull_C1, NULL);
        else
            blit_sprite_8(tx, ty, 3,
                          gfx->HUD.tankEmpty_C0, gfx->HUD.tankEmpty_C1, NULL);
    }

    /* Draw HP digits */
    x = HUD_LEFT + 34;
    blit_sprite_8(x, y + 1, 7,
                  gfx->HUD.numbers[samus->HP / 10].C0,
                  gfx->HUD.numbers[samus->HP / 10].C1, NULL);
    blit_sprite_8(x + 7, y + 1, 7,
                  gfx->HUD.numbers[samus->HP % 10].C0,
                  gfx->HUD.numbers[samus->HP % 10].C1, NULL);

    /* Draw weapon icons */
    x = HUD_LEFT + 50;
    blit_sprite_32_tranw(x, y + 1, 8,
                         gfx->HUD.weapons_C0, gfx->HUD.weapons_C1);

    /* Highlight selected weapon and show ammo */
    int ammo = 0;
    if (samus->weapons.gun == &templates->weapons.missile) {
        render_invert_rect(x, y + 1, x + 7, y + 8);
        ammo = samus->weapons.missile_ammo;
    } else if (samus->weapons.gun == &templates->weapons.superMissile) {
        render_invert_rect(x + 9, y + 1, x + 16, y + 8);
        ammo = samus->weapons.superMissile_ammo;
    } else if (samus->weapons.bomb == &templates->weapons.powerBomb) {
        render_invert_rect(x + 18, y + 1, x + 26, y + 8);
        ammo = samus->weapons.powerBomb_ammo;
    } else {
        return;
    }

    /* Draw ammo digits */
    x = HUD_LEFT + 78;
    blit_sprite_8(x, y + 1, 7,
                  gfx->HUD.numbers[ammo / 10].C0,
                  gfx->HUD.numbers[ammo / 10].C1, NULL);
    blit_sprite_8(x + 7, y + 1, 7,
                  gfx->HUD.numbers[ammo % 10].C0,
                  gfx->HUD.numbers[ammo % 10].C1, NULL);

    #undef HUD_LEFT
    #undef HUD_RIGHT
}
#endif /* REMOVED drawHud duplicate */

/*
 * drawScreen - Clear framebuffer and draw all layers, then present.
 * Ported from original Game Modes.c drawScreen().
 */
void drawScreen(void) {
    render_clear();
    drawMap();
    drawSamus();
    drawEnemies();
    drawObjects();
    drawHud();
    render_present();
}

/*
 * playingMode - Main game loop body. Returns 1 to continue, 0 to quit.
 * Ported from original Game Modes.c playingMode().
 */
int playingMode(void) {
    /* Check for ESC / quit */
    const uint8_t *state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE])
        return 0;

    if (checkDoors())
        return 1;

    /* Toggle HUD visibility */
    if (state[SDL_SCANCODE_TAB]) {
        if (cam->hud == 0)
            cam->hud = 11;
        else if (cam->hud == 1)
            cam->hud = -10;
    }

    getKeys();
    drawScreen();

    /* Update invincibility timer */
    if (samus->invincible > 0)
        samus->invincible--;

    /* Advance frame counter */
    if (++game->frame > 100)
        game->frame = 0;

    /* Animate samus sprite */
    if ((game->frame % samus->frames) == 0)
        samus->sprt = samus->sprt->link;

    weaponsSetup();
    samus->mode();
    adjustCamera();
    doEnemies();
    doObjects();

    return 1;
}

/*
 * mapTransition - Smooth camera pan to new room.
 * Ported from original Game Modes.c mapTransition().
 */
int mapTransition(void) {
    int x = 0, y = 0, d = game->frame;

    switch (map->doors[d].dir) {
        case 'l':
            x = map->w * 8 - LCD_WIDTH;
            y = map->doors[d].y * 8 + 16 - LCD_HEIGHT / 2;
            break;
        case 'r':
            x = 0;
            y = map->doors[d].y * 8 + 16 - LCD_HEIGHT / 2;
            break;
        case 'u':
            x = map->doors[d].x * 8 + 16 - LCD_WIDTH / 2;
            y = map->h * 8 - LCD_HEIGHT;
            break;
        case 'd':
            x = map->doors[d].x * 8 + 16 - LCD_WIDTH / 2;
            y = 0;
            break;
    }

    /* Clamp target camera position */
    if (y < 0)
        y = 0;
    else if ((int)y >= (int)(map->h * 8 - LCD_HEIGHT))
        y = map->h * 8 - LCD_HEIGHT;
    if (x < 0)
        x = 0;
    else if ((int)x >= (int)(map->w * 8 - LCD_WIDTH))
        x = map->w * 8 - LCD_WIDTH;

    if (cam->x == x && cam->y == y) {
        /* Transition complete */
        game->mode = playingMode;
        game->frame = 0;
        return 1;
    } else {
        /* Draw transition frame */
        render_clear();
        /* Fill screen with black */
        render_fill_rect(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1, COLOR_BLACK);
        drawDoors();
        render_present();

        /* Pan camera toward target.
         * Original ran in a tight loop with 15ms delays; we run once per
         * game frame (50ms), so move 8px per step to compensate. */
        int speed = 8;
        if (cam->x < x) {
            cam->x += speed;
            if (cam->x > x)
                cam->x = x;
        } else if (cam->x > x) {
            cam->x -= speed;
            if (cam->x < x)
                cam->x = x;
        }
        if (cam->y < y) {
            cam->y += speed;
            if (cam->y > y)
                cam->y = y;
        } else if (cam->y > y) {
            cam->y -= speed;
            if (cam->y < y)
                cam->y = y;
        }
    }
    return 1;
}
