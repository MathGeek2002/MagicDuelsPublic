#include "MarchingSq.h"

typedef MarchingSquares::contourData contourData;

std::vector<contourData> MarchingSquares::getContours(int *pixelData, int dimX, int dimY)
{
    int tempX = dimX + 2;
    int tempY = dimY + 2;
    int *tempData = new int[tempX * tempY];
    std::fill_n(tempData, tempX * tempY, 0);

    // prepare temp data
    for (int i = 0; i < dimY; i++)
    {
        for (int j = 0; j < dimX; j++)
        {
            int sourceIndex = j + i * dimX;
            int tempIndex = (j + 1) + (i + 1) * tempX;
            tempData[tempIndex] = pixelData[sourceIndex];
        }
    }

    // determine marching squares cases
    // cases 0-15:
    // 0 - empty        // 1 - lb corner        // 2 - rb corner        // 3 - b line
    // 4 - rt corner    // 5 - lb to rt gap    // 6 - r line           // 7 - lt empty corner
    // 8 - lt corner    // 9 - l line           // 10 - rb to lt gap   // 11 - rt empty corner
    // 12 - t line      // 13 - rb empty corner // 14 - lb empty corner // 15 - filled

    SNode **nodes = new SNode *[tempX * tempY];
    int *mSqData = new int[tempX * tempY];

    for (int i = 0; i < tempY - 1; i++)
    {
        for (int j = 0; j < tempX - 1; j++)
        {
            int index00 = j + i * tempX;
            int index01 = j + (i + 1) * tempX;
            int index10 = (j + 1) + i * tempX;
            int index11 = (j + 1) + (i + 1) * tempX;

            int mSqCase = 0;
            mSqCase += (tempData[index00]) ? 8 : 0;
            mSqCase += (tempData[index01]) ? 1 : 0;
            mSqCase += (tempData[index10]) ? 4 : 0;
            mSqCase += (tempData[index11]) ? 2 : 0;

            mSqData[index00] = mSqCase;
            nodes[index00] = new SNode;

            switch (mSqCase)
            {
            case 0:
            case 15:
                nodes[index00]->rNode = nullptr;
                nodes[index00]->lNode = nullptr;
                break;
            case 5:
            case 10:
                nodes[index00]->rNode = new Node{j, i, 0, mSqCase, nullptr};
                nodes[index00]->lNode = new Node{j, i, 0, mSqCase, nullptr};
                break;

            default:
                nodes[index00]->rNode = new Node{j, i, 0, mSqCase, nullptr};
                nodes[index00]->lNode = nullptr;
                break;
            }
        }
    }

    // solve cases
    for (int i = 0; i < tempY - 1; i++)
    {
        for (int j = 0; j < tempX - 1; j++)
        {
            int index = j + i * tempX;
            int mSqCase = mSqData[index];

            switch (mSqCase)
            {
            case 0:
            case 15:
                break;

            case 1:
            case 14:
                connectNodes(nodes[index], nodes[(j) + (i + 1) * tempX]);
                connectNodes(nodes[index], nodes[(j - 1) + i * tempX]);
                break;

            case 2:
            case 13:
                connectNodes(nodes[index], nodes[(j) + (i + 1) * tempX]);
                connectNodes(nodes[index], nodes[(j + 1) + i * tempX]);
                break;

            case 3:
            case 12:
                connectNodes(nodes[index], nodes[(j - 1) + i * tempX]);
                connectNodes(nodes[index], nodes[(j + 1) + i * tempX]);
                break;

            case 4:
            case 11:
                connectNodes(nodes[index], nodes[(j) + (i - 1) * tempX]);
                connectNodes(nodes[index], nodes[(j + 1) + i * tempX]);
                break;

            case 5:
                connectNodes(nodes[index], nodes[(j) + (i + 1) * tempX]);
                connectNodes(nodes[index], nodes[(j + 1) + i * tempX]);
                connectNodes(nodes[index], nodes[(j) + (i - 1) * tempX]);
                connectNodes(nodes[index], nodes[(j - 1) + i * tempX]);
                break;

            case 10:
                connectNodes(nodes[index], nodes[(j) + (i + 1) * tempX]);
                connectNodes(nodes[index], nodes[(j - 1) + i * tempX]);
                connectNodes(nodes[index], nodes[(j) + (i - 1) * tempX]);
                connectNodes(nodes[index], nodes[(j + 1) + i * tempX]);
                break;

            case 6:
            case 9:
                connectNodes(nodes[index], nodes[(j) + (i - 1) * tempX]);
                connectNodes(nodes[index], nodes[(j) + (i + 1) * tempX]);
                break;

            case 7:
            case 8:
                connectNodes(nodes[index], nodes[(j) + (i - 1) * tempX]);
                connectNodes(nodes[index], nodes[(j - 1) + i * tempX]);
                break;

            default:
                break;
            }
        }
    }

    // create contours
    std::vector<contourData> contours;
    std::stack<Node *> nodesStack;

    for (int i = 0; i < tempY - 1; i++)
    {
        for (int j = 0; j < tempX - 1; j++)
        {
            int index = j + i * tempX;
            if (nodes[index]->rNode != nullptr && nodes[index]->rNode->color == 0)
            {
                // std::cout << "new Node\n";
                nodesStack.push(nodes[index]->rNode);
                while (!nodesStack.empty())
                {
                whileStart:
                    Node *curNode = nodesStack.top();
                    // std::cout << "curNode: " << curNode->posX << " " << curNode->posY << std::endl;
                    if (curNode->color == 0)
                    {
                        curNode->color = 1;
                    }
                    for (Node *tNode : curNode->nodes)
                    {
                        if (tNode->color == 0)
                        {
                            tNode->parent = curNode;
                            nodesStack.push(tNode);
                            goto whileStart;
                        }

                        if (tNode->color == 1 && curNode->parent != tNode)
                        {
                            // std::cout << "backtracking\n";
                            contourData tempContour{2};
                            tempContour.points.push_back(glm::vec2(tNode->posX, tNode->posY));
                            tempContour.points.push_back(glm::vec2(curNode->posX, curNode->posY));
                            Node *backNode = curNode->parent;
                            while (backNode != tNode)
                            {
                                // std::cout << "bt: " << backNode->posX << " " << backNode->posY << std::endl;
                                tempContour.points.push_back(glm::vec2(backNode->posX, backNode->posY));
                                tempContour.pointsNum++;
                                backNode = backNode->parent;
                            }
                            contours.push_back(tempContour);
                        }
                    }
                    curNode->color = 2;
                    nodesStack.pop();
                }
            }
        }
    }

    // holes handling
    // hole and solid detection
    std::vector<contourData> holeContours;
    std::vector<contourData> solidContours;

    for (contourData c : contours)
    {
        int sum = 0;
        glm::vec2 prevPoint = c.points[0];
        for (int i = 1; i < (int)c.points.size(); i++)
        {
            glm::vec2 curPoint = c.points[i];
            sum += (curPoint.x - prevPoint.x) * (curPoint.y - prevPoint.y);
            prevPoint = curPoint;
        }
        bool clockwise = (sum < 0);
        glm::vec2 nextPoint = c.points[1];
        int indexX = (int)nextPoint.x + (int)(nextPoint.y - prevPoint.y > 0.001f);
        int indexY = (int)nextPoint.y + (int)(nextPoint.x - prevPoint.x < 0.001f);
        int index = indexX + indexY * tempX;
        bool filled = tempData[index] > 0;

        if (clockwise != filled) // xor
        {
            // std::cout << "hole detected\n";
            holeContours.push_back(c);
        }
        else
        {
            // std::cout << "filled detected\n";
            solidContours.push_back(c);
        }
    }

    // hole connecting
    // to do: fix points crossing when connecting - causes error in convex decomposition
    // error seed: 1677415485

    // for (contourData hole : holeContours)
    // {
    //     if (hole.pointsNum < 5)
    //     { // ignore small holes
    //         continue;
    //     }

    //     glm::vec2 holePoint = hole.points[0];
    //     int contourDataIndex = 0;
    //     glm::vec2 closest = solidContours[0].points[0];
    //     int closestIndex = 0;
    //     float curDist = (holePoint.x - closest.x) * (holePoint.x - closest.x) + (holePoint.y - closest.y) * (holePoint.y - closest.y);

    //     // find closest point from solid contours
    //     int solidIndex = 0;
    //     for (contourData solid : solidContours)
    //     {
    //         int index = 0;
    //         for (glm::vec2 p : solid.points)
    //         {
    //             float newDist = (holePoint.x - p.x) * (holePoint.x - p.x) + (holePoint.y - p.y) * (holePoint.y - p.y);
    //             if (newDist < curDist)
    //             {
    //                 curDist = newDist;
    //                 closest = p;
    //                 contourDataIndex = solidIndex;
    //                 closestIndex = index;
    //             }
    //             index++;
    //         }
    //         solidIndex++;
    //     }

    //     // find closest point within hole contour
    //     int holeIndex = 0;
    //     int index = 0;
    //     for (glm::vec2 hp : hole.points)
    //     {
    //         float newDist = (closest.x - hp.x) * (closest.x - hp.x) + (closest.y - hp.y) * (closest.y - hp.y);
    //         if (newDist < curDist)
    //         {
    //             holeIndex = index;
    //             holePoint = hp;
    //             curDist = newDist;
    //         }
    //         index++;
    //     }

    //     // connect contours
    //     contourData newContour;
    //     for (glm::vec2 p : solidContours[contourDataIndex].points)
    //     {
    //         newContour.points.push_back(glm::vec2(p.x, p.y));
    //     }
    //     newContour.points.insert(newContour.points.begin() + closestIndex, newContour.points[closestIndex]);

    //     for (int i = 0; i <= hole.pointsNum; i++)
    //     {
    //         int curIndex = holeIndex + i;
    //         if (curIndex >= hole.pointsNum)
    //         {
    //             curIndex -= hole.pointsNum;
    //         }
    //         glm::vec2 temp = hole.points[curIndex];
    //         newContour.points.insert(newContour.points.begin() + closestIndex + 1, temp);
    //     }
    //     newContour.pointsNum = newContour.points.size();

    //     solidContours.erase(solidContours.begin() + contourDataIndex);
    //     solidContours.push_back(newContour);
    // }

    // clear nodes
    for (int i = 0; i < tempY - 1; i++)
    {
        for (int j = 0; j < tempX - 1; j++)
        {
            int index = j + i * tempX;
            if (nodes[index] != nullptr)
            {
                if (nodes[index]->rNode != nullptr)
                {
                    delete nodes[index]->rNode;
                }
                if (nodes[index]->lNode != nullptr)
                {
                    delete nodes[index]->lNode;
                }
                delete nodes[index];
            }
        }
    }
    delete[] nodes;

    delete[] tempData;
    delete[] mSqData;

    // return contours;
    return solidContours;
}

void MarchingSquares::connectNodes(SNode *snode1, SNode *snode2)
{
    if (snode1->rNode == nullptr || snode2->rNode == nullptr)
    {
        return;
    }

    int diffX = snode2->rNode->posX - snode1->rNode->posX;
    int diffY = snode2->rNode->posY - snode1->rNode->posY;
    int dir = 0; // 0 - top, 1 - right, 2 - bottom, 3 - left

    if (diffX == 0)
    {
        dir = (diffY > 0) ? 2 : 0;
    }
    else
    {
        dir = (diffX > 0) ? 1 : 3;
    }

    int mSqCase1 = snode1->rNode->mSqCase;
    Node *node1 = snode1->rNode;
    if (snode1->lNode != nullptr) // special double node
    {
        bool condition = (dir == 3) || (dir == 2 && mSqCase1 == 5) || (dir == 0 && mSqCase1 == 10);
        if (condition)
        {
            node1 = snode1->lNode;
        }
    }

    int mSqCase2 = snode2->rNode->mSqCase;
    Node *node2 = snode2->rNode;
    if (snode2->lNode != nullptr)
    {
        bool condition = (dir == 1) || (dir == 2 && mSqCase2 == 10) || (dir == 0 && mSqCase2 == 5);
        if (condition)
        {
            node2 = snode2->lNode;
        }
    }

    // std::cout << "Connecting: " << node1->posX << " " << node1->posY << " with " <<
    // node2->posX << " " << node2->posY << " case: " << node1->mSqCase << " dir: " << dir << " addr: "<< node1 << std::endl;
    node1->nodes.push_back(node2);
}