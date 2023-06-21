#ifndef SPHERE_COLLIDER_H
#define SPHERE_COLLIDER_H

#include "Collider.h"
#include "../../thirdparty/glm/glm.hpp"
#include "../TransformComponent/TransformComponent.h"

class SphereCollider : public Collider
{

private:
    float radius;
    glm::vec3 origin;
    int numberOfGizmoVertices = 18;

public:
    SphereCollider( float _radious = 1.f,glm::vec3 _origin = glm::vec3(0) ) : 
    Collider(CT_SPHERE_COLLIDER),
    radius(_radious),
    origin(_origin)  
    {
        CreateGizmo();
        SetBoundary();
    }

    glm::vec3 FindFurtherstPoint(glm::vec3 direction) const override
    {
        return origin + radius * glm::normalize(direction) + entity -> getComponent<TransformComponent>().position;
    }

    inline float GetRadius() const {
        return radius;
    }

    inline glm::vec3 GetCenter() const {
        return origin + entity -> getComponent<TransformComponent>().GetPosition();
    }

    bool TestCollision(Collider* collider_) override;
    CollisionProduct* Solve(Collider * collider_) override;

    void CreateGizmo() override;
    void SetBoundary() override;
};

#endif