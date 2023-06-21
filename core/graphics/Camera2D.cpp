#include "Camera2D.h"
#include "../../thirdparty/glm/gtc/type_ptr.hpp"

Camera2D::Camera2D(
    float px, float py, float pz,                 // Camera position
    float ax, float ay, float az,                 // Look at position
    float ux, float uy, float uz,                 // Up vector
    float va, float ratio, float near, float far, // View angle, ratio, near, far
    unsigned int pt                               // Projection type
    ) : projectionType(pt),
        pos(px, py, pz), lookAt(ax, ay, az), up(ux, uy, uz),
        viewAngle(va), ratio(ratio), near(near), far(far)
{
}

Camera2D::~Camera2D()
{
}

glm::mat4 Camera2D::GetLook()
{
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(viewAngle), ratio, near, far);
    glm::mat4 view;
    view = glm::lookAt(pos, lookAt, up);
    return projection * view;
}

void Camera2D::SetLook(Shader *shader_)
{
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(viewAngle), ratio, near, far);
    glm::mat4 view;
    view = glm::lookAt(pos, lookAt, up);
    view = projection * view;
    shader_->SetMatrix4("view", glm::value_ptr(view));
    shader_->SetVector3("viewPosition", glm::value_ptr(pos));
}

void Camera2D::SetRatio(float ratio_)
{
    ratio = ratio_;
}

void Camera2D::Update()
{
    lookAt = pos - glm::vec3(0, 0, 2.f);
}