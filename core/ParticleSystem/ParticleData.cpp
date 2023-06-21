#include "ParticleData.h"
#include "ParticleSystem.h"
#include "../graphics/RenderEngine.h"

ParticleData::ParticleData(int id_, ParticleSystem * particleSystem) : Component(CT_CUSTOM), my_system(particleSystem) 
{
    id = id_;
    isAlive = false;
    particleVel = glm::vec3(0);
    isInLocalSpace = false;
}

void ParticleData::init()
{
    SphereCollider & sphereCollider = entity->addComponent<SphereCollider>(0.01f);
    sphereCollider.SetTriggerMode(true);
    CollisionManager::UnRegisterCollider(&sphereCollider);
    entity->onTriggerEnter.connect_member(this, &ParticleData::Hide);
}

void ParticleData::SetData(glm::vec3 pos, glm::vec3 vel,int _destroyTerrainChance)
{
    entity->getComponent<TransformComponent>().position = pos;
    particleVel = vel;
    localStartPos = pos - my_system->GetPos();
    destroyTerrainChance = _destroyTerrainChance;
}

void ParticleData::SetLifeTime(float lifeTime)
{
    particleLifeTimer = lifeTime;
    isAlive = true;
    CollisionManager::RegisterCollider(&entity->getComponent<SphereCollider>());
    RenderEngine::RegisterGizmo(&entity->getComponent<SphereCollider>());
}

void ParticleData::run(float dt)
{
    if(!isAlive)
        return;

    TransformComponent & transform = entity->getComponent<TransformComponent>();

    if( !isInLocalSpace )
        transform.position += particleVel * dt;
    else
        transform.position = localStartPos + my_system->GetPos();

    if(transform.position.x >= 1.5f)
        Disable();

    transform.UpdateModel();
    my_system->UpdatePos( id, transform.position );
    particleLifeTimer -= dt;

    if( particleLifeTimer <= 0.f )
        Disable();
}

void ParticleData::Hide(Entity* collider)
{
    if(!isAlive)
        return;

    if(entity->tag == "shield" && (collider->tag == "orange" || collider->tag == "blue"))
        return;
    
    if(collider->tag != entity->tag)
        Disable();
}

void ParticleData::Disable()
{
    isAlive = false;
    CollisionManager::UnRegisterCollider(&entity->getComponent<SphereCollider>());
    RenderEngine::UnRegisterGizmo(&entity->getComponent<SphereCollider>());

    my_system->Deactivate(id);
}

void ParticleData::SetSpace(bool _isInLocalSpace)
{
    isInLocalSpace = _isInLocalSpace;
}