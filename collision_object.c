#include "collision_object.h"

int isColliding(CollisionObject a, CollisionObject b) {
	return a.position.x < b.position.x + b.dimensions.x &&
			   a.position.x + a.dimensions.x > b.position.x &&
		     a.position.y < b.position.y + b.dimensions.y &&
		     a.position.y + a.dimensions.y > b.position.y &&
		     a.position.z < b.position.z + b.dimensions.z &&
		     a.position.z + a.dimensions.z > b.position.z;
}