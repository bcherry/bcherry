/*
 * pickups.c - Pickup item behaviors for Metroid TI-89 SDL2 port
 * Ported from original TIGCC pickups.c
 *
 * Pickups are small items dropped by enemies (energy, missiles, etc.).
 * They have a limited lifetime (timer) and are collected on contact with samus.
 */
#include <stdlib.h>

#include "pickups.h"
#include "objects.h"
#include "samus.h"
#include "game.h"

/*
 * pickup - Base pickup logic shared by all pickup types.
 *
 * Decrements the timer. If expired, removes the pickup and returns 0.
 * If samus touches the pickup, removes it and returns 1 (collected).
 * Otherwise returns 0 (still alive, not collected).
 */
int pickup(struct object* obj) {
    if (obj->timer-- < 0) {
        obj->gone = 1;
        return 0;
    } else {
        if (hitSamus(obj->x, obj->y, obj->w, obj->h)) {
            obj->gone = 1;
            return 1;
        }
    }
    return 0;
}

/*
 * energyPickup - Heal samus by a random amount (10-30 HP).
 *
 * Energy tank overflow logic: if HP exceeds 99, fill the next
 * energy tank and subtract 100, repeating until either HP <= 99
 * or all owned tanks are full. Cap final HP at 99.
 */
void energyPickup(struct object* obj) {
    if (pickup(obj)) {
        samus->HP += (rand() % 21) + 10;  /* random 10-30 */
        while (samus->HP > 99 && samus->items.tanks_full < samus->items.tanks_owned) {
            samus->HP -= 100;
            samus->items.tanks_full++;
        }
        if (samus->HP > 99)
            samus->HP = 99;
    }
}

/*
 * missilePickup - Add missiles (amount stored in obj->ATK).
 * Capped at the player's maximum missile capacity.
 */
void missilePickup(struct object* obj) {
    if (pickup(obj)) {
        samus->weapons.missile_ammo += obj->ATK;
        if (samus->weapons.missile_ammo > samus->weapons.missile_max)
            samus->weapons.missile_ammo = samus->weapons.missile_max;
    }
}

/*
 * superMissilePickup - Add super missiles (amount stored in obj->ATK).
 * Capped at the player's maximum super missile capacity.
 */
void superMissilePickup(struct object* obj) {
    if (pickup(obj)) {
        samus->weapons.superMissile_ammo += obj->ATK;
        if (samus->weapons.superMissile_ammo > samus->weapons.superMissile_max)
            samus->weapons.superMissile_ammo = samus->weapons.superMissile_max;
    }
}

/*
 * powerBombPickup - Add power bombs (amount stored in obj->ATK).
 * Capped at the player's maximum power bomb capacity.
 */
void powerBombPickup(struct object* obj) {
    if (pickup(obj)) {
        samus->weapons.powerBomb_ammo += obj->ATK;
        if (samus->weapons.powerBomb_ammo > samus->weapons.powerBomb_max)
            samus->weapons.powerBomb_ammo = samus->weapons.powerBomb_max;
    }
}
