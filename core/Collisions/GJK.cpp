#include "GJK.h"
#include "EPA2D.hpp"

glm::vec3 Support(
    const Collider* colliderA,
    const Collider* colliderB,
    glm::vec3 direction )
{
    return colliderA -> FindFurtherstPoint( direction )
    - colliderB -> FindFurtherstPoint( -direction );
}

CollisionProduct * GJK( const Collider * colliderA, const Collider * colliderB )
{
    glm::vec3 unit_x = glm::vec3( 1.f, 0.f, 0.f );
    glm::vec3 support = Support( colliderA, colliderB, unit_x );

    Simplex points;
    points.push_front( support );

    glm::vec3 direction = -support;

    while( true )
    {
        support = Support( colliderA, colliderB, direction );

        if( glm::dot( support, direction ) < 0.f ) {
            return new CollisionProduct(false); //no collision
        }

        points.push_front( support );

        if( NextSimplex( points, direction ) )
        {
            if( !colliderA->isTrigger && !colliderB->isTrigger )
                return EPA2D( points, colliderA, colliderB );
            else
                return new CollisionProduct(true);
        }
    }
}

bool NextSimplex( Simplex& points, glm::vec3& direction )
{
    switch ( points.size() )
    {
        case 2: return Line( points, direction );
        case 3: return Triangle( points, direction );
        case 4: return Tetrahedron( points, direction );
    }

    return false;
}

bool SameDirection( const glm::vec3& direction, const glm::vec3& AO_dir )
{
    return ( glm::dot( direction, AO_dir ) > 0.f );
}

bool Line( Simplex& points, glm::vec3& direction )
{
    glm::vec3 a = points[0];
    glm::vec3 b = points[1];

    glm::vec3 ab = b - a;
    glm::vec3 ao = -a;

    if( SameDirection( ab, ao ) ) {
        direction = glm::cross( glm::cross( ab, ao ), ab );
    }
    else{
        points = { a };
        direction = ao;
    }

    return false;
}

bool Triangle( Simplex& points, glm::vec3& direction )
{
    glm::vec3 a = points[0];
    glm::vec3 b = points[1];
    glm::vec3 c = points[2];

    glm::vec3 ab = b - a;
    glm::vec3 ac = c - a;
    glm::vec3 ao = -a;

    glm::vec3 abc = glm::cross( ab, ac );

    if( SameDirection( glm::cross( abc, ac ), ao) )
    {
        if( SameDirection( ac, ao ) ){
            
            points = { a, c };
            direction = glm::cross( glm::cross( ac, ao ), ac );
        }
        else{
            return Line( points = { a, b }, direction );
        }
    }
    else{
        if( SameDirection( glm::cross( ab, abc ) , ao ) ){
            return Line( points = { a, b }, direction );
        }
        else{
            if( SameDirection( abc, ao ) ){
                direction = abc;
            }
            else{
                points = { a, c, b };
                direction = -abc;
            }
        }
    }
    return false;
}

bool Tetrahedron( Simplex& points, glm::vec3& direction )
{
    glm::vec3 a = points[0];
    glm::vec3 b = points[1];
    glm::vec3 c = points[2];
    glm::vec3 d = points[3];

    glm::vec3 ab = b - a;
    glm::vec3 ac = c - a;
    glm::vec3 ad = d - a;
    glm::vec3 ao = -a;

    glm::vec3 abc = glm::cross( ab, ac );
    glm::vec3 acd = glm::cross( ac, ad );
    glm::vec3 adb = glm::cross( ad, ab );

    if( SameDirection( abc, ao ) ){
        points = { a, b, c };
        return Triangle( points, direction );
    }
    if( SameDirection( acd, ao ) ){
        points = { a, c, d };
        return Triangle( points , direction );
    }
    if( SameDirection( adb, ao ) ){
        points = { a, d, b };
        return Triangle( points, direction );
    }

    return true;
}
