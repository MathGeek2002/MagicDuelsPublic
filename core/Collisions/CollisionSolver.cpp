#include "CollisionSolver.h"
#include "CollisionTests.h"

CollisionProduct * ContactSphereSphere(
	SphereCollider * s1, 
  SphereCollider * s2
)
{
  glm::vec3 center = s1 -> GetCenter();
  glm::vec3 center2 = s2 -> GetCenter();
  glm::vec3 dir = center2 - center;
  glm::vec3 normal = glm::normalize(dir);
  float d = glm::length(dir);

  CollisionProduct * c = new CollisionProduct(true);

  c -> normal = normal;
  c -> Depth = s1 -> GetRadius() + s2 -> GetRadius() - d;
  return c;
}

CollisionProduct * ContactAABBAABB(
  ColliderAABB * a1_,
  ColliderAABB * a2_
)
{
    CollisionProduct * c_ = new CollisionProduct(true);

    glm::vec3 r1 = (a1_ -> min + a1_ -> max) * 0.5f;
    glm::vec3 r2 = (a2_ -> min + a2_ -> max) * 0.5f;
    glm::vec3 r = r2 - r1;
    glm::vec3 e1 = a1_ -> max - r1;
    glm::vec3 e2 = a2_ -> max - r2;
    glm::vec3 e = e1 + e2;
    glm::vec3 d = e - glm::abs(r);

    int i_min = 0;
    float depth = d[i_min];
    for( int i = 1; i < 2; i++ )
    if( d[i] < depth ){
        i_min = i;
        depth = d[i_min];
    }

    c_ -> Depth = depth;

    if( i_min == 0 ){
        
        if( r1[i_min] < r2[i_min] )
            c_ -> normal = glm::vec3(1.f, 0.f, 0.f);
        else
            c_ -> normal = glm::vec3(-1.f, 0.f, 0.f);
        return c_;
    }

    if( i_min == 1 ){

        if( r1[i_min] < r2[i_min] )
            c_ -> normal = glm::vec3(0.f, 1.f, 0.f);
        else
            c_ -> normal = glm::vec3(0.f, -1.f, 0.f);
        return c_;
    }
    return c_;
}

CollisionProduct * ContactAABBSphere(
  ColliderAABB * a_,
  SphereCollider * s_
)
{
    CollisionProduct * c_ = new CollisionProduct(true);
    glm::vec3 p = s_->GetCenter();
    glm::vec3 q;
    ClosestPointOnAABB(p, *a_, q);
    glm::vec3 r = p - q;
    glm::vec3 n = glm::normalize(r);
    float d = glm::length(r);
    
    //c_.position = q;
    c_ -> normal = n;
    c_->Depth = s_ -> GetRadius() - d;
    c_->Depth = -c_->Depth;
    return c_;
}

CollisionProduct * ContactCapsuleSphere(
  CapsuleCollider * caps_,
  SphereCollider * s_
)
{
    CollisionProduct * c_ = new CollisionProduct(true);

    float dist2 = SqDistPointSegment(caps_->GetBase(), caps_->GetTip(), s_->GetCenter());

    float radiusSum = s_->GetRadius() + caps_->GetRadius();

    glm::vec3 capsuleCenter = caps_->GetCenter();
    glm::vec3 sphereCenter = s_->GetCenter();

    if(capsuleCenter != sphereCenter)
        c_->normal =  glm::normalize(capsuleCenter - sphereCenter);
    else
        c_->normal = glm::vec3(1,0,0);

    c_->Depth = std::sqrt(dist2) - radiusSum;
    c_->Depth = c_->Depth;
    return c_;
}

CollisionProduct * ContactCapsuleCapsule(
  CapsuleCollider * const c1,
  CapsuleCollider * const c2
) 
{
    CollisionProduct * c_ = new CollisionProduct(true);

    float s, t;
    glm::vec3 c1_, c2_;

    float dist2 = ClosestPtSegmentSegment(c1->GetBase(), c1->GetTip(),
    c2->GetBase(), c2->GetTip(), s, t, c1_, c2_);
    
    glm::vec3 capsuleCenter = c1->GetCenter();
    glm::vec3 capsuleCenter2 = c2->GetCenter();

    float capsuleRadius = c1->GetRadius();
    float capsuleRadius2 = c2->GetRadius();

    if(capsuleCenter != capsuleCenter2)
        c_->normal = glm::normalize(capsuleCenter - capsuleCenter2);
    else
        c_->normal = glm::vec3(1,0,0);

    c_->Depth = std::sqrt(dist2) - capsuleRadius - capsuleRadius2;

    return c_;
}