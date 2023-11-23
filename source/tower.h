#ifndef TOWER_H
#define TOWER_H

#include "structs.h"

#define INVALID_TOWER_TARGET (Point){-1, -1}

typedef struct Bullet Bullet;

typedef struct Tower
{
    Point position;
    Point targetPosition;
    Color color;
    int size;
    Bullet *bulletsFired;
    int fireRate;
    struct Tower *previous;
    struct Tower *next;
} Tower;

extern Tower *towersHead;

void AddTower(Point position);

void RemoveTower(Tower *tower);

void SetLastTowerTarget(Point targetPosition);

void FireAllTowers(unsigned gameLoopCounter);

#endif
