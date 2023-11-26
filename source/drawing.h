#ifndef DRAWING_H
#define DRAWING_H

#include <stdbool.h>

#include "structs.h"
#include <coreinit/time.h>

#define DRC_SCREEN_WIDTH 854
#define DRC_SCREEN_HEIGHT 480

Point MapTouchToDrcScreen(int touchX, int touchY);

void DrawPoint(Point point, Color color, int size, bool drawOnBothScreens);

void DrawAllTowers();

void DrawAllBullets();

void DrawAllEnemies();

#endif
