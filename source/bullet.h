#pragma once

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
    struct Bullet *previous;
    struct Bullet *next;
} Bullet;

extern Bullet *bulletsHead;

void AddBullet(Tower *fromTower);

void RemoveBullet(Bullet *bullet);

void MoveAllBullets(void);
