#include <coreinit/screen.h>
#include <coreinit/time.h>
#include <malloc.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <vpad/input.h>
#include <whb/log.h>

#include "bullet.h"
#include "drawing.h"
#include "enemy.h"
#include "structs.h"
#include "tower.h"

void CheckBulletEnemyCollisions()
{
    Bullet *currentBullet = bulletsHead;

    while (currentBullet)
    {
        Enemy *currentEnemy = enemiesHead;

        while (currentEnemy)
        {
            float dx = currentBullet->position.x - currentEnemy->position.x;
            float dy = currentBullet->position.y - currentEnemy->position.y;
            float distance = sqrt(dx * dx + dy * dy);

            if (distance <= currentBullet->size + currentEnemy->size)
            {
                currentBullet->health -= 1;
                currentEnemy->health -= currentBullet->damage;

                if (currentBullet->health <= 0)
                {
                    RemoveBullet(currentBullet);
                }
                if (currentEnemy->health <= 0)
                {
                    RemoveEnemy(currentEnemy);
                }
            }

            currentEnemy = currentEnemy->next;
        }

        currentBullet = currentBullet->next;
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
            AddTower(screenTouchPoint);
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

void GameShutdown()
{
    // Free all towers, bullets, and enemies
    while (towersHead)
    {
        RemoveTower(towersHead);
    }
    while (bulletsHead)
    {
        RemoveBullet(bulletsHead);
    }
    while (enemiesHead)
    {
        RemoveEnemy(enemiesHead);
    }
}

int profilerMessageCounter = 0;
OSTick fromTime = 0;

void ProfilerStartFrame()
{
    profilerMessageCounter = 0;
    fromTime = OSGetTick();
}

void ProfilerLogTime(char *title, OSTick time)
{
    unsigned frametimeMicroseconds = OSTicksToMicroseconds(time);
    char profilerLogString[100];
    snprintf(profilerLogString, 100, "%s: %.2f ms", title, frametimeMicroseconds / 1000.0);
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
