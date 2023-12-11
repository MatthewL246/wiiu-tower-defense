#include "tower.h"

#include "bullet.h"
#include "macros.h"
#include <malloc.h>
#include <math.h>
#include <whb/log.h>

Tower *towersHead = NULL;
Tower *towersTail = NULL;

Tower *GetTowersHead(void)
{
    return towersHead;
}

int AddTower(Point position)
{
    Tower *newTower = malloc(sizeof(Tower));
    if (!newTower)
    {
        WHBLogPrint("Out of memory in AddTower (newTower)!");
        return -1;
    }

    *newTower = (Tower){
        .position = position,
        .targetPosition = INVALID_POINT,
        .color = {0, 255, 0},
        .size = 10,
        .fireRate = 0.05 * SPEED_CONSTANT,
        .bulletsFired = malloc(sizeof(Bullet)),
        .previous = towersTail,
        .next = NULL};

    if (!newTower->bulletsFired)
    {
        WHBLogPrint("Out of memory in AddTower (newTowerBullet)!");
        free(newTower);
        return -1;
    }

    *newTower->bulletsFired = (Bullet){
        .position = newTower->position,
        .initialPosition = newTower->position,
        .size = 5,
        .speed = 5 * SPEED_CONSTANT,
        .damage = 1,
        .health = 1,
    };

    if (towersHead == NULL)
    {
        towersHead = newTower;
    }
    else
    {
        towersTail->next = newTower;
    }
    towersTail = newTower;

    return 0;
}

void SetTowerTarget(Tower *tower, Point targetPosition)
{
    tower->targetPosition = targetPosition;
    tower->bulletsFired->targetPosition = targetPosition;
}

void SetLastTowerTarget(Point targetPosition)
{
    SetTowerTarget(towersTail, targetPosition);
}

void RemoveTower(Tower **towerPointer)
{
    if (!towerPointer || !*towerPointer)
    {
        WHBLogPrint("Attempted to remove NULL tower!");
        return;
    }

    Tower *tower = *towerPointer;

    // Set the previous tower's next
    if (tower->previous)
    {
        tower->previous->next = tower->next;
    }
    else
    {
        // This is the head of the list
        towersHead = tower->next;
    }

    // Set the next tower's previous
    if (tower->next)
    {
        tower->next->previous = tower->previous;
    }
    else
    {
        // This is the tail of the list
        towersTail = tower->previous;
    }

    free(tower->bulletsFired);
    free(tower);
    *towerPointer = NULL;
}

int FireAllTowers(unsigned int gameLoopCounter)
{
    for (Tower *currentTower = towersHead; currentTower; currentTower = currentTower->next)
    {
        if (!PointsEqual(currentTower->targetPosition, INVALID_POINT) && gameLoopCounter % (SPEED_CONSTANT / currentTower->fireRate) == 0)
        {
            int result = AddBullet(currentTower);
            if (result != 0)
            {
                WHBLogPrintf("AddBullet failed with error code %d.", result);
                return result;
            }
        }
    }

    return 0;
}
