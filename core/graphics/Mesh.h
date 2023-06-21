#ifndef MESH_H
#define MESH_H

#include<GL/glew.h>
#include<GL/gl.h>
#include "Vertex.hpp"
#include <iostream>

class Mesh
{
public:
    unsigned int vbo,ebo, vao;
    glm::mat4 localTransform;

    Mesh(
        int nVerives_, Vertex *vertices,
        int nIndices_, unsigned int *indices_,
        unsigned int vao_
    );

    Mesh(
        int nVerives_, Vertex *vertices_
    );

    ~Mesh();
    void SetLocalTransform(glm::mat4& localTransform_);
    const glm::mat4& GetLocalTransform() const;
    void SetVertices(Vertex * new_vertices, int new_n_Vertices);
    inline Vertex * GetVertices() const;
    inline void SetIndices(unsigned int * new_indices);
    inline unsigned int * GetIndices() const;
    inline unsigned int GetNumberOfIndices() const;
    inline int GetNumberOfVertices() const;

private:
    int nVertices;
    Vertex * vertices;
    unsigned int nIndices;
    unsigned int *indices;
};

inline Vertex * Mesh::GetVertices() const 
{
    return vertices;
}

inline void Mesh::SetIndices(unsigned int * new_indices)
{
    indices = new_indices;
    nIndices = sizeof(indices) / sizeof(unsigned int*);
}

inline unsigned int * Mesh::GetIndices() const
{
    return indices;
}

inline unsigned int Mesh::GetNumberOfIndices() const
{
    return nIndices;
}

inline int Mesh::GetNumberOfVertices() const
{
    return nVertices;
}

#endif