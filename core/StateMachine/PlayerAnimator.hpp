#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include "../ECS/ECS.hpp"
#include "PlayersStateMachine.hpp"

class PlayerAnimator : public Component
{
public:
    PlayerStateMachine::PlayerAnimatorStateMachine * stateMachine;

    PlayerAnimator() : Component(CT_STATE_MANAGER) { }

    void init() override
    {
        stateMachine = new PlayerStateMachine::PlayerAnimatorStateMachine(this->entity);
    }

    void run(float dt) override {
        stateMachine->Update(dt);
    }

    inline void EnterDeathState() const
    {
        stateMachine->ChangeState(stateMachine->death);
    }

    ~PlayerAnimator() {
        delete stateMachine;
    }
};

#endif