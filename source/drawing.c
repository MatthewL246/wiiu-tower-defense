#include "drawing.h"

#include "bullet.h"
#include "enemy.h"
#include "tower.h"
#include <coreinit/screen.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const int DRC_TOUCH_TOP = 3900;
const int DRC_TOUCH_BOTTOM = 180;
const int DRC_TOUCH_LEFT = 100;
const int DRC_TOUCH_RIGHT = 3960;

Point MapTouchToDrcScreen(int touchX, int touchY)
{
    Point screenPoint;
    screenPoint.x = ((touchX - DRC_TOUCH_LEFT) / (float)(DRC_TOUCH_RIGHT - DRC_TOUCH_LEFT)) * DRC_SCREEN_WIDTH;
    screenPoint.y = DRC_SCREEN_HEIGHT - (((touchY - DRC_TOUCH_BOTTOM) / (float)(DRC_TOUCH_TOP - DRC_TOUCH_BOTTOM)) * DRC_SCREEN_HEIGHT);

    return screenPoint;
}

const int TV_WIDTH = 1280;
const int TV_HEIGHT = 720;

void DrawPoint(Point point, Color color, int size, bool drawOnBothScreens)
{
    int halfSize = size / 2;
    for (int dx = -halfSize; dx <= halfSize; dx++)
    {
        for (int dy = -halfSize; dy <= halfSize; dy++)
        {
            if (dx * dx + dy * dy <= halfSize * halfSize)
            {
                OSScreenPutPixelEx(SCREEN_DRC, point.x + dx, point.y + dy, color.r << 24 | color.g << 16 | color.b << 8);
                if (drawOnBothScreens)
                {
                    float tvScaleX = (float)TV_WIDTH / DRC_SCREEN_WIDTH;
                    float tvScaleY = (float)TV_HEIGHT / DRC_SCREEN_HEIGHT;
                    int tvX = (point.x + dx) * tvScaleX;
                    int tvY = (point.y + dy) * tvScaleY;
                    for (int tvDx = -1; tvDx <= 1; tvDx++)
                    {
                        for (int tvDy = -1; tvDy <= 1; tvDy++)
                        {
                            OSScreenPutPixelEx(SCREEN_TV, tvX + tvDx, tvY + tvDy, color.r << 24 | color.g << 16 | color.b << 8);
                        }
                    }
                }
            }
        }
    }
}

void DrawEnemyPath(const Point *path)
{
    int i = 0;
    while (!PointEquals(path[i + 1], INVALID_POINT))
    {
        Point currentPoint = path[i];
        Point target = path[i + 1];
        Color pathColor = (Color){50, 50, 50};

        while (!PointEquals(currentPoint, target))
        {
            DrawPoint(currentPoint, pathColor, 5, true);
            if (currentPoint.x != target.x)
            {
                currentPoint.x += target.x > currentPoint.x ? 1 : -1;
            }
            if (currentPoint.y != target.y)
            {
                currentPoint.y += target.y > currentPoint.y ? 1 : -1;
            }
        }

        i++;
    }
}

void DrawAllTowers(void)
{
    Tower *currentTower = towersHead;

    while (currentTower)
    {
        DrawPoint(currentTower->position, currentTower->color, currentTower->size, true);
        if (!PointEquals(currentTower->targetPosition, INVALID_POINT))
        {
            DrawPoint(currentTower->targetPosition, (Color){255, 155, 0}, 10, true);
        }
        currentTower = currentTower->next;
    }
}

void DrawAllBullets(void)
{
    Bullet *currentBullet = bulletsHead;

    while (currentBullet)
    {
        DrawPoint(currentBullet->position, (Color){150, 150, 150}, currentBullet->size, true);
        currentBullet = currentBullet->next;
    }
}

void DrawAllEnemies(void)
{
    Enemy *currentEnemy = enemiesHead;

    while (currentEnemy)
    {
        DrawPoint(currentEnemy->position, currentEnemy->color, currentEnemy->size, true);
        currentEnemy = currentEnemy->next;
    }
}
