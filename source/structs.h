#pragma once

#include <stdbool.h>

typedef struct Point
{
    int x;
    int y;
} Point;

bool PointsEqual(Point a, Point b);

bool PointsWithinTolerance(Point a, Point b, int tolerance);

typedef struct Vector
{
    float x;
    float y;
} Vector;

bool VectorsEqual(Vector a, Vector b);

typedef struct Color
{
    char r;
    char g;
    char b;
} Color;

bool ColorsEqual(Color a, Color b);
