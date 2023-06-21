#ifndef EPA_H
#define EPA_H

#include "CollisionProduct.hpp"
#include "Simplex.h"
#include "Collider.h"
#include "GJK.h"
#include "../../thirdparty/glm/glm.hpp"
#include <vector>
#include <utility>

CollisionProduct* EPA(const Simplex& simplex, const Collider*  colliderA, const Collider* colliderB );

std::pair <std::vector<glm::vec4>, size_t > GetFaceNormals(
    const std::vector<glm::vec3>& polytope,
    const std::vector<size_t>& faces);

void AddIfUniqueEdge(
    std::vector< std::pair<size_t,size_t> >&edges,
    const std::vector<size_t>& faces,
    size_t a,
    size_t b);

#endif