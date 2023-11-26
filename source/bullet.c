#include "bullet.h"

#include "drawing.h"
#include <malloc.h>
#include <math.h>

Bullet *bulletsHead = NULL;
Bullet *bulletsTail = NULL;

void AddBullet(Tower *fromTower)
{
    Bullet *newBullet = malloc(sizeof(Bullet));
    *newBullet = *fromTower->bulletsFired;
    newBullet->position = fromTower->position;
    newBullet->previous = bulletsTail;
    newBullet->next = NULL;

    if (bulletsHead == NULL)
    {
        bulletsHead = newBullet;
    }
    else
    {
        bulletsTail->next = newBullet;
    }
    bulletsTail = newBullet;
}

void RemoveBullet(Bullet *bullet)
{
    // Set the previous bullet's next
    if (bullet->previous)
    {
        bullet->previous->next = bullet->next;
    }
    else
    {
        // This is the head of the list
        bulletsHead = bullet->next;
    }

    // Set the next bullet's previous
    if (bullet->next)
    {
        bullet->next->previous = bullet->previous;
    }
    else
    {
        // This is the tail of the list
        bulletsTail = bullet->previous;
    }

    free(bullet);
}

void MoveAllBullets(void)
{
    Bullet *currentBullet = bulletsHead;

    while (currentBullet)
    {
        currentBullet->position.x += roundf(currentBullet->direction.x * currentBullet->speed);
        currentBullet->position.y += roundf(currentBullet->direction.y * currentBullet->speed);

        if (currentBullet->position.x >= DRC_SCREEN_WIDTH || currentBullet->position.y >= DRC_SCREEN_HEIGHT ||
            currentBullet->position.x < 0 || currentBullet->position.y < 0)
        {
            Bullet *nextBullet = currentBullet->next;
            RemoveBullet(currentBullet);
            currentBullet = nextBullet;
        }
        else
        {
            currentBullet = currentBullet->next;
        }
    }
}
