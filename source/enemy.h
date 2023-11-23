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
    struct Enemy *previous;
    struct Enemy *next;
} Enemy;

extern Enemy *enemiesHead;

void AddEnemy();

void RemoveEnemy(Enemy *enemy);

void MoveAllEnemies();

#endif
