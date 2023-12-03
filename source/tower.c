#include "tower.h"

#include "bullet.h"
#include <malloc.h>
#include <math.h>
#include <whb/log.h>

Tower *towersHead = NULL;
Tower *towersTail = NULL;

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
        .fireRate = 5,
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
        .size = 5,
        .speed = 5,
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

    // Calculate the direction vector from the tower to the target
    Vector direction;
    direction.x = tower->targetPosition.x - tower->position.x;
    direction.y = tower->targetPosition.y - tower->position.y;

    // Normalize the direction vector to get a unit vector
    float length = sqrt(direction.x * direction.x + direction.y * direction.y);
    direction.x = direction.x / length;
    direction.y = direction.y / length;

    tower->bulletsFired->direction = direction;
}

void SetLastTowerTarget(Point targetPosition)
{
    SetTowerTarget(towersTail, targetPosition);
}

void RemoveTower(Tower *tower)
{
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
}

int FireAllTowers(unsigned gameLoopCounter)
{
    Tower *currentTower = towersHead;

    while (currentTower)
    {
        if (!PointEquals(currentTower->targetPosition, INVALID_POINT))
        {
            if (gameLoopCounter % (100 / currentTower->fireRate) == 0)
            {
                int result = AddBullet(currentTower);
                if (result != 0)
                {
                    WHBLogPrintf("AddBullet failed with error code %d.", result);
                    return result;
                }
            }
        }

        currentTower = currentTower->next;
    }

    return 0;
}
