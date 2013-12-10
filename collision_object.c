#include "collision_object.h"
#include <stdio.h>

unsigned int maxIndex(float *nums, int size) {
  if (size == 0) return -1;

  int curr = 0;
  for (int i = 1; i < size; i++) {
    if (nums[i] > nums[curr]) curr = i;
  }

  return curr;
}

Face isColliding(CollisionObject a, CollisionObject b) {

  if (a.position.x < b.position.x + b.dimensions.x &&
			a.position.x + a.dimensions.x > b.position.x &&
		  a.position.y < b.position.y + b.dimensions.y &&
		  a.position.y + a.dimensions.y > b.position.y &&
		  a.position.z < b.position.z + b.dimensions.z &&
		  a.position.z + a.dimensions.z > b.position.z) {

    float dTop = (b.position.y + b.dimensions.y) - (a.position.y + a.dimensions.y);
    float dBot = (b.position.y) - (a.position.y);

    float dRight = (b.position.x + b.dimensions.x) - (a.position.x + a.dimensions.x);
    float dLeft = (b.position.x) - (a.position.x);

    float dBack = (b.position.z + b.dimensions.z) - (a.position.z + a.dimensions.z);
    float dFront = (b.position.z) - (a.position.z);

    float deltas[] = {
      fabsf(dLeft), 
      fabsf(dRight),
      fabsf(dBot),
      fabsf(dTop), 
      fabsf(dFront),
      fabsf(dBack) 
    };

    return (Face) pow(2, maxIndex(deltas, 6));
  }
  else return 0;
}
