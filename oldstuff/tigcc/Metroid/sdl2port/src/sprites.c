/*
 * sprites.c - Sprite initialization for Metroid TI-89 SDL2 port
 * Ported from original TIGCC Sprites.c
 *
 * Every sprite definition from the original has been ported.
 * gfx-> references are replaced with gfx_data. (global struct, not pointer).
 * sprites-> self-references for animation chains remain as-is since
 * sprites is heap-allocated and the addresses are stable after memcpy.
 */
#include <stdlib.h>
#include <string.h>

#include "sprites.h"
#include "gfx_data.h"

/* These are declared in game.c */
extern struct graphics* gfx;
extern struct sprites* sprites;

void makeSprites(void) {
    sprites = malloc(sizeof(struct sprites));
    if (!sprites) return;

    struct sprites s = {
        /* ===== samus sprites (43 total) ===== */
        {
            /* ball1 */ {8, 8, (void*)&gfx_data.samus.ball1_C0, (void*)&gfx_data.samus.ball1_C1, NULL, &sprites->samus.ball2},
            /* ball2 */ {8, 8, (void*)&gfx_data.samus.ball2_C0, (void*)&gfx_data.samus.ball2_C1, NULL, &sprites->samus.ball3},
            /* ball3 */ {8, 8, (void*)&gfx_data.samus.ball3_C0, (void*)&gfx_data.samus.ball3_C1, NULL, &sprites->samus.ball1},
            /* leftFall */ {20, 12, (void*)&gfx_data.samus.leftFall_C0, (void*)&gfx_data.samus.leftFall_C1, NULL, &sprites->samus.leftFall},
            /* leftFlip1 */ {15, 14, (void*)&gfx_data.samus.leftFlip1_C0, (void*)&gfx_data.samus.leftFlip1_C1, NULL, &sprites->samus.leftFlip2},
            /* leftFlip2 */ {15, 15, (void*)&gfx_data.samus.leftFlip2_C0, (void*)&gfx_data.samus.leftFlip2_C1, NULL, &sprites->samus.leftFlip3},
            /* leftFlip3 */ {15, 14, (void*)&gfx_data.samus.leftFlip3_C0, (void*)&gfx_data.samus.leftFlip3_C1, NULL, &sprites->samus.leftFlip4},
            /* leftFlip4 */ {15, 15, (void*)&gfx_data.samus.leftFlip4_C0, (void*)&gfx_data.samus.leftFlip4_C1, NULL, &sprites->samus.leftFlip1},
            /* leftHurt */ {26, 13, (void*)&gfx_data.samus.leftHurt_C0, (void*)&gfx_data.samus.leftHurt_C1, NULL, &sprites->samus.leftHurt},
            /* leftJump1 */ {24, 10, (void*)&gfx_data.samus.leftJump1_C0, (void*)&gfx_data.samus.leftJump1_C1, NULL, &sprites->samus.leftJump2},
            /* leftJump2 */ {25, 13, (void*)&gfx_data.samus.leftJump2_C0, (void*)&gfx_data.samus.leftJump2_C1, NULL, &sprites->samus.leftJump2},
            /* leftKneel */ {18, 12, (void*)&gfx_data.samus.leftKneel_C0, (void*)&gfx_data.samus.leftKneel_C1, NULL, &sprites->samus.leftKneel},
            /* leftLand */ {25, 12, (void*)&gfx_data.samus.leftLand_C0, (void*)&gfx_data.samus.leftLand_C1, NULL, &sprites->samus.leftStand},
            /* leftMorph */ {12, 9, (void*)&gfx_data.samus.leftMorph_C0, (void*)&gfx_data.samus.leftMorph_C1, NULL, &sprites->samus.ball1},
            /* leftRun1 */ {23, 10, (void*)&gfx_data.samus.leftRun1_C0, (void*)&gfx_data.samus.leftRun1_C1, NULL, &sprites->samus.leftRun2},
            /* leftRun2 */ {24, 16, (void*)&gfx_data.samus.leftRun2_C0, (void*)&gfx_data.samus.leftRun2_C1, NULL, &sprites->samus.leftRun3},
            /* leftRun3 */ {23, 16, (void*)&gfx_data.samus.leftRun3_C0, (void*)&gfx_data.samus.leftRun3_C1, NULL, &sprites->samus.leftRun4},
            /* leftRun4 */ {23, 12, (void*)&gfx_data.samus.leftRun4_C0, (void*)&gfx_data.samus.leftRun4_C1, NULL, &sprites->samus.leftRun5},
            /* leftRun5 */ {23, 12, (void*)&gfx_data.samus.leftRun5_C0, (void*)&gfx_data.samus.leftRun5_C1, NULL, &sprites->samus.leftRun1},
            /* leftStand */ {23, 10, (void*)&gfx_data.samus.leftStand_C0, (void*)&gfx_data.samus.leftStand_C1, NULL, &sprites->samus.leftStand},
            /* rightFall */ {20, 12, (void*)&gfx_data.samus.rightFall_C0, (void*)&gfx_data.samus.rightFall_C1, NULL, &sprites->samus.rightFall},
            /* rightFlip1 */ {15, 14, (void*)&gfx_data.samus.rightFlip1_C0, (void*)&gfx_data.samus.rightFlip1_C1, NULL, &sprites->samus.rightFlip2},
            /* rightFlip2 */ {15, 15, (void*)&gfx_data.samus.rightFlip2_C0, (void*)&gfx_data.samus.rightFlip2_C1, NULL, &sprites->samus.rightFlip3},
            /* rightFlip3 */ {15, 14, (void*)&gfx_data.samus.rightFlip3_C0, (void*)&gfx_data.samus.rightFlip3_C1, NULL, &sprites->samus.rightFlip4},
            /* rightFlip4 */ {15, 15, (void*)&gfx_data.samus.rightFlip4_C0, (void*)&gfx_data.samus.rightFlip4_C1, NULL, &sprites->samus.rightFlip1},
            /* rightHurt */ {26, 13, (void*)&gfx_data.samus.rightHurt_C0, (void*)&gfx_data.samus.rightHurt_C1, NULL, &sprites->samus.rightHurt},
            /* rightJump1 */ {24, 10, (void*)&gfx_data.samus.rightJump1_C0, (void*)&gfx_data.samus.rightJump1_C1, NULL, &sprites->samus.rightJump2},
            /* rightJump2 */ {25, 13, (void*)&gfx_data.samus.rightJump2_C0, (void*)&gfx_data.samus.rightJump2_C1, NULL, &sprites->samus.rightJump2},
            /* rightKneel */ {18, 12, (void*)&gfx_data.samus.rightKneel_C0, (void*)&gfx_data.samus.rightKneel_C1, NULL, &sprites->samus.rightKneel},
            /* rightLand */ {25, 12, (void*)&gfx_data.samus.rightLand_C0, (void*)&gfx_data.samus.rightLand_C1, NULL, &sprites->samus.rightStand},
            /* rightMorph */ {12, 9, (void*)&gfx_data.samus.rightMorph_C0, (void*)&gfx_data.samus.rightMorph_C1, NULL, &sprites->samus.ball1},
            /* rightRun1 */ {23, 10, (void*)&gfx_data.samus.rightRun1_C0, (void*)&gfx_data.samus.rightRun1_C1, NULL, &sprites->samus.rightRun2},
            /* rightRun2 */ {24, 16, (void*)&gfx_data.samus.rightRun2_C0, (void*)&gfx_data.samus.rightRun2_C1, NULL, &sprites->samus.rightRun3},
            /* rightRun3 */ {23, 16, (void*)&gfx_data.samus.rightRun3_C0, (void*)&gfx_data.samus.rightRun3_C1, NULL, &sprites->samus.rightRun4},
            /* rightRun4 */ {23, 12, (void*)&gfx_data.samus.rightRun4_C0, (void*)&gfx_data.samus.rightRun4_C1, NULL, &sprites->samus.rightRun5},
            /* rightRun5 */ {23, 12, (void*)&gfx_data.samus.rightRun5_C0, (void*)&gfx_data.samus.rightRun5_C1, NULL, &sprites->samus.rightRun1},
            /* rightStand */ {23, 10, (void*)&gfx_data.samus.rightStand_C0, (void*)&gfx_data.samus.rightStand_C1, NULL, &sprites->samus.rightStand},
            /* rightWallJump */ {17, 16, (void*)&gfx_data.samus.rightWallJump_C0, (void*)&gfx_data.samus.rightWallJump_C1, NULL, &sprites->samus.rightWallJump},
            /* leftWallJump */ {16, 16, (void*)&gfx_data.samus.leftWallJump_C0, (void*)&gfx_data.samus.leftWallJump_C1, NULL, &sprites->samus.leftWallJump},
            /* spaceJump4 */ {16, 16, (void*)&gfx_data.samus.spaceJump4_C0, (void*)&gfx_data.samus.spaceJump4_C1, NULL, &sprites->samus.spaceJump1},
            /* spaceJump3 */ {13, 16, (void*)&gfx_data.samus.spaceJump3_C0, (void*)&gfx_data.samus.spaceJump3_C1, NULL, &sprites->samus.spaceJump2},
            /* spaceJump2 */ {16, 16, (void*)&gfx_data.samus.spaceJump2_C0, (void*)&gfx_data.samus.spaceJump2_C1, NULL, &sprites->samus.spaceJump3},
            /* spaceJump1 */ {13, 16, (void*)&gfx_data.samus.spaceJump1_C0, (void*)&gfx_data.samus.spaceJump1_C1, NULL, &sprites->samus.spaceJump4}
        },
        /* ===== enemy sprites (23 total) ===== */
        {
            /* waver */ {8, 16, (void*)&gfx_data.enemies.waver_C0, (void*)&gfx_data.enemies.waver_C1, NULL, &sprites->enemies.waver},
            /* sideHopper */ {12, 16, (void*)&gfx_data.enemies.sideHopper_C0, (void*)&gfx_data.enemies.sideHopper_C1, NULL, &sprites->enemies.sideHopper},
            /* ripper */ {8, 16, (void*)&gfx_data.enemies.ripper_C0, (void*)&gfx_data.enemies.ripper_C1, NULL, &sprites->enemies.ripper},
            /* ripper2 */ {8, 16, (void*)&gfx_data.enemies.ripper2_C0, (void*)&gfx_data.enemies.ripper2_C1, NULL, &sprites->enemies.ripper2},
            /* SpacePirateWalk1Left */ {19, 16, (void*)&gfx_data.enemies.SpacePirateWalk1Left_C0, (void*)&gfx_data.enemies.SpacePirateWalk1Left_C1, NULL, &sprites->enemies.SpacePirateWalk2Left},
            /* SpacePirateWalk2Left */ {20, 16, (void*)&gfx_data.enemies.SpacePirateWalk2Left_C0, (void*)&gfx_data.enemies.SpacePirateWalk2Left_C1, NULL, &sprites->enemies.SpacePirateWalk1Left},
            /* SpacePirateStandLeft */ {19, 16, (void*)&gfx_data.enemies.SpacePirateStandLeft_C0, (void*)&gfx_data.enemies.SpacePirateStandLeft_C1, NULL, &sprites->enemies.SpacePirateStandLeft},
            /* SpacePirateLookLeft */ {19, 16, (void*)&gfx_data.enemies.SpacePirateLookLeft_C0, (void*)&gfx_data.enemies.SpacePirateLookLeft_C1, NULL, &sprites->enemies.SpacePirateStandLeft},
            /* SpacePirateShootLeft */ {17, 16, (void*)&gfx_data.enemies.SpacePirateShootLeft_C0, (void*)&gfx_data.enemies.SpacePirateShootLeft_C1, NULL, &sprites->enemies.SpacePirateStandLeft},
            /* SpacePirateWalk1Right */ {19, 16, (void*)&gfx_data.enemies.SpacePirateWalk1Right_C0, (void*)&gfx_data.enemies.SpacePirateWalk1Right_C1, NULL, &sprites->enemies.SpacePirateWalk2Right},
            /* SpacePirateWalk2Right */ {20, 16, (void*)&gfx_data.enemies.SpacePirateWalk2Right_C0, (void*)&gfx_data.enemies.SpacePirateWalk2Right_C1, NULL, &sprites->enemies.SpacePirateWalk1Right},
            /* SpacePirateStandRight */ {19, 16, (void*)&gfx_data.enemies.SpacePirateStandRight_C0, (void*)&gfx_data.enemies.SpacePirateStandRight_C1, NULL, &sprites->enemies.SpacePirateStandRight},
            /* SpacePirateLookRight */ {19, 16, (void*)&gfx_data.enemies.SpacePirateLookRight_C0, (void*)&gfx_data.enemies.SpacePirateLookRight_C1, NULL, &sprites->enemies.SpacePirateStandRight},
            /* SpacePirateShootRight */ {17, 16, (void*)&gfx_data.enemies.SpacePirateShootRight_C0, (void*)&gfx_data.enemies.SpacePirateShootRight_C1, NULL, &sprites->enemies.SpacePirateStandRight},
            /* SpacePirateLaser */ {3, 16, (void*)&gfx_data.enemies.SpacePirateLaser_C0, (void*)&gfx_data.enemies.SpacePirateLaser_C1, NULL, &sprites->enemies.SpacePirateLaser},
            /* skree */ {11, 8, (void*)&gfx_data.enemies.skree_C0, (void*)&gfx_data.enemies.skree_C1, NULL, &sprites->enemies.skree},
            /* skree2 */ {11, 8, (void*)&gfx_data.enemies.skree2_C0, (void*)&gfx_data.enemies.skree2_C1, NULL, &sprites->enemies.skree3},
            /* skree3 */ {11, 8, (void*)&gfx_data.enemies.skree3_C0, (void*)&gfx_data.enemies.skree3_C1, NULL, &sprites->enemies.skree4},
            /* skree4 */ {11, 8, (void*)&gfx_data.enemies.skree4_C0, (void*)&gfx_data.enemies.skree4_C1, NULL, &sprites->enemies.skree2},
            /* skreeProjectile */ {4, 4, (void*)&gfx_data.enemies.skreeProjectile_C0, (void*)&gfx_data.enemies.skreeProjectile_C1, NULL, &sprites->enemies.skreeProjectile},
            /* zebboL */ {7, 8, (void*)&gfx_data.enemies.zebboL_C0, (void*)&gfx_data.enemies.zebboL_C1, NULL, &sprites->enemies.zebboL},
            /* zebboR */ {7, 8, (void*)&gfx_data.enemies.zebboR_C0, (void*)&gfx_data.enemies.zebboR_C1, NULL, &sprites->enemies.zebboR},
            /* zebboSpawn */ {7, 8, (void*)&gfx_data.tiles.pipe1[0], (void*)&gfx_data.tiles.pipe1[1], (void*)&gfx_data.tiles.pipe1[2], &sprites->enemies.zebboSpawn}
        },
        /* ===== weapon sprites (9 total) ===== */
        {
            /* missile1 */ {8, 8, (void*)&gfx_data.weapons.missile1_C0, (void*)&gfx_data.weapons.missile1_C1, NULL, &sprites->weapons.missile1},
            /* missile2 */ {8, 8, (void*)&gfx_data.weapons.missile2_C0, (void*)&gfx_data.weapons.missile2_C1, NULL, &sprites->weapons.missile2},
            /* missile3 */ {8, 8, (void*)&gfx_data.weapons.missile3_C0, (void*)&gfx_data.weapons.missile3_C1, NULL, &sprites->weapons.missile3},
            /* missile4 */ {8, 8, (void*)&gfx_data.weapons.missile4_C0, (void*)&gfx_data.weapons.missile4_C1, NULL, &sprites->weapons.missile4},
            /* missile5 */ {5, 8, (void*)&gfx_data.weapons.missile5_C0, (void*)&gfx_data.weapons.missile5_C1, NULL, &sprites->weapons.missile5},
            /* missile6 */ {5, 8, (void*)&gfx_data.weapons.missile6_C0, (void*)&gfx_data.weapons.missile6_C1, NULL, &sprites->weapons.missile6},
            /* missile7 */ {8, 5, (void*)&gfx_data.weapons.missile7_C0, (void*)&gfx_data.weapons.missile7_C1, NULL, &sprites->weapons.missile7},
            /* missile8 */ {8, 5, (void*)&gfx_data.weapons.missile8_C0, (void*)&gfx_data.weapons.missile8_C1, NULL, &sprites->weapons.missile8},
            /* powerBeam */ {4, 4, (void*)&gfx_data.weapons.powerBeam_C0, (void*)&gfx_data.weapons.powerBeam_C1, NULL, &sprites->weapons.powerBeam}
        },
        /* ===== sfx sprites (10 total) ===== */
        {
            /* bombExplode */ {8, 8, (void*)&gfx_data.sfx.bombExplode1_C0, (void*)&gfx_data.sfx.bombExplode1_C1, NULL, &sprites->sfx.bombExplode2},
            /* bombExplode2 */ {8, 8, (void*)&gfx_data.sfx.bombExplode2_C0, (void*)&gfx_data.sfx.bombExplode2_C1, NULL, &sprites->sfx.bombExplode3},
            /* bombExplode3 */ {8, 8, (void*)&gfx_data.sfx.bombExplode3_C0, (void*)&gfx_data.sfx.bombExplode3_C1, NULL, &sprites->sfx.bombExplode2},
            /* breakAway */ {8, 8, (void*)&gfx_data.sfx.breakAway1_C0, (void*)&gfx_data.sfx.breakAway1_C1, NULL, &sprites->sfx.breakAway2},
            /* breakAway2 */ {8, 8, (void*)&gfx_data.sfx.breakAway2_C0, (void*)&gfx_data.sfx.breakAway2_C1, NULL, &sprites->sfx.breakAway3},
            /* breakAway3 */ {8, 8, (void*)&gfx_data.sfx.breakAway3_C0, (void*)&gfx_data.sfx.breakAway3_C1, NULL, &sprites->sfx.empty8x8},
            /* empty8x8 */ {8, 8, (void*)&gfx_data.tiles.empty8x8[0], (void*)&gfx_data.tiles.empty8x8[1], (void*)&gfx_data.tiles.empty8x8[2], &sprites->sfx.empty8x8},
            /* simpleEnemyExplosion */ {16, 16, (void*)&gfx_data.sfx.enemyX1_C0, (void*)&gfx_data.sfx.enemyX1_C1, (void*)&gfx_data.sfx.enemyX1_M0, &sprites->sfx.enemyX2},
            /* enemyX2 */ {16, 16, (void*)&gfx_data.sfx.enemyX2_C0, (void*)&gfx_data.sfx.enemyX2_C1, (void*)&gfx_data.sfx.enemyX2_M0, &sprites->sfx.enemyX3},
            /* enemyX3 */ {16, 16, (void*)&gfx_data.sfx.enemyX3_C0, (void*)&gfx_data.sfx.enemyX3_C1, (void*)&gfx_data.sfx.enemyX3_M0, &sprites->sfx.empty8x8}
        },
        /* ===== pickup sprites (9 total) ===== */
        {
            /* energy */ {8, 8, (void*)&gfx_data.pickups.energy1_C0, (void*)&gfx_data.pickups.energy1_C1, (void*)&gfx_data.pickups.energy1_M0, &sprites->pickups.energy2},
            /* energy2 */ {8, 8, (void*)&gfx_data.pickups.energy2_C0, (void*)&gfx_data.pickups.energy2_C1, (void*)&gfx_data.pickups.energy2_M0, &sprites->pickups.energy3},
            /* energy3 */ {8, 8, (void*)&gfx_data.pickups.energy3_C0, (void*)&gfx_data.pickups.energy3_C1, (void*)&gfx_data.pickups.energy3_M0, &sprites->pickups.energy},
            /* missile */ {8, 8, (void*)&gfx_data.pickups.missile_C0, (void*)&gfx_data.pickups.missile_C1, (void*)&gfx_data.pickups.missile_M0, &sprites->pickups.missile2},
            /* missile2 */ {8, 8, (void*)&gfx_data.pickups.missile_C1, (void*)&gfx_data.tiles.empty8x8[0], (void*)&gfx_data.pickups.missile_M0, &sprites->pickups.missile},
            /* superMissile */ {8, 8, (void*)&gfx_data.pickups.superMissile_C0, (void*)&gfx_data.pickups.superMissile_C1, (void*)&gfx_data.pickups.superMissile_M0, &sprites->pickups.superMissile2},
            /* superMissile2 */ {8, 8, (void*)&gfx_data.pickups.superMissile_C1, (void*)&gfx_data.tiles.empty8x8[0], (void*)&gfx_data.pickups.superMissile_M0, &sprites->pickups.superMissile},
            /* powerBomb */ {8, 8, (void*)&gfx_data.pickups.powerBomb_C0, (void*)&gfx_data.pickups.powerBomb_C1, (void*)&gfx_data.pickups.powerBomb_M0, &sprites->pickups.powerBomb2},
            /* powerBomb2 */ {8, 8, (void*)&gfx_data.pickups.powerBomb_C1, (void*)&gfx_data.tiles.empty8x8[0], (void*)&gfx_data.pickups.powerBomb_M0, &sprites->pickups.powerBomb}
        },
        /* ===== item sprites (4 total) ===== */
        {
            /* missile */ {8, 8, (void*)&gfx_data.tiles.missile[0], (void*)&gfx_data.tiles.missile[1], (void*)&gfx_data.tiles.missile[2], &sprites->items.missile},
            /* superMissile */ {8, 8, (void*)&gfx_data.tiles.superMissile[0], (void*)&gfx_data.tiles.superMissile[1], (void*)&gfx_data.tiles.superMissile[2], &sprites->items.superMissile},
            /* powerBomb */ {8, 8, (void*)&gfx_data.tiles.powerBomb[0], (void*)&gfx_data.tiles.powerBomb[1], (void*)&gfx_data.tiles.powerBomb[2], &sprites->items.powerBomb},
            /* energy */ {8, 8, (void*)&gfx_data.tiles.energyTank[0], (void*)&gfx_data.tiles.energyTank[1], (void*)&gfx_data.tiles.energyTank[2], &sprites->items.energy}
        }
    };

    memcpy(sprites, &s, sizeof(s));
}
