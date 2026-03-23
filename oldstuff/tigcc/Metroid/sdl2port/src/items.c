/*
 * items.c - Major item pickup behaviors for Metroid TI-89 SDL2 port
 * Ported from original TIGCC Items.c
 *
 * Major items are one-time pickups placed in the world (missile expansions,
 * super missile packs, power bomb packs, energy tanks).
 *
 * The original blocked the game loop with textBox() + while(!_keytest(RR_ENTER)).
 * This port uses a non-blocking timed text overlay instead.
 */
#include "items.h"
#include "objects.h"
#include "samus.h"
#include "textbox.h"
#include "game.h"

/*
 * missileItem - Missile expansion item.
 * When samus touches it: grant 5 missiles (both ammo and max capacity),
 * show "MISSILES" text overlay, and remove the item.
 */
void missileItem(struct object* obj) {
    if (hitSamus(obj->x, obj->y, obj->w, obj->h)) {
        textBox("MISSILES");
        obj->gone = 1;
        samus->weapons.missile_ammo += 5;
        samus->weapons.missile_max += 5;
    }
}

/*
 * superMissileItem - Super missile expansion item.
 * When samus touches it: grant 5 super missiles (both ammo and max),
 * show "SUPER MISSILES" text overlay, and remove the item.
 */
void superMissileItem(struct object* obj) {
    if (hitSamus(obj->x, obj->y, obj->w, obj->h)) {
        textBox("SUPER MISSILES");
        obj->gone = 1;
        samus->weapons.superMissile_ammo += 5;
        samus->weapons.superMissile_max += 5;
    }
}

/*
 * powerBombItem - Power bomb expansion item.
 * When samus touches it: grant 5 power bombs (both ammo and max),
 * show "POWERBOMBS" text overlay, and remove the item.
 */
void powerBombItem(struct object* obj) {
    if (hitSamus(obj->x, obj->y, obj->w, obj->h)) {
        textBox("POWERBOMBS");
        obj->gone = 1;
        samus->weapons.powerBomb_ammo += 5;
        samus->weapons.powerBomb_max += 5;
    }
}

/*
 * energyTankItem - Energy tank item.
 * When samus touches it: grant 1 energy tank, fill all tanks,
 * set HP to 99 (full heal), show "ENERGY TANK" text overlay, and remove.
 */
void energyTankItem(struct object* obj) {
    if (hitSamus(obj->x, obj->y, obj->w, obj->h)) {
        textBox("ENERGY TANK");
        obj->gone = 1;
        samus->items.tanks_owned++;
        samus->items.tanks_full = samus->items.tanks_owned;
        samus->HP = 99;
    }
}
