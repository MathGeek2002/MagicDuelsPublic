#ifndef COLLISION_TESTS_H
#define COLLISION_TESTS_H

#include "SphereCollider.h"
#include "ColliderAABB.h"
#include "CollisionTools.h"
#include "CapsuleCollider.h"
#include "ColliderBoundary.hpp"

bool
TestSphereSphere(
  SphereCollider * const a,
  SphereCollider * const b
);

bool 
TestAABBAABB(
  ColliderAABB * const a,
  ColliderAABB * const b
);

bool TestAABBSphere(
  ColliderAABB * const a,
  SphereCollider * const s
);

bool TestCapsuleSphere(
  CapsuleCollider * const c,
  SphereCollider * const s
);

bool TestCapsuleCapsule(
  CapsuleCollider * const c1,
  CapsuleCollider * const c2
);

bool TestBoundaryBoundary(
  ColliderBoundary * const c1,
  ColliderBoundary * const c2
);

#endif