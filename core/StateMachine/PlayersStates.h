#ifndef EXAMPLE_STATES_H
#define EXAMPLE_STATES_H

#include "State.h"

namespace PlayerStateMachine
{
    class Idle : public State<class PlayerAnimatorStateMachine>
    {
    private:
        float timer;

    public:
        void Enter(PlayerAnimatorStateMachine* myStateMachine,Entity * owner) override;
        void Update(PlayerAnimatorStateMachine* myStateMachine,float dt,Entity * owner) override;
        void Exit(PlayerAnimatorStateMachine* myStateMachine,Entity * owner) override;
    };

    class Attack : public State<class PlayerAnimatorStateMachine>
    {
    public:
        void Enter(PlayerAnimatorStateMachine* myStateMachine,Entity * owner) override;
        void Update(PlayerAnimatorStateMachine* myStateMachine,float dt,Entity * owner) override;
        void Exit(PlayerAnimatorStateMachine* myStateMachine,Entity * owner) override;
    };

    class Run : public State<class PlayerAnimatorStateMachine>
    {
    public:
        void Enter(PlayerAnimatorStateMachine* myStateMachine,Entity * owner) override;
        void Update(PlayerAnimatorStateMachine* myStateMachine,float dt,Entity * owner) override;
        void Exit(PlayerAnimatorStateMachine* myStateMachine,Entity * owner) override;
    };

    class Jump : public State<class PlayerAnimatorStateMachine>
    {
    private:
        float previousVelocity_y;
    public:
        void Enter(PlayerAnimatorStateMachine* myStateMachine,Entity * owner) override;
        void Update(PlayerAnimatorStateMachine* myStateMachine,float dt,Entity * owner) override;
        void Exit(PlayerAnimatorStateMachine* myStateMachine,Entity * owner) override;
    };

    class Death : public State<class PlayerAnimatorStateMachine>
    {
    private:
        float timer;
        const float deathTime = 2.f;
    public:
        void Enter(PlayerAnimatorStateMachine* myStateMachine,Entity * owner) override;
        void Update(PlayerAnimatorStateMachine* myStateMachine,float dt,Entity * owner) override;
        void Exit(PlayerAnimatorStateMachine* myStateMachine,Entity * owner) override;
    };
}

#endif