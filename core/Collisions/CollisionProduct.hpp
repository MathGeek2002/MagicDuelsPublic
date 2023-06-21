#ifndef COLLISION_PRODUCT_HPP
#define COLLISION_PRODUCT_HPP

#include "../../thirdparty/glm/glm.hpp"
#include "../ECS/ECS.hpp"

class CollisionProduct
{
public:
    CollisionProduct(bool _hasCollision) : hasCollision(_hasCollision) {}

    glm::vec3 normal;
    float Depth;
    bool hasCollision;

    CollisionProduct* ReverseSing()
    {
        this->Depth *= -1.f;
        return this;
    }
};

struct CollisionInfo
{
  Entity * collider;
  glm::vec3 normal;

  CollisionInfo(Entity* coll,glm::vec3 _normal)
  {
    collider = coll;
    normal = _normal;
  }
};

#endif