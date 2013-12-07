#ifndef GEOMETRY_H
#define GEOMETRY_H 

typedef struct {
  float pitch;
  float yaw;
  float roll;
} EulerAngle;

typedef struct {
  int x;
  int y;
} Vector2Di;

typedef struct {
  float x;
  float y;
} Vector2Df;

typedef struct {
  int x;
  int y;
  int z;
} Vector3Di;

typedef struct {
  float x;
  float y;
  float z;
} Vector3Df;

#endif
