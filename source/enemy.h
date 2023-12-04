#pragma once

#include "structs.h"

typedef struct Enemy
{
    Point position;
    Color color;
    int size;
    int speed;
    int health;
    int pathIndex;
    int pathError;
    struct Enemy *previous;
    struct Enemy *next;
} Enemy;

extern Enemy *enemiesHead;

int AddEnemy(void);

void RemoveEnemy(Enemy *enemy);

void MoveAllEnemies(int gameLoopCounter);

const Point *getEnemyPath(void);
