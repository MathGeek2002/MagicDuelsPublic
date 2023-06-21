#pragma once

#include "../../core/ECS/ECS.hpp"
#include "../../core/Random/Random.hpp"
#include "../../core/Physics/Rigidbody.h"
#include "../../core/graphics/GraphicsComponent.h"
#include "../Player/PlayerHealth.hpp"
#include "../Player/PlayerMana.hpp"
//#include "../../core/Debug/Debug.hpp"

class PotionController : public Component
{
protected:
    float timer;
    float showDelay;
    bool isHiden;
    float amount;

    void onCollisionEnter(Entity * other)
    {
        if(other->tag == "Ground") {
            if(!isHiden)
            {
                Rigidbody& rb = entity->getComponent<Rigidbody>();
                rb.useGravity = false;
                rb.velocity = glm::vec3(0,0,0);
            }
            else
            {
                Reposition();
            }
        }
        else if( other->tag == "Player") {
            Reposition();
            HideAndWait();
            Use(other);
        }
    }

    void Reposition()
    {
        glm::vec3 randPos = glm::vec3( Random::Range(-0.9f,0.9f), Random::Range(-0.9f,0.9f), 0.f );
        entity->getComponent<TransformComponent>().position = randPos;
    }

    void HideAndWait()
    {
        isHiden = true;
        entity->getComponent<GraphicsComponent>().enable = false;
        timer = 0.f;
    }

    void Show()
    {
        isHiden = false;
        entity->getComponent<Rigidbody>().useGravity = true;
        entity->getComponent<GraphicsComponent>().enable = true;
    }

public:
    PotionController(float respawDelay,float amount_) : Component(CT_CUSTOM) , showDelay(respawDelay), amount(amount_)
    {
        isHiden = false;
    }

    void init() override 
    {
        entity->onCollisionEnter.connect_member(this, &PotionController::onCollisionEnter);
    }

    void run(float dt) override
    {
        if(isHiden)
        {
            timer += dt;
            if( timer > showDelay )
                Show();
        }
    } 

    virtual ~PotionController() { }

    virtual void Use(Entity* other) {}
};

class HealthPotion : public PotionController
{
public:
    HealthPotion(float respawDelay,float amount_) : PotionController(respawDelay, amount_) {} 

    void Use(Entity* other) override
    {
        other->getComponent<PlayerHealth>().Heal(amount);
    }

    ~HealthPotion() {}
};

class ManaPotion : public PotionController
{
public:
    ManaPotion(float respawDelay,float amount_) : PotionController(respawDelay, amount_) {} 

    void Use(Entity* other) override
    {
        other->getComponent<PlayerMana>().AddMana(amount);
    }

    ~ManaPotion() {}
};