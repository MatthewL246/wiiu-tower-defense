#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <coreinit/screen.h>
#include <vpad/input.h>
#include <whb/log.h>

#include "structs.h"
#include "bullet.h"
#include "tower.h"
#include "enemy.h"
#include "drawing.h"

void CheckBulletEnemyCollisions()
{
    for (int i = 0; i < bulletCount; i++)
    {
        for (int j = 0; j < enemyCount; j++)
        {
            float dx = bullets[i].position.x - enemies[j].position.x;
            float dy = bullets[i].position.y - enemies[j].position.y;
            float distance = sqrt(dx * dx + dy * dy);
            if (distance <= bullets[i].size + enemies[j].size)
            {
                bullets[i].health -= 1;
                enemies[j].health -= bullets[i].damage;

                if (bullets[i].health <= 0)
                {
                    for (int k = i; k < bulletCount - 1; k++)
                    {
                        bullets[k] = bullets[k + 1];
                    }
                    bulletCount--;
                    i--;
                }
                if (enemies[j].health <= 0)
                {
                    for (int k = j; k < enemyCount - 1; k++)
                    {
                        enemies[k] = enemies[k + 1];
                    }
                    enemyCount--;
                    j--;
                }
            }
        }
    }
}

unsigned gameLoopCounter = 0;
unsigned touchTimer = 0;
const int touchDelay = 1;
bool targetMode = false;

int GameLoop(VPADStatus status)
{
    VPADTouchData touchData = status.tpNormal;
    if (touchData.touched != 0)
    {
        Point screenTouchPoint = MapTouchToDrcScreen(touchData.x, touchData.y);
        DrawPoint(screenTouchPoint, (Color){255, 255, 255}, 10, false);
        touchTimer++;
        if (touchTimer == touchDelay && !targetMode)
        {
            AddTower(screenTouchPoint);
            targetMode = true;
        }
        else if (touchTimer == touchDelay && targetMode)
        {
            SetTowerTarget(towerCount - 1, screenTouchPoint);
            targetMode = false;
        }
    }
    else
    {
        touchTimer = 0;
    }

    if (gameLoopCounter % 100 == 0)
    {
        AddEnemy();
    }

    FireAllTowers(gameLoopCounter);

    MoveAllEnemies();
    MoveAllBullets();
    CheckBulletEnemyCollisions();

    DrawAllTowers();
    DrawAllBullets();
    DrawAllEnemies();

    gameLoopCounter++;
    return 0;
}
