#include <malloc.h>

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
        newTower.bulletsFired = &(Bullet){(Point){0, 0}, (Point){0, 0}, 5, 5, 1, 1};
        newTower.fireRate = 5;

        towers[towerCount] = newTower;
        towerCount++;
    }
}

void SetTowerTarget(int towerIndex, Point targetPosition)
{
    towers[towerIndex].targetPosition = targetPosition;
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
