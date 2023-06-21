#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "../ECS/ECS.hpp"
#include "../TransformComponent/TransformComponent.h"
#include "../../thirdparty/glm/glm.hpp"

class Rigidbody : public Component
{
private:
    const float g = 9.81f;
    float mass;
    float damping;
    float bounciness;

public:
    Rigidbody(float m = 1.f, float bounciness_ = 1.f);
    ~Rigidbody() { }

    void run(float dt) override;
    
    void AddForce(glm::vec3);

    bool useGravity;
    glm::vec3 velocity;

    inline float GetBounciness() const {  return bounciness;  }
    inline void SetBounciness(float newBounciness) {  bounciness = newBounciness; }

    inline float GetMass() const {  return mass;  }
    inline void SetMass(float newMass) { mass = std::max(newMass, 1e-6f); }

};

#endif