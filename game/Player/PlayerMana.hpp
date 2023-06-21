#pragma once

#include "../../core/ECS/ECS.hpp"
#include "Bar.hpp"
 
#define my_type float

class PlayerMana : public Component
{
private:
    my_type maxMana;
    my_type currentMana;
    my_type manaRegeneration;

    Bar * manaBar;

public:
    PlayerMana(my_type _maxMana, my_type _manaRegeneration) : Component(CT_CUSTOM) , maxMana(_maxMana), manaRegeneration(_manaRegeneration)
    {
        currentMana = maxMana;
        manaBar = new Bar( glm::vec3( .1f, .1f, 1.f ), glm::vec3{0, -.14f, 0} );
    }

    ~PlayerMana()
    {
        delete manaBar;
    }

    inline my_type GetMana() const {  return currentMana;  }

    void run(float dt) override
    {
        currentMana += (my_type)(manaRegeneration * dt);
        currentMana = std::min(currentMana, maxMana);
        
        manaBar->onValueChange.emit((float)currentMana / maxMana);

        manaBar->UpdateBarPosition( entity->getComponent<TransformComponent>().GetPosition() );
    }

    inline void UseMana(my_type amount)
    {
        currentMana -= amount;
        currentMana = glm::max(currentMana,0.f);
        manaBar->onValueChange.emit((float)currentMana / maxMana);
    }

    inline void ResetMana()
    {
        currentMana = maxMana;
        manaBar->onValueChange.emit((float)currentMana / maxMana);
    }

    inline void AddMana(my_type amount)
    {
        currentMana = std::min(currentMana + amount, maxMana);
    }
};
