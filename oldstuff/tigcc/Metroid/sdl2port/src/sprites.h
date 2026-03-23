/*
 * sprites.h - Sprite struct and sprite catalog for Metroid TI-89 SDL2 port
 * Ported from original TIGCC Sprites.h
 */
#ifndef SPRITES_H
#define SPRITES_H

struct sprite {
    int h, w;
    void* s1;   /* Plane 0 data pointer */
    void* s2;   /* Plane 1 data pointer */
    void* m;    /* Mask data pointer (can be NULL) */
    struct sprite* link;  /* Next animation frame */
};

struct sprites {
    struct {
        struct sprite ball1, ball2, ball3;
        struct sprite leftFall;
        struct sprite leftFlip1, leftFlip2, leftFlip3, leftFlip4;
        struct sprite leftHurt;
        struct sprite leftJump1, leftJump2;
        struct sprite leftKneel;
        struct sprite leftLand;
        struct sprite leftMorph;
        struct sprite leftRun1, leftRun2, leftRun3, leftRun4, leftRun5;
        struct sprite leftStand;
        struct sprite rightFall;
        struct sprite rightFlip1, rightFlip2, rightFlip3, rightFlip4;
        struct sprite rightHurt;
        struct sprite rightJump1, rightJump2;
        struct sprite rightKneel;
        struct sprite rightLand;
        struct sprite rightMorph;
        struct sprite rightRun1, rightRun2, rightRun3, rightRun4, rightRun5;
        struct sprite rightStand;
        struct sprite rightWallJump, leftWallJump;
        struct sprite spaceJump4, spaceJump3, spaceJump2, spaceJump1;
    } samus;
    struct {
        struct sprite waver;
        struct sprite sideHopper;
        struct sprite ripper, ripper2;
        struct sprite SpacePirateWalk1Left, SpacePirateWalk2Left, SpacePirateStandLeft, SpacePirateLookLeft, SpacePirateShootLeft;
        struct sprite SpacePirateWalk1Right, SpacePirateWalk2Right, SpacePirateStandRight, SpacePirateLookRight, SpacePirateShootRight;
        struct sprite SpacePirateLaser;
        struct sprite skree, skree2, skree3, skree4;
        struct sprite skreeProjectile;
        struct sprite zebboL;
        struct sprite zebboR;
        struct sprite zebboSpawn;
    } enemies;
    struct {
        struct sprite missile1, missile2, missile3, missile4, missile5, missile6, missile7, missile8;
        struct sprite powerBeam;
    } weapons;
    struct {
        struct sprite bombExplode, bombExplode2, bombExplode3;
        struct sprite breakAway, breakAway2, breakAway3;
        struct sprite empty8x8;
        struct sprite simpleEnemyExplosion, enemyX2, enemyX3;
    } sfx;
    struct {
        struct sprite energy, energy2, energy3;
        struct sprite missile, missile2;
        struct sprite superMissile, superMissile2;
        struct sprite powerBomb, powerBomb2;
    } pickups;
    struct {
        struct sprite missile, superMissile, powerBomb, energy;
    } items;
};

void makeSprites(void);

#endif /* SPRITES_H */
