#include "ModelDraw.h"
#include <iostream>

ModelDraw::ModelDraw()
{
    nMesh = 0;
    meshes = nullptr;
    vaos = nullptr;
    materials = nullptr;
    isMaterial = false;
}

ModelDraw::~ModelDraw() {}

void ModelDraw::Draw(Shader *shader_)
{
    if (type == MATERIAL)
    {
        for (int i = 0; i < nMesh; i++)
        {
            glm::mat4 model = localTransform * meshes[i]->localTransform;
            shader_->SetMatrix4("model", glm::value_ptr(model));
            glm::mat3 normal = glm::mat3(glm::inverse(glm::transpose(model)));
            shader_->SetMatrix3("normal", glm::value_ptr(normal));
            materials[i] -> SetMaterial(shader_);

            glBindVertexArray(vaos[i]);
            glDrawElements(GL_TRIANGLES, meshes[i] -> GetNumberOfIndices() , GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }
    else if(type == WIRED)
    {
        for (int i = 0; i < nMesh; i++)
        {
            glm::mat4 model = localTransform * meshes[i]->localTransform;
            shader_->SetMatrix4("model", glm::value_ptr(model));
            glm::mat3 normal = glm::mat3(glm::inverse(glm::transpose(model)));
            shader_->SetMatrix3("normal", glm::value_ptr(normal));

            glBindVertexArray(vaos[i]);
            glDrawElements(GL_LINE_STRIP, meshes[i] -> GetNumberOfIndices() , GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }
    else
    {
        for (int i = 0; i < nMesh; i++)
        {
            glm::mat4 model = localTransform * meshes[i]->localTransform;
            shader_->SetMatrix4("model", glm::value_ptr(model));
            glm::mat3 normal = glm::mat3(glm::inverse(glm::transpose(model)));
            shader_->SetMatrix3("normal", glm::value_ptr(normal));
            glBindVertexArray(vaos[i]);
            glDrawElements(GL_TRIANGLES, meshes[i] -> GetNumberOfIndices(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
    }
}