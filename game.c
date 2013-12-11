#undef __STRICT_ANSI__

#include "game.h"

// Declare game state as global variable
GameState g_state = {
  // Declare mouse position
  .mousePos = {
    .x = -1.0f,
    .y = -1.0f,
  },
  // Declare player
  .player = {
    // Delcare player position
    .collision_object = {
      .position = {
        .x = 1.5f,
        .y = 0.5f,
        .z = 1.5f
      },
      .dimensions = {
        .x = 0.25f,
        .y = 0.5f,
        .z = 0.25f
      },
      .dynamic = 1
    },
    .dy = 0.0f,
    .inAir = 1,
    // Declare player look (where player is looking)
    // Used to calculate where camera looks
    .look = {
      .pitch = 0.0f,
      .yaw = 0.0f,
      .roll = 0.0f
    },
    // Declare moveSpeed of player
    .moveSpeed = DEFAULT_WALK_SPEED,

    // Declare lookSensitivity of player
    .lookSensitivity = 0.75f
  }
};

GameStore g_store;

// game.c is the main file, this contains int main
int main(int argc, char **argv) {

  // Initialize glut
  glutInit(&argc, argv);

  // Allow double buffering and depth
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

  // Create and name window
  glutCreateWindow("My Game");

  // Set window to fullscreen (borderless)
  glutFullScreen();

  // Give glut display function
  glutDisplayFunc(display);

  // Give glut reshape function
  glutReshapeFunc(reshape);

  // Give glut keyboard press event function
  glutKeyboardFunc(keyPressed);

  // Give glut keyboard release event function
  glutKeyboardUpFunc(keyUp);

  // Give glut passive mouse movement event function
  glutPassiveMotionFunc(mouseMove);

  // Do not display cursor
  glutSetCursor(GLUT_CURSOR_NONE);

  // Give glut gameLoop, and call it at given rate in ms
  glutTimerFunc(GAME_LOOP_UPDATE_RATE, gameLoop, 0);

  // Other OpenGL and game initializations
  init();

  // Enter the glut event processing loop
  glutMainLoop();

  return 0;
}

int loadBMP(char *filename) {
  
  GLubyte *img;

  BITMAPINFO **img_info = malloc(sizeof(BITMAPINFO *));
  img = LoadDIBitmap(filename, img_info);

  glBindTexture(GL_TEXTURE_2D, g_store.image_store.current);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 0);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, (**img_info).bmiHeader.biWidth, (**img_info).bmiHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);

  return g_store.image_store.current++;
}

// Set up OpenGL for 3D rendering
void setup3D(void) {
  // Set matrix mode to projection and load identity
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Get window width and height
  int width = glutGet(GLUT_WINDOW_WIDTH);
  int height = glutGet(GLUT_WINDOW_HEIGHT);

  // Calculate window aspect ratio and set perspective to:
  // 45 degrees in y direction
  // aspect for aspect ratio
  // near clip to 0.1f
  // far clip to 100.0f
  float aspect = (float) width / (float) height;
  gluPerspective(45.0f, aspect, 0.1f, 100.0f);
  
  // Change matrix mode back to model view and load identity
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Enable depth test for 3D rendering
  glEnable(GL_DEPTH_TEST);

  // Set camera to player position
  // Set camera to look 1 unit in direction of players :look"
  // Set camera orientation to be +y
  float cameraX = g_state.player.collision_object.position.x + 
                  g_state.player.collision_object.dimensions.x/2;
  float cameraY = g_state.player.collision_object.position.y + 
                  g_state.player.collision_object.dimensions.y;
  float cameraZ = g_state.player.collision_object.position.z + 
                  g_state.player.collision_object.dimensions.z/2;

  float cameraLookX = cameraX+cos(g_state.player.look.yaw);
  float cameraLookY = cameraY-tan(g_state.player.look.pitch);
  float cameraLookZ = cameraZ+sin(g_state.player.look.yaw);

  gluLookAt(cameraX, cameraY, cameraZ, 
            cameraLookX, cameraLookY, cameraLookZ,
            0.0f, 1.0f, 0.0f);  
}

// Display 3D game
void display3D(void) {
  // Set up OpenGL for 3D rendering
  setup3D();

  glEnable(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, g_store.image_store.grass);
  glBegin(GL_QUADS);
  glColor3f(1.0f, 1.0f, 1.0f);
  for (int i = 0; i < g_store.maze_store.height*2+1; i++) {
    for (int j = 0; j < g_store.maze_store.width*2+1; j++) {

      glTexCoord2f(0.0f, 0.0f);
      glVertex3f((float)i, 0.0f, (float)j);
      glTexCoord2f(1.0f, 0.0f);
      glVertex3f((float)i+1, 0.0f, (float)j);
      glTexCoord2f(1.0f, 1.0f);
      glVertex3f((float)i+1, 0.0f, (float)j+1);
      glTexCoord2f(0.0f, 1.0f);
      glVertex3f((float)i, 0.0f, (float)j+1);
    }
  }
  glEnd();

  glBindTexture(GL_TEXTURE_2D, g_store.image_store.crate);
  glBegin(GL_QUADS);
  glColor3f(1.0f, 1.0f, 1.0f);
  for (int i = 0; i < g_store.maze_store.numBoxes; i++) {
    CollisionObject box = g_store.maze_store.boxes[i];
    drawTexturedBox(box.position, box.dimensions);
  }
  glEnd();

  glDisable(GL_TEXTURE_2D);
}

// Set up OpenGL for 2D overlay
void setup2D(void) {
  // Set matrix mode to projection and load identity
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Set glOrtho to enire window
  glOrtho(0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0, 0, 1);

  // Set matrix move back to model view and load identity
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Disable depth test since we are now in 2D
  glDisable(GL_DEPTH_TEST);
}

// Display 2D overlay on top of 3D game
void display2D(void) {
  // Set up OpenGL for 2D overlay
  setup2D();

  // Get width and height of screen
  int width = glutGet(GLUT_WINDOW_WIDTH);
  int height = glutGet(GLUT_WINDOW_HEIGHT);

  // Radius of compass
  int compassRadius = 100;

  // Offset from edge of screen
  int compassOffset = 50;

  // Width of compass's hand (technically radius, but not a circle)
  int compassHandWidth = 20;

  // Center of compass radius (the small circle in the center)
  int compassCenterRadius = 5;

  // Number of triangles to generate the pseudo circle
  // Incease for more accuracy
  int compassNumOfTriangles = 64;

  // Center of compass location on screen
  int compassCenterX = width-(compassRadius+compassOffset);
  int compassCenterY = height-(compassRadius+compassOffset);

  // Color of compass, off-white
  glColor3f(0.8f, 0.8f, 0.8f);

  // Draw the Basic circle of the compass
  glBegin(GL_POLYGON);
  for(int i = 0; i < compassNumOfTriangles; i++) {
    glVertex2f(
      compassCenterX+(compassRadius*cos((float)i/((float)compassNumOfTriangles/2)*M_PI)), 
      compassCenterY+(compassRadius*sin((float)i/((float)compassNumOfTriangles/2)*M_PI)));
  }
  glEnd();

  // Calculate the angle that would be North (+x)
  float cardinalAngle = g_state.player.look.yaw - M_PI_2;

  // Color of compass hand, pure red
  glColor3f(1.0f, 0.0f, 0.0f);

  // Draw the Compass Hand
  glBegin(GL_POLYGON);
  glVertex2f(
    compassCenterX-compassRadius*cos(cardinalAngle),
    compassCenterY+compassRadius*sin(cardinalAngle));
  glVertex2f(
    compassCenterX-compassHandWidth*cos(cardinalAngle+M_PI_2),
    compassCenterY+compassHandWidth*sin(cardinalAngle+M_PI_2));
  glVertex2f(
    compassCenterX+compassHandWidth*cos(cardinalAngle),
    compassCenterY-compassHandWidth*sin(cardinalAngle));
  glVertex2f(
    compassCenterX+compassHandWidth*cos(cardinalAngle+M_PI_2),
    compassCenterY-compassHandWidth*sin(cardinalAngle+M_PI_2));
  glEnd();

  // Color of compass center, pure black
  glColor3f(0.0f, 0.0f, 0.0f);

  // Draw the basic circle for the center of the compass
  glBegin(GL_POLYGON);
  for(int i = 0; i < compassNumOfTriangles; i++) {
    glVertex2f(
      compassCenterX+(compassCenterRadius*cos((float)i/16*M_PI)),
      compassCenterY+compassCenterRadius*sin((float)i/16*M_PI));
  }
  glEnd();

  // Color of crosshair, pure black
  glColor3f(0.0f, 0.0f, 0.0f);

  // Draw crosshair 2 pixels thick in center of screen
  glBegin(GL_QUADS);
  glVertex2f(width/2-1, height/2-10);
  glVertex2f(width/2+1, height/2-10);
  glVertex2f(width/2+1, height/2+10);
  glVertex2f(width/2-1, height/2+10);

  glVertex2f(width/2-10, height/2-1);
  glVertex2f(width/2+10, height/2-1);
  glVertex2f(width/2+10, height/2+1);
  glVertex2f(width/2-10, height/2+1);

  glEnd();
}

// Rendering method for OpenGL. Display game
void display(void) {

  // Clear buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Display 3D scene, then 2D overlay
  display3D();
  display2D();

  // Swap buffers
  glutSwapBuffers();
}

CollisionObject createMazeBlock(float i, float j) {
  return (CollisionObject) {
    .dynamic = 0,
    .position = (Vector3Df) {
      .x = i, 
      .y = 0, 
      .z = j
    },
    .dimensions = (Vector3Df) {
      .x = 1, 
      .y = 1, 
      .z = 1
    }
  };
}

void initMaze(int width, int height) {
  // Randomly generate the maze
  Cell** maze = generateMaze(height, width);

  int maxBoxes = (2*width+1) * (2*height+1);
  g_store.maze_store.boxes = malloc(maxBoxes * sizeof(CollisionObject));

  int place = 0;
  for (int i = 0; i < 2*height+1; i++) {
    for (int j = 0; j < 2*width+1; j++) {
      if (i == 0 || i == 2*height ||
         j == 0 || j == 2*width  ||
         (i%2 == 0 && j%2 == 0)) {
        g_store.maze_store.boxes[place++] = createMazeBlock(i, j);
      }
      else if (i%2 == 1 && j%2 == 1) {
        if (maze[i/2][j/2].right == 0) {
          g_store.maze_store.boxes[place++] = createMazeBlock(i, j+1);
        }
        if (maze[i/2][j/2].bot == 0) {
          g_store.maze_store.boxes[place++] = createMazeBlock(i+1, j);
        }
      }
    }
  }

  g_store.maze_store.height = height;
  g_store.maze_store.width = width;

  g_store.maze_store.numBoxes = place;
}

// Initialize OpenGL and game information
void init(void) {
  // Set background to black and clear depth
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClearDepth(1.0f);

  // Enable Depth Test (for 3D rendering) and use <= for depth fucntion
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  // Disply high quality
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  g_store.image_store.grass = loadBMP("grass.bmp");
  g_store.image_store.crate = loadBMP("crate.bmp");
  g_store.image_store.concrete = loadBMP("concrete.bmp");

  initMaze(10, 10);
}

// Callback for change window size event from OpenGL
void reshape(int width, int height) {
  // Make sure the window has a positive height
  if (height == 0) height = 1;

  // Calculate window aspect ratio
  float aspect = (float)width / (float)height;

  // Set viewport to entire window
  glViewport(0, 0, width, height);

  // Set matric mode to projection and load the identity
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // Set perspective to:
  // 45 degrees in y direction
  // aspect for aspect ratio
  // near clip to 0.1f
  // far clip to 100.0f
  gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

// Callback for key press event
void keyPressed(unsigned char key, int x, int y) {
  // Set key to pressed or active
  g_state.keyStates[key] = 1;

  // If hitting escape, exit program with no error
  if (key == 27) exit(0);
}

// Callback for key release event
void keyUp(unsigned char key, int x, int y) {
  // Set key to non pressed or inactive
  g_state.keyStates[key] = 0;
}

// Callback for passive movement of mouse
void mouseMove(int x, int y) {
  // Calculate change in mouse position
  int deltaX = x - g_state.mousePos.x;
  int deltaY = y - g_state.mousePos.y;

  // If mouse has not moved, return
  if (deltaX == 0 && deltaY == 0) return;

  // Change last mouse position to current mouse position
  g_state.mousePos.x = x;
  g_state.mousePos.y = y;

  // Get windows position and dimensions
  int windowX = glutGet(GLUT_WINDOW_X);
  int windowY = glutGet(GLUT_WINDOW_Y);
  int windowW = glutGet(GLUT_WINDOW_WIDTH);
  int windowH = glutGet(GLUT_WINDOW_HEIGHT);

  // Maximum pixels away from center in either x or y before mouse is
  // returned back to center of window
  int mouseBox = 200;

  // If the mouse is outside its allowed range, move it back to center
  if (x <= windowX + mouseBox || y <= windowY + mouseBox ||
      x >= windowX + windowW - mouseBox || y >= windowY + windowH - mouseBox) {
    // Change last mouse position to center of window
    g_state.mousePos.x = windowX + windowW/2;
    g_state.mousePos.y = windowY + windowH/2;

    // Move mouse to current mouse position(center of window)
    glutWarpPointer(g_state.mousePos.x, g_state.mousePos.y);
  }

  // Change players yaw based on deltaX and pitch based on deltaY
  g_state.player.look.yaw += (float) deltaX*g_state.player.lookSensitivity/500.0f;
  g_state.player.look.pitch += (float) deltaY*g_state.player.lookSensitivity/500.0f;

  // If player looking to high or low, stop
  // This prevents looking to far up and seeing behind you with camera
  // upside down
  if (g_state.player.look.pitch > 1.5f) g_state.player.look.pitch = 1.5f;
  else if (g_state.player.look.pitch < -1.5f) g_state.player.look.pitch = -1.5f;
}

// Evaluates new game state based on current keys pressed
void handleKeys(float dt) {
  Player *player = &g_state.player;

  // If walking, move player along floor (x-z axis)
  if (g_state.keyStates['w'] && !g_state.keyStates['s']) {
    player->collision_object.position.x += player->moveSpeed*dt*cos(player->look.yaw);
    player->collision_object.position.z += player->moveSpeed*dt*sin(player->look.yaw);
  }
  else if (g_state.keyStates['s'] && !g_state.keyStates['w']) {
    player->collision_object.position.x -= player->moveSpeed*dt*cos(player->look.yaw);
    player->collision_object.position.z -= player->moveSpeed*dt*sin(player->look.yaw);
  }

  if (g_state.keyStates['a'] && !g_state.keyStates['d']) {
    player->collision_object.position.x -= player->moveSpeed*dt*cos(player->look.yaw + M_PI_2);
    player->collision_object.position.z -= player->moveSpeed*dt*sin(player->look.yaw + M_PI_2);
  }
  else if (g_state.keyStates['d'] && !g_state.keyStates['a']) {
    player->collision_object.position.x += player->moveSpeed*dt*cos(player->look.yaw + M_PI_2);
    player->collision_object.position.z += player->moveSpeed*dt*sin(player->look.yaw + M_PI_2);
  }

  if (g_state.keyStates[' '] && !(player->inAir)) {
    player->dy = 5.0f;
    player->inAir = 1;
  }
}

void handleCollisions(void) {
  CollisionObject *player = &g_state.player.collision_object;

  for (int i = 0; i < g_store.maze_store.numBoxes; i++) {
    CollisionObject box = g_store.maze_store.boxes[i];

    Face collide = isColliding(*player, box);
    if(collide) {
      if(collide & 0x01) {
        player->position.x = box.position.x+box.dimensions.x;
      }
      else if(collide & 0x02) {
        player->position.x = box.position.x-player->dimensions.x;
      }
      else if(collide & 0x04) {
        player->position.y = box.position.y+box.dimensions.y;
        
        if(g_state.player.dy < 0) {
          g_state.player.dy = 0.0f;
          g_state.player.inAir = 0;
        }
      }
      else if(collide & 0x08) {
        player->position.y = box.position.y-player->dimensions.y;
      }
      else if(collide & 0x10) {
        player->position.z = box.position.z+box.dimensions.z;
      }
      else if(collide & 0x20) {
        player->position.z = box.position.z-player->dimensions.z;
      }
    }
  }
}

void handlePhysics(float dt) {
  g_state.player.inAir = 1;

  g_state.player.dy += dt*(float)GRAVITY;
  g_state.player.collision_object.position.y += dt*g_state.player.dy;

  if (g_state.player.collision_object.position.y < 0.0f) {
    g_state.player.collision_object.position.y = 0.0f;
    g_state.player.dy = 0.0f;
    g_state.player.inAir = 0;
  }

  handleCollisions();
}

// Main game loop
void gameLoop(int value) {
  clock_t timer = clock();
  int delta = timer - value;
  float dt = (float)delta/(float)CLOCKS_PER_SEC;

  // Handle keys and move player
  handleKeys(dt);

  // Handle physics of game
  handlePhysics(dt);

  // Redraw window
  glutPostRedisplay();

  // Call gameLoop again after given time in ms
  glutTimerFunc(GAME_LOOP_UPDATE_RATE, gameLoop, timer);
}
