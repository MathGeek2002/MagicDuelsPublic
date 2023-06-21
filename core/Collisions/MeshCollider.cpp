#include "MeshCollider.h"
#include "GJK.h"

bool MeshCollider::TestCollision(Collider* collider_)
{
    return true;
}

CollisionProduct * MeshCollider::Solve(Collider * collider_) 
{
    CollisionProduct* collProduct = GJK( this, collider_ );
    return collProduct;
}

void MeshCollider::CreateGizmo()
{
    gizmoVertices = new Vertex[nVeritces];

    for(size_t i = 0;i < nVeritces; i++)
    {
        gizmoVertices[i].position = vertices[i].position;
        gizmoVertices[i].color = glm::vec3(0.f,1.f,0.f);
        gizmoVertices[i].normal = glm::vec3(0.f,0.f,1.f);
        gizmoVertices[i].texcoord = glm::vec2(0.f,0.f);
    }

    gizmo = new Gizmo(gizmoVertices, nVeritces);

    Collider::CreateGizmo();
}

void MeshCollider::SetBoundary()
{
    float minX = FLT_MAX;
    float minY = FLT_MAX;
    float maxX = -FLT_MAX;
    float maxY = -FLT_MAX;

    for(size_t i = 0;i < nVeritces; i++)
    {
        minX = std::min(minX, vertices[i].position.x);
        minY = std::min(minY, vertices[i].position.y);
        maxX = std::max(maxX, vertices[i].position.x);
        maxY = std::max(maxY, vertices[i].position.y);
    }
    boundary.max = glm::vec2(maxX,maxY);
    boundary.min = glm::vec2(minX,minY);
    boundary.upperLeft = glm::vec2(minX,maxY);
}