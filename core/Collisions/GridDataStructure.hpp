#ifndef GRID_DATA_STRUCTURE
#define GRID_DATA_STRUCTURE

#include "Collider.h"
#include <vector>
#include <set>
#include "../../thirdparty/glm/glm.hpp"
#include "CollisionManager.h"
#include "CollisionTests.h"
#include "CollisionSolver.h"
#include "ColliderBoundary.hpp"
#include "GridCell.hpp"
#include "../TransformComponent/TransformComponent.h"
//#include "../Debug/Debug.hpp"

class Grid
{
    float cellSize;
    float minCellSize = 0.23f;
    glm::vec2 gridBegin;
    int xLength;
    int yLength;
    GridCell ** grid;

public:

    Grid(std::set<Collider*>& colliders,float worldSizeX,float worldSizeY)
    {
        cellSize = minCellSize;
        for (auto& collider : colliders) {
            cellSize = std::max(cellSize, collider->boundary.ReturnSize());
        }

        cellSize += 0.02f;

        float cornerX = -worldSizeX / 2;
        float cornerY = worldSizeY / 2;

        gridBegin.x = cornerX - cellSize / 2;
        gridBegin.y = cornerY + cellSize / 2;

        std::cout<<"Grid size: "<<cellSize<<'\n';
        
        xLength = glm::ceil(worldSizeX / cellSize) + 2;
        yLength = glm::ceil(worldSizeY / cellSize) + 2;

        grid = new GridCell* [xLength];
        for(int i = 0;i < xLength; i++)
            grid[i] = new GridCell [yLength];

        for(int i = 0;i < xLength; i++)
            for(int j = 0;j < yLength; j++)
            {
                float minX = gridBegin.x + (float)i * cellSize;
                float maxY = gridBegin.y - (float)j * cellSize;
                float maxX = minX + cellSize;
                float minY = maxY - cellSize;

                grid[i][j].boundary = new ColliderBoundary(minX,minY,maxX,maxY);
                grid[i][j].CreateGizmo();
            }
    }

    void AddToGrid(Collider* collider)
    {
        ColliderBoundary* boundary = collider->GetBoundary();
        glm::vec2 leftUpperCorner = boundary->upperLeft;
        
        int cellX = glm::floor( std::abs(leftUpperCorner.x - gridBegin.x ) / cellSize);
        int cellY = glm::floor( std::abs( gridBegin.y - leftUpperCorner.y ) / cellSize);

        if(cellX >= xLength || cellY >= yLength)
            return;

        grid[cellX][cellY].Add(collider);

        if( cellX + 1 < xLength && TestBoundaryBoundary(boundary, grid[cellX + 1][cellY].boundary) )
            grid[cellX + 1][cellY].Add(collider);

        if(cellY + 1 >= yLength)
            return;

        if( TestBoundaryBoundary(boundary, grid[cellX][cellY + 1].boundary) )
            grid[cellX][cellY + 1].Add(collider);

        if( TestBoundaryBoundary(boundary, grid[cellX + 1][cellY + 1].boundary) )
            grid[cellX + 1][cellY + 1].Add(collider);
        
        delete boundary;
    }

    void ClearGrid()
    {
        for(int i = 0;i < xLength; i++)
            for(int j = 0;j < yLength; j++)
                grid[i][j].Clear();
        
    }

    void ProcessCollisions()
    {
        for(int y = 0; y < yLength; y++)
            for(int x = 0; x < xLength; x++)              
            {
                grid[x][y].TestWithSelf();

                if(x < xLength - 1)
                    grid[x][y].TestAgainst( &grid[x + 1][y] );
                
                if(y < yLength - 1)
                {
                    grid[x][y].TestAgainst( &grid[x][y + 1] );

                    if(x < xLength - 1)
                        grid[x][y].TestAgainst( &grid[x + 1][y + 1] );
                }
            }
    }

    ~Grid()
    {
        for(int i = 0;i < xLength; i++)
            delete [] grid[i];

        delete [] grid;
    }
};

#endif

