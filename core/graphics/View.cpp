#include "View.h"
#include <GL/glew.h>
#include <iostream>

View::View(
    unsigned int sW, unsigned int sH,
    float x_, float y_,
    float w_, float h_)
    : screenWidth(sW), screenHeight(sH),
      x(x_), y(y_), width(w_), height(h_)
{
    camera = new Camera2D;
    camera->SetRatio((float)(sW * width) / (float)(sH * height));
}

View::~View()
{
    delete camera;
}

void View::SetScreen(unsigned int sW, unsigned int sH)
{
    screenWidth = sW;
    screenHeight = sH;
    camera->SetRatio((float)(sW * width) / (float)(sH * height));
}

glm::mat4 View::GetView()
{
    glViewport(
        x * screenWidth, y * screenHeight,
        width * screenWidth, height * screenHeight);

    return camera->GetLook();
}

void View::SetView(Shader *shader_)
{
    glViewport(
        x * screenWidth, y * screenHeight,
        width * screenWidth, height * screenHeight);
    camera->SetLook(shader_);
}

glm::vec2 View::screenToWorld(glm::vec2 screenPos)
{
    float halfFov = this->camera->viewAngle / 360.f * glm::pi<float>();
    glm::vec3 cameraPos = this->camera->pos;
    float dist = glm::abs(cameraPos.z);
    float aspectRatio = (float)screenWidth / (float)screenHeight;
    float halfHeight = dist * glm::tan(halfFov);
    float halfWidth = halfHeight * aspectRatio;

    float resultX = (2 * screenPos.x / screenWidth - 1) * halfWidth;
    float resultY = (-2 * screenPos.y / screenHeight + 1) * halfHeight;

    return glm::vec2{resultX, resultY};
}