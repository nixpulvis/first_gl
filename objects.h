#ifndef OBJECTS_H
#define OBJECTS_H 

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <math.h>
#include "geometry.h"

// All objects extend along the positive axes. For example,
// drawBox given position vector <0,0,0> and with dimension vector
// <1,2,3> will produce a box expanding from <0,0,0> positively
// on all axes, 1 in x, 2 in y, and 3 in z.
// This applies as expected with negative dimensions,
// that is negatively along the axes.

// Top and Bottom refer to the y axis
// Right and Left refer to the x axis
// Front and Back refer to the z axis

// Objects accepts an array of colors. The vectors x, y, and z
// represent r, g, and b.

// drawBox accepts 6 color vectors that respectively apply to:
// Top -> Bottom -> Left -> Right -> Front -> Back
void drawBox(Vector3Df position, Vector3Df dimensions, Vector3Df *colors);

// drawPyramid accepts 5 color vectors that respectively apply to:
// Bottom -> Left -> Right -> Front -> Back
void drawPyramid(Vector3Df position, Vector3Df dimensions, Vector3Df *colors);

// drawRectangle accepts 1 color and only a 2D dimension vector.
void drawRectangle(Vector3Df position, Vector2Df dimensions, Vector3Df *colors);

#endif