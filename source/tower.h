#pragma once

#include "structs.h"

typedef struct Bullet Bullet;

typedef struct Tower
{
    Point position;
    Point targetPosition;
    Color color;
    unsigned int size;
    Bullet *bulletsFired;
    unsigned int fireRate;
    struct Tower *previous;
    struct Tower *next;
} Tower;

Tower *GetTowersHead(void);

int AddTower(Point position);

void RemoveTower(Tower **towerPointer);

void SetLastTowerTarget(Point targetPosition);

int FireAllTowers(unsigned int gameLoopCounter);
