#include "Collider.h"
#include "CollisionManager.h"
#include "../graphics/RenderEngine.h"

int Collider::counter = 0;

Collider::Collider(ComponentType type_) : Component(type_)
{
    type = type_;
    isStatic = false;
    isTrigger = false;
    id = ++counter;
    CollisionManager::RegisterCollider(this);
}

Collider::~Collider()
{
    CollisionManager::UnRegisterCollider(this);
    
    RenderEngine::UnRegisterGizmo(this);

    delete  gizmo;
}

void Collider::CreateGizmo()
{
    RenderEngine::RegisterGizmo(this);
}

void Collider::SetStaticMode(bool isStatic_)
{
    CollisionManager::UnRegisterCollider(this);
    isStatic = isStatic_;
    CollisionManager::RegisterCollider(this);
}

void Collider::SetTriggerMode(bool isTrigger_)
{
    isTrigger = isTrigger_;
}

ColliderBoundary* Collider::GetBoundary() const
{
    ColliderBoundary* myBoundary = new ColliderBoundary();
    glm::vec2 entityPos = (glm::vec2)entity->getComponent<TransformComponent>().position;
    myBoundary->max = boundary.max + entityPos;
    myBoundary->min = boundary.min + entityPos;
    myBoundary->upperLeft = boundary.upperLeft + entityPos;
    return myBoundary;
}

