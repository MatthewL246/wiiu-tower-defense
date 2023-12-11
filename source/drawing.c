#include "drawing.h"

#include "bullet.h"
#include "enemy.h"
#include "macros.h"
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
const float tvScaleX = (float)TV_WIDTH / DRC_SCREEN_WIDTH;
const float tvScaleY = (float)TV_HEIGHT / DRC_SCREEN_HEIGHT;

void DrawPoint(Point point, Color color, int size, bool drawOnBothScreens)
{
    int pixelColor = color.r << 24 | color.g << 16 | color.b << 8;
    int halfSize = size / 2;

    for (int dx = -halfSize; dx <= halfSize; dx++)
    {
        for (int dy = -halfSize; dy <= halfSize; dy++)
        {
            if (dx * dx + dy * dy <= halfSize * halfSize)
            {
                OSScreenPutPixelEx(SCREEN_DRC, point.x + dx, point.y + dy, pixelColor);
                if (drawOnBothScreens)
                {
                    int tvX = (point.x + dx) * tvScaleX;
                    int tvY = (point.y + dy) * tvScaleY;
                    for (int tvDx = -1; tvDx <= 1; tvDx++)
                    {
                        for (int tvDy = -1; tvDy <= 1; tvDy++)
                        {
                            OSScreenPutPixelEx(SCREEN_TV, tvX + tvDx, tvY + tvDy, pixelColor);
                        }
                    }
                }
            }
        }
    }
}

void DrawLine(Point start, Point end, Color color, int width, bool drawOnBothScreens)
{
    // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
    int dx = abs(end.x - start.x);
    int dy = abs(end.y - start.y);
    int signX = start.x < end.x ? 1 : -1;
    int signY = start.y < end.y ? 1 : -1;
    int error = dx - dy;

    while (!PointsEqual(start, end))
    {
        DrawPoint(start, color, width, drawOnBothScreens);
        int error2 = 2 * error;
        if (error2 > -dy)
        {
            error -= dy;
            start.x += signX;
        }
        if (error2 < dx)
        {
            error += dx;
            start.y += signY;
        }
    }
}

void DrawEnemyPath(const Point *path)
{
    int i = 0;
    while (!PointsEqual(path[i + 1], INVALID_POINT))
    {
        Color pathColor = (Color){50, 50, 50};
        DrawLine(path[i], path[i + 1], pathColor, 2, true);
        i++;
    }
}

void DrawAllTowers(void)
{
    for (Tower *currentTower = towersHead; currentTower; currentTower = currentTower->next)
    {
        DrawPoint(currentTower->position, currentTower->color, currentTower->size, true);
        if (!PointsEqual(currentTower->targetPosition, INVALID_POINT))
        {
            DrawPoint(currentTower->targetPosition, (Color){255, 155, 0}, 10, true);
        }
    }
}

void DrawAllBullets(void)
{
    for (Bullet *currentBullet = bulletsHead; currentBullet; currentBullet = currentBullet->next)
    {
        DrawPoint(currentBullet->position, (Color){150, 150, 150}, currentBullet->size, true);
    }
}

void DrawAllEnemies(void)
{
    for (Enemy *currentEnemy = enemiesHead; currentEnemy; currentEnemy = currentEnemy->next)
    {
        DrawPoint(currentEnemy->position, currentEnemy->color, currentEnemy->size, true);
    }
}
