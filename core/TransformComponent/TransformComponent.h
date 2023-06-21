#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "../ECS/ECS.hpp"
#include "../../thirdparty/glm/glm.hpp"
#include "../../thirdparty/glm/gtc/quaternion.hpp"
#include "../../thirdparty/glm/gtc/matrix_transform.hpp"
#include <iostream>

class TransformComponent : public Component
{
public:
  TransformComponent(
      glm::vec3 position_ = {0.f, 0.f, 0.f},
      glm::vec3 scale_ = {1.f, 1.f, 1.f},
      glm::vec3 rotation_ = {0.f, 0.f, 1.f},
      float angle_ = 0.f);
  virtual ~TransformComponent();
  void SetPosition(const glm::vec3 &position_);
  glm::vec3 &GetPosition();
  void SetScale(const glm::vec3 &scale_);
  void SetRotation(const glm::vec3 &rotation_);
  void SetAngle(float angle_);
  virtual void UpdateModel();
  void SetModel(const glm::mat4 &model_);
  glm::mat4 &getModel();
  void SetOrientation(const glm::quat &orientation_);
  virtual void UpdateOrientation();
  glm::mat4 &GetUnscaledModel();

  friend std::ostream &operator<<(std::ostream &ost, TransformComponent &trans)
  {
    return ost << trans.position.x << " " << trans.position.y << '\n';
  }

  glm::vec3 position;
  glm::vec3 scale;
  glm::vec3 rotation;
  float angle;
  glm::mat4 model;
  glm::quat orientation;
  glm::mat4 unscaledModel;
};

#endif