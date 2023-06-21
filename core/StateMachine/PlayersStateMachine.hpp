#ifndef ANIMATOR_STATE_MACHINE_HPP
#define ANIMATOR_STATE_MACHINE_HPP

#include "StateMachine.h"
#include "PlayersStates.h"
#include "../Application.h"

namespace PlayerStateMachine
{
    class PlayerAnimatorStateMachine : public StateMachine<PlayerAnimatorStateMachine>
    {
    private:
        State<PlayerAnimatorStateMachine>* idle;
        State<PlayerAnimatorStateMachine>* attack;
        State<PlayerAnimatorStateMachine>* run;
        State<PlayerAnimatorStateMachine>* jump;

    public:

        PlayerAnimatorStateMachine(Entity* owner_)
        {
            idle = new Idle();
            attack = new PlayerStateMachine::Attack();
            run = new PlayerStateMachine::Run();
            jump = new PlayerStateMachine::Jump();
            death = new PlayerStateMachine::Death();

            Init(this, idle ,owner_);
            Application::onRestart.connect_member(this, &PlayerAnimatorStateMachine::Restart);
        }

        ~PlayerAnimatorStateMachine()
        {
            delete idle;
            delete attack;
            delete run;
            delete jump;
        }

        void Update(float dt) {
            StateMachine<PlayerAnimatorStateMachine>::Update(dt);
        }

        void Restart();

        State<PlayerAnimatorStateMachine>* death;

        friend class Idle;
        friend class Attack;
        friend class Run;
        friend class Jump;
        friend class Death;
    };
}
#endif