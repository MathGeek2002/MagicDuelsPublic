#include "TransformComponent.h"

TransformComponent::TransformComponent(
    glm::vec3 position_,
    glm::vec3 scale_,
    glm::vec3 rotation_,
    float angle_)
    : Component(CT_TRANSFORM),
      position(position_),
      scale(scale_),
      rotation(rotation_),
      angle(angle_)
{
  UpdateOrientation();
  UpdateModel();
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::SetPosition(const glm::vec3 &position_)
{
  position = position_;
}

glm::vec3 &
TransformComponent::GetPosition()
{
  return position;
}

void TransformComponent::SetScale(const glm::vec3 &scale_)
{
  scale = scale_;
}

void TransformComponent::SetRotation(const glm::vec3 &rotation_)
{
  rotation = rotation_;
}

void TransformComponent::SetAngle(float angle_)
{
  angle = angle_;
}

void TransformComponent::UpdateModel()
{
  model = glm::mat4(1.f);
  model = glm::translate(model, position);
  glm::mat4 tmp = glm::mat4_cast(orientation);
  model *= tmp;
  unscaledModel = model;
  model = glm::scale(model, scale);
}

void TransformComponent::SetModel(const glm::mat4 &model_)
{
  model = model_;
}

glm::mat4 &TransformComponent::getModel()
{
  UpdateModel();
  UpdateOrientation();
  return model;
}

void TransformComponent::SetOrientation(const glm::quat &orientation_)
{
  orientation = orientation_;
}

void TransformComponent::UpdateOrientation()
{
  glm::mat4 tmp(1.f);
  tmp = glm::rotate(tmp, glm::radians(angle), rotation);
  orientation = glm::quat_cast(tmp);
}

glm::mat4 & TransformComponent::GetUnscaledModel()
{
  return unscaledModel;
}
