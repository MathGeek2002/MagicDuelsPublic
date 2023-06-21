#ifndef CAPSULE_COLLIDER_H
#define CAPSULE_COLLIDER_H

#include "Collider.h"
#include "../../thirdparty/glm/glm.hpp"

enum Axis {
    X, Y, Z
};

class CapsuleCollider : public Collider
{
private:
    glm::vec3 p0;
    glm::vec3 p1;
    float radius;
    glm::vec3 my_dir;
    Axis _axis;
    const int numberOfGizmoVertices = 20;
    float height;

public:
    CapsuleCollider( glm::vec3 origin = glm::vec3(0), float _radius = 0.08f, float _height = 0.35f, Axis axis = Y);

    ~CapsuleCollider();

    glm::vec3 FindFurtherstPoint(glm::vec3 direction) const override;

    bool TestCollision(Collider* collider_) override;
    CollisionProduct* Solve(Collider * collider_) override;

    inline float GetRadius() const {
        return radius;
    }

    inline glm::vec3 GetCenter() const {
        return entity->getComponent<TransformComponent>().position;
    }

    inline glm::vec3 GetTip() const{
        return GetCenter() + p1;
    }

    inline glm::vec3 GetBase() const{
        return GetCenter() + p0;
    }

    void UpdateCoordinates();
    void CreateGizmo() override;
    void SetBoundary() override;
};

#endif