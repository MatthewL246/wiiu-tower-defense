#include "structs.h"

#include <stdbool.h>
#include <stdlib.h>

bool PointsEqual(Point a, Point b)
{
    return a.x == b.x && a.y == b.y;
}

bool PointsWithinTolerance(Point a, Point b, int tolerance)
{
    // This uses a square collision for the points because sqrt is slow
    return abs(a.x - b.x) <= tolerance && abs(a.y - b.y) <= tolerance;
}

bool VectorsEqual(Vector a, Vector b)
{
    return a.x == b.x && a.y == b.y;
}

bool ColorsEqual(Color a, Color b)
{
    return a.r == b.r && a.g == b.g && a.b == b.b;
}
