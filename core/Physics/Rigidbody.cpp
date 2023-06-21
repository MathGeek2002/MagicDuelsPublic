#include "Rigidbody.h"


Rigidbody::Rigidbody(float mass_,float bounciness_) : Component(CT_RIGIDBODY) , mass(mass_)
{
    velocity = glm::vec3(0);
    useGravity = true;
    damping = 0.6f;
    mass = std::max(mass, 1e-6f);
    bounciness = std::max(bounciness_, 0.f);
}

void Rigidbody::run(float dt)
{
    if( useGravity )
        velocity -= glm::vec3(0.f, dt * g, 0.f);
    
    velocity *= powf(damping,dt);
    this->entity->getComponent<TransformComponent>().position += velocity * dt;
    //this->entity->getComponent<TransformComponent>().UpdateModel();
}

void Rigidbody::AddForce(glm::vec3 force)
{
    velocity += force / mass;
}