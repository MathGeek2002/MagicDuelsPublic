#ifndef MODEL_DRAW_H
#define MODEL_DRAW_H

#include "Mesh.h"
#include "Shader.h"
#include "Model.h"
#include "Materials.h"

class ModelDraw : public Model
{
public: // TODO make it private
    enum
    {
        RANDOM = 0,
        COLOR = 1,
        MATERIAL = 2,
        WIRED = 3
    };
    int type;
    int nMesh;
    Mesh **meshes;
    unsigned int *vaos;
    Material **materials;
    bool isMaterial;

public:
    ModelDraw();
    virtual ~ModelDraw();
    virtual void Draw(Shader *shader_);
};

#endif