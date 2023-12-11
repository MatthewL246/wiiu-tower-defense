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
    for (Bullet *currentBullet = GetBulletsHead(); currentBullet; currentBullet = currentBullet->next)
    {
        for (Enemy *currentEnemy = GetEnemiesHead(); currentEnemy; currentEnemy = currentEnemy->next)
        {
            int hitSize = (currentEnemy->size + currentBullet->size) / 2;

            if (PointsWithinTolerance(currentBullet->position, currentEnemy->position, hitSize))
            {
                currentBullet->health -= 1;
                currentEnemy->health -= currentBullet->damage;
                bool shouldReturn = false;

                if (currentBullet->health == 0)
                {
                    Bullet *previous = currentBullet->previous;
                    RemoveBullet(&currentBullet);

                    currentBullet = previous ? previous : GetBulletsHead();

                    if (!currentBullet)
                    {
                        // There are no more bullets to iterate
                        shouldReturn = true;
                    }
                }
                if (currentEnemy->health == 0)
                {
                    Enemy *previous = currentEnemy->previous;
                    RemoveEnemy(&currentEnemy);
                    currentEnemy = previous ? previous : GetEnemiesHead();

                    if (!currentEnemy)
                    {
                        // There are no more enemies to iterate
                        shouldReturn = true;
                    }
                }

                if (shouldReturn)
                {
                    return;
                }
            }
        }
    }
}

unsigned int gameLoopCounter = 0;
unsigned int touchTimer = 0;
const int touchDelay = 1;
bool targetMode = false;

int GameLoop(VPADStatus status, OSTick deltaTime)
{
    VPADTouchData touchData = status.tpNormal;
    if (touchData.touched != 0)
    {
        Point touchPoint = (Point){touchData.x, touchData.y};
        Point screenTouchPoint = MapTouchToDrcScreen(touchPoint);
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
    while (GetTowersHead())
    {
        Tower *toRemove = GetTowersHead();
        RemoveTower(&toRemove);
    }
    while (GetBulletsHead())
    {
        Bullet *toRemove = GetBulletsHead();
        RemoveBullet(&toRemove);
    }
    while (GetEnemiesHead())
    {
        Enemy *toRemove = GetEnemiesHead();
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
    unsigned int frametimeMicroseconds = OSTicksToMicroseconds(time);
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
