#include "enemy.h"
#include "drawing.h"

Enemy enemies[MAX_ENEMIES];
int enemyCount = 0;

void AddEnemy()
{
    if (enemyCount < MAX_ENEMIES)
    {
        Enemy newEnemy;
        newEnemy.position = (Point){0, DRC_SCREEN_HEIGHT / 2};
        newEnemy.color = (Color){255, 0, 0};
        newEnemy.size = 20;
        newEnemy.speed = 1;
        newEnemy.health = 1;

        enemies[enemyCount] = newEnemy;
        enemyCount++;
    }
}

void MoveAllEnemies()
{
    for (int i = 0; i < enemyCount; i++)
    {
        enemies[i].position.x += enemies[i].speed;

        if (enemies[i].position.x >= DRC_SCREEN_WIDTH)
        {
            for (int j = i; j < enemyCount - 1; j++)
            {
                enemies[j] = enemies[j + 1];
            }
            enemyCount--;
            i--;
        }
    }
}
