/*
 * enemies.h - Enemy data structures and AI for Metroid TI-89 SDL2 port
 * Ported from original TIGCC Enemies.h
 */
#ifndef ENEMIES_H
#define ENEMIES_H

struct sprite;

struct enemy {
    int x, y;
    int h, w;
    int vel_x, vel_y;
    int extra1, extra2, extra3;
    int HP, ATK, E, M, SM, PB;
    char dead, hurt;
    int frames;
    struct sprite* sprt;
    int immunity;
    void (*behavior)(struct enemy*);
    void (*onDeath)(struct enemy*);
};

struct enemyNode {
    struct enemy* value;
    struct enemyNode* next;
};

struct enemyList {
    struct enemyNode* first;
};

/* Core enemy functions */
void makeEnemies(void);
void pushEnemy(int x, int y, int key);
void hurtEnemy(struct enemy* e, int ATK, int type);
void drawEnemy(struct enemy* e);
void drawEnemies(void);
void doEnemies(void);
void freeEnemies(void);

/* Death handlers */
void simpleDeath(struct enemy* e);
void skreeDeath(struct enemy* e);

/* Pickup spawning */
int getPickup(struct enemy* e);

/* AI functions */
void waverAI(struct enemy* e);
void sideHopperAI(struct enemy* e);
void skreeAI(struct enemy* e);
void projectileAI(struct enemy* e);
void zebboAI(struct enemy* e);
void zebboSpawnAI(struct enemy* e);
void ripperAI(struct enemy* e);
void spacePirateAI(struct enemy* e);
void spacePirateLaserAI(struct enemy* e);

#endif /* ENEMIES_H */
