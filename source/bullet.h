#pragma once

#include "structs.h"
#include "tower.h"

typedef struct Bullet
{
    Point position;
    Point initialPosition;
    Point targetPosition;
    int pathError;
    int size;
    int speed;
    int damage;
    int health;
    struct Bullet *previous;
    struct Bullet *next;
} Bullet;

Bullet *GetBulletsHead(void);

int AddBullet(Tower *fromTower);

void RemoveBullet(Bullet **bulletPointer);

void MoveAllBullets(int gameLoopCounter);
