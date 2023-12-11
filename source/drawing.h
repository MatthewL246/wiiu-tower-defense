#pragma once

#include "structs.h"
#include <coreinit/time.h>
#include <stdbool.h>

Point MapTouchToDrcScreen(Point touchPoint);

void DrawPoint(Point point, Color color, unsigned int size, bool drawOnBothScreens);

void DrawLine(Point start, Point end, Color color, unsigned int width, bool drawOnBothScreens);

void DrawEnemyPath(const Point *path);

void DrawAllTowers(void);

void DrawAllBullets(void);

void DrawAllEnemies(void);
