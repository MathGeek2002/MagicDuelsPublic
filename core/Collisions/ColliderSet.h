#ifndef COLLIDER_SET_H
#define COLLIDER_SET_H

#include "../ECS/ECS.hpp"
#include "Collider.h"
#include <vector>

class ColliderSet : public Component
{
public:
    std::vector<Collider *> colliders;

    ColliderSet();
    ~ColliderSet();

    void addCollider(Collider *);
    void clearColliders();
    void run(float dt) override;
};

#endif