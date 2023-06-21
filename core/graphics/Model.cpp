#include "Model.h"

Model::Model()
{
    localTransform = glm::mat4(1.f);
    SetModelType( model_3d_T );
}

Model::~Model() {}