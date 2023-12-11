#pragma once

#include "structs.h"

typedef struct Enemy
{
    Point position;
    Color color;
    unsigned int pathIndex;
    int pathError;
    unsigned int size;
    unsigned int speed;
    unsigned int health;
    struct Enemy *previous;
    struct Enemy *next;
} Enemy;

Enemy *GetEnemiesHead(void);

int AddEnemy(void);

void RemoveEnemy(Enemy **enemyPointer);

void MoveAllEnemies(unsigned int gameLoopCounter);

const Point *getEnemyPath(void);
