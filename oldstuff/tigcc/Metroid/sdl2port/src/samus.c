/*
 * samus.c - Player (Samus) state machine and physics for Metroid TI-89 SDL2 port
 * Ported from original TIGCC "Samus Functions.c" (all 1793 lines)
 */

#include <stdlib.h>
#include <string.h>
#include <SDL.h>

#include "game.h"
#include "samus.h"
#include "sprites.h"
#include "render.h"
#include "map_data.h"
#include "gfx_data.h"
#include "tiles.h"
#include "objects.h"
#include "enemies.h"

/* -----------------------------------------------------------------------
 * Tile mask lookup helper
 * The original code does: tiles[map->array[1 - map->array[(r)*map->w] + (r)*map->w + c]].m
 * This macro encapsulates the map indirection for a tile at row r, col c.
 * ----------------------------------------------------------------------- */
#define TILE_AT(r, c) tiles[map->array[1 - map->array[(r) * map->w] + (r) * map->w + (c)]]

/* -----------------------------------------------------------------------
 * makeSamus - allocate and initialise the samus struct
 * ----------------------------------------------------------------------- */
void makeSamus(void) {
    samus = malloc(sizeof(struct samus));
    if (samus == NULL)
        exit(1);

    samus->x = 16;
    samus->y = 88;
    samus->vel_x = 0;
    samus->vel_y = 0;
    samus->beenHurt = 0;
    samus->invincible = 0;
    samus->HP = 99;
    samus->frames = 1;
    samus->spiderWall = 0;
    samus->sprt = &sprites->samus.rightFall;
    samus->mode = fallingRight;

    /* Weapons */
    samus->weapons.charge_equipped = 1;
    samus->weapons.charge_owned = 1;
    samus->weapons.spazer_equipped = 0;
    samus->weapons.spazer_owned = 0;
    samus->weapons.wave_equipped = 0;
    samus->weapons.wave_owned = 0;
    samus->weapons.plasma_equipped = 0;
    samus->weapons.plasma_owned = 0;
    samus->weapons.ice_equipped = 0;
    samus->weapons.ice_owned = 0;
    samus->weapons.bomb_owned = 1;
    samus->weapons.missile_ammo = 10;
    samus->weapons.missile_max = 10;
    samus->weapons.superMissile_ammo = 5;
    samus->weapons.superMissile_max = 5;
    samus->weapons.powerBomb_ammo = 5;
    samus->weapons.powerBomb_max = 5;
    samus->weapons.gun = &templates->weapons.powerBeam;
    samus->weapons.bomb = &templates->weapons.bomb;
    samus->weapons.beam = &templates->weapons.powerBeam;
    samus->weapons.charge = 0;
    samus->weapons.wait = 0;

    /* Items */
    samus->items.tanks_owned = 8;
    samus->items.tanks_full = 8;
    samus->items.springBall_owned = 0;
    samus->items.springBall_equipped = 0;
    samus->items.hiJump_owned = 0;
    samus->items.hiJump_equipped = 0;
}

/* -----------------------------------------------------------------------
 * wallJumpRight / wallJumpLeft
 * ----------------------------------------------------------------------- */
void wallJumpRight(void) {
    if (game->keys.right == 2 && game->keys.jump == 1) {
        game->keys.jump++;
        samus->sprt = &sprites->samus.rightFlip1;
        samus->mode = flippingUpRight;
        samus->vel_x = WALL_JUMP_X_VEL;
        samus->vel_y = WALL_JUMP_Y_VEL;
        return;
    } else {
        samus->sprt = &sprites->samus.rightFall;
        samus->mode = fallingRight;
        return;
    }
}

void wallJumpLeft(void) {
    if (game->keys.left == 2 && game->keys.jump == 1) {
        game->keys.jump++;
        samus->sprt = &sprites->samus.leftFlip1;
        samus->mode = flippingUpLeft;
        samus->vel_x = WALL_JUMP_X_VEL;
        samus->vel_y = WALL_JUMP_Y_VEL;
        return;
    } else {
        samus->sprt = &sprites->samus.leftFall;
        samus->mode = fallingLeft;
        return;
    }
}

/* -----------------------------------------------------------------------
 * hurtingLeft / hurtingRight
 * ----------------------------------------------------------------------- */
void hurtingLeft(void) {
    if (samus->beenHurt > 0) {
        if (--samus->beenHurt == 0) {
            samus->mode = fallingLeft;
            samus->sprt = &sprites->samus.leftFall;
            samus->vel_x = 0;
            samus->vel_y = 0;
            samus->invincible = 8;
        } else {
            samus->sprt = &sprites->samus.leftHurt;
        }
        if (!hitWallX(SAMUS_RIGHT + 2, SAMUS_FEET - 2, SAMUS_HEAD - 2, 1, 'r'))
            samus->x += 2;
        if (!hitWallY(samus->y - 26, SAMUS_LEFT, SAMUS_RIGHT, 1, 'b'))
            samus->y -= 2;
    }
}

void hurtingRight(void) {
    if (samus->beenHurt > 0) {
        if (--samus->beenHurt == 0) {
            samus->mode = fallingRight;
            samus->sprt = &sprites->samus.rightFall;
            samus->vel_x = 0;
            samus->vel_y = 0;
            samus->invincible = 8;
        } else {
            samus->sprt = &sprites->samus.rightHurt;
        }
        if (!hitWallX(SAMUS_LEFT - 2, SAMUS_FEET - 2, SAMUS_HEAD - 2, 1, 'l'))
            samus->x -= 2;
        if (!hitWallY(samus->y - 26, SAMUS_LEFT, SAMUS_RIGHT, 1, 'b'))
            samus->y -= 2;
    }
}

/* -----------------------------------------------------------------------
 * hitWallX - pixel-level horizontal collision against tile masks
 *
 * x     : the x pixel coordinate to test
 * y1    : bottom y bound (higher value)
 * y2    : top y bound (lower value, because y increases downward-ish in
 *         the original coordinate system -- but note the loop goes y1..y2
 *         decrementing, so y1 >= y2)
 * collide: 0 = return immediately on first hit
 *          1 = accumulate hits, calling onCollide per tile
 *          3 = return the value from onCollide
 * dir   : 'l' or 'r'
 *
 * Returns 1 if a wall was hit, 0 otherwise (or onCollide result when collide==3).
 * ----------------------------------------------------------------------- */
int hitWallX(int x, int y1, int y2, int collide, char dir) {
    int y, y_store = -1, hit = 0;
    for (y = y1; y >= y2; y--) {
        int r = y / 8;
        int c = x / 8;
        const unsigned char *m = TILE_AT(r, c).m;
        if (m != NULL) {
            if (x % 8 >= m[24 + y % 8] &&
                x % 8 <= m[16 + y % 8] &&
                y % 8 <= m[8 + x % 8] &&
                y % 8 >= m[0 + x % 8]) {
                hit = 1;
                if (!collide)
                    return 1;
                if (y / 8 != y_store) {
                    y_store = y / 8;
                    int s = onCollide(dir, (x / 8) * 8, (y / 8) * 8);
                    if (collide == 3)
                        hit = s;
                }
            }
        }
    }
    return hit;
}

/* -----------------------------------------------------------------------
 * hitWallY - pixel-level vertical collision against tile masks
 *
 * y     : the y pixel coordinate to test
 * x1    : left x bound
 * x2    : right x bound
 * collide: same semantics as hitWallX
 * dir   : 'u', 'd', 't', or 'b' (top/bottom in the original sense)
 * ----------------------------------------------------------------------- */
int hitWallY(int y, int x1, int x2, int collide, char dir) {
    int x, x_store = -1, hit = 0;
    for (x = x1; x <= x2; x++) {
        int r = y / 8;
        int c = x / 8;
        const unsigned char *m = TILE_AT(r, c).m;
        if (m != NULL) {
            if (x % 8 >= m[24 + y % 8] &&
                x % 8 <= m[16 + y % 8] &&
                y % 8 <= m[8 + x % 8] &&
                y % 8 >= m[0 + x % 8]) {
                hit = 1;
                if (!collide)
                    return 1;
                if (x / 8 != x_store) {
                    x_store = x / 8;
                    onCollide(dir, (x / 8) * 8, (y / 8) * 8);
                }
            }
        }
    }
    return hit;
}

/* -----------------------------------------------------------------------
 * standingLeft / standingRight - idle state
 * ----------------------------------------------------------------------- */
void standingLeft(void) {
    samus->vel_x = samus->vel_y = 0;
    if (samus->beenHurt > 0) {
        samus->sprt = &sprites->samus.leftHurt;
        samus->mode = hurtingLeft;
        return;
    }
    if (!hitWallY(samus->y, SAMUS_LEFT, SAMUS_RIGHT, 1, 't')) {
        samus->mode = fallingLeft;
        samus->sprt = &sprites->samus.leftFall;
        samus->vel_y = 0;
        samus->vel_x = 0;
        return;
    }
    if (game->keys.left == 1) {
        game->keys.left++;
        if (!hitWallX(SAMUS_LEFT, SAMUS_FEET, SAMUS_HEAD, 1, 'l')) {
            samus->mode = runningLeft;
            samus->sprt = &sprites->samus.leftRun1;
            samus->vel_x = RUN_INIT;
            return;
        }
    } else if (game->keys.right) {
        samus->mode = standingRight;
        samus->sprt = &sprites->samus.rightStand;
        return;
    } else if (game->keys.down == 1) {
        game->keys.down++;
        samus->mode = kneelingLeft;
        samus->sprt = &sprites->samus.leftKneel;
        return;
    } else if (game->keys.jump == 1) {
        game->keys.jump++;
        if (!hitWallY(SAMUS_HEAD - 1, SAMUS_LEFT, SAMUS_RIGHT, 1, 'b')) {
            samus->mode = jumpingLeft;
            samus->sprt = &sprites->samus.leftJump1;
            samus->vel_x = 0;
            samus->vel_y = JUMP_INIT_Y;
        }
    }
    if (game->keys.fire || samus->weapons.charge) {
        shoot(SAMUS_LEFT, samus->y - 10, -1);
        if (game->keys.fire)
            game->keys.fire = 2;
    }
}

void standingRight(void) {
    samus->vel_x = samus->vel_y = 0;
    if (samus->beenHurt > 0) {
        samus->sprt = &sprites->samus.rightHurt;
        samus->mode = hurtingRight;
        return;
    }
    if (!hitWallY(samus->y, SAMUS_LEFT, SAMUS_RIGHT, 1, 't')) {
        samus->mode = fallingRight;
        samus->sprt = &sprites->samus.rightFall;
        samus->vel_y = 0;
        samus->vel_x = 0;
        return;
    }
    if (game->keys.right == 1) {
        game->keys.right++;
        if (!hitWallX(SAMUS_RIGHT, SAMUS_FEET, SAMUS_HEAD, 1, 'r')) {
            samus->mode = runningRight;
            samus->sprt = &sprites->samus.rightRun1;
            samus->vel_x = RUN_INIT;
            return;
        }
    } else if (game->keys.left) {
        samus->mode = standingLeft;
        samus->sprt = &sprites->samus.leftStand;
        return;
    } else if (game->keys.down == 1) {
        game->keys.down++;
        samus->mode = kneelingRight;
        samus->sprt = &sprites->samus.rightKneel;
        return;
    } else if (game->keys.jump == 1) {
        game->keys.jump++;
        if (!hitWallY(SAMUS_HEAD - 1, SAMUS_LEFT, SAMUS_RIGHT, 1, 'b')) {
            samus->mode = jumpingRight;
            samus->sprt = &sprites->samus.rightJump1;
            samus->vel_x = 0;
            samus->vel_y = JUMP_INIT_Y;
        }
    }
    if (game->keys.fire || samus->weapons.charge) {
        shoot(SAMUS_LEFT + 10, samus->y - 10, 1);
        if (game->keys.fire)
            game->keys.fire = 2;
    }
}

/* -----------------------------------------------------------------------
 * runningCollision - check walls while running
 * Returns 1 if movement is OK, 0 if samus stopped/transitioned.
 * ----------------------------------------------------------------------- */
int runningCollision(int dir) {
    int i;
    for (i = 0; i < samus->vel_x; i++) {
        int backupX = samus->x;
        int backupY = samus->y;
        samus->x += dir;
        groundHug(SAMUS_CRIT, samus->y - 1, &samus->y, 0);
        if (hitWallX(SAMUS_LEFT, SAMUS_FEET, SAMUS_HEAD, 1, 'l') ||
            hitWallX(SAMUS_RIGHT, SAMUS_FEET, SAMUS_HEAD, 1, 'r')) {
            samus->x = backupX;
            samus->y = backupY;
            if (dir < 0) {
                samus->mode = standingLeft;
                samus->sprt = &sprites->samus.leftStand;
            } else {
                samus->mode = standingRight;
                samus->sprt = &sprites->samus.rightStand;
            }
            return 0;
        }
        if (!hitWallY(samus->y, SAMUS_LEFT, SAMUS_RIGHT, 1, 't')) {
            if (dir < 0) {
                samus->mode = fallingLeft;
                samus->sprt = &sprites->samus.leftFall;
            } else {
                samus->mode = fallingRight;
                samus->sprt = &sprites->samus.rightFall;
            }
            samus->vel_y = 0;
            samus->vel_x /= FALLING_X_VEL_DIVISOR;
            return 0;
        }
    }
    return 1;
}

/* -----------------------------------------------------------------------
 * runningLeft / runningRight
 * ----------------------------------------------------------------------- */
void runningLeft(void) {
    samus->frames = RUN_FRAMES;
    if (samus->beenHurt > 0) {
        samus->sprt = &sprites->samus.leftHurt;
        samus->mode = hurtingLeft;
        return;
    }
    if (game->keys.right == 1) {
        samus->mode = standingRight;
        samus->sprt = &sprites->samus.rightStand;
        return;
    } else if (game->keys.down == 1) {
        game->keys.down++;
        samus->mode = kneelingLeft;
        samus->sprt = &sprites->samus.leftKneel;
        return;
    } else if (!game->keys.left) {
        if ((samus->vel_x -= 4) <= 0) {
            samus->mode = standingLeft;
            samus->sprt = &sprites->samus.leftStand;
            return;
        }
    } else if (game->keys.jump == 1) {
        game->keys.jump++;
        if (samus->vel_x > FLIP_CUTOFF) {
            samus->mode = flippingUpLeft;
            samus->sprt = &sprites->samus.leftFlip1;
        } else {
            samus->mode = jumpingLeft;
            samus->sprt = &sprites->samus.leftJump1;
        }
        samus->vel_y = JUMP_INIT_Y;
        return;
    }

    if (runningCollision(-1) == 0)
        return;

    if (game->keys.fire || samus->weapons.charge) {
        shoot(SAMUS_LEFT, samus->y - 10, -1);
        if (game->keys.fire)
            game->keys.fire = 2;
    }
    if (game->keys.left) {
        game->keys.left = 2;
        samus->vel_x++;
        int max = RUN_MAX;
        if (game->keys.sprint)
            max = SPRINT_MAX;
        if (++samus->vel_x > max)
            samus->vel_x = max;
    }
}

void runningRight(void) {
    samus->frames = RUN_FRAMES;
    if (samus->beenHurt > 0) {
        samus->sprt = &sprites->samus.rightHurt;
        samus->mode = hurtingRight;
        return;
    }
    if (game->keys.left == 1) {
        samus->mode = standingLeft;
        samus->sprt = &sprites->samus.leftStand;
        return;
    } else if (game->keys.down == 1) {
        game->keys.down++;
        samus->mode = kneelingRight;
        samus->sprt = &sprites->samus.rightKneel;
        return;
    } else if (!game->keys.right) {
        if ((samus->vel_x -= 4) <= 0) {
            samus->mode = standingRight;
            samus->sprt = &sprites->samus.rightStand;
            return;
        }
    } else if (game->keys.jump == 1) {
        game->keys.jump++;
        if (samus->vel_x > FLIP_CUTOFF) {
            samus->mode = flippingUpRight;
            samus->sprt = &sprites->samus.rightFlip1;
        } else {
            samus->mode = jumpingRight;
            samus->sprt = &sprites->samus.rightJump1;
        }
        samus->vel_y = JUMP_INIT_Y;
        return;
    }
    if (runningCollision(1) == 0)
        return;

    if (game->keys.fire || samus->weapons.charge) {
        shoot(SAMUS_RIGHT, samus->y - 10, 1);
        if (game->keys.fire)
            game->keys.fire = 2;
    }
    if (game->keys.right) {
        game->keys.right = 2;
        samus->vel_x++;
        int max = RUN_MAX;
        if (game->keys.sprint)
            max = SPRINT_MAX;
        if (++samus->vel_x > max)
            samus->vel_x = max;
    }
}

/* -----------------------------------------------------------------------
 * kneelingLeft / kneelingRight
 * ----------------------------------------------------------------------- */
void kneelingLeft(void) {
    samus->vel_x = samus->vel_y = 0;
    if (samus->beenHurt > 0) {
        samus->sprt = &sprites->samus.leftHurt;
        samus->mode = hurtingLeft;
        return;
    }
    if (!hitWallY(samus->y, SAMUS_LEFT, SAMUS_RIGHT, 1, 't')) {
        samus->mode = fallingLeft;
        samus->sprt = &sprites->samus.leftFall;
        samus->vel_y = 0;
        samus->vel_x = 0;
        return;
    }
    if (game->keys.left == 1) {
        game->keys.left = 2;
        if (!hitWallY(SAMUS_HEAD, SAMUS_LEFT, SAMUS_RIGHT, 1, 'b') &&
            !hitWallX(SAMUS_LEFT, SAMUS_FEET, SAMUS_HEAD, 1, 'l')) {
            samus->mode = runningLeft;
            samus->sprt = &sprites->samus.leftRun1;
            samus->vel_x = RUN_INIT;
            return;
        }
    } else if (game->keys.right) {
        samus->mode = kneelingRight;
        samus->sprt = &sprites->samus.rightKneel;
        return;
    } else if (game->keys.down == 1) {
        game->keys.down = 2;
        samus->mode = morphedLeft;
        samus->sprt = &sprites->samus.leftMorph;
        return;
    } else if (game->keys.up == 1) {
        game->keys.up = 2;
        if (!hitWallY(SAMUS_HEAD, SAMUS_LEFT, SAMUS_RIGHT, 1, 'b')) {
            samus->mode = standingLeft;
            samus->sprt = &sprites->samus.leftStand;
            return;
        }
    }
    if (game->keys.fire || samus->weapons.charge) {
        shoot(SAMUS_LEFT, samus->y - 5, -1);
        if (game->keys.fire)
            game->keys.fire = 2;
    }
}

void kneelingRight(void) {
    samus->vel_x = samus->vel_y = 0;
    if (samus->beenHurt > 0) {
        samus->sprt = &sprites->samus.rightHurt;
        samus->mode = hurtingRight;
        return;
    }
    if (!hitWallY(samus->y, SAMUS_LEFT, SAMUS_RIGHT, 1, 't')) {
        samus->mode = fallingRight;
        samus->sprt = &sprites->samus.rightFall;
        samus->vel_y = 0;
        samus->vel_x = 0;
        return;
    }
    if (game->keys.right == 1) {
        game->keys.right = 2;
        if (!hitWallY(SAMUS_HEAD, SAMUS_LEFT, SAMUS_RIGHT, 1, 'b') &&
            !hitWallX(SAMUS_RIGHT, SAMUS_FEET, SAMUS_HEAD, 1, 'r')) {
            samus->mode = runningRight;
            samus->sprt = &sprites->samus.rightRun1;
            samus->vel_x = RUN_INIT;
            return;
        }
    } else if (game->keys.left) {
        samus->mode = kneelingLeft;
        samus->sprt = &sprites->samus.leftKneel;
        return;
    } else if (game->keys.down == 1) {
        game->keys.down = 2;
        samus->mode = morphedRight;
        samus->sprt = &sprites->samus.rightMorph;
        return;
    } else if (game->keys.up == 1) {
        game->keys.up = 2;
        if (!hitWallY(SAMUS_HEAD, SAMUS_LEFT, SAMUS_RIGHT, 1, 'b')) {
            samus->mode = standingRight;
            samus->sprt = &sprites->samus.rightStand;
            return;
        }
    }
    if (game->keys.fire || samus->weapons.charge) {
        shoot(SAMUS_RIGHT, samus->y - 5, 1);
        if (game->keys.fire)
            game->keys.fire = 2;
    }
}

/* -----------------------------------------------------------------------
 * jumpingCollision - check ceiling/walls while jumping (moving upward)
 * Returns 1 if movement OK, 0 if transitioned to falling.
 * ----------------------------------------------------------------------- */
int jumpingCollision(int dir) {
    int moved_y = 0, moved_x = 0;
    while (moved_y < samus->vel_y / 3 || moved_x < samus->vel_x) {
        int backupX = samus->x;
        int backupY = samus->y;
        if (++moved_y <= samus->vel_y / 3)
            samus->y--;
        if (++moved_x <= samus->vel_x)
            samus->x += dir;
        if (hitWallX(SAMUS_LEFT, SAMUS_FEET, SAMUS_HEAD, 1, 'l') ||
            hitWallX(SAMUS_RIGHT, SAMUS_FEET, SAMUS_HEAD, 1, 'r')) {
            samus->x = backupX;
            samus->vel_x = 0;
        }
        if (hitWallY(SAMUS_HEAD, SAMUS_LEFT, SAMUS_RIGHT, 1, 'b')) {
            samus->y = backupY;
            samus->vel_x /= FALLING_X_VEL_DIVISOR;
            samus->vel_y = 5;
            if (dir < 0) {
                samus->mode = fallingLeft;
                samus->sprt = &sprites->samus.leftFall;
            } else {
                samus->mode = fallingRight;
                samus->sprt = &sprites->samus.rightFall;
            }
            return 0;
        }
    }
    return 1;
}

/* -----------------------------------------------------------------------
 * jumpingLeft / jumpingRight
 * ----------------------------------------------------------------------- */
void jumpingLeft(void) {
    samus->frames = 2;
    if (samus->beenHurt > 0) {
        samus->sprt = &sprites->samus.leftHurt;
        samus->mode = hurtingLeft;
        return;
    }
    if (game->keys.left) {
        game->keys.left = 2;
        samus->vel_x = IN_AIR_X_VEL;
    } else
        samus->vel_x = 0;

    if (game->keys.fire || samus->weapons.charge) {
        shoot(SAMUS_LEFT, samus->y - 10, -1);
        if (game->keys.fire)
            game->keys.fire = 2;
    }

    if (jumpingCollision(-1) == 0)
        return;

    samus->vel_y -= JUMPING_GRAVITY;

    if (game->keys.down == 1) {
        game->keys.down = 2;
        samus->mode = bouncingLeft;
        samus->sprt = &sprites->samus.ball1;
        samus->vel_y /= 3;
        return;
    }
    if (game->keys.right) {
        game->keys.right = 2;
        samus->vel_x = 0;
        samus->mode = jumpingRight;
        samus->sprt = &sprites->samus.rightJump2;
        return;
    }
    if (!(game->keys.jump == 2) || samus->vel_y <= 5) {
        samus->vel_y = 5;
        samus->mode = fallingLeft;
        samus->sprt = &sprites->samus.leftFall;
        return;
    }
}

void jumpingRight(void) {
    samus->frames = 2;
    if (samus->beenHurt > 0) {
        samus->sprt = &sprites->samus.rightHurt;
        samus->mode = hurtingRight;
        return;
    }
    if (game->keys.right) {
        game->keys.right = 2;
        samus->vel_x = IN_AIR_X_VEL;
    } else
        samus->vel_x = 0;

    if (game->keys.fire || samus->weapons.charge) {
        shoot(SAMUS_RIGHT, samus->y - 10, 1);
        if (game->keys.fire)
            game->keys.fire = 2;
    }

    if (jumpingCollision(1) == 0)
        return;

    samus->vel_y -= JUMPING_GRAVITY;
    if (game->keys.down == 1) {
        game->keys.down = 2;
        samus->mode = bouncingRight;
        samus->sprt = &sprites->samus.ball1;
        samus->vel_y /= 3;
        return;
    }
    if (game->keys.left) {
        game->keys.left = 2;
        samus->vel_x = 0;
        samus->mode = jumpingLeft;
        samus->sprt = &sprites->samus.leftJump2;
        return;
    }
    if (!(game->keys.jump == 2) || samus->vel_y <= 5) {
        samus->vel_y = 5;
        samus->mode = fallingRight;
        samus->sprt = &sprites->samus.rightFall;
        return;
    }
}

/* -----------------------------------------------------------------------
 * fallingCollision - check floor/walls while falling (moving downward)
 * Returns 1 if still falling, 0 if landed.
 * ----------------------------------------------------------------------- */
int fallingCollision(int dir) {
    int moved_y = 0, moved_x = 0;
    while (moved_y < samus->vel_y / 3 || moved_x < samus->vel_x) {
        int backupX = samus->x;
        int backupY = samus->y;
        if (++moved_y <= samus->vel_y / 3)
            samus->y++;
        if (++moved_x <= samus->vel_x)
            samus->x += dir;
        int bottom = SAMUS_FEET;
        if (!hitWallY(samus->y - 1, SAMUS_CRIT, SAMUS_CRIT, 1, 't'))
            bottom = samus->y - 1;
        if (hitWallX(SAMUS_LEFT, bottom, SAMUS_HEAD, 1, 'l') ||
            hitWallX(SAMUS_RIGHT, bottom, SAMUS_HEAD, 1, 'r')) {
            samus->x = backupX;
            samus->vel_x = 0;
        }
        if (hitWallY(samus->y - 1, SAMUS_LEFT, SAMUS_RIGHT, 1, 't')) {
            samus->y = backupY;
            samus->vel_x = 0;
            samus->vel_y = 0;
            if (dir < 0) {
                if (game->keys.left) {
                    game->keys.left = 2;
                    samus->mode = runningLeft;
                    samus->sprt = &sprites->samus.leftRun1;
                } else {
                    samus->mode = standingLeft;
                    samus->sprt = &sprites->samus.leftLand;
                }
            } else {
                if (game->keys.right) {
                    game->keys.right = 2;
                    samus->mode = runningRight;
                    samus->sprt = &sprites->samus.rightRun1;
                } else {
                    samus->mode = standingRight;
                    samus->sprt = &sprites->samus.rightLand;
                }
            }
            return 0;
        }
    }
    return 1;
}

/* -----------------------------------------------------------------------
 * fallingLeft / fallingRight
 * ----------------------------------------------------------------------- */
void fallingLeft(void) {
    samus->frames = 2;
    if (samus->beenHurt > 0) {
        samus->sprt = &sprites->samus.leftHurt;
        samus->mode = hurtingLeft;
        return;
    }
    if (game->keys.jump)
        game->keys.jump = 2;
    if (game->keys.left) {
        game->keys.left = 2;
        samus->vel_x = IN_AIR_X_VEL;
    } else
        samus->vel_x = 0;

    if (game->keys.fire || samus->weapons.charge) {
        shoot(SAMUS_LEFT, samus->y - 7, -1);
        if (game->keys.fire)
            game->keys.fire = 2;
    }

    if (fallingCollision(-1) == 0)
        return;

    samus->vel_y += FALLING_GRAVITY;
    if (samus->vel_y > TERMINAL_VELOCITY)
        samus->vel_y = TERMINAL_VELOCITY;

    if (game->keys.down == 1) {
        game->keys.down = 2;
        samus->mode = fallingBallLeft;
        samus->sprt = &sprites->samus.ball1;
        samus->vel_y /= 3;
        return;
    }
    if (game->keys.right) {
        game->keys.right = 2;
        samus->vel_x = 0;
        samus->mode = fallingRight;
        samus->sprt = &sprites->samus.rightFall;
        return;
    }
}

void fallingRight(void) {
    samus->frames = 2;
    if (samus->beenHurt > 0) {
        samus->sprt = &sprites->samus.rightHurt;
        samus->mode = hurtingRight;
        return;
    }
    if (game->keys.jump)
        game->keys.jump = 2;
    if (game->keys.right) {
        game->keys.right = 2;
        samus->vel_x = IN_AIR_X_VEL;
    } else
        samus->vel_x = 0;

    if (game->keys.fire || samus->weapons.charge) {
        shoot(SAMUS_RIGHT, samus->y - 7, 1);
        if (game->keys.fire)
            game->keys.fire = 2;
    }

    if (fallingCollision(1) == 0)
        return;

    samus->vel_y += FALLING_GRAVITY;
    if (samus->vel_y > TERMINAL_VELOCITY)
        samus->vel_y = TERMINAL_VELOCITY;

    if (game->keys.down == 1) {
        game->keys.down = 2;
        samus->mode = fallingBallRight;
        samus->sprt = &sprites->samus.ball1;
        samus->vel_y /= 3;
        return;
    }
    if (game->keys.left) {
        game->keys.left = 2;
        samus->vel_x = 0;
        samus->mode = fallingLeft;
        samus->sprt = &sprites->samus.leftFall;
        return;
    }
}

/* -----------------------------------------------------------------------
 * flippingUpCollision - collision during upward somersault
 * Returns: 1 = keep going, 0 = hit ceiling (transitioned to flippingDown),
 *          2 = hit wall and opposite direction held (wall jump possible)
 * ----------------------------------------------------------------------- */
int flippingUpCollision(int dir) {
    int moved_y = 0, moved_x = 0, r = 1;
    while (moved_y < samus->vel_y / 3 || moved_x < samus->vel_x) {
        int backupX = samus->x;
        int backupY = samus->y;
        if (++moved_y <= samus->vel_y / 3)
            samus->y--;
        if (++moved_x <= samus->vel_x)
            samus->x += dir;
        if (hitWallY(samus->y - 14, SAMUS_LEFT, SAMUS_RIGHT, 1, 'b')) {
            samus->y = backupY;
            samus->vel_y = 5;
            if (dir > 0)
                samus->mode = flippingDownRight;
            else
                samus->mode = flippingDownLeft;
            r = 0;
        }
        if (hitWallX(SAMUS_LEFT, SAMUS_FEET, FLIP_TOP, 1, 'l') ||
            hitWallX(SAMUS_RIGHT, SAMUS_FEET, FLIP_TOP, 1, 'r')) {
            samus->x = backupX;
            samus->vel_x = 0;
            /* Check if opposite direction is held for wall jump */
            const uint8_t *state = SDL_GetKeyboardState(NULL);
            if ((dir < 0 && state[SDL_SCANCODE_RIGHT]) ||
                (dir > 0 && state[SDL_SCANCODE_LEFT]))
                r = 2;
        }
    }
    return r;
}

/* -----------------------------------------------------------------------
 * flippingUpLeft / flippingUpRight - somersault upward
 * ----------------------------------------------------------------------- */
void flippingUpLeft(void) {
    samus->frames = FLIP_FRAMES;
    if (samus->beenHurt > 0) {
        samus->sprt = &sprites->samus.leftHurt;
        samus->mode = hurtingLeft;
        return;
    }

    if (game->keys.fire || samus->weapons.charge) {
        if (shoot(SAMUS_LEFT, samus->y - 10, -1) == 1) {
            samus->vel_x = 0;
            samus->vel_y = 5;
            samus->mode = fallingLeft;
            samus->sprt = &sprites->samus.leftFall;
        }
        if (game->keys.fire)
            game->keys.fire = 2;
    }

    int collide = flippingUpCollision(-1);
    if (collide == 0)
        return;
    else if (collide == 2) {
        samus->mode = wallJumpRight;
        samus->sprt = &sprites->samus.rightWallJump;
        return;
    }
    samus->vel_y -= JUMPING_GRAVITY;
    if (!(game->keys.jump == 2))
        samus->vel_y -= JUMPING_GRAVITY;
    if (samus->vel_y <= 5) {
        samus->vel_y = 5;
        samus->mode = flippingDownLeft;
        return;
    }
    if (game->keys.right) {
        game->keys.right = 2;
        samus->mode = flippingUpRight;
        samus->sprt = &sprites->samus.rightFlip1;
        return;
    }
    if (game->keys.up) {
        game->keys.up = 2;
        samus->vel_x = 0;
        samus->mode = jumpingLeft;
        samus->sprt = &sprites->samus.leftJump2;
        return;
    }
    if (game->keys.down) {
        game->keys.down = 2;
        samus->vel_x = 0;
        samus->vel_y = 5;
        samus->mode = fallingLeft;
        samus->sprt = &sprites->samus.leftFall;
    }
}

void flippingUpRight(void) {
    samus->frames = FLIP_FRAMES;
    if (samus->beenHurt > 0) {
        samus->sprt = &sprites->samus.rightHurt;
        samus->mode = hurtingRight;
        return;
    }

    if (game->keys.fire || samus->weapons.charge) {
        if (shoot(SAMUS_RIGHT, samus->y - 7, 1) == 1) {
            samus->vel_x = 0;
            samus->vel_y = 5;
            samus->mode = fallingRight;
            samus->sprt = &sprites->samus.rightFall;
        }
        if (game->keys.fire)
            game->keys.fire = 2;
    }

    int collide = flippingUpCollision(1);
    if (collide == 0)
        return;
    else if (collide == 2) {
        samus->mode = wallJumpLeft;
        samus->sprt = &sprites->samus.leftWallJump;
        return;
    }
    samus->vel_y -= JUMPING_GRAVITY;
    if (!(game->keys.jump == 2))
        samus->vel_y -= JUMPING_GRAVITY;
    if (samus->vel_y <= 0) {
        samus->vel_y = 0;
        samus->mode = flippingDownRight;
        return;
    }
    if (game->keys.left) {
        game->keys.left = 2;
        samus->mode = flippingUpLeft;
        samus->sprt = &sprites->samus.leftFlip1;
        return;
    }
    if (game->keys.up) {
        game->keys.up = 2;
        samus->vel_x = 0;
        samus->mode = jumpingRight;
        samus->sprt = &sprites->samus.rightJump2;
        return;
    }
    if (game->keys.down == 1) {
        game->keys.down = 2;
        samus->vel_x = 0;
        samus->vel_y = 0;
        samus->mode = fallingRight;
        samus->sprt = &sprites->samus.rightFall;
    }
}

/* -----------------------------------------------------------------------
 * flippingDownCollision - collision during downward somersault
 * Returns: 1 = keep going, 0 = landed, 2 = wall jump opportunity
 * ----------------------------------------------------------------------- */
int flippingDownCollision(int dir) {
    int moved_y = 0, moved_x = 0, r = 1;
    while (moved_y < samus->vel_y / 3 || moved_x < samus->vel_x) {
        int backupX = samus->x;
        int backupY = samus->y;
        if (++moved_y <= samus->vel_y / 3)
            samus->y++;
        if (++moved_x <= samus->vel_x)
            samus->x += dir;
        int bottom = SAMUS_FEET;
        if (!hitWallY(samus->y - 1, SAMUS_CRIT, SAMUS_CRIT, 1, 't'))
            bottom = samus->y - 1;
        if (hitWallX(SAMUS_LEFT, bottom, FLIP_TOP, 1, 'l') ||
            hitWallX(SAMUS_RIGHT, bottom, FLIP_TOP, 1, 'r')) {
            samus->x = backupX;
            samus->vel_x = 0;
            const uint8_t *state = SDL_GetKeyboardState(NULL);
            if ((dir < 0 && state[SDL_SCANCODE_RIGHT]) ||
                (dir > 0 && state[SDL_SCANCODE_LEFT]))
                r = 2;
        }
        if (hitWallY(samus->y - 1, SAMUS_LEFT, SAMUS_RIGHT, 1, 't')) {
            samus->y = backupY;
            samus->vel_x = 0;
            samus->vel_y = 0;
            if (hitWallY(SAMUS_HEAD, SAMUS_LEFT, SAMUS_RIGHT, 1, 'b')) {
                if (dir < 0) {
                    samus->mode = kneelingLeft;
                    samus->sprt = &sprites->samus.leftKneel;
                } else {
                    samus->mode = kneelingRight;
                    samus->sprt = &sprites->samus.rightKneel;
                }
            } else {
                if (dir < 0) {
                    if (game->keys.left) {
                        game->keys.left = 2;
                        samus->mode = runningLeft;
                        samus->sprt = &sprites->samus.leftRun1;
                    } else {
                        samus->mode = standingLeft;
                        samus->sprt = &sprites->samus.leftLand;
                    }
                } else {
                    if (game->keys.right) {
                        game->keys.right = 2;
                        samus->mode = runningRight;
                        samus->sprt = &sprites->samus.rightRun1;
                    } else {
                        samus->mode = standingRight;
                        samus->sprt = &sprites->samus.rightLand;
                    }
                }
            }
            r = 0;
        }
    }
    return r;
}

/* -----------------------------------------------------------------------
 * flippingDownLeft / flippingDownRight - somersault downward
 * ----------------------------------------------------------------------- */
void flippingDownLeft(void) {
    samus->frames = FLIP_FRAMES;
    if (samus->beenHurt > 0) {
        samus->sprt = &sprites->samus.leftHurt;
        samus->mode = hurtingLeft;
        return;
    }

    if (game->keys.fire || samus->weapons.charge) {
        if (shoot(SAMUS_LEFT, samus->y - 10, -1) == 1) {
            samus->vel_x = 0;
            samus->vel_y = 5;
            samus->mode = fallingLeft;
            samus->sprt = &sprites->samus.leftFall;
        }
        if (game->keys.fire)
            game->keys.fire = 2;
    }

    int collide = flippingDownCollision(-1);
    if (collide == 0)
        return;
    else if (collide == 2) {
        samus->mode = wallJumpRight;
        samus->sprt = &sprites->samus.rightWallJump;
        return;
    }
    samus->vel_y += FALLING_GRAVITY;
    if (samus->vel_y > TERMINAL_VELOCITY)
        samus->vel_y = TERMINAL_VELOCITY;

    if (game->keys.right) {
        game->keys.right = 2;
        samus->mode = flippingDownRight;
        samus->sprt = &sprites->samus.rightFlip1;
        return;
    }
    if (game->keys.up) {
        game->keys.up = 2;
        samus->vel_x = 0;
        samus->mode = fallingLeft;
        samus->sprt = &sprites->samus.leftFall;
        return;
    }
    if (game->keys.down == 1) {
        game->keys.down = 2;
        samus->vel_x = 0;
        samus->mode = fallingLeft;
        samus->sprt = &sprites->samus.leftFall;
    }
}

void flippingDownRight(void) {
    samus->frames = FLIP_FRAMES;
    if (samus->beenHurt > 0) {
        samus->sprt = &sprites->samus.rightHurt;
        samus->mode = hurtingRight;
        return;
    }

    if (game->keys.fire || samus->weapons.charge) {
        if (shoot(SAMUS_RIGHT, samus->y - 7, 1) == 1) {
            samus->vel_x = 0;
            samus->vel_y = 5;
            samus->mode = fallingRight;
            samus->sprt = &sprites->samus.rightFall;
        }
        if (game->keys.fire)
            game->keys.fire = 2;
    }

    int collide = flippingDownCollision(1);
    if (collide == 0)
        return;
    else if (collide == 2) {
        samus->mode = wallJumpLeft;
        samus->sprt = &sprites->samus.leftWallJump;
        return;
    }

    samus->vel_y += FALLING_GRAVITY;
    if (samus->vel_y > TERMINAL_VELOCITY)
        samus->vel_y = TERMINAL_VELOCITY;

    if (game->keys.left) {
        game->keys.left = 2;
        samus->mode = flippingDownLeft;
        samus->sprt = &sprites->samus.leftFlip1;
        return;
    }
    if (game->keys.up) {
        game->keys.up = 2;
        samus->vel_x = 0;
        samus->mode = fallingRight;
        samus->sprt = &sprites->samus.rightFall;
        return;
    }
    if (game->keys.down == 1) {
        game->keys.down = 2;
        samus->vel_x = 0;
        samus->mode = fallingRight;
        samus->sprt = &sprites->samus.rightFall;
    }
}

/* -----------------------------------------------------------------------
 * morphedLeft / morphedRight / morphed - morphball idle state
 * ----------------------------------------------------------------------- */
void morphedLeft(void) {
    /* Check for spider ball activation via sprint key (was _keytest(RR_SHIFT)) */
    if (game->keys.sprint) {
        samus->mode = spiderLeft;
        samus->spiderWall = 0;
        spiderLeft();
        return;
    }
    if (game->keys.up == 1) {
        game->keys.up = 2;
        if (!hitWallY(KNEEL_HEAD, SAMUS_LEFT, SAMUS_RIGHT, 1, 'b')) {
            samus->mode = kneelingLeft;
            samus->sprt = &sprites->samus.leftKneel;
            return;
        }
    }
    if (!hitWallY(samus->y, SAMUS_LEFT, SAMUS_RIGHT, 1, 't')) {
        samus->mode = fallingBallLeft;
        samus->vel_y = 0;
        samus->vel_x = 0;
        return;
    }
    morphed();
}

void morphedRight(void) {
    if (game->keys.sprint) {
        samus->mode = spiderRight;
        samus->spiderWall = 0;
        spiderRight();
        return;
    }
    if (game->keys.up == 1) {
        game->keys.up = 2;
        if (!hitWallY(KNEEL_HEAD, SAMUS_LEFT, SAMUS_RIGHT, 1, 'b')) {
            samus->mode = kneelingRight;
            samus->sprt = &sprites->samus.rightKneel;
            return;
        }
    }
    if (!hitWallY(samus->y, SAMUS_LEFT, SAMUS_RIGHT, 1, 't')) {
        samus->mode = fallingBallLeft;
        samus->vel_y = 0;
        samus->vel_x = 0;
        return;
    }
    morphed();
}

void morphed(void) {
    samus->frames = BALL_FRAMES;
    if (samus->beenHurt > 0) {
        if (--samus->beenHurt == 0) {
            ; /* no-op, just clear hurt */
        } else {
            ; /* still hurting, no-op */
        }
    }
    if (game->keys.left == 1) {
        game->keys.left = 2;
        samus->mode = rollingLeft;
        samus->vel_x = ROLL_INIT;
        return;
    } else if (game->keys.right == 1) {
        game->keys.right = 2;
        samus->mode = rollingRight;
        samus->vel_x = ROLL_INIT;
        return;
    }
    layBomb();
}

/* -----------------------------------------------------------------------
 * rollingCollision - collision while rolling in morphball
 * ----------------------------------------------------------------------- */
int rollingCollision(int dir) {
    int i;
    for (i = 0; i < samus->vel_x; i++) {
        int backupX = samus->x;
        int backupY = samus->y;
        samus->x += dir;
        groundHug(SAMUS_CRIT, samus->y - 1, &samus->y, 0);
        if (hitWallX(SAMUS_LEFT, SAMUS_FEET, MORPH_TOP, 1, 'l') ||
            hitWallX(SAMUS_RIGHT, SAMUS_FEET, MORPH_TOP, 1, 'r')) {
            samus->x = backupX;
            samus->y = backupY;
            if (dir < 0) {
                samus->mode = morphedLeft;
            } else {
                samus->mode = morphedRight;
            }
            return 0;
        }
        if (!hitWallY(samus->y, SAMUS_LEFT, SAMUS_RIGHT, 1, 't')) {
            if (dir < 0) {
                samus->mode = fallingBallLeft;
            } else {
                samus->mode = fallingBallRight;
            }
            samus->vel_y = 0;
            samus->vel_x /= FALLING_X_VEL_DIVISOR;
            return 0;
        }
    }
    return 1;
}

/* -----------------------------------------------------------------------
 * rollingLeft / rollingRight
 * ----------------------------------------------------------------------- */
void rollingLeft(void) {
    samus->frames = BALL_FRAMES;
    if (samus->beenHurt > 0) {
        if (--samus->beenHurt == 0) {
            samus->mode = morphedLeft;
        } else {
            samus->vel_x = 0;
        }
    }
    if (game->keys.right == 1) {
        game->keys.right = 2;
        samus->mode = rollingRight;
        samus->vel_x = ROLL_INIT;
        return;
    } else if (game->keys.up == 1) {
        game->keys.up = 2;
        if (!hitWallY(KNEEL_HEAD, SAMUS_LEFT, SAMUS_RIGHT, 1, 'b')) {
            samus->mode = kneelingLeft;
            samus->sprt = &sprites->samus.leftKneel;
            return;
        }
    } else if (!(game->keys.left)) {
        if ((samus->vel_x -= 2) <= 0) {
            samus->mode = morphedLeft;
            return;
        }
    }

    if (rollingCollision(-1) == 0)
        return;

    layBomb();
    if (game->keys.left) {
        game->keys.left = 2;
        if (++samus->vel_x > ROLL_MAX)
            samus->vel_x = ROLL_MAX;
    }
}

void rollingRight(void) {
    samus->frames = BALL_FRAMES;
    if (samus->beenHurt > 0) {
        if (--samus->beenHurt == 0) {
            samus->mode = morphedRight;
        } else {
            samus->vel_x = 0;
        }
    }
    if (game->keys.left == 1) {
        game->keys.left = 2;
        samus->mode = rollingLeft;
        samus->vel_x = ROLL_INIT;
        return;
    } else if (game->keys.up == 1) {
        game->keys.up = 2;
        if (!hitWallY(KNEEL_HEAD, SAMUS_LEFT, SAMUS_RIGHT, 1, 'b')) {
            samus->mode = kneelingRight;
            samus->sprt = &sprites->samus.rightKneel;
            return;
        }
    } else if (!(game->keys.right)) {
        if ((samus->vel_x -= 2) <= 0) {
            samus->mode = morphedRight;
            return;
        }
    }

    if (rollingCollision(1) == 0)
        return;

    layBomb();
    if (game->keys.right) {
        game->keys.right = 2;
        if (++samus->vel_x > ROLL_MAX)
            samus->vel_x = ROLL_MAX;
    }
}

/* -----------------------------------------------------------------------
 * fallingBallCollision - collision while morphball is falling
 * ----------------------------------------------------------------------- */
int fallingBallCollision(int dir) {
    int moved_y = 0, moved_x = 0;
    while (moved_y < samus->vel_y / 3 || moved_x < samus->vel_x) {
        int backupX = samus->x;
        int backupY = samus->y;
        if (++moved_y <= samus->vel_y / 3)
            samus->y++;
        if (++moved_x <= samus->vel_x)
            samus->x += dir;
        int bottom = SAMUS_FEET;
        if (!hitWallY(samus->y - 1, SAMUS_CRIT, SAMUS_CRIT, 1, 't'))
            bottom = samus->y - 1;
        if (hitWallX(SAMUS_LEFT, bottom, MORPH_TOP, 1, 'l') ||
            hitWallX(SAMUS_RIGHT, bottom, MORPH_TOP, 1, 'r')) {
            samus->x = backupX;
            samus->vel_x = 0;
        }
        if (hitWallY(samus->y - 1, SAMUS_LEFT, SAMUS_RIGHT, 1, 't')) {
            samus->y = backupY;
            samus->vel_x /= LANDING_X_VEL_DIVISOR;
            samus->vel_y = 0;
            if (dir < 0)
                samus->mode = rollingLeft;
            else
                samus->mode = rollingRight;
            return 0;
        }
    }
    return 1;
}

/* -----------------------------------------------------------------------
 * fallingBallLeft / fallingBallRight
 * ----------------------------------------------------------------------- */
void fallingBallLeft(void) {
    samus->frames = BALL_FRAMES;
    if (samus->beenHurt > 0) {
        if (--samus->beenHurt == 0) {
            ; /* no-op */
        } else {
            samus->vel_x = 0;
        }
    }
    if (game->keys.left) {
        game->keys.left = 2;
        samus->vel_x = IN_AIR_X_VEL;
    } else
        samus->vel_x = 0;
    layBomb();
    if (fallingBallCollision(-1) == 0)
        return;
    samus->vel_y += FALLING_GRAVITY;
    if (samus->vel_y > TERMINAL_VELOCITY)
        samus->vel_y = TERMINAL_VELOCITY;
    if (game->keys.up == 1) {
        game->keys.up = 2;
        if (!hitWallY(KNEEL_HEAD, SAMUS_LEFT, SAMUS_RIGHT, 1, 'b')) {
            samus->mode = fallingLeft;
            samus->sprt = &sprites->samus.leftFall;
            samus->vel_y /= 3;
            return;
        }
    }
    if (game->keys.right == 1) {
        game->keys.right = 2;
        samus->vel_x = 0;
        samus->mode = fallingBallRight;
        return;
    }
}

void fallingBallRight(void) {
    samus->frames = BALL_FRAMES;
    if (samus->beenHurt > 0) {
        if (--samus->beenHurt == 0) {
            ; /* no-op */
        } else {
            samus->vel_x = 0;
        }
    }
    if (game->keys.right) {
        game->keys.right = 2;
        samus->vel_x = IN_AIR_X_VEL;
    } else
        samus->vel_x = 0;
    layBomb();
    if (fallingBallCollision(1) == 0)
        return;
    samus->vel_y += FALLING_GRAVITY;
    if (samus->vel_y > TERMINAL_VELOCITY)
        samus->vel_y = TERMINAL_VELOCITY;
    if (game->keys.up == 1) {
        game->keys.up = 2;
        if (!hitWallY(KNEEL_HEAD, SAMUS_LEFT, SAMUS_RIGHT, 1, 'b')) {
            samus->mode = fallingRight;
            samus->sprt = &sprites->samus.rightFall;
            samus->vel_y /= 3;
            return;
        }
    }
    if (game->keys.left == 1) {
        game->keys.left = 2;
        samus->vel_x = 0;
        samus->mode = fallingBallLeft;
        return;
    }
}

/* -----------------------------------------------------------------------
 * bouncingCollision - collision while morphball is bouncing upward (bomb jump)
 * ----------------------------------------------------------------------- */
int bouncingCollision(int dir) {
    int moved_y = 0, moved_x = 0;
    while (moved_y < samus->vel_y / 3 || moved_x < samus->vel_x) {
        int backupX = samus->x;
        int backupY = samus->y;
        if (++moved_y <= samus->vel_y / 3)
            samus->y--;
        if (++moved_x <= samus->vel_x)
            samus->x += dir;
        if (hitWallX(SAMUS_LEFT, SAMUS_FEET, MORPH_TOP, 1, 'l') ||
            hitWallX(SAMUS_RIGHT, SAMUS_FEET, MORPH_TOP, 1, 'r')) {
            samus->x = backupX;
            samus->vel_x = 0;
        }
        if (hitWallY(MORPH_TOP, SAMUS_LEFT, SAMUS_RIGHT, 1, 'b')) {
            samus->y = backupY;
            samus->vel_x /= FALLING_X_VEL_DIVISOR;
            samus->vel_y = 0;
            if (dir < 0)
                samus->mode = fallingBallLeft;
            else
                samus->mode = fallingBallRight;
            return 0;
        }
    }
    return 1;
}

/* -----------------------------------------------------------------------
 * bouncingLeft / bouncingRight - morphball bouncing state (from bomb jump)
 * ----------------------------------------------------------------------- */
void bouncingLeft(void) {
    samus->frames = BALL_FRAMES;
    if (samus->beenHurt > 0) {
        if (--samus->beenHurt == 0) {
            samus->mode = fallingBallLeft;
        } else {
            samus->vel_x = 0;
        }
    }
    if (game->keys.left) {
        game->keys.left = 2;
        samus->vel_x = IN_AIR_X_VEL;
    } else
        samus->vel_x = 0;
    layBomb();
    if (!bouncingCollision(-1))
        return;
    samus->vel_y -= JUMPING_GRAVITY;
    if (game->keys.up == 1) {
        game->keys.up = 2;
        if (!hitWallY(KNEEL_HEAD, SAMUS_LEFT, SAMUS_RIGHT, 1, 'b')) {
            samus->mode = jumpingLeft;
            samus->sprt = &sprites->samus.leftJump1;
            samus->vel_y /= 3;
            return;
        }
    }
    if (game->keys.right == 1) {
        game->keys.right = 2;
        samus->vel_x = 0;
        samus->mode = bouncingRight;
        return;
    }
    if (samus->vel_y <= 0) {
        samus->vel_y = 0;
        samus->mode = fallingBallLeft;
        return;
    }
}

void bouncingRight(void) {
    samus->frames = BALL_FRAMES;
    if (samus->beenHurt > 0) {
        if (--samus->beenHurt == 0) {
            samus->mode = fallingBallRight;
        } else {
            samus->vel_x = 0;
        }
    }
    if (game->keys.right) {
        game->keys.right = 2;
        samus->vel_x = IN_AIR_X_VEL;
    } else
        samus->vel_x = 0;
    layBomb();
    if (!bouncingCollision(1))
        return;
    samus->vel_y -= JUMPING_GRAVITY;
    if (game->keys.up == 1) {
        game->keys.up = 2;
        if (!hitWallY(KNEEL_HEAD, SAMUS_LEFT, SAMUS_RIGHT, 1, 'b')) {
            samus->mode = jumpingRight;
            samus->sprt = &sprites->samus.rightJump1;
            samus->vel_y /= 3;
            return;
        }
    }
    if (game->keys.left == 1) {
        game->keys.left = 2;
        samus->vel_x = 0;
        samus->mode = bouncingLeft;
        return;
    }
    if (samus->vel_y <= 0) {
        samus->vel_y = 0;
        samus->mode = fallingBallRight;
        return;
    }
}

/* -----------------------------------------------------------------------
 * Helper: check raw SDL key state (replaces _keytest(RR_xxx))
 * ----------------------------------------------------------------------- */
static int raw_key(SDL_Scancode sc) {
    const uint8_t *state = SDL_GetKeyboardState(NULL);
    return state[sc] != 0;
}

/* -----------------------------------------------------------------------
 * spiderLeft / spiderRight / spiderUp / spiderDown
 *
 * The original uses _keytest(RR_xxx) for raw (non-edge-detected) key checks
 * in spider mode. We use raw_key() with the SDL scancodes that correspond
 * to the original TI-89 keys:
 *   RR_LEFT   -> SDL_SCANCODE_LEFT
 *   RR_RIGHT  -> SDL_SCANCODE_RIGHT
 *   RR_UP     -> SDL_SCANCODE_UP
 *   RR_DOWN   -> SDL_SCANCODE_DOWN
 *   RR_SHIFT  -> SDL_SCANCODE_C (sprint key)
 * ----------------------------------------------------------------------- */
void spiderLeft(void) {
    if (raw_key(SDL_SCANCODE_LEFT) ||
        ((samus->vel_y == -1 && raw_key(SDL_SCANCODE_UP)) ||
         (samus->vel_y == 1 && raw_key(SDL_SCANCODE_DOWN)))) {
        samus->vel_x++;
        if (samus->vel_x > SPIDER_MAX)
            samus->vel_x = SPIDER_MAX;
    } else {
        samus->vel_x = 0;
        if (raw_key(SDL_SCANCODE_RIGHT)) {
            samus->vel_y *= -1;
            samus->mode = spiderRight;
            spiderRight();
            return;
        }
    }
    layBomb();
    if (samus->vel_x)
        spider(-1);
    if (!raw_key(SDL_SCANCODE_C))
        samus->mode = rollingLeft;
}

void spiderRight(void) {
    if (raw_key(SDL_SCANCODE_RIGHT) ||
        ((samus->vel_y == -1 && raw_key(SDL_SCANCODE_UP)) ||
         (samus->vel_y == 1 && raw_key(SDL_SCANCODE_DOWN)))) {
        samus->vel_x++;
        if (samus->vel_x > SPIDER_MAX)
            samus->vel_x = SPIDER_MAX;
    } else {
        samus->vel_x = 0;
        if (raw_key(SDL_SCANCODE_LEFT)) {
            samus->vel_y *= -1;
            samus->mode = spiderLeft;
            spiderLeft();
            return;
        }
    }
    layBomb();
    if (samus->vel_x)
        spider(1);
    if (!raw_key(SDL_SCANCODE_C))
        samus->mode = rollingRight;
}

void spiderUp(void) {
    if (raw_key(SDL_SCANCODE_UP) ||
        ((samus->vel_x == 1 && raw_key(SDL_SCANCODE_RIGHT)) ||
         (samus->vel_x == -1 && raw_key(SDL_SCANCODE_LEFT)))) {
        samus->vel_y++;
        if (samus->vel_y > SPIDER_MAX)
            samus->vel_y = SPIDER_MAX;
    } else {
        samus->vel_y = 0;
        if (raw_key(SDL_SCANCODE_DOWN)) {
            samus->vel_x *= -1;
            samus->mode = spiderDown;
            spiderDown();
            return;
        }
    }
    layBomb();
    if (samus->vel_y)
        spider(-1);
    if (!raw_key(SDL_SCANCODE_C)) {
        if (samus->spiderWall == 2)
            samus->mode = morphedLeft;
        else
            samus->mode = morphedRight;
    }
}

void spiderDown(void) {
    if (raw_key(SDL_SCANCODE_DOWN) ||
        ((samus->vel_x == 1 && raw_key(SDL_SCANCODE_RIGHT)) ||
         (samus->vel_x == -1 && raw_key(SDL_SCANCODE_LEFT)))) {
        samus->vel_y++;
        if (samus->vel_y > SPIDER_MAX)
            samus->vel_y = SPIDER_MAX;
    } else {
        samus->vel_x = 0;
        samus->vel_y = 0;
        if (raw_key(SDL_SCANCODE_UP)) {
            samus->mode = spiderUp;
            spiderUp();
            return;
        }
    }
    layBomb();
    if (samus->vel_y)
        spider(1);
    if (!raw_key(SDL_SCANCODE_C)) {
        if (samus->spiderWall == 2)
            samus->mode = morphedLeft;
        else
            samus->mode = morphedRight;
    }
}

/* -----------------------------------------------------------------------
 * spider(dir) - main spiderball movement logic
 *
 * Uses the pointer-aliasing trick: depending on spiderWall orientation
 * (0=floor, 1=ceiling, 2=left wall, 3=right wall), x/y and vel pointers
 * are swapped so the same movement code works for both axes.
 * ----------------------------------------------------------------------- */
void spider(char dir) {
    int *x = &samus->x, *y = &samus->y, *vel = &samus->vel_x, *v = &samus->y;
    switch (samus->spiderWall) {
        case 2:
        case 3:
            x = &samus->y;
            y = &samus->x;
            vel = &samus->vel_y;
            v = &samus->x;
            break;
    }
    int i;
    for (i = 0; i < *vel; i++) {
        int backupX = *x;
        int backupY = *y;
        int backupV = *v;
        (void)backupV; /* suppress unused warning; original used it only conceptually */
        (*x) += dir;
        int cx, cy;
        cx = samus->x + 8;
        cy = samus->y - 1;
        switch (samus->spiderWall) {
            case 1:
                cy = samus->y - 8;
                break;
            case 2:
                cy = samus->y - 5;
                cx = samus->x + 4;
                break;
            case 3:
                cy = samus->y - 5;
                cx = samus->x + 11;
                break;
        }
        if (!groundHug(cx, cy, v, samus->spiderWall)) {
            /* Lost contact with surface -- wrap around corner */
            switch (samus->spiderWall) {
                case 0:
                    samus->mode = spiderDown;
                    samus->vel_y = samus->vel_x;
                    if (dir < 0) {
                        samus->spiderWall = 3;
                        samus->y = cy + 5;
                        samus->x = cx - 11;
                    } else {
                        samus->spiderWall = 2;
                        samus->y = cy + 5;
                        samus->x = cx - 4;
                    }
                    break;
                case 1:
                    samus->mode = spiderUp;
                    samus->vel_y = samus->vel_x;
                    if (dir < 0) {
                        samus->spiderWall = 3;
                        samus->y = cy + 5;
                        samus->x = cx - 11;
                    } else {
                        samus->spiderWall = 2;
                        samus->y = cy + 5;
                        samus->x = cx - 4;
                    }
                    break;
                case 2:
                    samus->mode = spiderLeft;
                    samus->vel_x = samus->vel_y;
                    if (dir < 0) {
                        samus->spiderWall = 0;
                        samus->x = cx - 8;
                        samus->y = cy + 1;
                    } else {
                        samus->spiderWall = 1;
                        samus->x = cx - 8;
                        samus->y = cy + 8;
                    }
                    break;
                case 3:
                    samus->mode = spiderRight;
                    samus->vel_x = samus->vel_y;
                    if (dir < 0) {
                        samus->spiderWall = 0;
                        samus->x = cx - 8;
                        samus->y = cy + 1;
                    } else {
                        samus->spiderWall = 1;
                        samus->x = cx - 8;
                        samus->y = cy + 8;
                    }
                    break;
            }
            *vel = dir;
            return;
        }
        /* Check if we hit a wall (inner corner) */
        char wall = 0;
        switch (samus->spiderWall) {
            case 0:
                if ((dir < 0 && hitWallX(SAMUS_LEFT, SAMUS_FEET, MORPH_TOP, 1, 'l')) ||
                    (dir > 0 && hitWallX(SAMUS_RIGHT, SAMUS_FEET, MORPH_TOP, 1, 'r'))) {
                    samus->mode = spiderUp;
                    samus->vel_y = samus->vel_x;
                    if (dir < 0) {
                        samus->spiderWall = 2;
                    } else {
                        samus->spiderWall = 3;
                    }
                    wall = 1;
                }
                break;
            case 1:
                if ((dir < 0 && hitWallX(SAMUS_LEFT, SAMUS_FEET + 4, MORPH_TOP + 4, 1, 'l')) ||
                    (dir > 0 && hitWallX(SAMUS_RIGHT, SAMUS_FEET + 4, MORPH_TOP + 4, 1, 'r'))) {
                    samus->mode = spiderDown;
                    samus->vel_y = samus->vel_x;
                    if (dir < 0) {
                        samus->spiderWall = 2;
                    } else {
                        samus->spiderWall = 3;
                    }
                    wall = 1;
                }
                break;
            case 2:
                if ((dir < 0 && hitWallY(MORPH_TOP, SAMUS_LEFT + 4, SAMUS_RIGHT, 1, 'b')) ||
                    (dir > 0 && hitWallY(SAMUS_FEET + 4, SAMUS_LEFT + 4, SAMUS_RIGHT, 1, 't'))) {
                    samus->mode = spiderRight;
                    samus->vel_x = samus->vel_y;
                    if (dir < 0) {
                        samus->spiderWall = 1;
                    } else {
                        samus->spiderWall = 0;
                    }
                    wall = 1;
                }
                break;
            case 3:
                if ((dir < 0 && hitWallY(MORPH_TOP, SAMUS_LEFT, SAMUS_RIGHT - 4, 1, 'b')) ||
                    (dir > 0 && hitWallY(SAMUS_FEET + 4, SAMUS_LEFT, SAMUS_RIGHT - 4, 1, 't'))) {
                    samus->mode = spiderLeft;
                    samus->vel_x = samus->vel_y;
                    if (dir < 0) {
                        samus->spiderWall = 1;
                    } else {
                        samus->spiderWall = 0;
                    }
                    wall = 1;
                }
                break;
        }
        if (wall) {
            *x = backupX;
            *vel = dir;
            return;
        }
    }
}

/* -----------------------------------------------------------------------
 * drawSamus - render samus sprite
 *
 * Original used GrayClipSprite16_OR_R for 16-wide sprites.
 * In the port, we use blit_sprite_16 (OR mode, no mask) via
 * the blit_sprite_8_or / blit_sprite_16 functions from render.h.
 *
 * The original swapped virtual planes for flicker when hurt/invincible.
 * We approximate that by using blit_sprite_16 with the mask pointer
 * (NULL for OR mode) and adjusting which planes we pass.
 * ----------------------------------------------------------------------- */
void drawSamus(void) {
    int sx = samus->x - cam->x;
    int sy = samus->y - cam->y - samus->sprt->h;
    int h = samus->sprt->h;

    /* When hurt or invincible, alternate visibility by swapping planes
     * (original used v0/v1 virtual plane trick for flicker) */
    /* All samus sprites (including 8-wide morphball) are stored as uint16_t
     * arrays in the original TI-89 code. The ball sprites are 8 pixels wide
     * but use 16-bit storage with the pixel data in bits 15-8. So we always
     * use blit_sprite_16 for samus. */
    blit_sprite_16(sx, sy, h,
                   (const uint16_t *)samus->sprt->s1,
                   (const uint16_t *)samus->sprt->s2,
                   NULL);  /* NULL mask = OR mode */
}

/* -----------------------------------------------------------------------
 * groundHug - follow terrain contour for spiderball
 *
 * cx, cy : the center/contact pixel to check
 * v      : pointer to the coordinate to adjust (samus->y for floor/ceiling,
 *          samus->x for walls)
 * wall   : 0=floor, 1=ceiling, 2=left wall, 3=right wall
 *
 * Returns 1 if surface found (and adjusts *v), 0 if no surface.
 * ----------------------------------------------------------------------- */
int groundHug(int cx, int cy, int *v, int wall) {
    switch (wall) {
        case 0: {
            const unsigned char *m = TILE_AT(cy / 8, cx / 8).m;
            if (m != NULL) {
                *v = m[0 + cx % 8] + (cy / 8) * 8 + (*v - cy - 1);
            } else {
                m = TILE_AT((cy + 8) / 8, cx / 8).m;
                if (m != NULL) {
                    *v = m[0 + cx % 8] + ((cy + 8) / 8) * 8 + (*v - cy - 1);
                } else {
                    return 0;
                }
            }
            break;
        }
        case 1: {
            const unsigned char *m = TILE_AT(cy / 8, cx / 8).m;
            if (m != NULL) {
                *v = m[8 + cx % 8] + (cy / 8) * 8 + (*v - cy + 1);
            } else {
                m = TILE_AT((cy - 8) / 8, cx / 8).m;
                if (m != NULL) {
                    *v = m[8 + cx % 8] + ((cy - 8) / 8) * 8 + (*v - cy + 1);
                } else {
                    return 0;
                }
            }
            break;
        }
        case 2: {
            const unsigned char *m = TILE_AT(cy / 8, cx / 8).m;
            if (m != NULL) {
                *v = m[16 + cy % 8] + (cx / 8) * 8 + (*v - cx + 1);
            } else {
                m = TILE_AT(cy / 8, (cx - 8) / 8).m;
                if (m != NULL) {
                    *v = m[16 + cy % 8] + ((cx - 8) / 8) * 8 + (*v - cx + 1);
                } else {
                    return 0;
                }
            }
            break;
        }
        case 3: {
            const unsigned char *m = TILE_AT(cy / 8, cx / 8).m;
            if (m != NULL) {
                *v = m[24 + cy % 8] + (cx / 8) * 8 + (*v - cx - 1);
            } else {
                m = TILE_AT(cy / 8, (cx + 8) / 8).m;
                if (m != NULL) {
                    *v = m[24 + cy % 8] + ((cx + 8) / 8) * 8 + (*v - cx - 1);
                } else {
                    return 0;
                }
            }
            break;
        }
    }
    return 1;
}

/* -----------------------------------------------------------------------
 * hitSamus - check if a rectangle (x,y,w,h) overlaps samus
 * ----------------------------------------------------------------------- */
int hitSamus(int x, int y, int w, int h) {
    return boundingBoxCollide(x, y, w, h,
                              samus->x, samus->y, samus->sprt->w, samus->sprt->h, 1);
}

/* -----------------------------------------------------------------------
 * boundingBoxCollide - general AABB collision with multi-point check
 *
 * Checks 10 test points (corners, midpoints, third-points) of one box
 * against the other. If t==1, also tries the reverse (box2 vs box1).
 * ----------------------------------------------------------------------- */
int boundingBoxCollide(int x, int y, int w, int h,
                       int x2, int y2, int w2, int h2, int t) {
    int r =
        (x < x2 && x + w > x2 && y > y2 && y - h < y2) ||
        (x < x2 + w2 && x + w > x2 + w2 && y > y2 && y - h < y2) ||
        (x < x2 && x + w > x2 && y > y2 - h2 && y - h < y2 - h2) ||
        (x < x2 + w2 && x + w > x2 + w2 && y > y2 - h2 && y - h < y2 - h2) ||
        (x < x2 + w2 / 2 && x + w > x2 + w2 / 2 && y > y2 && y - h < y2) ||
        (x < x2 + w2 / 2 && x + w > x2 + w2 / 2 && y > y2 - h2 && y - h < y2 - h2) ||
        (x < x2 && x + w > x2 && y > y2 - h2 / 3 && y - h < y2 - h2 / 3) ||
        (x < x2 + w2 && x + w > x2 + w2 && y > y2 - h2 / 3 && y - h < y2 - h2 / 3) ||
        (x < x2 && x + w > x2 && y > y2 - 2 * h2 / 3 && y - h < y2 - 2 * h2 / 3) ||
        (x < x2 + w2 && x + w > x2 + w2 && y > y2 - 2 * h2 / 3 && y - h < y2 - 2 * h2 / 3);

    return t > 1 ? r : (r ? r : boundingBoxCollide(x2, y2, w2, h2, x, y, w, h, 2));
}

/* -----------------------------------------------------------------------
 * hurtSamus - apply damage with energy tank overflow
 * ----------------------------------------------------------------------- */
void hurtSamus(int ATK) {
    if (samus->beenHurt > 0 || samus->invincible != 0)
        return;
    samus->beenHurt = HURT_LENGTH;
    samus->HP -= ATK;
    while (samus->HP < 0) {
        if (samus->items.tanks_full > 0) {
            samus->items.tanks_full--;
            samus->HP += 100;
        } else {
            /* Game over -- original called exit(0) after waiting for ENTER.
             * In the SDL2 port, we set HP to 0 and let the main loop handle it. */
            samus->HP = 0;
            break;
        }
    }
}
