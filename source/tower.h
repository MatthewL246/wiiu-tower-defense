#ifndef TOWER_H
#define TOWER_H

#include "structs.h"

typedef struct Bullet Bullet;

typedef struct Tower
{
    Point position;
    Point targetPosition;
    Color color;
    int size;
    Bullet* bulletsFired;
    int fireRate;
} Tower;

#define MAX_TOWERS 100
extern Tower towers[MAX_TOWERS];
extern int towerCount;

void AddTower(Point position);

void SetTowerTarget(int towerIndex, Point targetPosition);

void FireAllTowers(unsigned gameLoopCounter);

#endif
