#pragma once

#include "structs.h"

// This is an invalid point (without a typecast).
#define CONST_INVALID_POINT {-1, -1}
// This is a point that is invalid because it is not on the screen.
#define INVALID_POINT (Point) CONST_INVALID_POINT

// This is the width of the Wii U GamePad screen in pixels.
#define DRC_SCREEN_WIDTH 854
// This is the height of the Wii U GamePad screen in pixels.
#define DRC_SCREEN_HEIGHT 480

// This is a constant that is used for fixed-point speed calculations. It is
// ususally used in rates of something happening (such as pixels moved) per this
// number of game loops.
#define SPEED_CONSTANT 1000

// This is the maximum of two values.
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
// This is the minimum of two values.
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
