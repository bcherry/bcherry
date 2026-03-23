/*
 * sfx.c - Special effect behaviors for Metroid TI-89 SDL2 port
 * Ported from original TIGCC sfx.c
 */
#include <stdlib.h>

#include "sfx.h"
#include "objects.h"
#include "map_data.h"
#include "samus.h"
#include "game.h"

/*
 * bombExplodeFX - Visual effect for bomb explosion.
 * Simply counts down the timer, then removes itself.
 * The sprite animation is handled by the object's sprite->link chain.
 */
void bombExplodeFX(struct object* obj) {
    obj->timer--;
    if (obj->timer < 0)
        obj->gone = 1;
}

/*
 * crumblingFX - Effect for crumbling tiles (e.g. breakaway blocks).
 *
 * When the timer expires AND samus is not standing on the tile,
 * restore the original tile in the map array and remove the effect.
 *
 * obj->e1 stores the original tile ID to restore.
 * obj->x/obj->y store the tile position (obj->y is one row below the tile row,
 * so the actual tile row is (obj->y - 8) / 8).
 *
 * The map indirection: map->array[1 - map->array[row*map->w] + row*map->w + col]
 * gives the actual index into the map data for the visible tile.
 */
void crumblingFX(struct object* obj) {
    if (--obj->timer < 0 && !hitSamus(obj->x + 4, obj->y, 8, 8)) {
        obj->gone = 1;
        int row = (obj->y - 8) / 8;
        int col = obj->x / 8;
        map->array[1 - map->array[row * map->w] + row * map->w + col] = obj->e1;
    }
}

/*
 * spawnPickup - Create a pickup object based on the dying enemy/effect's type.
 *
 * Type codes (from the enemy drop table):
 *   'e' = energy pickup
 *   'm' = missile pickup
 *   's' = super missile pickup
 *   'p' = power bomb pickup
 *   0   = no drop
 *
 * The pickup spawns centered on the source object.
 */
void spawnPickup(struct object* obj) {
    struct object* t = NULL;

    switch (obj->type) {
        case 0:
            return;
        case 'e':
            t = &templates->pickups.energy;
            break;
        case 'm':
            t = &templates->pickups.missile;
            break;
        case 's':
            t = &templates->pickups.superMissile;
            break;
        case 'p':
            t = &templates->pickups.powerBomb;
            break;
        default:
            return;
    }

    /* Center the pickup on the source object */
    pushObject(obj->x + obj->w / 2 - 4, obj->y - obj->h / 2 + 4, 0, 0, t);
}

/*
 * simpleEnemyExplosionFX - Visual effect for enemy death explosion.
 * Counts down timer, then spawns a pickup (if the enemy had one)
 * and removes itself.
 *
 * The obj->type field carries the pickup type code from the dead enemy.
 */
void simpleEnemyExplosionFX(struct object* obj) {
    if (--obj->timer < 0) {
        spawnPickup(obj);
        obj->gone = 1;
    }
}
