#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "State.h"
#include "../ECS/ECS.hpp"
#include <iostream>

template <class T>
class StateMachine
{
private:
    T * owner;
    State<T>* currentState;

protected:
    Entity * myEntity;

    void Init(T* owner_, State<T>* initialState,Entity* entiry_);

public:
    void Update(float dt);
    void ChangeState(State<T>* newState);
};

template <class T>
inline void StateMachine<T>::Init(T* owner_, State<T>* initialState,Entity* entity_)
{
    myEntity = entity_;
    owner = owner_;
    currentState = initialState;
    currentState->Enter(owner,myEntity);
}

template <class T>
inline void StateMachine<T>::Update(float dt)
{
    currentState->Update(owner,dt,myEntity);
}

template <class T>
inline void StateMachine<T>::ChangeState(State<T>* newState)
{
    currentState->Exit(owner,myEntity);
    currentState = newState;
    currentState->Enter(owner,myEntity);
}

#endif