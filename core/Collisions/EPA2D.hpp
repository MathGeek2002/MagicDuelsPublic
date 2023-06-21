#ifndef EPA2D_HPP
#define EPA2D_HPP

#include "CollisionProduct.hpp"
#include "Simplex.h"
#include "Collider.h"
#include "GJK.h"
#include "../../thirdparty/glm/glm.hpp"
#include <vector>
#include <utility>

#define MAX_ITERATIONS 64

CollisionProduct* EPA2D ( Simplex& simplex, 
const Collider* colliderA, const Collider* colliderB )
{
    int minIndex = 0;
    float minDistance = FLT_MAX;
    glm::vec3 minNormal;
    std::vector <glm::vec3> polytope;
    int iterations = 0;

    for ( size_t i = 1;i < simplex.size(); i++) 
        polytope.push_back(simplex[i]);

    while ( minDistance == FLT_MAX )
    {
        for( size_t i = 0;i < polytope.size(); i++) {
            size_t j = (i + 1) % polytope.size();

            glm::vec3 vertexI = polytope[i];
            glm::vec3 vertexJ = polytope[j];
            glm::vec3 vecIJ = vertexJ - vertexI;

            glm::vec3 normal = glm::normalize( glm::vec3( vecIJ.y, -vecIJ.x , 0.f) );

            float distance = glm::dot( normal, vertexI );

            if( distance < 0.f )
            {
                distance *= -1.f;
                normal *= -1.f;
                
            }

            if( distance < minDistance )
            {
                minDistance = distance;
                minNormal = normal;
                minIndex = j;
            }
        }

        glm::vec3 support = Support( colliderA, colliderB , minNormal );
        float sDistance = glm::dot( minNormal, support );

        if( std::abs( sDistance - minDistance ) > 0.001f  && iterations < MAX_ITERATIONS )
        {
            minDistance = FLT_MAX;
            polytope.insert( polytope.begin() + minIndex ,support );
        }
        iterations++;
    }

    CollisionProduct * ans = new CollisionProduct( true );
    ans -> normal = minNormal * glm::sqrt( (minDistance ) + 0.000001f ); 
    ans -> Depth = glm::length( ans -> normal );
    return ans;
}

#endif