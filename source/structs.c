#include <stdbool.h>

#include "structs.h"

bool PointEquals(Point a, Point b)
{
    return a.x == b.x && a.y == b.y;
}

bool VectorEquals(Vector a, Vector b)
{
    return a.x == b.x && a.y == b.y;
}

bool ColorEquals(Color a, Color b)
{
    return a.r == b.r && a.g == b.g && a.b == b.b;
}
