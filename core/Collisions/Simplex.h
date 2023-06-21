#ifndef SIMPLEX_HPP
#define SIMPLEX_HPP

#include "../../thirdparty/glm/glm.hpp"
#include <array>

class Simplex 
{
private: 
    std::array<glm::vec3, 4> m_points;
    unsigned int m_size;

public: 
    Simplex():
     m_points({ glm::vec3(0), glm::vec3(0), glm::vec3(0), glm::vec3(0) }),
     m_size(0)
     {}

     Simplex& operator= (std::initializer_list<glm::vec3> list);
     void push_front(glm::vec3 point);
     glm::vec3& operator[](unsigned i) { return m_points[i]; }
     unsigned size() const;
     auto begin() const { return m_points.begin(); }
     auto end() const { return m_points.end() - (4 - m_size); }
     void insert(unsigned i, glm::vec3 point);
};

#endif