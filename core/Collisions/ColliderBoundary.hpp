#ifndef COLLIDER_BOUNDARY
#define COLLIDER_BOUNDARY

#include "../../thirdparty/glm/glm.hpp"

struct ColliderBoundary
{
    glm::vec2 min;
    glm::vec2 max;
    glm::vec2 upperLeft;

    ColliderBoundary() {};

    ColliderBoundary(float minX,float minY,float maxX,float maxY)
    {
        min = glm::vec2(minX,minY);
        max = glm::vec2(maxX,maxY);
        upperLeft = glm::vec2(minX,maxY);
    }

    inline float ReturnSize() { return std::max(std::abs(max.x - min.x),std::abs(max.y - min.y)); }
};

#endif