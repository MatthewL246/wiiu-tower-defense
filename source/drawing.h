#pragma once

#include "structs.h"
#include <coreinit/time.h>
#include <stdbool.h>

#define DRC_SCREEN_WIDTH 854
#define DRC_SCREEN_HEIGHT 480

Point MapTouchToDrcScreen(int touchX, int touchY);

void DrawPoint(Point point, Color color, int size, bool drawOnBothScreens);

void DrawAllTowers(void);

void DrawAllBullets(void);

void DrawAllEnemies(void);
