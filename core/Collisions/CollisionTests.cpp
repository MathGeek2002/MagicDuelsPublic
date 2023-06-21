#include "CollisionTests.h"
#include "../Debug/Debug.hpp"

bool
TestSphereSphere(
  SphereCollider * const a,
  SphereCollider * const b
)
{
    glm::vec3 dist = a -> GetCenter() - b -> GetCenter();
    float squeare_dist = glm::dot(dist, dist);

    float rSum = a -> GetRadius() + b -> GetRadius();

    return (squeare_dist <= rSum * rSum);
}

bool 
TestAABBAABB(
  ColliderAABB * const collA,
  ColliderAABB * const collB
)
{
    if( collA -> max.x < collB -> min.x || collA -> min.x > collB -> max.x ) return false;
    if( collA -> max.y < collB -> min.y || collA -> min.y > collB -> max.y ) return false;
    if( collA -> max.z < collB -> min.z || collA -> min.z > collB -> max.z ) return false;

    return true;
}

bool TestAABBSphere(
  ColliderAABB * const a,
  SphereCollider * const s
)
{
  float sD = SqDistPointAABB(s->GetCenter(), *a);
  float sR =  s->GetRadius();
  return sD <= sR * sR;
}

bool TestCapsuleSphere(
  CapsuleCollider * const c,
  SphereCollider * const s
)
{
  float dist2 = SqDistPointSegment(c->GetBase(), c->GetTip(), s->GetCenter());

   float radiusSum = s->GetRadius() + c->GetRadius();

  return dist2 <= radiusSum * radiusSum;
}

bool TestCapsuleCapsule(
  CapsuleCollider * const c1,
  CapsuleCollider * const c2
)
{
    float s, t;
    glm::vec3 c1_, c2_;

    float dist2 = ClosestPtSegmentSegment(c1->GetBase(), c1->GetTip(),
    c2->GetBase(), c2->GetTip(), s, t, c1_, c2_);

    float radiusSum = c1->GetRadius() + c2->GetRadius();

    return dist2 <= radiusSum * radiusSum;
}

bool TestBoundaryBoundary(
  ColliderBoundary * const c1,
  ColliderBoundary * const c2
)
{
    if( c1 -> max.x < c2 -> min.x || c1 -> min.x > c2 -> max.x ) return false;
    if( c1 -> max.y < c2 -> min.y || c1 -> min.y > c2 -> max.y ) return false;

    return true;
}