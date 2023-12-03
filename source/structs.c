#include "structs.h"

#include <stdbool.h>
#include <stdlib.h>

bool PointEquals(Point a, Point b)
{
    return a.x == b.x && a.y == b.y;
}

bool PointInTolerance(Point a, Point b, int tolerance)
{
    return abs(a.x - b.x) <= tolerance && abs(a.y - b.y) <= tolerance;
}

bool VectorEquals(Vector a, Vector b)
{
    return a.x == b.x && a.y == b.y;
}

bool ColorEquals(Color a, Color b)
{
    return a.r == b.r && a.g == b.g && a.b == b.b;
}
