#include "game.h"

#include "bullet.h"
#include "drawing.h"
#include "enemy.h"
#include "structs.h"
#include "tower.h"
#include <coreinit/screen.h>
#include <coreinit/time.h>
#include <malloc.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <vpad/input.h>
#include <whb/log.h>

void CheckBulletEnemyCollisions(void)
{
    for (Bullet *currentBullet = bulletsHead; currentBullet; currentBullet = currentBullet->next)
    {
        for (Enemy *currentEnemy = enemiesHead; currentEnemy; currentEnemy = currentEnemy->next)
        {
            int hitSize = (currentEnemy->size + currentBullet->size) / 2;

            if (PointInTolerance(currentBullet->position, currentEnemy->position, hitSize))
            {
                currentBullet->health -= 1;
                currentEnemy->health -= currentBullet->damage;

                if (currentBullet->health <= 0)
                {
                    Bullet *previous = currentBullet->previous;
                    RemoveBullet(&currentBullet);

                    currentBullet = previous ? previous : bulletsHead;

                    if (!currentBullet)
                    {
                        return;
                    }
                }
                if (currentEnemy->health <= 0)
                {
                    Enemy *previous = currentEnemy->previous;
                    RemoveEnemy(&currentEnemy);
                    currentEnemy = previous ? previous : enemiesHead;

                    if (!currentEnemy)
                    {
                        return;
                    }
                }
            }
        }
    }
}

unsigned gameLoopCounter = 0;
unsigned touchTimer = 0;
const int touchDelay = 1;
bool targetMode = false;

int GameLoop(VPADStatus status, OSTick deltaTime)
{
    VPADTouchData touchData = status.tpNormal;
    if (touchData.touched != 0)
    {
        Point screenTouchPoint = MapTouchToDrcScreen(touchData.x, touchData.y);
        DrawPoint(screenTouchPoint, (Color){255, 255, 255}, 10, false);
        touchTimer++;
        if (touchTimer == touchDelay && !targetMode)
        {
            int result = AddTower(screenTouchPoint);
            if (result != 0)
            {
                WHBLogPrintf("AddTower failed with error code %d.", result);
                return result;
            }
            targetMode = true;
        }
        else if (touchTimer == touchDelay && targetMode)
        {
            SetLastTowerTarget(screenTouchPoint);
            targetMode = false;
        }
    }
    else
    {
        touchTimer = 0;
    }

    if (gameLoopCounter % 100 == 0)
    {
        int result = AddEnemy();
        if (result != 0)
        {
            WHBLogPrintf("AddEnemy failed with error code %d.", result);
            return result;
        }
    }

    int result = FireAllTowers(gameLoopCounter);
    if (result != 0)
    {
        WHBLogPrintf("FireAllTowers failed with error code %d.", result);
        return result;
    }

    MoveAllEnemies(gameLoopCounter);
    MoveAllBullets(gameLoopCounter);
    CheckBulletEnemyCollisions();

    DrawEnemyPath(getEnemyPath());
    DrawAllTowers();
    DrawAllBullets();
    DrawAllEnemies();

    gameLoopCounter++;
    return 0;
}

void GameShutdown(void)
{
    // Free all towers, bullets, and enemies
    while (towersHead)
    {
        Tower *toRemove = towersHead;
        RemoveTower(&toRemove);
    }
    while (bulletsHead)
    {
        Bullet *toRemove = bulletsHead;
        RemoveBullet(&toRemove);
    }
    while (enemiesHead)
    {
        Enemy *toRemove = enemiesHead;
        RemoveEnemy(&toRemove);
    }
}

int profilerMessageCounter = 0;
OSTick fromTime = 0;

void ProfilerStartFrame(void)
{
    profilerMessageCounter = 0;
    fromTime = OSGetTick();
}

void ProfilerLogTime(char *title, OSTick time)
{
    unsigned frametimeMicroseconds = OSTicksToMicroseconds(time);
    char profilerLogString[100];
    snprintf(profilerLogString, sizeof(profilerLogString) / sizeof(char), "%s: %.2f ms", title, frametimeMicroseconds / 1000.0);
    OSScreenPutFontEx(SCREEN_DRC, 0, profilerMessageCounter, profilerLogString);
    OSScreenPutFontEx(SCREEN_TV, 0, profilerMessageCounter, profilerLogString);
    profilerMessageCounter++;
}

void ProfilerLog(char *title)
{
    OSTick time = OSGetTick();
    ProfilerLogTime(title, time - fromTime);
    fromTime = time;
}
