#pragma once

#include "structs.h"

typedef struct Enemy
{
    Point position;
    Color color;
    int size;
    int speed;
    int health;
    struct Enemy *previous;
    struct Enemy *next;
} Enemy;

extern Enemy *enemiesHead;

void AddEnemy(void);

void RemoveEnemy(Enemy *enemy);

void MoveAllEnemies(void);
