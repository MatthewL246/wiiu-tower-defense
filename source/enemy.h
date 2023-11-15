#ifndef ENEMY_H
#define ENEMY_H

#include "structs.h"

typedef struct Enemy
{
    Point position;
    Color color;
    int size;
    int speed;
    int health;
} Enemy;

#define MAX_ENEMIES 200
extern Enemy enemies[MAX_ENEMIES];
extern int enemyCount;

void AddEnemy();

void MoveAllEnemies();

#endif
