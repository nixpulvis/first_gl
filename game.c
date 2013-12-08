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
    .position = {
      .x = 1.5f,
      .y = 1.0f,
      .z = 1.5f
    },
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
    .lookSensitivity = 1.2f
  }
};

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

  // return 0
  return 0;
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
  gluLookAt(g_state.player.position.x, g_state.player.position.y, g_state.player.position.z,
    g_state.player.position.x+cos(g_state.player.look.yaw),
    g_state.player.position.y-tan(g_state.player.look.pitch),
    g_state.player.position.z+sin(g_state.player.look.yaw),
    0.0f, 1.0f, 0.0f);  
}

// Display 3D game
void display3D(void) {
  // Set up OpenGL for 3D rendering
  setup3D();

  // Draw floor
  glBegin(GL_QUADS);  
  for(int i = 0; i < g_state.mazeHeight*2+1; i++) {
    for(int j = 0; j < g_state.mazeWidth*2+1; j++) {
      if((i+j)%2) glColor3f(0.5f, 0.4f, 0.4f);
      else glColor3f(0.2f, 0.2f, 0.3f);

      glVertex3f((float)i, 0.0f, (float)j);
      glVertex3f((float)i+1, 0.0f, (float)j);
      glVertex3f((float)i+1, 0.0f, (float)j+1);
      glVertex3f((float)i, 0.0f, (float)j+1);
    }
  }

  // Draw ceiling
  for(int i = 0; i < g_state.mazeHeight*2+1; i++) {
    for(int j = 0; j < g_state.mazeWidth*2+1; j++) {
      if((i+j)%2) glColor3f(0.9f, 0.8f, 0.8f);
      else glColor3f(0.6f, 0.6f, 0.7f);

      glVertex3f((float)i, 1.5f, (float)j);
      glVertex3f((float)i+1, 1.5f, (float)j);
      glVertex3f((float)i+1, 1.5f, (float)j+1);
      glVertex3f((float)i, 1.5f, (float)j+1);
    }
  }

  // Set color vector for maze walls
  Vector3Df colors[] = {
    (Vector3Df){.x = 0.5f, .y = 0.0f, .z = 0.0f},
    (Vector3Df){.x = 0.5f, .y = 0.5f, .z = 0.0f},
    (Vector3Df){.x = 0.0f, .y = 0.5f, .z = 0.0f},
    (Vector3Df){.x = 0.5f, .y = 0.0f, .z = 0.5f},
    (Vector3Df){.x = 0.0f, .y = 0.0f, .z = 0.5f},
    (Vector3Df){.x = 0.0f, .y = 0.5f, .z = 0.5f},
  };

  // Set dimension vector for maze walls
  Vector3Df dimensions = {.x = 1, .y = 1.5, .z = 1};

  // Render maze
  for(int i = 0; i < 2*g_state.mazeHeight+1; i++) {
    for(int j = 0; j < 2*g_state.mazeWidth+1; j++) {
      if(i == 0 || i == 2*g_state.mazeHeight ||
         j == 0 || j == 2*g_state.mazeWidth ||
         (i%2 == 0 && j%2 == 0)) {
        drawBox(
          (Vector3Df){.x = i, .y = 0, .z = j},
          dimensions,
          colors);
      }
      if(i%2 == 1 && j%2 == 1) {
        if(g_state.maze[i/2][j/2].right == 0) {
          drawBox(
            (Vector3Df){.x = i, .y = 0, .z = j+1},
            dimensions,
            colors);
        }
        if(g_state.maze[i/2][j/2].bot == 0) {
          drawBox(
            (Vector3Df){.x = i+1, .y = 0, .z = j},
            dimensions,
            colors);
        }
      }
    }
  }

  // Draw starting location
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      if((i+j)%2) glColor3f(0.5f, 0.0f, 0.0f);
      else glColor3f(0.0f, 0.0f, 0.0f);

      glVertex3f(1.0f+(float)i/4, 0.001f, 1.0f+(float)j/4);
      glVertex3f(1.0f+(float)i/4+.25, 0.001f, 1.0f+(float)j/4);
      glVertex3f(1.0f+(float)i/4+.25, 0.001f, 1.0f+(float)j/4+.25);
      glVertex3f(1.0f+(float)i/4, 0.001f, 1.0f+(float)j/4+.25);
    }
  }

  // Draw ending location
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      if((i+j)%2) glColor3f(0.5f, 0.5f, 0.0f);
      else glColor3f(0.0f, 0.0f, 0.0f);

      glVertex3f(2*g_state.mazeHeight+(float)i/4-1, 0.001f, 2*g_state.mazeWidth+(float)j/4-1);
      glVertex3f(2*g_state.mazeHeight+(float)i/4+.25-1, 0.001f, 2*g_state.mazeWidth+(float)j/4-1);
      glVertex3f(2*g_state.mazeHeight+(float)i/4+.25-1, 0.001f, 2*g_state.mazeWidth+(float)j/4+.25-1);
      glVertex3f(2*g_state.mazeHeight+(float)i/4-1, 0.001f, 2*g_state.mazeWidth+(float)j/4+.25-1);
    }
  }
  glEnd();
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
  int compassNumOfTriangles = 32;

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

  // Set maze dimensions
  g_state.mazeHeight = 10;
  g_state.mazeWidth = 10;

  // Randomly generate the maze
  g_state.maze = generateMaze(g_state.mazeHeight, g_state.mazeWidth);
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
  g_state.player.look.yaw += (float)deltaX*g_state.player.lookSensitivity/500.0f;
  g_state.player.look.pitch += (float)deltaY*g_state.player.lookSensitivity/500.0f;

  // If player looking to high or low, stop
  // This prevents looking to far up and seeing behind you with camera
  // upside down
  if (g_state.player.look.pitch > 1.5f) g_state.player.look.pitch = 1.5f;
  else if (g_state.player.look.pitch < -1.5f) g_state.player.look.pitch = -1.5f;
}

// Evaluates new game state based on current keys pressed
void handleKeys(void) {
  // If walking, move player along floor (x-z axis)
  if (g_state.keyStates['w'] && !g_state.keyStates['s']) {
    g_state.player.position.x += g_state.player.moveSpeed*cos(g_state.player.look.yaw);
    g_state.player.position.z += g_state.player.moveSpeed*sin(g_state.player.look.yaw);
  }
  else if (g_state.keyStates['s'] && !g_state.keyStates['w']) {
    g_state.player.position.x -= g_state.player.moveSpeed*cos(g_state.player.look.yaw);
    g_state.player.position.z -= g_state.player.moveSpeed*sin(g_state.player.look.yaw);
  }

  if (g_state.keyStates['a'] && !g_state.keyStates['d']) {
    g_state.player.position.x -= g_state.player.moveSpeed*cos(g_state.player.look.yaw + M_PI_2);
    g_state.player.position.z -= g_state.player.moveSpeed*sin(g_state.player.look.yaw + M_PI_2);
  }
  else if (g_state.keyStates['d'] && !g_state.keyStates['a']) {
    g_state.player.position.x += g_state.player.moveSpeed*cos(g_state.player.look.yaw + M_PI_2);
    g_state.player.position.z += g_state.player.moveSpeed*sin(g_state.player.look.yaw + M_PI_2);
  }

  // Move player vertically (along y)
  if (g_state.keyStates['q'] && !g_state.keyStates['e']) {
    g_state.player.position.y += g_state.player.moveSpeed;
  }
  else if (g_state.keyStates['e'] && !g_state.keyStates['q']) {
    g_state.player.position.y -= g_state.player.moveSpeed;
  }

  // If player pressing spacebar, increase speed
  if (g_state.keyStates[' ']) {
    g_state.player.moveSpeed = DEFAULT_WALK_SPEED * 2;
  } else {
    g_state.player.moveSpeed = DEFAULT_WALK_SPEED;
  }
}

// Main game loop
void gameLoop(int value) {
  // Handle keys and move player
  handleKeys();

  // Redraw window
  glutPostRedisplay();

  // Call gameLoop again after given time in ms
  glutTimerFunc(GAME_LOOP_UPDATE_RATE, gameLoop, 0);
}
