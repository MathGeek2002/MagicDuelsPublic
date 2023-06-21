#include "PolygonSimplify.h"

std::vector<glm::vec2> PolygonSimplify::simplify(std::vector<glm::vec2> points, float eps)
{
    std::vector<glm::vec2> resultData;
    std::stack<PSData> PDStack;

    if (points.size() == 0)
    {
        return resultData;
    }

    resultData.push_back(points[0]);

    PSData startData{0, (int)points.size() - 1};
    PDStack.push(startData);

    while (!PDStack.empty())
    {
        float maxDist = 0;
        PSData temp = PDStack.top();
        PDStack.pop();
        int tempIndex = temp.startIndex;
        for (int i = temp.startIndex; i < temp.endIndex; i++)
        {
            float dist = perpDistance(points[i], points[temp.startIndex], points[temp.endIndex]);
            if (dist > maxDist)
            {
                maxDist = dist;
                tempIndex = i;
            }
        }

        if (maxDist > eps)
        {
            PDStack.push(PSData{tempIndex, temp.endIndex});
            PDStack.push(PSData{temp.startIndex, tempIndex});
        }
        else
        {
            resultData.push_back(points[temp.endIndex]);
        }
    }

    return resultData;
}

contoursData PolygonSimplify::getSimplifiedContours(int *pixelData, int dimX, int dimY, float eps)
{
    contoursData simplifiedResult;
    std::vector<MarchingSquares::contourData> contours = MarchingSquares::getContours(pixelData, dimX, dimY);

    for (MarchingSquares::contourData c : contours)
    {
        std::vector<glm::vec2> simplified = simplify(c.points, eps);
        simplifiedResult.push_back(simplified);
    }

    return simplifiedResult;
}

float PolygonSimplify::perpDistance(glm::vec2 point, glm::vec2 line1, glm::vec2 line2)
{
    float lineLen = glm::sqrt((line2.x - line1.x) * (line2.x - line1.x) + (line2.y - line1.y) * (line2.y - line1.y));

    if (lineLen < 0.0001f)
    {
        return glm::sqrt((line2.x - point.x) * (line2.x - point.x) + (line2.y - point.y) * (line2.y - point.y));
    }

    float numerator = glm::abs((line2.x - line1.x) * (line1.y - point.y) - (line1.x - point.x) * (line2.y - line1.y));
    return numerator / lineLen;
}