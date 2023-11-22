#ifndef BULLET_H
#define BULLET_H

#include "structs.h"
#include "tower.h"

typedef struct Bullet
{
    Point position;
    Vector direction;
    int size;
    int speed;
    int damage;
    int health;
} Bullet;

#define MAX_BULLETS 1000
extern Bullet bullets[MAX_BULLETS];
extern int bulletCount;

void AddBullet(Tower fromTower);

void MoveAllBullets();

#endif
