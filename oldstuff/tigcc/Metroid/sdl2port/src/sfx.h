/*
 * sfx.h - Special effect behaviors for Metroid TI-89 SDL2 port
 * Ported from original TIGCC sfx.c
 */
#ifndef SFX_H
#define SFX_H

struct object;

void bombExplodeFX(struct object* obj);
void crumblingFX(struct object* obj);
void spawnPickup(struct object* obj);
void simpleEnemyExplosionFX(struct object* obj);

#endif /* SFX_H */
