#pragma once

#include <stdbool.h>

// This represents a 2D point with x and y coordinates.
typedef struct Point
{
    // This is the x coordinate of the point.
    int x;
    // This is the y coordinate of the point.
    int y;
} Point;

// This checks if two points are equal.
bool PointsEqual(Point a, Point b);

// This checks if two points are within a specified tolerance of each other.
// Note that this treats the points as squares, not circles.
bool PointsWithinTolerance(Point a, Point b, int tolerance);

// This represents a 2D vector with x and y components.
typedef struct Vector
{
    // This is the x component of the vector.
    float x;
    // This is the y component of the vector.
    float y;
} Vector;

// This checks if two vectors are equal.
bool VectorsEqual(Vector a, Vector b);

// This represents an RGB color with red, green, and blue components.
typedef struct Color
{
    // This is the red component of the color, ranging from 0 to 255.
    unsigned char r;
    // This is the green component of the color, ranging from 0 to 255.
    unsigned char g;
    // This is the blue component of the color, ranging from 0 to 255.
    unsigned char b;
} Color;

// This checks if two colors are equal.
bool ColorsEqual(Color a, Color b);
