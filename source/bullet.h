#pragma once

#include "structs.h"
#include "tower.h"

typedef struct Bullet
{
    Point position;
    Point initialPosition;
    Point targetPosition;
    int pathError;
    unsigned int size;
    unsigned int speed;
    unsigned int damage;
    unsigned int health;
    struct Bullet *previous;
    struct Bullet *next;
} Bullet;

Bullet *GetBulletsHead(void);

int AddBullet(Tower *fromTower);

void RemoveBullet(Bullet **bulletPointer);

void MoveAllBullets(unsigned int gameLoopCounter);
