#ifndef COLLISION_OBJECTS_H
#define COLLISION_OBJECTS_H

#include <math.h>
#include "geometry.h"

typedef struct {
	Vector3Df position;
	Vector3Df dimensions;
	int dynamic;
} CollisionObject;

int isColliding(CollisionObject a, CollisionObject b);


#endif