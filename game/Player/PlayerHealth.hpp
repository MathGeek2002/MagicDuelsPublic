#pragma once

#include "../../core/ECS/ECS.hpp"
#include "../../core/TransformComponent/TransformComponent.h"
#include "../../core/graphics/TextureSprite.h"
#include "Bar.hpp"
#include "../../core/StateMachine/PlayerAnimator.hpp"
#include "PlayerMove.hpp"
#include "../../core/Collisions/CapsuleCollider.h"
#include "../../core/ParticleSystem/ParticlesManager.hpp"
#include "../../core/Signals/Signal.hpp"
#include "../../core/Physics/Rigidbody.h"

#define my_type float

class PlayerHealth : public Component
{
private:
    my_type maxHp;
    my_type currentHp;
    bool isDead = false;
    Bar * healthBar;
    int my_id;

public:
    Signal <int> onDeath;

    PlayerHealth(my_type _maxHp,int id) : Component(CT_CUSTOM), maxHp(_maxHp), my_id(id)
    {
        currentHp = _maxHp;
        healthBar = new Bar( glm::vec3( 1.f, .1f, .1f ), glm::vec3{0, -.13f, 0} );
    }

    ~PlayerHealth()
    {
        delete healthBar;
        onDeath.disconnect_all();
    }

    void init() override
    {
        this->entity->onTriggerEnter.connect_member(this,&PlayerHealth::OnTrigger);
    }

    inline my_type GetHp() const { return currentHp; }

    inline void GetDamage(my_type damage)
    {
        if( isDead )
            return;

        currentHp -= damage;

        healthBar->onValueChange.emit( (float)currentHp / maxHp );

        if (currentHp <= 0.f && !isDead)
            Death();
    }

    void run(float dt) override
    {
        currentHp = std::max( (my_type)0, currentHp );
        healthBar->UpdateBarPosition( entity->getComponent<TransformComponent>().GetPosition() );
    }

    void Death()
    {
        isDead = true;

        entity->getComponent<PlayerAnimator>().EnterDeathState();
        entity->getComponent<PlayerMove>().enable = false;
        entity->getComponent<CapsuleCollider>().enable = false;
        entity->getComponent<ParticleManager>().enable = false;
        entity->getComponent<Rigidbody>().velocity = glm::vec3(0,0,0);

        onDeath.emit(my_id);
    }

    void OnTrigger(Entity * collider)
    {
        if(collider->tag == "orange" || collider->tag == "blue" || collider->tag == "shield")
            GetDamage( 1.f );
    }

    inline void ResetHealth()
    {
        currentHp = maxHp;
        healthBar->onValueChange.emit( 1.f );
        isDead = false;
    }

    inline void Heal(my_type amount)
    {
        currentHp = std::min(currentHp + amount, maxHp);
        healthBar->onValueChange.emit( (float)currentHp / maxHp );
    }
};
