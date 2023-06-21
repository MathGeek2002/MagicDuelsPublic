#ifndef GRID_CELL_HPP
#define GRID_CELL_HPP

#include "Collider.h"
#include <vector>
#include <set>
#include "../../thirdparty/glm/glm.hpp"
#include "CollisionManager.h"
#include "CollisionTests.h"
#include "CollisionSolver.h"
#include "ColliderBoundary.hpp"
#include "../graphics/Gizmo.h"
#include "../graphics/Vertex.hpp"
#include "../graphics/RenderEngine.h"

class GridCell
{
public:
    std::vector<Collider*> colliders;
    std::vector<Collider*> staticColliders;
    ColliderBoundary* boundary;
    Gizmo * myGizmo;

    inline void Add(Collider* collider)
    {
        if( !collider->isStatic )
            colliders.push_back(collider);
        else
            staticColliders.push_back(collider);
    }

    inline void Clear()
    {
        colliders.clear();
        staticColliders.clear();
    }

    void TestAgainst(GridCell* secondCell)
    {
        for(auto it = colliders.begin(); it != colliders.end(); it++)
        {
            Collider * coll = *it;

            if( !TestBoundaryBoundary( &coll->boundary, secondCell->boundary) )
                continue;

            for(auto it2 = secondCell->colliders.begin(); it2 != secondCell->colliders.end(); it2++)
            {
                Collider * coll2 = *it2;

                if( coll == coll2 )
                    continue;

                if( coll-> entity == coll2 -> entity )
                    continue;

                //if( !TestBoundaryBoundary(&coll->boundary, &coll2->boundary) )
                //    continue;
                
                if( !coll -> TestCollision(coll2) )
                    continue;

                CollisionProduct* collisionProduct = coll->Solve( coll2 );

                if( collisionProduct -> hasCollision ) {
                    CollisionManager::RegisterCollision(coll,coll2,collisionProduct,false);
                }
                else{
                    delete collisionProduct;
                }
            }

            for(auto it2 = secondCell->staticColliders.begin(); it2 != secondCell->staticColliders.end(); it2++)
            {
                Collider * coll2 = *it2;

                //if( !TestBoundaryBoundary(&coll->boundary, &coll2->boundary) )
                //    continue;

                if( !coll -> TestCollision(coll2) )
                    continue;
                
                CollisionProduct* collisionProduct = coll->Solve( coll2 );
                if( collisionProduct -> hasCollision ) {
                    CollisionManager::RegisterCollision(coll,coll2,collisionProduct,true);
                }
                else{
                    delete collisionProduct;
                }
            }
        }
    }

    void TestWithSelf()
    {
        for(auto it = colliders.begin(); it != colliders.end(); it++)
        {
            Collider * coll = *it;
            for(auto it2 = it; it2 != colliders.end(); it2++)
            {
                Collider * coll2 = *it2;

                if( coll == coll2 )
                    continue;

                if( coll-> entity == coll2 -> entity )
                    continue;

                //if( !TestBoundaryBoundary(&coll->boundary, &coll2->boundary) )
                //    continue;
                
                if( !coll -> TestCollision(coll2) )
                    continue;

                CollisionProduct* collisionProduct = coll->Solve( coll2 );

                if( collisionProduct -> hasCollision ) {
                    CollisionManager::RegisterCollision(coll,coll2,collisionProduct,false);
                }
                else{
                    delete collisionProduct;
                }
            }

            for(auto it2 = staticColliders.begin(); it2 != staticColliders.end(); it2++)
            {
                Collider * coll2 = *it2;

                //if( !TestBoundaryBoundary(&coll->boundary, &coll2->boundary) )
                //    continue;

                if( !coll -> TestCollision(coll2) )
                    continue;
                
                CollisionProduct* collisionProduct = coll->Solve( coll2 );
                if( collisionProduct -> hasCollision ) {
                    CollisionManager::RegisterCollision(coll,coll2,collisionProduct,true);
                }
                else{
                    delete collisionProduct;
                }
            }
        }
    }

    inline void CreateGizmo()
    {
        Vertex *vertices = new Vertex[4];
           
        vertices[0].position = glm::vec3( boundary->min.x, boundary->max.y, 0.f );
        vertices[1].position = glm::vec3( boundary->max.x, boundary->max.y, 0.f );
        vertices[2].position = glm::vec3( boundary->max.x, boundary->min.y, 0.f );
        vertices[3].position = glm::vec3( boundary->min.x, boundary->min.y, 0.f );

        for(int i = 0;i < 4; i++){
            vertices[i].color = glm::vec3(1.f,0.f,0.f);
            vertices[i].normal = glm::vec3(0.f,0.f,1.f);
            vertices[i].texcoord = glm::vec2(0.f,0.f);
        }
        myGizmo = new Gizmo(vertices,4);
        delete [] vertices;

        RenderEngine::RegisterModel(myGizmo);
    }

    ~GridCell()
    {
        delete boundary;
        Clear();
        RenderEngine::UnRegisterModel(myGizmo);
        delete myGizmo;
    }
};

#endif