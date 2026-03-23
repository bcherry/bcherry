/*
 * objects.h - Object data structures for Metroid TI-89 SDL2 port
 * Ported from original TIGCC Objects.h
 */
#ifndef OBJECTS_H
#define OBJECTS_H

struct sprite;

struct object {
    int x, y;
    int h, w;
    int vel_x, vel_y;
    int ATK, timer;
    int e1, e2, e3;
    int type;
    int frames, gone;
    struct sprite* sprt;
    void (*behavior)(struct object*);
};

struct objnode {
    struct object* value;
    struct objnode* link;
};

struct objlist {
    struct objnode* first;
};

struct templates {
    struct {
        struct object powerBeam, missile, superMissile, bomb, powerBomb;
    } weapons;
    struct {
        struct object bombExplosion;
        struct object crumble;
        struct object simpleEnemyExplosion;
    } effects;
    struct {
        struct object energy;
        struct object missile;
        struct object superMissile;
        struct object powerBomb;
    } pickups;
    struct {
        struct object missile;
        struct object superMissile;
        struct object powerBomb;
        struct object energy;
    } items;
};

/* Core object functions */
void makeObjects(void);
void pushObject(int x, int y, int vel_x, int vel_y, struct object* tmpl);
void drawObject(struct object* obj);
void drawObjects(void);
void doObjects(void);
void freeObjects(void);

/* Weapon behaviors (from Weapons.c) */
void basicShot(struct object* obj);
void missileShot(struct object* obj);
void bombShot(struct object* obj);

/* Effect behaviors (from sfx.c) */
void bombExplodeFX(struct object* obj);
void crumblingFX(struct object* obj);
void simpleEnemyExplosionFX(struct object* obj);
void spawnPickup(struct object* obj);

/* Pickup behaviors (from pickups.c) */
int pickup(struct object* obj);
void energyPickup(struct object* obj);
void missilePickup(struct object* obj);
void superMissilePickup(struct object* obj);
void powerBombPickup(struct object* obj);

/* Item behaviors (from Items.c) */
void missileItem(struct object* obj);
void superMissileItem(struct object* obj);
void powerBombItem(struct object* obj);
void energyTankItem(struct object* obj);

#endif /* OBJECTS_H */
