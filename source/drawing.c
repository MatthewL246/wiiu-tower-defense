#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <coreinit/screen.h>

#include "drawing.h"
#include "tower.h"
#include "bullet.h"
#include "enemy.h"

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
            if (sqrt(dx * dx + dy * dy) <= halfSize)
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

void DrawAllTowers()
{
    for (int i = 0; i < towerCount; i++)
    {
        DrawPoint(towers[i].position, towers[i].color, towers[i].size, true);
        DrawPoint(towers[i].targetPosition, (Color){255, 155, 0}, 10, true);
    }
}

void DrawAllBullets()
{
    for (int i = 0; i < bulletCount; i++)
    {
        DrawPoint(bullets[i].position, (Color){150, 150, 150}, bullets[i].size, true);
    }
}

void DrawAllEnemies()
{
    for (int i = 0; i < enemyCount; i++)
    {
        DrawPoint(enemies[i].position, enemies[i].color, enemies[i].size, true);
    }
}
