#include "Mesh.h"

Mesh::Mesh(
    int nVertixes_,
    Vertex *vertices_,
    int nIndices_, unsigned int *indices_,
    unsigned int vao_
) : nVertices(nVertixes_), vertices(vertices_), nIndices(nIndices_), indices(indices_)
{
    localTransform = glm::mat4(1.f);
    vao = vao_;

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, nVertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    Vertex::ConfigureAttributes();

    glBindVertexArray(0);
}

Mesh::Mesh(
        int nVeritices_, Vertex * vertices_
) : nVertices(nVeritices_), vertices(vertices_) { }

Mesh::~Mesh()
{
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);
}

void 
Mesh::SetLocalTransform(glm::mat4& localTransform_)
{
    localTransform = localTransform_;
}

const glm::mat4&
Mesh::GetLocalTransform() const
{
    return localTransform;
}

void Mesh::SetVertices(Vertex * new_vertices, int new_n_Vertices)
{
    vertices = new_vertices;
    nVertices = new_n_Vertices;

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * nVertices, vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}