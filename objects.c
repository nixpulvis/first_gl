#include "objects.h"

// Render a 3D box (6 faces) at given position, dimensions and colors
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

// Render a 3D pyramid at given position, dimensions and colors
void drawPyramid(Vector3Df position, Vector3Df dimensions, Vector3Df *colors) {
  // nope
}
