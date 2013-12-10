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
#include "collision_object.h"
#include "maze.h"

// Constants
#define GAME_LOOP_UPDATE_RATE 10
#define DEFAULT_WALK_SPEED 0.02f

// Player struct
typedef struct {
  CollisionObject collision_object;
  EulerAngle look;
  float moveSpeed;
  float lookSensitivity;
} Player;

// GameState struct
typedef struct {
  Vector2Di mousePos;
  Player player;
  unsigned char keyStates[256];
} GameState;

// ImageStore struct
typedef struct {
  int current;

  int grass;
  int crate;
  int concrete;
} ImageStore;

// MazeStore struct
typedef struct {
  int height;
  int width;

  int numBoxes;

  CollisionObject *boxes;
} MazeStore;

typedef struct {
  ImageStore image_store;
  MazeStore maze_store;
} GameStore;

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
void handleCollisions(void);
void gameLoop(int value);

#endif
