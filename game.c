#undef __STRICT_ANSI__

#include "game.h"

GameState g_state = {
  .mousePos = {
    .x = -1.0f,
    .y = -1.0f,
  },
  .player = {
    .position = {
      .x = 0.0f,
      .y = 1.0f,
      .z = 0.0f
    },
    .look = {
      .pitch = 0.0f,
      .yaw = 0.0f,
      .roll = 0.0f
    },
    .moveSpeed = 0.08f,
    .lookSensitivity = 1.2f
  }
};

int main(int argc, char **argv) {
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("My Game");
  glutReshapeWindow(800, 600);
  glutPositionWindow(150, 150);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutTimerFunc(GAME_LOOP_UPDATE_RATE, gameLoop, 0);
  glutKeyboardFunc(keyPressed);
  glutKeyboardUpFunc(keyUp);
  glutPassiveMotionFunc(mouseMove);
  glutSetCursor(GLUT_CURSOR_NONE);

  init();

  glutMainLoop();

  return 0;
}

void drawBox(Vector3Df position, Vector3Df dimensions, Vector3Df *colors) {

  // Bot
  glColor3f(colors[0].x, colors[0].y, colors[0].z);
  glVertex3f(position.x, position.y, position.z);
  glVertex3f(position.x+dimensions.x, position.y, position.z);
  glVertex3f(position.x+dimensions.x, position.y, position.z+dimensions.z);
  glVertex3f(position.x, position.y, position.z+dimensions.z);

  // Top
  glColor3f(colors[1].x, colors[1].y, colors[1].z);
  glVertex3f(position.x, position.y+dimensions.y, position.z);
  glVertex3f(position.x+dimensions.x, position.y+dimensions.y, position.z);
  glVertex3f(position.x+dimensions.x, position.y+dimensions.y, position.z+dimensions.z);
  glVertex3f(position.x, position.y+dimensions.y, position.z+dimensions.z);

  // Left
  glColor3f(colors[2].x, colors[2].y, colors[2].z);
  glVertex3f(position.x, position.y, position.z);
  glVertex3f(position.x+dimensions.x, position.y, position.z);
  glVertex3f(position.x+dimensions.x, position.y+dimensions.y, position.z);
  glVertex3f(position.x, position.y+dimensions.y, position.z);

  // Right
  glColor3f(colors[3].x, colors[3].y, colors[3].z);
  glVertex3f(position.x, position.y, position.z+dimensions.z);
  glVertex3f(position.x+dimensions.x, position.y, position.z+dimensions.z);
  glVertex3f(position.x+dimensions.x, position.y+dimensions.y, position.z+dimensions.z);
  glVertex3f(position.x, position.y+dimensions.y, position.z+dimensions.z);

  // Front
  glColor3f(colors[4].x, colors[4].y, colors[4].z);
  glVertex3f(position.x, position.y, position.z);
  glVertex3f(position.x, position.y+dimensions.y, position.z);
  glVertex3f(position.x, position.y+dimensions.y, position.z+dimensions.z);
  glVertex3f(position.x, position.y, position.z+dimensions.z);

  // Back
  glColor3f(colors[5].x, colors[5].y, colors[5].z);
  glVertex3f(position.x+dimensions.x, position.y, position.z);
  glVertex3f(position.x+dimensions.x, position.y+dimensions.y, position.z);
  glVertex3f(position.x+dimensions.x, position.y+dimensions.y, position.z+dimensions.z);
  glVertex3f(position.x+dimensions.x, position.y, position.z+dimensions.z);
}

void display(void) {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();

  Player player = g_state.player;

  gluLookAt(player.position.x, player.position.y, player.position.z,
    player.position.x+cos(player.look.yaw),
    player.position.y-tan(player.look.pitch),
    player.position.z+sin(player.look.yaw),
    0.0f, 1.0f, 0.0f);

  glBegin(GL_QUADS);

  for(int i = -25; i < 25; i ++) {
    for(int j = -25; j < 25; j ++) {
      if((i+j)%2) glColor3f(0.7f, 0.6f, 0.6f);
      else glColor3f(0.4f, 0.4f, 0.5f);

      glVertex3f((float)i, 0.0f, (float)j);
      glVertex3f((float)i+1, 0.0f, (float)j);
      glVertex3f((float)i+1, 0.0f, (float)j+1);
      glVertex3f((float)i, 0.0f, (float)j+1);
    }
  }

  for(int i = -25; i < 25; i ++) {
    for(int j = -25; j < 25; j ++) {
      if((i+j)%2) glColor3f(0.7f, 0.6f, 0.6f);
      else glColor3f(0.4f, 0.4f, 0.5f);

      glVertex3f((float)i, 10.0f, (float)j);
      glVertex3f((float)i+1, 10.0f, (float)j);
      glVertex3f((float)i+1, 10.0f, (float)j+1);
      glVertex3f((float)i, 10.0f, (float)j+1);
    }
  }

  for(int i = -25; i < 25; i ++) {
    for(int j = 0; j < 10; j ++) {
      if((i+j)%2) glColor3f(0.8f, 0.6f, 0.6f);
      else glColor3f(0.4f, 0.4f, 0.5f);

      glVertex3f((float)i, (float)j, 25.0f);
      glVertex3f((float)i+1, (float)j, 25.0f);
      glVertex3f((float)i+1, (float)j+1, 25.0f);
      glVertex3f((float)i, (float)j+1, 25.0f);
    }
  }

  for(int i = -25; i < 25; i ++) {
    for(int j = 0; j < 10; j ++) {
      if((i+j)%2) glColor3f(0.8f, 0.6f, 0.6f);
      else glColor3f(0.4f, 0.4f, 0.5f);

      glVertex3f((float)i, (float)j, -25.0f);
      glVertex3f((float)i+1, (float)j, -25.0f);
      glVertex3f((float)i+1, (float)j+1, -25.0f);
      glVertex3f((float)i, (float)j+1, -25.0f);
    }
  }

  for(int i = -25; i < 25; i ++) {
    for(int j = 0; j < 10; j ++) {
      if((i+j)%2) glColor3f(0.6f, 0.6f, 0.6f);
      else glColor3f(0.4f, 0.4f, 0.5f);

      glVertex3f(25.0f, (float)j, (float)i);
      glVertex3f(25.0f, (float)j+1, (float)i);
      glVertex3f(25.0f, (float)j+1, (float)i+1);
      glVertex3f(25.0f, (float)j, (float)i+1);
    }
  }

  for(int i = -25; i < 25; i ++) {
    for(int j = 0; j < 10; j ++) {
      if((i+j)%2) glColor3f(0.6f, 0.6f, 0.6f);
      else glColor3f(0.4f, 0.4f, 0.5f);

      glVertex3f(-25.0f, (float)j, (float)i);
      glVertex3f(-25.0f, (float)j+1, (float)i);
      glVertex3f(-25.0f, (float)j+1, (float)i+1);
      glVertex3f(-25.0f, (float)j, (float)i+1);
    }
  }

  Vector3Df colors[] = {
    (Vector3Df){.x = 1.0f, .y = 0.0f, .z = 0.0f},
    (Vector3Df){.x = 1.0f, .y = 1.0f, .z = 0.0f},
    (Vector3Df){.x = 0.0f, .y = 1.0f, .z = 0.0f},
    (Vector3Df){.x = 1.0f, .y = 0.0f, .z = 1.0f},
    (Vector3Df){.x = 0.0f, .y = 0.0f, .z = 1.0f},
    (Vector3Df){.x = 0.0f, .y = 1.0f, .z = 1.0f},
  };

  for(int i = 0; i < 2*g_state.mazeHeight+1; i++) {
    for(int j = 0; j < 2*g_state.mazeWidth+1; j++) {
      if(i == 0 || i == 2*g_state.mazeHeight ||
         j == 0 || j == 2*g_state.mazeWidth ||
         (i%2 == 0 && j%2 == 0)) {
        drawBox(
          (Vector3Df){.x = i, .y = 0, .z = j},
          (Vector3Df){.x = 1, .y = 0.5, .z = 1},
          colors);
      }
      if(i%2 == 1 && j%2 == 1) {
        if(g_state.maze[i/2][j/2].right == 0) {
          drawBox(
            (Vector3Df){.x = i, .y = 0, .z = j+1},
            (Vector3Df){.x = 1, .y = 0.5, .z = 1},
            colors);
        }
        if(g_state.maze[i/2][j/2].bot == 0) {
          drawBox(
            (Vector3Df){.x = i+1, .y = 0, .z = j},
            (Vector3Df){.x = 1, .y = 0.5, .z = 1},
            colors);
        }
      }
    }
  }

  glEnd();

  glutSwapBuffers();

  glutPostRedisplay();
}

void init(void) {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClearDepth(1.0f);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  g_state.mazeHeight = 10;
  g_state.mazeWidth = 10;
  g_state.maze = generateMaze(g_state.mazeHeight, g_state.mazeWidth);
}

void reshape(int width, int height) {
  if (height == 0) height = 1;

  float aspect = (float)width / (float)height;

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void keyPressed(unsigned char key, int x, int y) {
  g_state.keyStates[key] = 1;

  if (key == 27) exit(0);
}

void keyUp(unsigned char key, int x, int y) {
  g_state.keyStates[key] = 0;
}

void mouseMove(int x, int y) {
  int deltaX = x - g_state.mousePos.x;
  int deltaY = y - g_state.mousePos.y;

  g_state.mousePos.x = x;
  g_state.mousePos.y = y;

  if (deltaX == 0 && deltaY == 0) return;

  int windowX = glutGet(GLUT_WINDOW_X);
  int windowY = glutGet(GLUT_WINDOW_Y);
  int windowW = glutGet(GLUT_WINDOW_WIDTH);
  int windowH = glutGet(GLUT_WINDOW_HEIGHT);

  if (x <= windowX + 200 || y <= windowY + 200 ||
      x >= windowX + windowW - 200 || y >= windowY + windowH - 200) {
    g_state.mousePos.x = windowX + windowW/2;
    g_state.mousePos.y = windowY + windowH/2;
    glutWarpPointer(g_state.mousePos.x, g_state.mousePos.y);
  }

  g_state.player.look.yaw += (float)deltaX*g_state.player.lookSensitivity/500.0f;
  g_state.player.look.pitch += (float)deltaY*g_state.player.lookSensitivity/500.0f;

  if (g_state.player.look.pitch > 1.0f) g_state.player.look.pitch = 1.0f;
  else if (g_state.player.look.pitch < -1.0f) g_state.player.look.pitch = 1.0f;
}

void gameLoop(int value) {
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

  if (g_state.keyStates['q'] && !g_state.keyStates['e']) {
    g_state.player.position.y += g_state.player.moveSpeed;
  }
  else if (g_state.keyStates['e'] && !g_state.keyStates['q']) {
    g_state.player.position.y -= g_state.player.moveSpeed;
  }

  glutPostRedisplay();
  glutTimerFunc(GAME_LOOP_UPDATE_RATE, gameLoop, 0);
}
