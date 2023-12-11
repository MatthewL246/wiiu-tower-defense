#pragma once

#include "structs.h"
#include <coreinit/time.h>
#include <stdbool.h>

// This maps a touch point from the GamePad to the GamePad screen.
Point MapTouchToDrcScreen(Point touchPoint);

// This draws a point as a circle.
void DrawPoint(Point point, Color color, unsigned int size, bool drawOnBothScreens);

// This draws a line from a start point to end point.
void DrawLine(Point start, Point end, Color color, unsigned int width, bool drawOnBothScreens);

// This draws the path that enemies follow.
void DrawEnemyPath(const Point *path);

// This draws all of the towers as points.
void DrawAllTowers(void);

// This draws all of the bullets as points.
void DrawAllBullets(void);

// This draws all of the enemies as points.
void DrawAllEnemies(void);
