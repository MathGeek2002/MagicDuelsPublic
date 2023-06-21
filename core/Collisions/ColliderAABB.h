#ifndef AABB_COLLIDER_H
#define AABB_COLLIDER_H

#include "Collider.h"
#include "../../thirdparty/glm/glm.hpp"

class ColliderAABB : public Collider
{

private:
    float sizeX;
    float sizeY;
    float sizeZ;
    glm::vec3 offset;

public:
    glm::vec3 min;
    glm::vec3 max;

    ColliderAABB(float _sizeX = 1.f, float _sizeY = 1.f, float _sizeZ = 0.f, glm::vec3 _offset = glm::vec3(0))  : 
    Collider(CT_AABB_COLLIDER),
    sizeX(_sizeX),
    sizeY(_sizeY),
    sizeZ(_sizeZ),
    offset( _offset) {
        CreateGizmo();
        SetBoundary();
    }

    void run(float dt) override
    {
        UpdateCoordinates();
    }


    glm::vec3 FindFurtherstPoint(glm::vec3 direction) const override
    {
        glm::vec3 out = offset + entity -> getComponent<TransformComponent>().position + 
        glm::vec3(
             glm::sign(direction.x) * sizeX * 0.5f,
             glm::sign(direction.y) * sizeY * 0.5f,
             glm::sign(direction.z) * sizeZ * 0.5f
        );

        return out;
    }

    bool TestCollision(Collider* collider_) override;
    CollisionProduct* Solve(Collider * collider_) override;

    void UpdateCoordinates();
    void CreateGizmo() override;
    void SetBoundary() override;
};

#endif