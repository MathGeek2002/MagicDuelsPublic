#ifndef POLYGON_SIMPLIFY_H
#define POLYGON_SIMPLIFY_H

#include <vector>
#include <stack>
#include "../../thirdparty/glm/glm.hpp"
#include "MarchingSq.h"

typedef std::vector<std::vector<glm::vec2>> contoursData;

class PolygonSimplify
{
private:
    struct PSData
    {
        int startIndex;
        int endIndex;
    };

public:
    static std::vector<glm::vec2> simplify(std::vector<glm::vec2> points, float eps = 0.7f);
    static contoursData getSimplifiedContours(int *pixelData, int dimX, int dimY, float eps = 0.8f);

private:
    static float perpDistance(glm::vec2 point, glm::vec2 line1, glm::vec2 line2);
};

#endif
