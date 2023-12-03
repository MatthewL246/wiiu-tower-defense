#pragma once

#include <stdbool.h>

typedef struct Point
{
    int x;
    int y;
} Point;

bool PointEquals(Point a, Point b);

#define CONST_INVALID_POINT { -1, -1 }
#define INVALID_POINT (Point) CONST_INVALID_POINT

typedef struct Vector
{
    float x;
    float y;
} Vector;

bool VectorEquals(Vector a, Vector b);

typedef struct Color
{
    char r;
    char g;
    char b;
} Color;

bool ColorEquals(Color a, Color b);
