#include "Simplex.h"
#include <iostream>

Simplex& Simplex::operator= (std::initializer_list<glm::vec3> list)
{
    for(auto v = list.begin(); v != list.end(); v++)
        m_points[std::distance(list.begin(), v)] = *v;

    m_size = list.size();
    return *this;
}

void Simplex::push_front(glm::vec3 point)
{
    m_points = { point ,m_points[0], m_points[1], m_points[2] };
    m_size = std::min(m_size + 1, 4u);
}

unsigned Simplex::size() const
{
    return m_size;
}

void Simplex::insert(unsigned index, glm::vec3 point)
{
    for( size_t i = 2;i >= index; i--)
        m_points[i + 1] = m_points[i];
    
    m_points[index] = point;

    m_size = std::min(m_size + 1, 4u);
}
