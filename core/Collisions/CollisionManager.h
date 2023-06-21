#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <set>
#include "Collider.h"
#include "CollisionProduct.hpp"
#include "CollisionTests.h"
#include "CollisionSolver.h"
#include <queue>
#include "../TransformComponent/TransformComponent.h"

struct CollisionPair
{
    Collider *first;
    Collider *second;
    CollisionProduct *collisionInfo;
    bool isSecondStatic;
};

class CollisionManager
{
private:
    static std::set<Collider *> colliders;
    static std::set<Collider *> staticColliders;
    static std::queue<CollisionPair *> collisionPairs;
    static std::set<int> pairsUUID;

public:
    static bool initialized;
    static int counter;

    static void RegisterCollider(Collider *);
    static void UnRegisterCollider(Collider *);
    static void Init(float cornerX, float cornerY);
    static void Update();
    static void RegisterCollision(Collider *, Collider *, CollisionProduct *, bool);
    static void Close();
};

#endif