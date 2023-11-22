#include <malloc.h>
#include <math.h>

#include "tower.h"
#include "bullet.h"

Tower towers[MAX_TOWERS];
int towerCount = 0;

void AddTower(Point position)
{
    if (towerCount < MAX_TOWERS)
    {
        Tower newTower;
        newTower.position = position;
        newTower.targetPosition = (Point){-20, -20};
        newTower.color = (Color){0, 255, 0};
        newTower.size = 10;
        newTower.fireRate = 5;

        Bullet* newTowerBullet = malloc(sizeof(Bullet));
        newTowerBullet->position = (Point){0, 0};
        newTowerBullet->direction = (Vector){0, 0};
        newTowerBullet->size = 5;
        newTowerBullet->speed = 5;
        newTowerBullet->damage = 1;
        newTowerBullet->health = 1;

        newTower.bulletsFired = newTowerBullet;
        towers[towerCount] = newTower;
        towerCount++;
    }
}

void SetTowerTarget(int towerIndex, Point targetPosition)
{
    Tower* tower = &towers[towerIndex];
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

void FireAllTowers(unsigned gameLoopCounter)
{
    for (int i = 0; i < towerCount; i++)
    {
        if (towers[i].targetPosition.x != -20 && towers[i].targetPosition.y != -20)
        {
            if (gameLoopCounter % (100 / towers[i].fireRate) == 0)
            {
                AddBullet(towers[i]);
            }
        }
    }
}
