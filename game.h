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

#include "bitmap.h"
#include "geometry.h"
#include "objects.h"
#include "maze.h"

// Constants
#define GAME_LOOP_UPDATE_RATE 10
#define DEFAULT_WALK_SPEED 0.02f

// Player struct
typedef struct {
  Vector3Df position;
  EulerAngle look;
  float moveSpeed;
  float lookSensitivity;

} Player;

// GameState struct
typedef struct {
  Vector2Di mousePos;
  Player player;
  unsigned char keyStates[256];
  Cell** maze;
  int mazeHeight;
  int mazeWidth;
} GameState;

// ImageStore struct
typedef struct {
  int current;

  int grass;
  int crate;
  int concrete;
} ImageStore;

// Headers for functions
void setup3D(void);
void display3D(void);
void setup2D(void);
void display2D(void);

int loadBMP(char* filename);

void display(void);
void init(void);
void reshape(int width, int height);
void keyPressed(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void mouseMove(int x, int y);
void handleKeys(void); 
void gameLoop(int value);

#endif
