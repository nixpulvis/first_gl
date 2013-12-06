#undef __STRICT_ANSI__

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

typedef struct {
  float pitch;
  float yaw;
  float roll;
} EulerAngle;

typedef struct {
  float x;
  float y;
} Posn2;

typedef struct {
  float x;
  float y;
  float z;
} Posn3;

typedef struct {
  Posn3 position;
  EulerAngle look;
  float moveSpeed;
  float lookSensitivity;

} Player;

typedef struct {
  Posn2 mousePos;
  Player player;
  unsigned char keyStates[256];
} gameState;

void display(void) {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();

  /*gluLookAt(player1.position.x, player1.position.y, player1.position.z,
    player1.position.x+cos(player1.eye.yaw),
    player1.position.y-tan(player1.eye.pitch),
    player1.position.z+sin(player1.eye.yaw),
    0.0f, 1.0f, 0.0f);*/

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
  keyStates[key] = 1;
}

void keyUp(unsigned char key, int x, int y) {
  keyStates[key] = 0;
}

void mouseMove(int x, int y) {

}

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DOUBLE);
  glutCreateWindow("My Game");
  glutReshapeWindow(800, 600);
  glutPositionWindow(150, 150);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyPressed);
  glutKeyboardUpFunc(keyUp);
  glutPassiveMotionFunc(mouseMove);
  glutSetCursor(GLUT_CURSOR_NONE);

  init();

  glutMainLoop();

  return 0;
}


/*switch (key) {
    case 'w':
      player1.position.x += player1.speed*cos(player1.eye.yaw);
      player1.position.z += player1.speed*sin(player1.eye.yaw);
      break;
    case 's':
      player1.position.x -= player1.speed*cos(player1.eye.yaw);
      player1.position.z -= player1.speed*sin(player1.eye.yaw);
      break;
    case 'a':
      player1.position.x -= player1.speed*cos(player1.eye.yaw + M_PI/2);
      player1.position.z -= player1.speed*sin(player1.eye.yaw + M_PI/2);
      break;
    case 'd':
      player1.position.x += player1.speed*cos(player1.eye.yaw + M_PI/2);
      player1.position.z += player1.speed*sin(player1.eye.yaw + M_PI/2);
      break;
    case 'q':
      player1.position.y += 1;
      break;
    case 'e':
      player1.position.y -= 1;
      break;
    case 'x':
      exit(0);
  }*/
