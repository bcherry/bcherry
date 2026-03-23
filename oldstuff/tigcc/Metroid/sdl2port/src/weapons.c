/*
 * weapons.c - Weapon behavior functions for Metroid TI-89 SDL2 port
 * Ported from original TIGCC Weapons.c
 */
#include <stdlib.h>

#include "weapons.h"
#include "render.h"
#include "map_data.h"
#include "tiles.h"
#include "sprites.h"
#include "samus.h"
#include "objects.h"
#include "enemies.h"
#include "game.h"

/* Map array indirection macro - same as original TI-89 code:
 * map->array[1 - map->array[r*map->w] + r*map->w + c]
 * The first element of each row stores a flag, and actual tile data
 * starts at offset 1 from the row start. */
#define TILE_AT(r, c) tiles[map->array[1 - map->array[(r) * map->w] + (r) * map->w + (c)]]

/*
 * basicShot - Move a projectile pixel by pixel along its velocity vector.
 * At each step, check for tile collision (via onShot) and enemy collision.
 * Remove the projectile if it hits something.
 *
 * Original: moves by incrementing moved_x/moved_y until both reach
 * abs(vel_x)/abs(vel_y). Each step moves one pixel in x and/or y direction.
 */
void basicShot(struct object* obj) {
    int moved_x = 0, moved_y = 0;

    while ((moved_x < abs(obj->vel_x) || moved_y < abs(obj->vel_y)) && !obj->gone) {
        if (++moved_x < abs(obj->vel_x)) {
            if (obj->vel_x < 0)
                obj->x--;
            else
                obj->x++;
        }
        if (++moved_y < abs(obj->vel_y)) {
            if (obj->vel_y < 0)
                obj->y--;
            else
                obj->y++;
        }

        /* Check tile collision: original uses (x+2)/8 for column, (y-2)/8 for row */
        {
            int col = (obj->x + 2) / 8;
            int row = (obj->y - 2) / 8;
            struct tile t = TILE_AT(row, col);
            if (t.m != NULL) {
                onShot(col * 8, row * 8, obj);
                obj->gone = 1;
            }
        }

        checkEnemies(obj);
    }
}

/*
 * missileShot - Select directional missile sprite based on velocity,
 * accelerate, then delegate movement to basicShot.
 *
 * 8 directional sprites:
 *   missile1: vel_x<0, vel_y<0 (up-left)
 *   missile2: vel_x>0, vel_y<0 (up-right)
 *   missile3: vel_x>0, vel_y>0 (down-right)
 *   missile4: vel_x<0, vel_y>0 (down-left)
 *   missile5: vel_x<0, vel_y==0 (left)
 *   missile6: vel_x>0, vel_y==0 (right)
 *   missile7: vel_x==0, vel_y<0 (up)
 *   missile8: vel_x==0, vel_y>0 (down)
 */
void missileShot(struct object* obj) {
    /* Select directional sprite */
    if (obj->vel_x < 0 && obj->vel_y < 0)
        obj->sprt = &sprites->weapons.missile1;
    else if (obj->vel_x > 0 && obj->vel_y < 0)
        obj->sprt = &sprites->weapons.missile2;
    else if (obj->vel_x > 0 && obj->vel_y > 0)
        obj->sprt = &sprites->weapons.missile3;
    else if (obj->vel_x < 0 && obj->vel_y > 0)
        obj->sprt = &sprites->weapons.missile4;
    else if (obj->vel_x < 0 && obj->vel_y == 0)
        obj->sprt = &sprites->weapons.missile5;
    else if (obj->vel_x > 0 && obj->vel_y == 0)
        obj->sprt = &sprites->weapons.missile6;
    else if (obj->vel_x == 0 && obj->vel_y < 0)
        obj->sprt = &sprites->weapons.missile7;
    else if (obj->vel_x == 0 && obj->vel_y > 0)
        obj->sprt = &sprites->weapons.missile8;

    /* Accelerate missile */
    if (obj->vel_x < 0)
        obj->vel_x--;
    else if (obj->vel_x > 0)
        obj->vel_x++;
    if (obj->vel_y < 0)
        obj->vel_y--;
    else if (obj->vel_y > 0)
        obj->vel_y++;

    basicShot(obj);
}

/*
 * bombShot - Bomb countdown timer. When it expires:
 * 1. If samus is overlapping the bomb, apply bomb-jump bounce
 * 2. Check a 3x3 grid of tiles around the bomb via onShot (skip corners)
 * 3. Spawn a bomb explosion visual effect
 * 4. Check for enemy hits
 * 5. Remove the bomb object
 */
void bombShot(struct object* obj) {
    if (--obj->timer < 0) {
        /* Check if samus overlaps and apply bomb jump */
        if (boundingBoxCollide(obj->x, obj->y, 4, 4, samus->x, samus->y, 8, 8, 1)) {
            /* Check if samus is in a left-facing morph mode */
            if (samus->mode == morphedLeft ||
                samus->mode == fallingBallLeft ||
                samus->mode == rollingLeft ||
                samus->mode == bouncingLeft ||
                samus->mode == spiderLeft ||
                samus->mode == spiderUp) {
                samus->mode = bouncingLeft;
                samus->vel_y = BOMB_BOUNCE;
                samus->vel_x = 0;
            }
            /* Check if samus is in a right-facing morph mode */
            else if (samus->mode == morphedRight ||
                     samus->mode == fallingBallRight ||
                     samus->mode == rollingRight ||
                     samus->mode == bouncingRight ||
                     samus->mode == spiderRight ||
                     samus->mode == spiderDown) {
                samus->mode = bouncingRight;
                samus->vel_y = BOMB_BOUNCE;
                samus->vel_x = 0;
            }
        }

        /* Check 3x3 grid of tiles around bomb, skipping corners.
         * Original iterates x from (this->x-3) to (this->x+6) step 3,
         * y from (this->y-7) to (this->y+2) step 3. */
        {
            int x, y;
            for (x = obj->x - 3; x <= obj->x + 6; x += 3) {
                for (y = obj->y - 7; y <= obj->y + 2; y += 3) {
                    /* Skip corners */
                    if ((x == obj->x - 3 && y == obj->y - 7) ||
                        (x == obj->x - 3 && y == obj->y + 2) ||
                        (x == obj->x + 6 && y == obj->y - 7) ||
                        (x == obj->x + 6 && y == obj->y + 2))
                        continue;
                    onShot((x / 8) * 8, (y / 8) * 8, obj);
                }
            }
        }

        /* Spawn bomb explosion visual effect */
        pushObject(obj->x - 2, obj->y + 2, 0, 0, &templates->effects.bombExplosion);

        /* Check for enemy hits */
        checkEnemies(obj);

        obj->gone = 1;
    }
}

/*
 * shoot - Called when the fire button is pressed/held.
 *
 * If fire is held and charge beam is equipped (and not using missiles),
 * charge up. Otherwise, calculate direction from aiming keys and fire.
 *
 * Direction logic:
 * - dx = facing direction (-1 or +1)
 * - dy from up/down keys
 * - aim button allows diagonal aiming without moving
 * - if aiming up/down and not pressing horizontal, dx becomes 0 (straight up/down)
 * - diagonal velocity uses 0.7 approximation
 *
 * After firing, add charge bonus to ATK, consume ammo, set cooldown.
 */
int shoot(int x, int y, int dir) {
    if (samus->weapons.wait)
        return 1;

    /* Charge beam logic: hold fire to charge (not for missiles) */
    if (game->keys.fire &&
        samus->weapons.charge_equipped &&
        samus->weapons.gun != &templates->weapons.superMissile &&
        samus->weapons.gun != &templates->weapons.missile) {
        if (++samus->weapons.charge > CHARGE_MAX)
            samus->weapons.charge = CHARGE_MAX;
        return 0;
    } else {
        int dx = dir;
        int dy = 0;

        /* Determine aim direction */
        if (game->keys.aim) {
            if (game->keys.down)
                dy = 1;
            if (game->keys.up)
                dy = -1;
            /* If aiming vertically and not pressing horizontal in facing direction,
             * zero out horizontal component for straight up/down shot */
            if ((game->keys.up || game->keys.down) &&
                ((dir < 0 && !game->keys.left) || (dir > 0 && !game->keys.right)))
                dx = 0;
        } else {
            if (game->keys.up) {
                dy = -1;
                if ((dir < 0 && !game->keys.left) || (dir > 0 && !game->keys.right))
                    dx = 0;
            }
            if (game->keys.down) {
                dy = 1;
                if ((dir < 0 && !game->keys.left) || (dir > 0 && !game->keys.right))
                    dx = 0;
            }
        }

        /* Calculate projectile velocity.
         * If purely horizontal or vertical, use full gun speed + samus velocity.
         * If diagonal, use 0.7 * gun speed + samus velocity. */
        int vel_x, vel_y;
        vel_x = (dy == 0)
            ? (samus->weapons.gun->vel_x + samus->vel_x) * dx
            : (int)(0.7 * samus->weapons.gun->vel_x + samus->vel_x) * dx;
        vel_y = (dx == 0)
            ? (samus->weapons.gun->vel_x + samus->vel_y) * dy
            : (int)(0.7 * samus->weapons.gun->vel_x + samus->vel_y) * dy;

        /* Spawn the projectile */
        pushObject(x, y, vel_x, vel_y, samus->weapons.gun);

        /* Consume ammo for missile types */
        if (samus->weapons.gun == &templates->weapons.missile)
            samus->weapons.missile_ammo--;
        else if (samus->weapons.gun == &templates->weapons.superMissile)
            samus->weapons.superMissile_ammo--;

        /* Add charge bonus to the newly created projectile's ATK.
         * objects->first is the most recently pushed object. */
        objects->first->value->ATK += samus->weapons.charge;

        /* If fully charged, increment the projectile type (for special effects) */
        if (samus->weapons.charge == CHARGE_MAX)
            objects->first->value->type++;

        samus->weapons.charge = 0;
        samus->weapons.wait = 10;
    }
    return 1;
}

/*
 * layBomb - Place a bomb at samus's position.
 * Only fires on the first frame the fire button is pressed (fire == 1).
 * Respects the weapon cooldown timer.
 * Power bombs consume ammo.
 */
void layBomb(void) {
    if (samus->weapons.wait)
        return;
    if (game->keys.fire == 1) {
        game->keys.fire = 2;
        pushObject(samus->x + 6, samus->y - 2, 0, 0, samus->weapons.bomb);
        if (samus->weapons.bomb == &templates->weapons.powerBomb)
            samus->weapons.powerBomb_ammo--;
        samus->weapons.wait = 10;
    }
}

/*
 * weaponsSetup - Cycle weapons when the weapon-switch key (F5/D) is pressed.
 *
 * Cycle order: beam -> missile -> super missile -> power bomb (bomb slot) -> beam
 * Skips weapons with 0 ammo.
 * Also auto-deselects weapons that run out of ammo.
 * Decrements the weapon cooldown timer each frame.
 */
void weaponsSetup(void) {
    if (game->keys.f5 == 1) {
        if (samus->weapons.bomb == &templates->weapons.powerBomb) {
            /* Currently on power bombs, cycle back to normal bombs */
            samus->weapons.bomb = &templates->weapons.bomb;
        } else if (samus->weapons.gun == samus->weapons.beam) {
            /* Currently on beam, try missile */
            samus->weapons.gun = &templates->weapons.missile;
            if (samus->weapons.missile_ammo == 0) {
                /* No missiles, try super missiles */
                samus->weapons.gun = &templates->weapons.superMissile;
                if (samus->weapons.superMissile_ammo == 0) {
                    /* No supers either, go to power bombs */
                    samus->weapons.gun = samus->weapons.beam;
                    samus->weapons.bomb = &templates->weapons.powerBomb;
                }
            }
        } else if (samus->weapons.gun == &templates->weapons.missile) {
            /* Currently on missiles, try super missiles */
            samus->weapons.gun = &templates->weapons.superMissile;
            if (samus->weapons.superMissile_ammo == 0) {
                /* No supers, go to power bombs */
                samus->weapons.gun = samus->weapons.beam;
                samus->weapons.bomb = &templates->weapons.powerBomb;
            }
        } else if (samus->weapons.gun == &templates->weapons.superMissile) {
            /* Currently on super missiles, go to power bombs */
            samus->weapons.gun = samus->weapons.beam;
            samus->weapons.bomb = &templates->weapons.powerBomb;
        }
        game->keys.f5 = 2;  /* Mark key as consumed */
    }

    /* Auto-deselect weapons that have run out of ammo */
    if (samus->weapons.bomb == &templates->weapons.powerBomb &&
        samus->weapons.powerBomb_ammo <= 0)
        samus->weapons.bomb = &templates->weapons.bomb;
    else if (samus->weapons.gun == &templates->weapons.missile &&
             samus->weapons.missile_ammo <= 0)
        samus->weapons.gun = samus->weapons.beam;
    else if (samus->weapons.gun == &templates->weapons.superMissile &&
             samus->weapons.superMissile_ammo <= 0)
        samus->weapons.gun = samus->weapons.beam;

    /* Decrement weapon cooldown timer */
    if (--samus->weapons.wait < 0)
        samus->weapons.wait = 0;
}

/*
 * checkEnemies - Iterate through the enemy linked list.
 * For each living enemy, check bounding box collision with the projectile.
 * If hit, call hurtEnemy and mark the projectile as gone.
 */
void checkEnemies(struct object* obj) {
    struct enemyNode* curr = enemies->first;

    while (curr != NULL) {
        if (!(curr->value->HP < 0)) {
            if (boundingBoxCollide(obj->x, obj->y, obj->w, obj->h,
                                   curr->value->x, curr->value->y,
                                   curr->value->w, curr->value->h, 1)) {
                hurtEnemy(curr->value, obj->ATK, obj->type);
                obj->gone = 1;
            }
        }
        curr = curr->next;
    }
}
