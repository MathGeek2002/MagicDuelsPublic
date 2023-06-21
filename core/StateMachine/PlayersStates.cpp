#include "PlayersStates.h"
#include "PlayersStateMachine.hpp"
#include "../graphics/GraphicsComponent.h"
#include "../Input/Input.h"
#include <SDL2/SDL.h>
#include "../../game/Player/PlayerMove.hpp"
#include "../Physics/Rigidbody.h"
#include "../../game/Player/PlayerRestartMananger.hpp"

void PlayerStateMachine::Idle::Enter(PlayerAnimatorStateMachine *myStateMachine, Entity *owner)
{
    owner->getComponent<GraphicsComponent>().ChangeAnimation("idle");
}

void PlayerStateMachine::Idle::Update(PlayerAnimatorStateMachine *myStateMachine, float dt, Entity *owner)
{

    if (Input::getKey(SDLK_e))
    {
        myStateMachine->ChangeState(myStateMachine->attack);
        return;
    }

    if (owner->hasComponent<PlayerMove>() && owner->getComponent<PlayerMove>().IsJumping())
    {
        myStateMachine->ChangeState(myStateMachine->jump);
        return;
    }

    if (owner->hasComponent<Rigidbody>() && abs(owner->getComponent<Rigidbody>().velocity.x) > 0.01f)
        myStateMachine->ChangeState(myStateMachine->run);
}

void PlayerStateMachine::Idle::Exit(PlayerAnimatorStateMachine *myStateMachine, Entity *owner) {}

void PlayerStateMachine::Attack::Enter(PlayerAnimatorStateMachine *myStateMachine, Entity *owner)
{
    owner->getComponent<GraphicsComponent>().ChangeAnimation("attack");
}

void PlayerStateMachine::Attack::Update(PlayerAnimatorStateMachine *myStateMachine, float dt, Entity *owner)
{
    if (owner->getComponent<GraphicsComponent>().GetAnimationProcentage() == 1.f)
        myStateMachine->ChangeState(myStateMachine->idle);
}

void PlayerStateMachine::Attack::Exit(PlayerAnimatorStateMachine *myStateMachine, Entity *owner) {}

void PlayerStateMachine::Run::Enter(PlayerAnimatorStateMachine *myStateMachine, Entity *owner)
{
    owner->getComponent<GraphicsComponent>().ChangeAnimation("run");
}

void PlayerStateMachine::Run::Update(PlayerAnimatorStateMachine *myStateMachine, float dt, Entity *owner)
{
    if (Input::getKey(SDLK_e))
    {
        myStateMachine->ChangeState(myStateMachine->attack);
        return;
    }

    if (owner->hasComponent<PlayerMove>() && owner->getComponent<PlayerMove>().IsJumping())
    {
        myStateMachine->ChangeState(myStateMachine->jump);
        return;
    }

    if (owner->hasComponent<Rigidbody>() && abs(owner->getComponent<Rigidbody>().velocity.x) <= 0.01f)
        myStateMachine->ChangeState(myStateMachine->idle);
}

void PlayerStateMachine::Run::Exit(PlayerAnimatorStateMachine *myStateMachine, Entity *owner) {}

void PlayerStateMachine::Jump::Enter(PlayerAnimatorStateMachine *myStateMachine, Entity *owner)
{
    owner->getComponent<GraphicsComponent>().ChangeAnimation("jump");
    previousVelocity_y = owner->getComponent<Rigidbody>().velocity.y;
}

void PlayerStateMachine::Jump::Update(PlayerAnimatorStateMachine *myStateMachine, float dt, Entity *owner)
{
    if (owner->hasComponent<PlayerMove>() && !owner->getComponent<PlayerMove>().IsJumping())
        myStateMachine->ChangeState(myStateMachine->idle);

    float currentVelocity_y = owner->getComponent<Rigidbody>().velocity.y;

    if (currentVelocity_y * previousVelocity_y < 0.f)
        owner->getComponent<GraphicsComponent>().ChangeAnimation("fall");

    previousVelocity_y = currentVelocity_y;
}

void PlayerStateMachine::Jump::Exit(PlayerAnimatorStateMachine *myStateMachine, Entity *owner) {}

void PlayerStateMachine::Death::Enter(PlayerAnimatorStateMachine *myStateMachine, Entity *owner)
{
    owner->getComponent<GraphicsComponent>().ChangeAnimation("death");
    timer = 0.f;
}

void PlayerStateMachine::Death::Update(PlayerAnimatorStateMachine *myStateMachine, float dt, Entity *owner) 
{
    timer += dt;

    if(timer > deathTime)
        owner->getComponent<PlayerRestartManager>().RestartGame();
}

void PlayerStateMachine::Death::Exit(PlayerAnimatorStateMachine *myStateMachine, Entity *owner) 
{

}

void PlayerStateMachine::PlayerAnimatorStateMachine::Restart()
{
    this->ChangeState(idle);
}