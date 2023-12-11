#pragma once

#include "structs.h"

typedef struct Tower Tower;

// Represents a bullet fired by a tower.
typedef struct Bullet
{
    // This is the bullet's current position.
    Point position;
    // This is the position of the tower the fired the bullet.
    Point initialPosition;
    // This is the position of the bullet's target.
    Point targetPosition;
    // This is used by the bullet movement algorithm.
    int pathError;
    // This is the bullet's size when drawn as a point.
    unsigned int size;
    // This is the bullet's speed in pixels per SPEED_CONSTANT game loops.
    unsigned int speed;
    // This is the amount of damage that the bullet does to enemies.
    unsigned int damage;
    // This is the amount of health that the bullet has.
    int health;
    // This is a pointer to the previous bullet in the linked list.
    struct Bullet *previous;
    // This is a pointer to the next bullet in the linked list.
    struct Bullet *next;
} Bullet;

// This returns a pointer to the head of the linked list.
Bullet *GetBulletsHead(void);

// This adds a bullet to the linked list from a tower's bullet template.
int AddBullet(Tower *fromTower);

// This removes a bullet from the linked list.
void RemoveBullet(Bullet **bulletPointer);

// This moves all of the bullets if necessary.
void MoveAllBullets(unsigned int gameLoopCounter);
