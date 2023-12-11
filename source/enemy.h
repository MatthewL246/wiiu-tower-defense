#pragma once

#include "structs.h"

// This represents an enemy moving along a path.
typedef struct Enemy
{
    // This is the enemy's current position.
    Point position;
    // This is the enemy's color when drawn.
    Color color;
    // This is the index of the enemy's current path point.
    unsigned int pathIndex;
    // This is used by the enemy movement algorithm.
    int pathError;
    // This is the enemy's size when drawn as a point.
    unsigned int size;
    // This is the enemy's speed in pixels per SPEED_CONSTANT game loops.
    unsigned int speed;
    // This is the amount of health that the enemy has.
    int health;
    // This is a pointer to the previous enemy in the linked list.
    struct Enemy *previous;
    // This is a pointer to the next enemy in the linked list.
    struct Enemy *next;
} Enemy;

// This returns a pointer to the head of the linked list.
Enemy *GetEnemiesHead(void);

// This adds an enemy to the linked list at the start position of the path.
int AddEnemy(void);

// This removes an enemy from the linked list.
void RemoveEnemy(Enemy **enemyPointer);

// This moves all of the enemies if necessary.
void MoveAllEnemies(unsigned int gameLoopCounter);

// This returns a pointer to the start of the enemy path array.
const Point *getEnemyPath(void);
