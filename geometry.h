#ifndef GEOMETRY_H
#define GEOMETRY_H 

typedef struct {
  float pitch;
  float yaw;
  float roll;
} EulerAngle;

typedef struct {
  float x;
  float y;
} Vector2D;

typedef struct {
  float x;
  float y;
  float z;
} Vector3D;

#endif
