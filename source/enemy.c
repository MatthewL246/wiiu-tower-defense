#include "enemy.h"

#include "drawing.h"
#include <malloc.h>
#include <whb/log.h>

Enemy *enemiesHead = NULL;
Enemy *enemiesTail = NULL;

int AddEnemy(void)
{
    Enemy *newEnemy = malloc(sizeof(Enemy));
    if (!newEnemy)
    {
        WHBLogPrint("Out of memory in AddEnemy (newEnemy)!");
        return -1;
    }

    newEnemy->position = (Point){0, DRC_SCREEN_HEIGHT / 2};
    newEnemy->color = (Color){255, 0, 0};
    newEnemy->size = 20;
    newEnemy->speed = 1;
    newEnemy->health = 1;
    newEnemy->previous = enemiesTail;
    newEnemy->next = NULL;

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
        currentEnemy->position.x += currentEnemy->speed;

        if (currentEnemy->position.x >= DRC_SCREEN_WIDTH)
        {
            Enemy *nextEnemy = currentEnemy->next;
            RemoveEnemy(currentEnemy);
            currentEnemy = nextEnemy;
        }
        else
        {
            currentEnemy = currentEnemy->next;
        }
    }
}
