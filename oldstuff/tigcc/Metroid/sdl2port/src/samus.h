/*
 * samus.h - Player (Samus) data structures and declarations for Metroid TI-89 SDL2 port
 * Ported from original TIGCC Samus.h
 */
#ifndef SAMUS_H
#define SAMUS_H

struct sprite;
struct object;

struct samus {
    int x;
    int y;
    int vel_x;
    int vel_y;
    int beenHurt;
    int invincible;
    int HP;
    int frames;
    char spiderWall;
    struct sprite* sprt;
    void (*mode)(void);
    struct {
        int charge_equipped, charge_owned;
        int spazer_equipped, spazer_owned;
        int wave_equipped, wave_owned;
        int plasma_equipped, plasma_owned;
        int ice_equipped, ice_owned;
        int bomb_owned;
        int missile_ammo, missile_max;
        int superMissile_ammo, superMissile_max;
        int powerBomb_ammo, powerBomb_max;
        struct object* gun;
        struct object* bomb;
        struct object* beam;
        int charge;
        int wait;
    } weapons;
    struct {
        int tanks_owned, tanks_full;
        int springBall_owned, springBall_equipped;
        int hiJump_owned, hiJump_equipped;
    } items;
};

/* Physics constants */
#define RUN_INIT 1
#define RUN_MAX 3
#define SPRINT_MAX 8
#define ROLL_INIT 1
#define ROLL_MAX 3
#define SPIDER_MAX 3
#define JUMPING_GRAVITY 1
#define FALLING_GRAVITY 2
#define TERMINAL_VELOCITY 24
#define FALLING_X_VEL_DIVISOR 2
#define LANDING_X_VEL_DIVISOR 2
#define IN_AIR_X_VEL 2
#define JUMP_INIT_Y 24
#define WALL_JUMP_Y_VEL 20
#define WALL_JUMP_X_VEL 6
#define SPACE_JUMP_Y_VEL 15
#define SPACE_JUMP_CUTOFF 20
#define FLIP_CUTOFF 2
#define FLIP_FRAMES 2
#define BALL_FRAMES 2
#define RUN_FRAMES (10 - samus->vel_x) / 2
#define BOMB_BOUNCE 11
#define BOUNCE_DIVISOR 2
#define CHARGE_MAX 20
#define HURT_LENGTH 2
#define SHOT_VEL 4

/* Hitbox constants */
#define SAMUS_CRIT  (samus->x + 7)
#define SAMUS_LEFT  (samus->x + 4)
#define SAMUS_RIGHT (samus->x + 11)
#define SAMUS_HEAD  (samus->y - 22)
#define SAMUS_FEET  (samus->y - 5)
#define MORPH_TOP   (samus->y - 8)
#define FLIP_TOP    (samus->y - 14)
#define KNEEL_HEAD  (samus->y - 16)

/* Core functions */
void makeSamus(void);
void drawSamus(void);
int groundHug(int, int, int*, int);
void weaponsSetup(void);
void layBomb(void);
int shoot(int, int, int);

/* Collision / damage */
void checkEnemies(struct object*);
int hitSamus(int, int, int, int);
void hurtSamus(int);
int hitWallX(int, int, int, int, char);
int hitWallY(int, int, int, int, char);
int boundingBoxCollide(int, int, int, int, int, int, int, int, int);

/* State functions */
void standingLeft(void);
void standingRight(void);
void runningLeft(void);
void runningRight(void);
void kneelingLeft(void);
void kneelingRight(void);
void morphed(void);
void morphedLeft(void);
void morphedRight(void);
void rollingLeft(void);
void rollingRight(void);
void fallingBallLeft(void);
void fallingBallRight(void);
void bouncingLeft(void);
void bouncingRight(void);
void jumpingLeft(void);
void jumpingRight(void);
void fallingLeft(void);
void fallingRight(void);
void flippingUpLeft(void);
void flippingUpRight(void);
void flippingDownLeft(void);
void flippingDownRight(void);
void hurtingLeft(void);
void hurtingRight(void);
void wallJumpLeft(void);
void wallJumpRight(void);
void spiderLeft(void);
void spiderRight(void);
void spiderDown(void);
void spiderUp(void);
void spider(char);

#endif /* SAMUS_H */
