#ifndef GIZMO_H
#define GIZMO_H

#include "ModelDraw.h"
#include "Shader.h"
#include "../../thirdparty/glm/glm.hpp"
#include "Vertex.hpp"

class Gizmo : public ModelDraw
{
public:
    Gizmo( Vertex *, int );
    ~Gizmo();

protected:
    void Create(Vertex *);
    void Update();


    Mesh *mesh;
    float offsetX = 0.f, offsetY = 0.f;
    float scaleX = 1.f, scaleY = 1.f;
    float r = 0.f, g = 1.f, b = 0.f;
    int _number0fVertices;
};

#endif