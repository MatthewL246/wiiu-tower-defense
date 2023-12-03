#pragma once

#include "structs.h"
#include <coreinit/time.h>
#include <stdbool.h>

Point MapTouchToDrcScreen(int touchX, int touchY);

void DrawPoint(Point point, Color color, int size, bool drawOnBothScreens);

void DrawLine(Point start, Point end, Color color, int width, bool drawOnBothScreens);

void DrawEnemyPath(const Point *path);

void DrawAllTowers(void);

void DrawAllBullets(void);

void DrawAllEnemies(void);
