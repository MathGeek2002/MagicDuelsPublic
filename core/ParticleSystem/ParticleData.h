#pragma once

#include "../ECS/ECS.hpp"
#include "../TransformComponent/TransformComponent.h"
#include "../Collisions/SphereCollider.h"
#include <string>
#include "../Collisions/CollisionManager.h"

class ParticleSystem;

class ParticleData : public Component
{
public:
    int id;
    float particleLifeTimer;
    glm::vec3 particleVel;
    ParticleSystem * my_system;
    bool isAlive;
    bool isInLocalSpace;
    glm::vec3 localStartPos;
    int destroyTerrainChance = 0;

    ParticleData(int id_, ParticleSystem * particleSystem);

    void init() override;

    void SetData(glm::vec3 pos, glm::vec3 vel,int _destroyTerrainChance);

    void SetLifeTime(float lifeTime);

    void run(float dt) override;

    void Hide(Entity* collider);

    void Disable();

    void SetSpace(bool _isInLocalSpace);
};