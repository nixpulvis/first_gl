#ifndef GAME_H
#define GAME_H

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "geometry.h"
#include "objects.h"
#include "maze.h"

#define GAME_LOOP_UPDATE_RATE 10
#define DEFUALT_WALK_SPEED 0.03f

typedef struct {
  Vector3Df position;
  EulerAngle look;
  float moveSpeed;
  float lookSensitivity;

} Player;

typedef struct {
  Vector2Di mousePos;
  Player player;
  unsigned char keyStates[256];
  Cell** maze;
  int mazeHeight;
  int mazeWidth;
} GameState;

void setup3D(void);
void display3D(void);
void setup2D(void);
void display2D(void);

void display(void);
void init(void);
void reshape(int width, int height);
void keyPressed(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void mouseMove(int x, int y);
void gameLoop(int value);

#endif
