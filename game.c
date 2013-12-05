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
  float x;
  float y;
  float z;
} posn;

typedef struct {
  float pitch;
  float yaw;
} look;

typedef struct {
  posn position;
  look eye;
  float speed;
  float lookSpeed;
} player;

player player1 = {
  {0.0f, 1.0f, 0.0f},
  {0.0f, 0.0f},
  0.1f,
  0.03f
};

int keyStates[256];

void drawBox(float x, float y, float z, float w, float h, float d) {
  glBegin(GL_QUADS);

  glColor3f(0.7f, 0.7f, 0.8f);
  glVertex3f(x, y, z);
  glVertex3f(x+w, y, z);
  glVertex3f(x+w, y+h, z);
  glVertex3f(x, y+h, z);

  glVertex3f(x, y, z+d);
  glVertex3f(x+w, y, z+d);
  glVertex3f(x+w, y+h, z+d);
  glVertex3f(x, y+h, z+d);

  glColor3f(0.5f, 0.5f, 0.6f);
  glVertex3f(x, y, z);
  glVertex3f(x+w, y, z);
  glVertex3f(x+w, y, z+d);
  glVertex3f(x, y, z+d);

  glVertex3f(x, y+h, z);
  glVertex3f(x+w, y+h, z);
  glVertex3f(x+w, y+h, z+d);
  glVertex3f(x, y+h, z+d);

  glColor3f(0.6f, 0.6f, 0.7f);
  glVertex3f(x, y, z);
  glVertex3f(x, y+h, z);
  glVertex3f(x, y+h, z+d);
  glVertex3f(x, y, z+d);

  glVertex3f(x+w, y, z);
  glVertex3f(x+w, y+h, z);
  glVertex3f(x+w, y+h, z+d);
  glVertex3f(x+w, y, z+d);

  glEnd();
}

void display(void) {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();

  gluLookAt(player1.position.x, player1.position.y, player1.position.z,
    player1.position.x+cos(player1.eye.yaw),
    player1.position.y-tan(player1.eye.pitch),
    player1.position.z+sin(player1.eye.yaw),
    0.0f, 1.0f, 0.0f);

  glBegin(GL_LINES);

  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(1.0f, 0.0f, 0.0f);

  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 1.0f, 0.0f);

  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex3f(0.0f, 0.0f, 0.0f);
  glVertex3f(0.0f, 0.0f, 1.0f);

  glEnd();

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

  drawBox(0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.75f);

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

void keyUp(unsigned char key, int x, int y) {
  keyStates[key] = 1;

  switch (key) {
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
      exit(1);
  }
}


int warped = 1;
void mouse(int x, int y) {

  if(!warped) {
    player1.eye.yaw += (float)(x-400)/1000;
    player1.eye.pitch += (float)(y-300)/1000;

    warped = 1;

    glutWarpPointer(400, 300);
  }
  else warped = 0;
}

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DOUBLE);
  glutCreateWindow("My Game");
  glutReshapeWindow(800, 600);
  glutPositionWindow(150, 150);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyUp);
  glutWarpPointer(400, 300);
  glutPassiveMotionFunc(mouse);

  init();

  glutMainLoop();

  return 0;
}
