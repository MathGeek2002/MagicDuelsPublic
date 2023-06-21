#include "ColliderSet.h"

void ColliderSet::addCollider(Collider *collider)
{
    collider->entity = this->entity;
    colliders.push_back(collider);
}

void ColliderSet::run(float dt)
{
    for (Collider *coll : colliders)
    {
        coll->run(dt);
    }
}

void ColliderSet::clearColliders()
{
    for (Collider *coll : colliders)
    {
        delete coll;
    }
    colliders.clear();
}

ColliderSet::ColliderSet() : Component(CT_COLLIDER_SET){};

ColliderSet::~ColliderSet()
{
    for (Collider *coll : colliders)
    {
        delete coll;
    }
};