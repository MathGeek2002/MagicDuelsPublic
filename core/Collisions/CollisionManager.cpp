#include "CollisionManager.h"
#include "GridDataStructure.hpp"
#include "../Physics/Rigidbody.h"
#include "../Utils/PairUUID.hpp"
//#include "../Debug/Debug.hpp"

int CollisionManager::counter = 0;
std::set<Collider *> CollisionManager::colliders;
std::set<Collider *> CollisionManager::staticColliders;
std::queue<CollisionPair *> CollisionManager::collisionPairs;
std::set<int> CollisionManager::pairsUUID;
CollisionPair *collisionPair;
Grid *grid = nullptr;

bool CollisionManager::initialized = false;

void CollisionManager::RegisterCollider(Collider *collider)
{
    colliders.insert(collider);
}

void CollisionManager::UnRegisterCollider(Collider *collider)
{
    colliders.erase(collider);
}

void CollisionManager::Update()
{
    if(!initialized) return;
    
    pairsUUID.clear();

    grid->ClearGrid();

    for(auto it = colliders.begin(); it != colliders.end(); it++)
        if( (*it)->enable )
            grid->AddToGrid(*it);

    grid->ProcessCollisions();

    while (!collisionPairs.empty())
    {
        collisionPair = collisionPairs.front();
        collisionPairs.pop();

        Collider *coll = collisionPair->first;
        Collider *coll2 = collisionPair->second;

        glm::vec3 disp = collisionPair->collisionInfo->normal * collisionPair->collisionInfo->Depth;

        if (!collisionPair->isSecondStatic)
        {
            coll->entity->getComponent<TransformComponent>().position -= disp * 0.5f;
            coll->entity->getComponent<TransformComponent>().UpdateModel();

            coll2->entity->getComponent<TransformComponent>().position += disp * 0.5f;
            coll2->entity->getComponent<TransformComponent>().UpdateModel();
        }
        else
        {
            coll->entity->getComponent<TransformComponent>().position -= disp;
            coll->entity->getComponent<TransformComponent>().UpdateModel();

            if (coll->entity->hasComponent<Rigidbody>() && coll->entity->getComponent<Rigidbody>().useGravity)
            {
                glm::vec3 normal = collisionPair->collisionInfo->normal;
                glm::vec3 vel = coll->entity->getComponent<Rigidbody>().velocity;
                float bounciness = coll->entity->getComponent<Rigidbody>().GetBounciness();
                coll->entity->getComponent<Rigidbody>().velocity = glm::reflect(vel, normal) * bounciness;
            }
        }
        delete collisionPair->collisionInfo;
        delete collisionPair;
    }
}

void CollisionManager::Close()
{
    colliders.clear();
    staticColliders.clear();

    while (!collisionPairs.empty())
    {
        CollisionPair *collisionPair = collisionPairs.front();
        collisionPairs.pop();

        delete collisionPair;
    }
    delete grid;
}

void CollisionManager::RegisterCollision(Collider *coll, Collider *coll2, CollisionProduct *collisionProduct, bool isSecondStatic)
{
    int uuid = PairUUID(coll->id,coll2->id);

    if(pairsUUID.find(uuid) != pairsUUID.end())
        return;
    else
        pairsUUID.insert(uuid);

    if (coll->isTrigger || coll2->isTrigger)
    {
        coll->entity->onTriggerEnter.emit(coll2->entity);
        coll2->entity->onTriggerEnter.emit(coll->entity);
        delete collisionProduct;
        return;
    }
    else
    {
        coll->entity->onCollisionEnter.emit(coll2->entity);
        coll2->entity->onCollisionEnter.emit(coll->entity);
    }

    collisionPair = new CollisionPair();
    collisionPair->first = coll;
    collisionPair->second = coll2;
    collisionPair->collisionInfo = collisionProduct;
    collisionPair->isSecondStatic = isSecondStatic;
    collisionPairs.push(collisionPair);
}

void CollisionManager::Init(float worldSizeX, float worldSizeY)
{
    if (grid != nullptr)
    {
        delete grid;
    }

    grid = new Grid(colliders, worldSizeX, worldSizeY);
    initialized = true;
}