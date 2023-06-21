#ifndef COLLIDER_H
#define COLLIDER_H

#include "../ECS/ECS.hpp"
#include "../../thirdparty/glm/glm.hpp"
#include "../TransformComponent/TransformComponent.h"
#include "CollisionProduct.hpp"
#include "../graphics/Vertex.hpp"
#include "../graphics/Gizmo.h"
#include "ColliderBoundary.hpp"


class Collider : public Component
{
public:
    Collider(ComponentType type_);

    ~Collider();

    virtual glm::vec3 FindFurtherstPoint(glm::vec3 direction) const = 0;
    virtual bool TestCollision(Collider* collider_) = 0;
    virtual CollisionProduct* Solve(Collider * collider_) = 0;
    virtual void CreateGizmo();
    void SetStaticMode(bool isStatic_);
    void SetTriggerMode(bool isTrigger_);
    void virtual SetBoundary() = 0;
    ColliderBoundary* GetBoundary() const;

    ComponentType type;
    Vertex * gizmoVertices;
    Gizmo * gizmo;
    bool isTrigger;
    bool isStatic;
    int id;
    ColliderBoundary boundary;
    static int counter;
};

#endif