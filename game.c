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
    .moveSpeed = 0.1f,
    .lookSensitivity = 0.1
  }
};

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DOUBLE);
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

  glutPostRedisplay();
  glutTimerFunc(GAME_LOOP_UPDATE_RATE, gameLoop, 0);
}
