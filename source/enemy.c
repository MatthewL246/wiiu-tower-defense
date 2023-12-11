#include "enemy.h"

#include "drawing.h"
#include "macros.h"
#include <malloc.h>
#include <stdlib.h>
#include <whb/log.h>

Enemy *enemiesHead = NULL;
Enemy *enemiesTail = NULL;

Enemy *GetEnemiesHead(void)
{
    return enemiesHead;
}

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

void RemoveEnemy(Enemy **enemyPointer)
{
    if (!enemyPointer || !*enemyPointer)
    {
        WHBLogPrint("Attempted to remove NULL enemy!");
        return;
    }

    Enemy *enemy = *enemyPointer;

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
    *enemyPointer = NULL;
}

void MoveAllEnemies(unsigned int gameLoopCounter)
{
    for (Enemy *currentEnemy = enemiesHead; currentEnemy; currentEnemy = currentEnemy->next)
    {
        if (gameLoopCounter % MAX((SPEED_CONSTANT / currentEnemy->speed), 1) != 0)
        {
            continue;
        }

        Point target = enemyPath[currentEnemy->pathIndex];
        Point lastTarget = enemyPath[currentEnemy->pathIndex - (currentEnemy->pathIndex == 0 ? 0 : 1)];

        if (PointsEqual(target, INVALID_POINT))
        {
            // The enemy is at the end of its path
            Enemy *previous = currentEnemy->previous;
            RemoveEnemy(&currentEnemy);

            // The loop will continue with the next enemy of the previous enemy
            // (which is same as the next enemy of the one that was just
            // removed) or the head of the list if the first enemy was removed
            currentEnemy = previous ? previous : enemiesHead;

            if (!currentEnemy)
            {
                // There are no more enemies if enemiesHead is null
                return;
            }

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

        for (unsigned int i = 0; i < MAX(currentEnemy->speed / SPEED_CONSTANT, 1); i++)
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

        if (PointsWithinTolerance(currentEnemy->position, target, MAX(currentEnemy->speed / SPEED_CONSTANT, 1)))
        {
            // The enemy has reached the path target
            // Snap its position to the target to prevent offsets
            currentEnemy->position = target;
            currentEnemy->pathIndex++;
            currentEnemy->pathError = 0;
        }
    }
}

const Point *getEnemyPath(void)
{
    return enemyPath;
}
