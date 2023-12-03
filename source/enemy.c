#include "enemy.h"

#include "drawing.h"
#include <malloc.h>
#include <stdlib.h>
#include <whb/log.h>

Enemy *enemiesHead = NULL;
Enemy *enemiesTail = NULL;

const Point enemyPath[] = {
    {0, DRC_SCREEN_HEIGHT / 2},
    {DRC_SCREEN_WIDTH / 4, DRC_SCREEN_HEIGHT},
    {DRC_SCREEN_WIDTH / 4, DRC_SCREEN_HEIGHT / 2},
    {DRC_SCREEN_WIDTH / 2, DRC_SCREEN_HEIGHT / 2},
    {DRC_SCREEN_WIDTH / 2, DRC_SCREEN_HEIGHT},
    {DRC_SCREEN_WIDTH * 3 / 4, 0},
    {DRC_SCREEN_WIDTH / 2, DRC_SCREEN_HEIGHT * 1 / 3},
    {DRC_SCREEN_WIDTH, DRC_SCREEN_HEIGHT / 2},
    CONST_INVALID_POINT};

int AddEnemy(void)
{
    Enemy *newEnemy = malloc(sizeof(Enemy));
    if (!newEnemy)
    {
        WHBLogPrint("Out of memory in AddEnemy (newEnemy)!");
        return -1;
    }

    *newEnemy = (Enemy){
        .position = enemyPath[0],
        .color = {255, 0, 0},
        .size = 20,
        .speed = 1,
        .health = 1,
        .pathIndex = 0,
        .previous = enemiesTail,
        .next = NULL};

    if (enemiesHead == NULL)
    {
        enemiesHead = newEnemy;
    }
    else
    {
        enemiesTail->next = newEnemy;
    }
    enemiesTail = newEnemy;

    return 0;
}

void RemoveEnemy(Enemy *enemy)
{
    if (!enemy)
    {
        WHBLogPrint("Attempted to remove NULL enemy!");
        return;
    }

    // Set the previous enemy's next
    if (enemy->previous)
    {
        enemy->previous->next = enemy->next;
    }
    else
    {
        // This is the head of the list
        enemiesHead = enemy->next;
    }

    // Set the next enemy's previous
    if (enemy->next)
    {
        enemy->next->previous = enemy->previous;
    }
    else
    {
        // This is the tail of the list
        enemiesTail = enemy->previous;
    }

    free(enemy);
}

void MoveAllEnemies(void)
{
    Enemy *currentEnemy = enemiesHead;

    while (currentEnemy)
    {
        Point target = enemyPath[currentEnemy->pathIndex];

        if (PointEquals(target, INVALID_POINT))
        {
            // The enemy is at the end of its path
            Enemy *nextEnemy = currentEnemy->next;
            RemoveEnemy(currentEnemy);
            currentEnemy = nextEnemy;
        }
        else
        {
            int dx = target.x - currentEnemy->position.x;
            int dy = target.y - currentEnemy->position.y;

            if (abs(dx) >= currentEnemy->speed)
            {
                currentEnemy->position.x += currentEnemy->speed * (dx > 0 ? 1 : -1);
            }
            if (abs(dy) >= currentEnemy->speed)
            {
                currentEnemy->position.y += currentEnemy->speed * (dy > 0 ? 1 : -1);
            }
            if (abs(dx) <= currentEnemy->speed && abs(dy) <= currentEnemy->speed)
            {
                currentEnemy->pathIndex++;
            }

            currentEnemy = currentEnemy->next;
        }
    }
}

const Point *getEnemyPath(void)
{
    return enemyPath;
}
