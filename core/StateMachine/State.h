#ifndef STATE_H
#define STATE_H

#include "../ECS/ECS.hpp"

template <class T>
class State
{
public:
    virtual ~State() {}
    virtual void Enter(T* myStateMachine,Entity * owner) = 0;
    virtual void Update(T* myStateMachine,float dt,Entity * owner) = 0;
    virtual void Exit(T* myStateMachine,Entity * owner) = 0;
};

#endif