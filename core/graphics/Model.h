#ifndef MODEL_H
#define MODEL_H

#include "Shader.h"
#include "../../thirdparty/glm/glm.hpp"
#include "../ECS/ECS.hpp"

enum ModelType {
    sprite_T,
    model_3d_T,
    animated_sprite_T
};

class Model
{
protected: 
    glm::mat4 localTransform;
    ModelType type;

public:
    Model();
    virtual ~Model();
    virtual void Draw(Shader* shader) = 0;
    inline void SetLocalTransform(glm::mat4& localTransform_);
    inline const glm::mat4& GetLocalTransform() const;
    inline ModelType GetType() const;
    inline void SetModelType(ModelType);
};

inline void 
Model::SetLocalTransform(glm::mat4& localTransform_)
{
    localTransform = localTransform_;
}

inline const glm::mat4&
Model::GetLocalTransform() const
{
    return localTransform;
}

inline ModelType 
Model::GetType() const
{
    return type;
}

inline void 
Model::SetModelType(ModelType myType)
{
    type = myType;
}

#endif