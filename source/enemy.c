#include "enemy.h"

#include "drawing.h"
#include "macros.h"
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
        .speed = 1 * SPEED_CONSTANT,
        .health = 1,
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

void MoveAllEnemies(int gameLoopCounter)
{
    Enemy *currentEnemy = enemiesHead;

    while (currentEnemy)
    {
        if (gameLoopCounter % MAX((SPEED_CONSTANT / currentEnemy->speed), 1) != 0)
        {
            currentEnemy = currentEnemy->next;
            continue;
        }

        Point target = enemyPath[currentEnemy->pathIndex];
        Point lastTarget = enemyPath[currentEnemy->pathIndex - (currentEnemy->pathIndex == 0 ? 0 : 1)];

        if (PointEquals(target, INVALID_POINT))
        {
            // The enemy is at the end of its path
            Enemy *nextEnemy = currentEnemy->next;
            RemoveEnemy(currentEnemy);
            currentEnemy = nextEnemy;
            continue;
        }

        // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
        int dx = abs(target.x - lastTarget.x);
        int dy = abs(target.y - lastTarget.y);
        int signX = lastTarget.x < target.x ? 1 : -1;
        int signY = lastTarget.y < target.y ? 1 : -1;

        if (currentEnemy->pathError == 0)
        {
            // This is the first movement iteration for this path index
            currentEnemy->pathError = dx - dy;
        }

        for (int i = 0; i < MAX(currentEnemy->speed / SPEED_CONSTANT, 1); i++)
        {
            int error2 = 2 * currentEnemy->pathError;
            if (error2 > -dy)
            {
                currentEnemy->pathError -= dy;
                currentEnemy->position.x += signX;
            }
            if (error2 < dx)
            {
                currentEnemy->pathError += dx;
                currentEnemy->position.y += signY;
            }
        }

        if (PointInTolerance(currentEnemy->position, target, MAX(currentEnemy->speed / SPEED_CONSTANT, 1)))
        {
            // The enemy has reached the path target
            // Snap its position to the target to prevent offsets
            currentEnemy->position = target;
            currentEnemy->pathIndex++;
            currentEnemy->pathError = 0;
        }

        currentEnemy = currentEnemy->next;
    }
}

const Point *getEnemyPath(void)
{
    return enemyPath;
}
