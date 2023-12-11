#pragma once

#include "structs.h"

typedef struct Bullet Bullet;

// This represents a defending tower.
typedef struct Tower
{
    // This is the tower's position.
    Point position;
    // This is the position that the tower targets when firing bullets.
    Point targetPosition;
    // This is the tower's color when drawn.
    Color color;
    // This is the tower's size when drawn as a point.
    unsigned int size;
    // This is a template for the bullets that this tower fires.
    Bullet *bulletsFired;
    // This is the number of bullets that this tower fires per SPEED_CONSTANT
    // game loops. This cannot be greater than SPEED_CONSTANT.
    unsigned int fireRate;
    // This is a pointer to the previous tower in the linked list.
    struct Tower *previous;
    // This is a pointer to the next tower in the linked list.
    struct Tower *next;
} Tower;

// This returns a pointer to the head of the linked list.
Tower *GetTowersHead(void);

// This adds a tower to the linked list at a position on the screen.
int AddTower(Point position);

// This removes a tower from the linked list.
void RemoveTower(Tower **towerPointer);

// This sets the target position of the last tower in the linked list to a
// position on the screen.
void SetLastTowerTarget(Point targetPosition);

// This fires bullets from all of the towers if necessary.
int FireAllTowers(unsigned int gameLoopCounter);
