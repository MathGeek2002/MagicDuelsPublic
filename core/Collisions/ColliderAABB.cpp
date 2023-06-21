#include "ColliderAABB.h"
#include "CollisionTests.h"
#include "CollisionSolver.h"
#include "SphereCollider.h"
#include <iostream>

bool ColliderAABB::TestCollision(Collider* collider_)
{
    switch ( collider_ -> type ) {

    case  CT_AABB_COLLIDER:
        return TestAABBAABB( this, static_cast<ColliderAABB*>(collider_));
    
    case CT_SPHERE_COLLIDER:
        return TestAABBSphere( this , static_cast<SphereCollider*>(collider_) );

    default:
        return true;
    }
}

CollisionProduct* ColliderAABB::Solve(Collider * collider_)
{
    switch ( collider_ -> type ) {

    case  CT_AABB_COLLIDER:
        return ContactAABBAABB( this, static_cast<ColliderAABB*>(collider_) );

    case CT_SPHERE_COLLIDER:
        return ContactAABBSphere( this , static_cast<SphereCollider*>(collider_) );
    
    default:
        return GJK( this, collider_ );
    }
}

void ColliderAABB::UpdateCoordinates()
{
    glm::vec3 pos = entity -> getComponent<TransformComponent>().GetPosition() + offset;

    glm::vec3 v = glm::vec3( sizeX, sizeY, sizeZ ) * 0.5f;

    min = pos - v;
    max = pos + v;
}

void ColliderAABB::CreateGizmo()
{
    gizmoVertices = new Vertex[4];

    gizmoVertices[0].position =  glm::vec3(sizeX,sizeY,0.f) * 0.5f + offset;
    gizmoVertices[0].color = glm::vec3(0.f,1.f,0.f);
    gizmoVertices[1].position = glm::vec3(sizeX,-sizeY,0.f) * 0.5f + offset; 
    gizmoVertices[1].color = glm::vec3(0.f,1.f,0.f);
    gizmoVertices[2].position = glm::vec3(-sizeX,-sizeY,0.f) * 0.5f + offset;
    gizmoVertices[2].color = glm::vec3(0.f,1.f,0.f);
    gizmoVertices[3].position = glm::vec3(-sizeX,sizeY,0.f) * 0.5f + offset;
    gizmoVertices[3].color = glm::vec3(0.f,1.f,0.f);

    for(int i = 0;i < 4; i++)
    {
        gizmoVertices[i].normal = glm::vec3(0.f,0.f,1.f);
        gizmoVertices[i].texcoord = glm::vec2(0.f,0.f);
    }

    gizmo = new Gizmo(gizmoVertices, 4);

    Collider::CreateGizmo();
}

void ColliderAABB::SetBoundary()
{
    boundary.min = glm::vec2(-sizeX,-sizeY) * 0.5f;
    boundary.max = glm::vec2(sizeX,sizeY) * 0.5f;
    boundary.upperLeft = glm::vec2(-sizeX,sizeY) * 0.5f;
}