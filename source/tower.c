#include "tower.h"

#include "bullet.h"
#include <malloc.h>
#include <math.h>

Tower *towersHead = NULL;
Tower *towersTail = NULL;

void AddTower(Point position)
{
    Tower *newTower = malloc(sizeof(Tower));
    newTower->position = position;
    newTower->targetPosition = INVALID_TOWER_TARGET;
    newTower->color = (Color){0, 255, 0};
    newTower->size = 10;
    newTower->fireRate = 5;

    Bullet *newTowerBullet = malloc(sizeof(Bullet));
    newTowerBullet->position = (Point){0, 0};
    newTowerBullet->direction = (Vector){0, 0};
    newTowerBullet->size = 5;
    newTowerBullet->speed = 5;
    newTowerBullet->damage = 1;
    newTowerBullet->health = 1;

    newTower->bulletsFired = newTowerBullet;
    newTower->previous = towersTail;
    newTower->next = NULL;

    if (towersHead == NULL)
    {
        towersHead = newTower;
    }
    else
    {
        towersTail->next = newTower;
    }
    towersTail = newTower;
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

void FireAllTowers(unsigned gameLoopCounter)
{
    Tower *currentTower = towersHead;

    while (currentTower)
    {
        if (!PointEquals(currentTower->targetPosition, INVALID_TOWER_TARGET))
        {
            if (gameLoopCounter % (100 / currentTower->fireRate) == 0)
            {
                AddBullet(currentTower);
            }
        }

        currentTower = currentTower->next;
    }
}
