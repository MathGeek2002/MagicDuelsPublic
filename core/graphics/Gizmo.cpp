#include "Gizmo.h"
#include "Mesh.h"
#include <iostream>

Gizmo::Gizmo(Vertex * vertices,int numberOfVertices) : _number0fVertices(numberOfVertices)
{
    type = WIRED;
    nMesh = 1;
    Create(vertices);
}

Gizmo::~Gizmo()
{
    glDeleteVertexArrays(1, vaos);
    delete[] vaos;
    delete mesh;
    delete [] meshes;
}

void Gizmo::Create(Vertex * vertices)
{
    vaos = new unsigned int[1];
    glGenVertexArrays(1, vaos);

    int numberOfIndices = _number0fVertices * 2;

    unsigned int *indices = new unsigned int[numberOfIndices];

    for(int i = 0;i < _number0fVertices; i++)
    {
        indices[i * 2] = i;
        indices[i * 2 + 1] = i + 1;
    }

    indices[numberOfIndices - 1] = 0;

    mesh = new Mesh(
        _number0fVertices, vertices,
        numberOfIndices, indices,
        vaos[0]);

    meshes = new Mesh*[1];
    meshes[0] = mesh;

    delete [] indices;
}