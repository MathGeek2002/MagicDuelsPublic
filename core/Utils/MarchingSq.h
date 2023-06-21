#ifndef MARCHING_SQ_H
#define MARCHING_SQ_H

#include <algorithm>
#include <vector>
#include <stack>
#include "../../thirdparty/glm/glm.hpp"

class MarchingSquares
{
public:
    struct contourData
    {
        int pointsNum;
        std::vector<glm::vec2> points;
    };

private:
    struct Node
    {
        int posX, posY;
        int color;
        int mSqCase;
        Node *parent;
        std::vector<Node *> nodes;
    };
    struct SNode
    {
        Node *rNode; // main node
        Node *lNode; // secondary node for special cases
    };

public:
    static std::vector<contourData> getContours(int *pixelData, int dimX, int dimY);

private:
    static void connectNodes(SNode *snode1, SNode *snode2);
};

#endif