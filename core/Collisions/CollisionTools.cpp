#include "CollisionTools.h"
#include <cmath>

float
SqDistPointAABB(
  glm::vec3 const &p,
  ColliderAABB const &b
)
{
  float sD = 0.;
  for(int i = 0; i < 2; i++){
    float x = p[i];
    if(x < b.min[i]){
      float tmp = b.min[i] - x;
      sD += tmp * tmp;
    }
    if(x > b.max[i]){
      float tmp = x - b.max[i];
      sD += tmp * tmp;
    }
  }

  return sD;
}

void
ClosestPointOnAABB(
  glm::vec3 const &p, 
  ColliderAABB const &a, 
  glm::vec3 &q 
)
{
  for( int i = 0; i < 3; i++ ){
    float v = p[i];
    if( v < a.min[i] ) v = a.min[i];
    if( v > a.max[i] ) v = a.max[i];
    q[i] = v;
  }
}

glm::vec3 ClosestPointOnLineSegment(glm::vec3 A, glm::vec3 B, glm::vec3 Point)
{
  glm::vec3 AB = B - A;
  float t = glm::dot(Point - A, AB) / glm::dot(AB, AB);
  return A + std::min(std::max(t, 0.f), 1.f) * AB; 
}

float SqDistPointSegment(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
    glm::vec3 ab = b - a, ac = c - a, bc = c - b;
    float e = glm::dot(ac, ab);
    // Handle cases where c projects outside ab
    if (e <= 0.0f) return glm::dot(ac, ac);
    float f = glm::dot(ab, ab);
    if (e >= f) return glm::dot(bc, bc);
    // Handle cases where c projects onto ab
    return glm::dot(ac, ac) - e * e / f;
}

float ClosestPtSegmentSegment(glm::vec3 p1, glm::vec3 q1, glm::vec3 p2, glm::vec3 q2,
float &s, float &t, glm::vec3 &c1, glm::vec3 &c2)
{
    glm::vec3 d1 = q1 - p1; // Direction vector of segment S1
    glm::vec3 d2 = q2 - p2; // Direction vector of segment S2
    glm::vec3 r = p1 - p2;
    float a = glm::dot(d1, d1); // Squared length of segment S1, always nonnegative
    float e = glm::dot(d2, d2); // Squared length of segment S2, always nonnegative
    float f = glm::dot(d2, r);
    float EPSILON = 1e-5;
    // Check if either or both segments degenerate into points
    if (a <= EPSILON && e <= EPSILON) {
        // Both segments degenerate into points
        s = t = 0.0f;
        c1 = p1;
        c2 = p2;
        return glm::dot(c1 - c2, c1 - c2);
    }
    if (a <= EPSILON) {
        // First segment degenerates into a point
        s = 0.0f;
        t = f / e; // s = 0 => t = (b*s + f) / e = f / e
        t = glm::clamp(t, 0.0f, 1.0f);
    } else {
        float c = glm::dot(d1, r);
        if (e <= EPSILON) {
            // Second segment degenerates into a point
            t = 0.0f;
            s = glm::clamp(-c / a, 0.0f, 1.0f); // t = 0 => s = (b*t - c) / a = -c / a
        } else {
            // The general nondegenerate case starts here
            float b = glm::dot(d1, d2);
            float denom = a*e-b*b; // Always nonnegative
            // If segments not parallel, compute closest point on L1 to L2 and
            // clamp to segment S1. Else pick arbitrary s (here 0)
            if (denom != 0.0f) {
                s = glm::clamp((b*f - c*e) / denom, 0.0f, 1.0f);
            } else s = 0.0f;
            // Compute point on L2 closest to S1(s) using
            // t = Dot((P1 + D1*s) - P2,D2) / Dot(D2,D2) = (b*s + f) / e
            t = (b*s + f) / e;
            // If t in [0,1] done. Else clamp t, recompute s for the new value
            // of t using s = Dot((P2 + D2*t) - P1,D1) / Dot(D1,D1)= (t*b - c) / a
            // and clamp s to [0, 1]
            if (t < 0.0f) {
                t = 0.0f;
                s = glm::clamp(-c / a, 0.0f, 1.0f);
            } else if (t > 1.0f) {
                t = 1.0f;
                s = glm::clamp((b - c) / a, 0.0f, 1.0f);
            }
        }
    }
    c1 = p1 + d1 * s;
    c2 = p2 + d2 * t;
    return glm::dot(c1 - c2, c1 - c2);
}