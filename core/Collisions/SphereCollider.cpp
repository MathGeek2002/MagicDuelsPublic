#include "SphereCollider.h"
#include "CollisionTests.h"
#include "CollisionSolver.h"
#include "ColliderAABB.h"
#include "CapsuleCollider.h"

bool SphereCollider::TestCollision(Collider* collider_)
{
    switch ( collider_ -> type ) {

    case  CT_SPHERE_COLLIDER:
        return TestSphereSphere( this, static_cast<SphereCollider*>(collider_));
    
    case CT_AABB_COLLIDER:
        return TestAABBSphere( static_cast<ColliderAABB*>(collider_) , this );

    case CT_CAPSULE_COLLIDER:
        return TestCapsuleSphere( static_cast<CapsuleCollider*>(collider_) ,this );

    default:
        return true;
    }
}

CollisionProduct * SphereCollider::Solve(Collider * collider_) 
{

    switch ( collider_ -> type ) {

    case  CT_SPHERE_COLLIDER:
        return ContactSphereSphere( this, static_cast<SphereCollider*>(collider_) );
    
    case CT_AABB_COLLIDER:
        return ContactAABBSphere( static_cast<ColliderAABB*>(collider_) , this );

    case CT_CAPSULE_COLLIDER:
        return ContactCapsuleSphere( static_cast<CapsuleCollider*>(collider_) ,this );

    default:
        return GJK( this, collider_ );
    }
}

void SphereCollider::CreateGizmo()
{
    gizmoVertices = new Vertex[numberOfGizmoVertices];

    float d_ang = 360.f / numberOfGizmoVertices;
    float ang = 0.f;

    for(int i = 0;i < numberOfGizmoVertices; i++)
    {
        gizmoVertices[i].position = glm::vec3( glm::cos(glm::radians(ang)), glm::sin(glm::radians(ang)) , 0.f) * radius;
        ang += d_ang;
    }

    for(int i = 0;i < numberOfGizmoVertices; i++)
    {
        gizmoVertices[i].color = glm::vec3(0.f,1.f,0.f);
        gizmoVertices[i].normal = glm::vec3(0.f,0.f,1.f);
        gizmoVertices[i].texcoord = glm::vec2(0.f,0.f);
    }

    gizmo = new Gizmo(gizmoVertices, numberOfGizmoVertices);

    Collider::CreateGizmo();
}

void SphereCollider::SetBoundary()
{
    boundary.min = glm::vec2(-radius,-radius);
    boundary.max = glm::vec2(radius,radius);
    boundary.upperLeft = glm::vec2(-radius,radius);
}