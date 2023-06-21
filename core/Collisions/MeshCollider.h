#ifndef MESH_COLLIDER_H
#define MESH_COLLIDER_H

#include "Collider.h"
#include <GL/glew.h>
#include "../graphics/Vertex.hpp"
#include "../graphics/Mesh.h"
#include "../../thirdparty/glm/glm.hpp"
#include <iostream>
#include "../Debug/Debug.hpp"

class MeshCollider : public Collider
{
private:
    Vertex * vertices;
    unsigned int nVeritces;

public:
    MeshCollider(Vertex * vertices_, unsigned int nVertices_) : Collider(CT_MESH_COLLIDER)
    {
        vertices = vertices_;
        nVeritces = nVertices_;
        CreateGizmo();
        SetBoundary();
    }
    
    MeshCollider(Mesh* mesh) : Collider(CT_MESH_COLLIDER)
    {
        vertices = mesh -> GetVertices();
        nVeritces = mesh -> GetNumberOfVertices();
        SetBoundary();
    }

    ~MeshCollider()
    {
        delete [] vertices;
    }

    glm::vec3 FindFurtherstPoint(glm::vec3 direction) const override
    {
        glm::vec3 maxPoint;
        float maxDist = -FLT_MAX;

        for(size_t i = 0;i < nVeritces; i++)
        {
            glm::vec3 vertexPos = vertices[i].position;
            
            vertexPos += entity -> getComponent<TransformComponent>().position;
            float distance = glm::dot(vertexPos, direction);
            if(distance > maxDist)
            {
                maxDist = distance;
                maxPoint = vertexPos;
            }
        }
        return maxPoint;
    }

    Vertex* GetVertices() {
        return vertices;
    }

    bool TestCollision(Collider* collider_) override;
    CollisionProduct * Solve(Collider * collider_) override; 

    void CreateGizmo() override;
    void SetBoundary() override;

};
#endif