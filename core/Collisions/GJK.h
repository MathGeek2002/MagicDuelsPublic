#ifndef GJK_HPP
#define GJK_HPP

#include "Collider.h"
#include "MeshCollider.h"
#include "Simplex.h"
#include "../../thirdparty/glm/glm.hpp"
#include "CollisionProduct.hpp"

glm::vec3 Support(
    const Collider* colliderA,
    const Collider* colliderB,
    glm::vec3 direction
);

CollisionProduct * GJK(const Collider * colliderA, 
        const Collider * colliderB);

bool NextSimplex(Simplex& points, glm::vec3& direction);

bool SameDirection(
    const glm::vec3& direction,
    const glm::vec3& AO_dir
);

bool Line(
    Simplex& points,
    glm::vec3& direction
);

bool Triangle(
    Simplex& points,
    glm::vec3& direction
);

bool Tetrahedron(
    Simplex& points,
    glm::vec3& direction
);

#endif