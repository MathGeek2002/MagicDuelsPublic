#ifndef COLLISION_TOOLS_H
#define COLLISION_TOOLS_H

#include "../../thirdparty/glm/glm.hpp"
#include "ColliderAABB.h"
#include "CapsuleCollider.h"
#include "ColliderAABB.h"

float
SqDistPointAABB(
  glm::vec3 const &p,
  ColliderAABB const &b
);

void
ClosestPointOnAABB(
  glm::vec3 const &p, 
  ColliderAABB const &a, 
  glm::vec3 &q 
);

float SqDistPointSegment(glm::vec3 a, glm::vec3 b, glm::vec3 c);

float ClosestPtSegmentSegment(glm::vec3 p1, glm::vec3 q1, glm::vec3 p2, glm::vec3 q2,
float &s, float &t, glm::vec3 &c1, glm::vec3 &c2);

#endif