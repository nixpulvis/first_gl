// #include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
 
#include <stdio.h>
 
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
} player;

player player1 = {
  {0.0f, 0.0f, -5.0f},
  {0.0f, 0.0f}
};


void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);

  glLoadIdentity();
  glTranslatef(player1.position.x, player1.position.y, player1.position.z);

  glBegin(GL_TRIANGLES);

  glColor3f(1.0f, 0.0f, 0.0f);     
  glVertex3f(0.0f, 1.0f, 0.0f);
  glColor3f(0.0f, 1.0f, 0.0f);     
  glVertex3f(-1.0f, -1.0f, 1.0f);
  glColor3f(0.0f, 0.0f, 1.0f);     
  glVertex3f(1.0f, -1.0f, 1.0f);

  
  glColor3f(1.0f, 0.0f, 0.0f);     
  glVertex3f(0.0f, 1.0f, 0.0f);
  glColor3f(0.0f, 0.0f, 1.0f);     
  glVertex3f(1.0f, -1.0f, 1.0f);
  glColor3f(0.0f, 1.0f, 0.0f);     
  glVertex3f(1.0f, -1.0f, -1.0f);

  
  glColor3f(1.0f, 0.0f, 0.0f);     
  glVertex3f(0.0f, 1.0f, 0.0f);
  glColor3f(0.0f, 1.0f, 0.0f);     
  glVertex3f(1.0f, -1.0f, -1.0f);
  glColor3f(0.0f, 0.0f, 1.0f);     
  glVertex3f(-1.0f, -1.0f, -1.0f);

  
  glColor3f(1.0f,0.0f,0.0f);       
  glVertex3f(0.0f, 1.0f, 0.0f);
  glColor3f(0.0f,0.0f,1.0f);       
  glVertex3f(-1.0f,-1.0f,-1.0f);
  glColor3f(0.0f,1.0f,0.0f);       
  glVertex3f(-1.0f,-1.0f, 1.0f);

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
 
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 'w':
      player1.position.y += 0.1f;
      break;
    case 's':
      player1.position.y -= 0.1f;
      break;
    case 'a':
      player1.position.x -= 0.1f;
      break;
    case 'd':
      player1.position.x += 0.1f;
      break;
    case 'q':
      player1.position.z -= 0.1f;
      break;
    case 'e':
      player1.position.z += 0.1f;
      break;
  }
}
 
int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
 
  glutInitDisplayMode(GLUT_DOUBLE);
  glutCreateWindow("My Game");
  glutReshapeWindow(900, 600);
  glutPositionWindow(150, 150);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);

  init();

  glutMainLoop();
 
  return 0;
}