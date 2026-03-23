/*
 * objects.c - Object initialization, templates, and management
 * Ported from original TIGCC Objects.c and object-related parts of Game Modes.c
 *
 * Behavior functions are in their respective files:
 *   weapons.c: basicShot, missileShot, bombShot
 *   sfx.c: bombExplodeFX, crumblingFX, simpleEnemyExplosionFX, spawnPickup
 *   pickups.c: pickup, energyPickup, missilePickup, superMissilePickup, powerBombPickup
 *   items.c: missileItem, superMissileItem, powerBombItem, energyTankItem
 */
#include <stdlib.h>
#include <string.h>

#include "objects.h"
#include "enemies.h"
#include "game.h"
#include "render.h"
#include "sprites.h"
#include "tiles.h"
#include "samus.h"
#include "map_data.h"
#include "weapons.h"
#include "sfx.h"
#include "pickups.h"
#include "items.h"

void makeObjects(void) {
    struct templates t = {
        {   /* ===== WEAPONS ===== */
            {   /* powerBeam */
                0, 0,                               /* x, y */
                4, 4,                               /* h, w */
                5, 0,                               /* vel_x, vel_y */
                10, 0,                              /* ATK, timer */
                0, 0, 0,                            /* e1, e2, e3 */
                1,                                  /* type */
                1, 0,                               /* frames, gone */
                &sprites->weapons.powerBeam,        /* sprt */
                basicShot                           /* behavior */
            },
            {   /* missile */
                0, 0,
                8, 8,
                2, 0,
                100, 0,
                0, 0, 0,
                2,
                1, 0,
                &sprites->weapons.missile1,
                missileShot
            },
            {   /* superMissile */
                0, 0,
                8, 8,
                2, 0,
                500, 0,
                0, 0, 0,
                3,
                1, 0,
                &sprites->weapons.missile1,
                missileShot
            },
            {   /* bomb */
                0, 0,
                4, 4,
                0, 0,
                50, 20,
                0, 0, 0,
                2,
                1, 0,
                &sprites->weapons.powerBeam,
                bombShot
            },
            {   /* powerBomb */
                0, 0,
                4, 4,
                0, 0,
                200, 20,
                0, 0, 0,
                4,
                1, 0,
                &sprites->weapons.powerBeam,
                bombShot
            }
        },
        {   /* ===== EFFECTS ===== */
            {   /* bombExplosion */
                0, 0,
                0, 0,
                0, 0,
                0, 3,
                0, 0, 0,
                0,
                2, 0,
                &sprites->sfx.bombExplode,
                bombExplodeFX
            },
            {   /* crumble */
                0, 0,
                8, 8,
                0, 0,
                0, 15,
                0, 0, 0,
                0,
                2, 0,
                &sprites->sfx.breakAway,
                crumblingFX
            },
            {   /* simpleEnemyExplosion */
                0, 0,
                16, 16,
                0, 0,
                0, 8,
                0, 0, 0,
                0,
                3, 0,
                &sprites->sfx.simpleEnemyExplosion,
                simpleEnemyExplosionFX
            }
        },
        {   /* ===== PICKUPS ===== */
            {   /* energy */
                0, 0,
                8, 8,
                0, 0,
                25, 150,
                0, 0, 0,
                0,
                3, 0,
                &sprites->pickups.energy,
                energyPickup
            },
            {   /* missile */
                0, 0,
                8, 4,
                0, 0,
                2, 150,
                0, 0, 0,
                0,
                3, 0,
                &sprites->pickups.missile,
                missilePickup
            },
            {   /* superMissile */
                0, 0,
                8, 4,
                0, 0,
                2, 150,
                0, 0, 0,
                0,
                3, 0,
                &sprites->pickups.superMissile,
                superMissilePickup
            },
            {   /* powerBomb */
                0, 0,
                8, 4,
                0, 0,
                2, 150,
                0, 0, 0,
                0,
                3, 0,
                &sprites->pickups.powerBomb,
                powerBombPickup
            }
        },
        {   /* ===== ITEMS ===== */
            {   /* missile */
                0, 0,
                8, 8,
                0, 0,
                5, 0,
                0, 0, 0,
                0,
                1, 0,
                &sprites->items.missile,
                missileItem
            },
            {   /* superMissile */
                0, 0,
                8, 8,
                0, 0,
                5, 0,
                0, 0, 0,
                0,
                1, 0,
                &sprites->items.superMissile,
                superMissileItem
            },
            {   /* powerBomb */
                0, 0,
                8, 8,
                0, 0,
                5, 0,
                0, 0, 0,
                0,
                1, 0,
                &sprites->items.powerBomb,
                powerBombItem
            },
            {   /* energy tank */
                0, 0,
                8, 8,
                0, 0,
                100, 0,
                0, 0, 0,
                0,
                1, 0,
                &sprites->items.energy,
                energyTankItem
            }
        }
    };

    if ((templates = malloc(sizeof(t))) == NULL)
        exit(1);
    memcpy(templates, &t, sizeof(t));

    if ((objects = malloc(sizeof(struct objlist))) == NULL)
        exit(1);
    objects->first = NULL;
}

void drawObject(struct object* obj) {
    if (obj->sprt->w > 8) {
        blit_sprite_16(obj->x - cam->x, obj->y - cam->y - obj->sprt->h,
                       obj->sprt->h,
                       (const uint16_t *)obj->sprt->s1,
                       (const uint16_t *)obj->sprt->s2,
                       (const uint16_t *)obj->sprt->m);
    } else {
        if (obj->sprt->m == NULL) {
            blit_sprite_8_or(obj->x - cam->x, obj->y - cam->y - obj->sprt->h,
                             obj->sprt->h,
                             (const uint8_t *)obj->sprt->s1,
                             (const uint8_t *)obj->sprt->s2);
        } else {
            blit_sprite_8(obj->x - cam->x, obj->y - cam->y - obj->sprt->h,
                          obj->sprt->h,
                          (const uint8_t *)obj->sprt->s1,
                          (const uint8_t *)obj->sprt->s2,
                          (const uint8_t *)obj->sprt->m);
        }
    }
}

void pushObject(int x, int y, int vel_x, int vel_y, struct object* tmpl) {
    struct object* obj = malloc(sizeof(struct object));
    if (!obj) return;
    memcpy(obj, tmpl, sizeof(struct object));
    obj->x = x;
    obj->y = y;
    obj->vel_x = vel_x;
    obj->vel_y = vel_y;

    struct objnode* node = malloc(sizeof(struct objnode));
    if (!node) { free(obj); return; }
    node->value = obj;
    node->link = objects->first;
    objects->first = node;
}

void drawObjects(void) {
    struct objnode* curr;
    for (curr = objects->first; curr != NULL; curr = curr->link)
        drawObject(curr->value);
}

void doObjects(void) {
    struct objnode* curr = objects->first;
    struct objnode* prev = NULL;
    while (curr != NULL) {
        /* Animate sprite */
        if (game->frame % curr->value->frames == 0)
            curr->value->sprt = curr->value->sprt->link;

        /* Check out of bounds */
        if (curr->value->x < map->l * 8 - 16 ||
            curr->value->x > map->r * 8 + 16 ||
            curr->value->y < map->t * 8 - 16 ||
            curr->value->y > map->b * 8 + 16)
            curr->value->gone = 1;

        if (curr->value->gone) {
            if (curr == objects->first) {
                objects->first = curr->link;
                free(curr->value);
                free(curr);
                curr = objects->first;
            } else {
                prev->link = curr->link;
                free(curr->value);
                free(curr);
                curr = prev->link;
            }
        } else {
            curr->value->behavior(curr->value);
            prev = curr;
            curr = curr->link;
        }
    }
}

void freeObjects(void) {
    if (!objects) return;
    struct objnode* curr = objects->first;
    while (curr != NULL) {
        struct objnode* next = curr->link;
        free(curr->value);
        free(curr);
        curr = next;
    }
    objects->first = NULL;
}
