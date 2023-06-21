#pragma once

#include "../../core/ECS/ECS.hpp"
#include "../../core/TransformComponent/TransformComponent.h"
#include "../../core/Application.h"
#include "../../core/StateMachine/PlayerAnimator.hpp"
#include "PlayerMove.hpp"
#include "PlayerHealth.hpp"
#include "PlayerMana.hpp"
#include "../../core/Collisions/CapsuleCollider.h"
#include "../../core/Physics/Rigidbody.h"
#include "../../core/ParticleSystem/ParticlesManager.hpp"
//#include "../../core/Debug/Debug.hpp"

class PlayerRestartManager : public Component
{
private:
    glm::vec3 respawnPos;

    void RestartPlayer()
    {
        this->entity->getComponent<TransformComponent>().position = respawnPos;
        this->entity->getComponent<PlayerHealth>().ResetHealth();
        this->entity->getComponent<PlayerMana>().ResetMana();
        this->entity->getComponent<CapsuleCollider>().enable = true;
        this->entity->getComponent<PlayerMove>().enable = true;
        this->entity->getComponent<ParticleManager>().enable = true;
        this->entity->getComponent<Rigidbody>().useGravity = true;
    }

public:
    PlayerRestartManager(glm::vec3 startPos) : Component(CT_CUSTOM)
    {
        respawnPos = startPos;
    }

    ~PlayerRestartManager() { }

    void init() override
    {
        Application::onRestart.connect_member(this, &PlayerRestartManager::RestartPlayer);
    }

    void RestartGame()
    {
        if( Application::gameFinished == false )
            Application::onRestart.emit();
    }
};
