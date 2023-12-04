#include "bullet.h"

#include "drawing.h"
#include "macros.h"
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <whb/log.h>

Bullet *bulletsHead = NULL;
Bullet *bulletsTail = NULL;

int AddBullet(Tower *fromTower)
{
    Bullet *newBullet = malloc(sizeof(Bullet));
    if (!newBullet)
    {
        WHBLogPrint("Out of memory in AddBullet (newBullet)!");
        return -1;
    }

    *newBullet = *fromTower->bulletsFired;
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

    return 0;
}

void RemoveBullet(Bullet **bulletPointer)
{
    if (!bulletPointer || !*bulletPointer)
    {
        WHBLogPrint("Attempted to remove NULL bullet!");
        return;
    }

    Bullet *bullet = *bulletPointer;

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
    *bulletPointer = NULL;
}

void MoveAllBullets(int gameLoopCounter)
{
    for (Bullet *currentBullet = bulletsHead; currentBullet; currentBullet = currentBullet->next)
    {
        if (gameLoopCounter % MAX((SPEED_CONSTANT / currentBullet->speed), 1) != 0)
        {
            continue;
        }

        if (currentBullet->position.x >= DRC_SCREEN_WIDTH || currentBullet->position.y >= DRC_SCREEN_HEIGHT ||
            currentBullet->position.x < 0 || currentBullet->position.y < 0)
        {
            Bullet *previous = currentBullet->previous;
            RemoveBullet(&currentBullet);

            currentBullet = previous ? previous : bulletsHead;

            if (!currentBullet)
            {
                return;
            }

            continue;
        }

        Point target = currentBullet->targetPosition;
        Point initial = currentBullet->initialPosition;

        // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
        int dx = abs(target.x - initial.x);
        int dy = abs(target.y - initial.y);
        int signX = initial.x < target.x ? 1 : -1;
        int signY = initial.y < target.y ? 1 : -1;

        if (currentBullet->pathError == 0)
        {
            currentBullet->pathError = dx - dy;
        }

        for (int i = 0; i < MAX(currentBullet->speed / SPEED_CONSTANT, 1); i++)
        {
            int error2 = 2 * currentBullet->pathError;
            if (error2 > -dy)
            {
                currentBullet->pathError -= dy;
                currentBullet->position.x += signX;
            }
            if (error2 < dx)
            {
                currentBullet->pathError += dx;
                currentBullet->position.y += signY;
            }
        }
    }
}
