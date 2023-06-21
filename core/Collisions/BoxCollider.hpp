#ifndef BOX_COLLIDER_HPP
#define BOX_COLLIDER_HPP

#include "Collider.h"
#include "../../thirdparty/glm/glm.hpp"

class BoxCollider : public Collider
{

private:
    glm::vec3 center;
    float sizeX;
    float sizeY;
    float sizeZ;

    float sign(float x) const { return x < 0.f ? -1.f : 1.f; }

public:
    BoxCollider(glm::vec3  _center,float _sizeX,float _sizeY,float _sizeZ) : 
    center( _center), 
    sizeX(_sizeX),
    sizeY(_sizeY),
    sizeZ(_sizeZ), Collider(CT_BOX_COLLIDER)  {}

    glm::vec3 FindFurtherstPoint(glm::vec3 direction) const override
    {
        return center + glm::vec3(
             sign(direction.x) * sizeX / 2.f,
             sign(direction.y) * sizeY / 2.f,
             sign(direction.z) * sizeZ / 2.f
        );
    }
};

#endif