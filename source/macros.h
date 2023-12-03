#pragma once

#include "structs.h"

#define CONST_INVALID_POINT {-1, -1}
#define INVALID_POINT (Point) CONST_INVALID_POINT

#define DRC_SCREEN_WIDTH 854
#define DRC_SCREEN_HEIGHT 480

#define SPEED_CONSTANT 1000

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
