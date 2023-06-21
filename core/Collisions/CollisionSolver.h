#ifndef COLLISION_SOLVER_H
#define COLLISION_SOLVER_H

#include "CollisionProduct.hpp"
#include "SphereCollider.h"
#include "ColliderAABB.h"
#include "GJK.h"
#include "CollisionTools.h"
#include "CapsuleCollider.h"

CollisionProduct* ContactSphereSphere(
	SphereCollider * s1, 
  SphereCollider * s2
);

CollisionProduct * ContactAABBAABB(
  ColliderAABB * a1,
  ColliderAABB * a2
);

CollisionProduct * ContactAABBSphere(
  ColliderAABB * a_,
  SphereCollider * s_
);

CollisionProduct * ContactCapsuleSphere(
  CapsuleCollider * c_,
  SphereCollider * s_
);

CollisionProduct * ContactCapsuleCapsule(
  CapsuleCollider * const c1,
  CapsuleCollider * const c2
);

CollisionProduct * ContactCapsuleAABB(
    CapsuleCollider * const c,
    ColliderAABB * const a
);

#endif