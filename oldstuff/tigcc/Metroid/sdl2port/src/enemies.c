/*
 * enemies.c - Enemy initialization, AI, and management
 * Ported from original TIGCC Enemies.c and enemy-related parts of Game Modes.c
 */
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "enemies.h"
#include "objects.h"
#include "game.h"
#include "render.h"
#include "sprites.h"
#include "tiles.h"
#include "samus.h"
#include "map_data.h"

/* Helper macro for tile lookup using original map indirection */
#define TILE_AT(c, r) (map->array[1 - map->array[(r) * map->w] + (r) * map->w + (c)])

/*
 * hurtEnemy - Deal damage to an enemy. Respects immunity levels.
 * Ported from original Enemies.c hurtEnemy().
 */
void hurtEnemy(struct enemy* e, int ATK, int type) {
    if (type < e->immunity || e->immunity < 0)
        return;
    e->HP -= ATK;
    e->hurt = 4;
    if (e->HP <= 0) {
        e->dead = 1;
        if (e->onDeath != NULL)
            e->onDeath(e);
    }
}

/*
 * getPickup - Roll random drop based on enemy's drop rates.
 * Ported from original Enemies.c getPickup().
 */
int getPickup(struct enemy* e) {
    int p = rand() % 100;
    if (p < e->E)
        return 'e';
    else if ((p += e->E) < e->M)
        return 'm';
    else if ((p += e->M) < e->SM)
        return 's';
    else if ((p += e->SM) < e->PB)
        return 'p';
    return 0;
}

/*
 * skreeDeath - Skree death handler: spawn 4 projectiles.
 * Ported from original Enemies.c skreeDeath().
 */
void skreeDeath(struct enemy* e) {
    enemyKey[3].vel_y = -4;
    enemyKey[3].vel_x = -1;
    pushEnemy(e->x + 2, e->y, 3);
    enemyKey[3].vel_x = 1;
    pushEnemy(e->x + 2, e->y, 3);
    enemyKey[3].vel_x = -2;
    enemyKey[3].vel_y = -2;
    pushEnemy(e->x + 2, e->y, 3);
    enemyKey[3].vel_x = 2;
    pushEnemy(e->x + 2, e->y, 3);
    simpleDeath(e);
}

/*
 * simpleDeath - Default death handler: spawn explosion effect with pickup type.
 * Ported from original Enemies.c simpleDeath().
 */
void simpleDeath(struct enemy* e) {
    if (e->dead < 0)
        return;
    pushObject(e->x + e->w / 2 - 8, e->y - e->h / 2 + 8, 0, 0,
               &templates->effects.simpleEnemyExplosion);
    objects->first->value->type = getPickup(e);
}

/*
 * zebboAI - Zebbo flight behavior.
 * Ported from original Enemies.c zebboAI().
 */
void zebboAI(struct enemy* e) {
    if (e->vel_x < 0) {
        e->sprt = &sprites->enemies.zebboL;
    }
    if (e->extra1 > 0) {
        e->y--;
        e->extra1--;
    } else {
        e->x += e->vel_x * 2;
    }
}

/*
 * zebboSpawnAI - Zebbo spawner: periodically creates zebbos when samus is near.
 * Ported from original Enemies.c zebboSpawnAI().
 */
void zebboSpawnAI(struct enemy* e) {
    map->array[1 - map->array[((e->y - 8) / 8) * map->w] +
               ((e->y - 8) / 8) * map->w + e->x / 8] = 44;
    if (abs(samus->x - e->x) < 100 && e->y - samus->y < 30) {
        e->extra1++;
        if ((e->extra1 %= 30) == 0) {
            if (samus->x < e->x)
                enemyKey[4].vel_x = -1;
            else
                enemyKey[4].vel_x = 1;
            enemyKey[4].extra1 = e->y - samus->y + 8;
            pushEnemy(e->x + 4, e->y, 4);
        }
    }
}

/*
 * spacePirateLaserAI - Space pirate laser projectile movement.
 * Ported from original Enemies.c spacePirateLaserAI().
 */
void spacePirateLaserAI(struct enemy* e) {
    int i;
    for (i = 0; i < 3; i++) {
        e->x += e->vel_x;
        if (hitWallX(e->x + 8 + 8 * e->vel_x, e->y - 1, e->y - 3, 0, 0))
            e->dead = -1;
    }
}

/*
 * spacePirateAI - Full space pirate behavior: walk, shoot, turn.
 * Ported from original Enemies.c spacePirateAI().
 */
void spacePirateAI(struct enemy* e) {
    e->extra1--;

    /* Check if samus is at shooting height */
    if ((e->y - enemyKey[7].h) - samus->y < -6 &&
        e->y - (samus->y - samus->sprt->h) > 6) {
        if (e->sprt == &sprites->enemies.SpacePirateShootRight) {
            if (!(rand() % 15)) {
                enemyKey[8].vel_x = 1;
                pushEnemy(e->x + 8, e->y - 7 - (rand() % 6), 8);
            }
        } else if (e->sprt == &sprites->enemies.SpacePirateShootLeft) {
            if (!(rand() % 15)) {
                enemyKey[8].vel_x = -1;
                pushEnemy(e->x - 8, e->y - 7 - (rand() % 6), 8);
            }
        } else if (e->vel_x > 0 && samus->x - e->x > 0) {
            e->sprt = &sprites->enemies.SpacePirateShootRight;
            return;
        } else if (e->vel_x < 0 && samus->x - e->x < 0) {
            e->sprt = &sprites->enemies.SpacePirateShootLeft;
            return;
        }
    }

    if (e->extra1 == 0) {
        if (e->sprt == &sprites->enemies.SpacePirateStandRight) {
            e->sprt = &sprites->enemies.SpacePirateWalk1Left;
            e->vel_x = -1;
        } else {
            e->sprt = &sprites->enemies.SpacePirateWalk1Right;
            e->vel_x = 1;
        }
    } else if (e->extra1 < 0 &&
               e->sprt != &sprites->enemies.SpacePirateShootRight &&
               e->sprt != &sprites->enemies.SpacePirateShootLeft) {
        if (e->sprt == &sprites->enemies.SpacePirateStandRight)
            e->sprt = &sprites->enemies.SpacePirateWalk1Right;
        else if (e->sprt == &sprites->enemies.SpacePirateStandLeft)
            e->sprt = &sprites->enemies.SpacePirateWalk1Left;

        int i;
        for (i = 0; i < enemyKey[7].vel_x; i++) {
            if (hitWallX(e->x + 8 + (e->vel_x * 4), e->y - 7, e->y - enemyKey[7].h, 0, 0) ||
                !hitWallY(e->y + 3, (e->x + 8 + (e->vel_x * 4)),
                          (e->x + 8 + (e->vel_x * 4)), 0, 0)) {
                if (e->vel_x < 0)
                    e->sprt = &sprites->enemies.SpacePirateStandLeft;
                else
                    e->sprt = &sprites->enemies.SpacePirateStandRight;
                e->extra1 = enemyKey[7].extra2;
                return;
            } else {
                e->x += e->vel_x;
                /* Slope following logic */
                int tx = (e->x + 8) / 8;
                int ty_top = (e->y - 1) / 8;
                int ty_bot = (e->y + 7) / 8;
                int row_off_top = map->array[ty_top * map->w];
                int row_off_bot = map->array[ty_bot * map->w];
                int tidx_top = map->array[1 - row_off_top + ty_top * map->w + tx];
                int tidx_bot = map->array[1 - row_off_bot + ty_bot * map->w + tx];

                if (tiles[tidx_top].m != NULL)
                    e->y = tiles[tidx_top].m[0 + (e->x + 8) % 8] + ty_top * 8;
                else if (tiles[tidx_bot].m != NULL)
                    e->y = tiles[tidx_bot].m[0 + (e->x + 8) % 8] + ty_bot * 8;
            }
        }
    } else if (e->extra1 == enemyKey[7].extra3) {
        if (e->sprt == &sprites->enemies.SpacePirateStandRight)
            e->sprt = &sprites->enemies.SpacePirateLookRight;
        else
            e->sprt = &sprites->enemies.SpacePirateLookLeft;
    }
}

/*
 * ripperAI - Ripper horizontal patrol with wall bounce.
 * Ported from original Enemies.c ripperAI().
 */
void ripperAI(struct enemy* e) {
    e->x += e->vel_x;
    if (e->vel_x < 0) {
        if (hitWallX(e->x, e->y - 4, e->y - 4, 0, 'r')) {
            int y = e->y - 4;
            int x = e->x;
            int row = y / 8;
            int col = x / 8;
            int row_off = map->array[row * map->w];
            int tidx = map->array[1 - row_off + row * map->w + col];
            if (tiles[tidx].m != NULL)
                e->x = tiles[tidx].m[16 + y % 8] + col * 8;
            e->vel_x *= -1;
            e->sprt = &sprites->enemies.ripper;
        }
    } else if (e->vel_x > 0) {
        if (hitWallX(e->x + 16, e->y - 4, e->y - 4, 0, 'l')) {
            int y = e->y - 4;
            int x = e->x + 16;
            int row = y / 8;
            int col = x / 8;
            int row_off = map->array[row * map->w];
            int tidx = map->array[1 - row_off + row * map->w + col];
            if (tiles[tidx].m != NULL)
                e->x = tiles[tidx].m[24 + y % 8] + col * 8 - 16;
            e->vel_x *= -1;
            e->sprt = &sprites->enemies.ripper2;
        }
    }
}

/*
 * waverAI - Waver sinusoidal movement.
 * Ported from original Enemies.c waverAI().
 */
void waverAI(struct enemy* e) {
    e->x += e->vel_x;
    e->vel_y += e->extra1;
    if (e->vel_y > 9 || e->vel_y < -9 ||
        hitWallY(e->y, e->x, e->x + 16, 0, 't') ||
        hitWallY(e->y - 8, e->x, e->x + 16, 0, 'b')) {
        e->extra1 *= -1;
    }
    e->y += e->extra1;
    int x = e->x;
    if (e->vel_x > 0)
        x += 16;
    if (hitWallX(x, e->y, e->y - 8, 0, 'r'))
        e->vel_x *= -1;
}

/*
 * sideHopperAI - Side hopper jumping/bouncing behavior.
 * Ported from original Enemies.c sideHopperAI().
 */
void sideHopperAI(struct enemy* e) {
    if (e->extra1 == 0) {
        if ((rand() % 3) == 2) {
            e->extra1 = 1;
            e->vel_y = -5;
            e->vel_x = 3;
            if ((rand() % 2) == 1)
                e->vel_x = -3;
        }
    } else {
        int backupX = e->x;
        int backupY = e->y;
        e->x += e->vel_x;
        e->y += e->vel_y;
        if (++e->vel_y > 5)
            e->vel_y = 5;
        if (e->vel_x) {
            int x = e->x;
            if (e->vel_x > 0)
                x = e->x + 16;
            if (hitWallX(x, e->y, e->y - 12, 0, 'l')) {
                e->x = backupX;
                e->vel_x = 0;
            }
        }
        if (hitWallY(e->y - 12, e->x, e->x + 16, 0, 'b')) {
            e->y = backupY;
            e->vel_y = 1;
        } else if (hitWallY(e->y, e->x, e->x + 16, 0, 't')) {
            int x = e->x + 8;
            int row = (e->y) / 8;
            int row_off = map->array[row * map->w];
            if (tiles[map->array[1 - row_off + row * map->w + x / 8]].m == NULL)
                x = e->x + 1;
            row_off = map->array[row * map->w];
            if (tiles[map->array[1 - row_off + row * map->w + x / 8]].m == NULL)
                x = e->x + 15;
            row_off = map->array[row * map->w];
            int tidx = map->array[1 - row_off + row * map->w + x / 8];
            if (tiles[tidx].m != NULL)
                e->y = tiles[tidx].m[0 + x % 8] + row * 8;
            e->extra1 = 0;
        }
    }
}

/*
 * skreeAI - Skree dive attack when samus is below.
 * Ported from original Enemies.c skreeAI().
 */
void skreeAI(struct enemy* e) {
    if (e->vel_y == 0 && e->vel_x == 0) {
        if (samus->y > e->y && samus->y - e->y < 64) {
            if (e->x - samus->x < 20 && samus->x - e->x < 4) {
                e->sprt = &sprites->enemies.skree2;
                e->vel_y = 8;
                e->vel_x = 2;
                if (samus->x < e->x)
                    e->vel_x = -2;
            }
        }
    } else {
        e->y += e->vel_y;
        e->x += e->vel_x;
        if (hitWallY(e->y, e->x + 2, e->x + 6, 0, 't')) {
            e->dead = -1;
            skreeDeath(e);
        }
    }
}

/*
 * projectileAI - Skree projectile with gravity.
 * Ported from original Enemies.c projectileAI().
 */
void projectileAI(struct enemy* e) {
    e->x += e->vel_x;
    e->y += e->vel_y;
    if (++e->vel_y > 8)
        e->vel_y = 8;
}

/*
 * drawEnemy - Render enemy sprite at screen position.
 * Ported from original Enemies.c drawEnemy().
 * In the original, when hurt the sprite was drawn to a different plane for a flash effect.
 * Here we use OR mode for normal, and we could invert or skip plane for hurt flash.
 */
void drawEnemy(struct enemy* e) {
    /* Simple hurt flash: skip rendering every other frame when hurt */
    if (e->hurt) {
        e->hurt--;
        if (e->hurt % 2)
            return;  /* Flash by skipping render on odd hurt frames */
    }
    if (e->w > 8) {
        blit_sprite_16(e->x - cam->x, e->y - cam->y - e->sprt->h,
                       e->sprt->h,
                       (const uint16_t *)e->sprt->s1,
                       (const uint16_t *)e->sprt->s2,
                       NULL);
    } else {
        blit_sprite_8_or(e->x - cam->x, e->y - cam->y - e->sprt->h,
                         e->sprt->h,
                         (const uint8_t *)e->sprt->s1,
                         (const uint8_t *)e->sprt->s2);
    }
}

/*
 * makeEnemies - Initialize enemy template array and enemy linked list.
 * Ported from original Enemies.c makeEnemies().
 * enemyKey[0..8] are the 9 enemy templates.
 */
void makeEnemies(void) {
    struct enemy e[] = {
        {   /* 0: Waver (200) */
            0, 0,                               /* x, y */
            8, 16,                              /* h, w */
            4, 0,                               /* vel_x, vel_y */
            1, 0, 0,                            /* extra1, extra2, extra3 */
            30, 10, 48, 24, 24, 3,              /* HP, ATK, E, M, SM, PB */
            0, 0,                               /* dead, hurt */
            1,                                  /* frames */
            &sprites->enemies.waver,            /* sprt */
            0,                                  /* immunity */
            waverAI,                            /* behavior */
            simpleDeath                         /* onDeath */
        },
        {   /* 1: Side Hopper (201) */
            0, 0,
            12, 16,
            0, 0,
            0, 0, 0,
            320, 80, 16, 22, 22, 2,
            0, 0,
            1,
            &sprites->enemies.sideHopper,
            0,
            sideHopperAI,
            simpleDeath
        },
        {   /* 2: Skree (202) */
            0, 0,
            11, 8,
            0, 0,
            0, 0, 0,
            15, 10, 9, 33, 2, 2,
            0, 0,
            1,
            &sprites->enemies.skree,
            0,
            skreeAI,
            skreeDeath
        },
        {   /* 3: Skree Projectile (203) */
            0, 0,
            4, 4,
            -3, -5,
            0, 0, 0,
            -1, 10, 9, 33, 2, 2,
            0, 0,
            1,
            &sprites->enemies.skreeProjectile,
            -1,
            projectileAI,
            NULL
        },
        {   /* 4: Zebbo (204) */
            0, 0,
            7, 8,
            1, -1,
            16, 0, 0,
            30, 20, 55, 4, 29, 2,
            0, 0,
            1,
            &sprites->enemies.zebboR,
            0,
            zebboAI,
            simpleDeath
        },
        {   /* 5: Zebbo Spawn (205) */
            0, 0,
            8, 8,
            0, 0,
            0, 0, 0,
            -1, 0, 0, 0, 0, 0,
            0, 0,
            1,
            &sprites->enemies.zebboSpawn,
            -1,
            zebboSpawnAI,
            NULL
        },
        {   /* 6: Ripper (206) */
            0, 0,
            8, 16,
            1, 0,
            0, 0, 0,
            200, 5, 39, 31, 2, 2,
            0, 0,
            1,
            &sprites->enemies.ripper,
            2,
            ripperAI,
            simpleDeath
        },
        {   /* 7: Space Pirate (207) */
            0, 0,
            19, 16,
            2, 0,
            1, 12, 9,
            20, 15, 67, 31, 2, 0,
            0, 0,
            6,
            &sprites->enemies.SpacePirateStandRight,
            2,
            spacePirateAI,
            simpleDeath
        },
        {   /* 8: Space Pirate Laser (208) */
            0, 0,
            3, 16,
            3, 0,
            0, 0, 0,
            -1, 20, 0, 0, 0, 0,
            0, 0,
            1,
            &sprites->enemies.SpacePirateLaser,
            -1,
            spacePirateLaserAI,
            NULL
        }
    };

    if ((enemyKey = malloc(sizeof(e))) == NULL)
        exit(1);
    memcpy(enemyKey, e, sizeof(e));

    if ((enemies = malloc(sizeof(struct enemyList))) == NULL)
        exit(1);
    enemies->first = NULL;
}

/*
 * pushEnemy - Clone a template enemy at given position and add to linked list.
 * Ported from original Enemies.c pushEnemy().
 */
void pushEnemy(int x, int y, int key) {
    struct enemy* e = malloc(sizeof(struct enemy));
    if (!e) return;
    memcpy(e, enemyKey + key, sizeof(struct enemy));
    e->x = x;
    e->y = y;

    struct enemyNode* node = malloc(sizeof(struct enemyNode));
    if (!node) { free(e); return; }
    node->value = e;
    node->next = enemies->first;
    enemies->first = node;
}

/*
 * drawEnemies - Render all enemies in the list.
 * Ported from original Game Modes.c drawEnemies().
 */
void drawEnemies(void) {
    struct enemyNode* curr;
    for (curr = enemies->first; curr != NULL; curr = curr->next)
        drawEnemy(curr->value);
}

/*
 * doEnemies - Update all enemies: run AI, check collisions, remove dead.
 * Ported from original Game Modes.c doEnemies().
 */
void doEnemies(void) {
    struct enemyNode* curr = enemies->first;
    struct enemyNode* prev = NULL;
    while (curr != NULL) {
        if (curr->value->dead != 0) {
            /* Remove dead enemy from list */
            if (curr == enemies->first) {
                enemies->first = curr->next;
                free(curr->value);
                free(curr);
                curr = enemies->first;
            } else {
                prev->next = curr->next;
                free(curr->value);
                free(curr);
                curr = prev->next;
            }
        } else {
            /* Animate sprite */
            if (game->frame % curr->value->frames == 0)
                curr->value->sprt = curr->value->sprt->link;

            /* Run AI behavior */
            curr->value->behavior(curr->value);

            /* Check collision with samus */
            if (curr->value->ATK > 0) {
                if (hitSamus(curr->value->x, curr->value->y,
                             curr->value->w, curr->value->h)) {
                    hurtEnemy(curr->value, 15, 0);
                    hurtSamus(curr->value->ATK);
                }
            }

            /* Remove if out of bounds */
            if (curr->value->x < map->l * 8 - 16 ||
                curr->value->x > map->r * 8 + 16 ||
                curr->value->y < map->t * 8 - 16 ||
                curr->value->y > map->b * 8 + 16)
                curr->value->dead = -1;

            prev = curr;
            curr = curr->next;
        }
    }
}

/*
 * freeEnemies - Free all enemies in the linked list (but keep the list struct).
 */
void freeEnemies(void) {
    if (!enemies) return;
    struct enemyNode* curr = enemies->first;
    while (curr != NULL) {
        struct enemyNode* next = curr->next;
        free(curr->value);
        free(curr);
        curr = next;
    }
    enemies->first = NULL;
}
