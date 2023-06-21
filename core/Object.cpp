#include "Object.h"
#include <iostream>

void Object::Update(float dt_)
{
    UserUpdate(dt_);
}

void Object::UserUpdate(float dt_)
{
}

Object::~Object()
{
}

void Object::destroy()
{
}
