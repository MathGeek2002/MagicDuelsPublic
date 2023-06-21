#include "CapsuleCollider.h"
#include "CollisionSolver.h"
#include "CollisionTests.h"
#include "CollisionTools.h"
#include "ColliderAABB.h"
#include "SphereCollider.h"

CapsuleCollider::CapsuleCollider( glm::vec3 origin , float _radius , float _height , Axis axis ) : 
Collider(CT_CAPSULE_COLLIDER) , height(_height)
{
    switch ( axis ){
        case X:
            my_dir = glm::vec3( 1, 0, 0 );
            break;
        case Y:
            my_dir = glm::vec3( 0, 1, 0 );
            break;
        default:
            my_dir = glm::vec3( 0, 0, 1 );
            break;
    }

    p0 = origin - (_height * 0.5f - _radius) * my_dir;
    p1 = origin + (_height * 0.5f - _radius) * my_dir;
    radius = _radius;
    _axis = axis;
    CreateGizmo();
    SetBoundary();
}

glm::vec3 CapsuleCollider::FindFurtherstPoint(glm::vec3 direction) const
{
    const glm::vec3 normal_dir = glm::normalize(direction);
    const float Dy = glm::dot( normal_dir, my_dir );
    return (Dy < 0.f ? p0 : p1) + radius * normal_dir + GetCenter(); 
}

bool CapsuleCollider::TestCollision(Collider* collider_)
{
    switch ( collider_ -> type ) {
    
    case CT_SPHERE_COLLIDER:
        return TestCapsuleSphere(this, static_cast<SphereCollider*>(collider_));

    case CT_CAPSULE_COLLIDER:
        return TestCapsuleCapsule(this, static_cast<CapsuleCollider*>(collider_));

    default:
        return true;
    }
}

CollisionProduct* CapsuleCollider::Solve(Collider * collider_)
{
    switch ( collider_ -> type ) {

    case CT_SPHERE_COLLIDER:
        return ContactCapsuleSphere( this, static_cast<SphereCollider*>(collider_) );
    
    case CT_CAPSULE_COLLIDER:
        return ContactCapsuleCapsule( this, static_cast<CapsuleCollider*>(collider_) );
    
    default:
        return GJK( this, collider_ );
    }
}

void CapsuleCollider::CreateGizmo()
{
    gizmoVertices = new Vertex[numberOfGizmoVertices];

    float d_ang = 360.f / numberOfGizmoVertices;
    float ang = 0.f;

    for(int i = 0;i < numberOfGizmoVertices; i++)
    {
        gizmoVertices[i].position = glm::vec3( glm::cos(glm::radians(ang)), glm::sin(glm::radians(ang)) , 0.f) * radius;
        ang += d_ang;

        if(i < numberOfGizmoVertices / 2)
            gizmoVertices[i].position += p1;
        else
            gizmoVertices[i].position += p0;
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

void CapsuleCollider::SetBoundary()
{
    boundary.min = glm::vec2(-radius, -height / 2);
    boundary.max = glm::vec2( radius, height / 2);
    boundary.upperLeft = glm::vec2(-radius,height / 2);
}

CapsuleCollider::~CapsuleCollider() { }