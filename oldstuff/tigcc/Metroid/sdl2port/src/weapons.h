/*
 * weapons.h - Weapon behavior functions for Metroid TI-89 SDL2 port
 * Ported from original TIGCC Weapons.c
 */
#ifndef WEAPONS_H
#define WEAPONS_H

struct object;

/* Projectile behaviors */
void basicShot(struct object* obj);
void missileShot(struct object* obj);
void bombShot(struct object* obj);

/* Firing functions */
int shoot(int x, int y, int dir);
void layBomb(void);

/* Weapon cycling */
void weaponsSetup(void);

/* Enemy collision check for projectiles */
void checkEnemies(struct object* obj);

#endif /* WEAPONS_H */
