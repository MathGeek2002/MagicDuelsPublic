#ifndef CAMERA2D_H
#define CAMERA2D_H

#include "../../thirdparty/glm/glm.hpp"
#include "../../thirdparty/glm/gtc/matrix_transform.hpp"
#include "Shader.h"

class Camera2D
{
public:
    enum
    {
        PERSPECTIVE_PROJECTION = 0,
        ORTHO_PROJECTION = 1
    };

    unsigned int projectionType;
    glm::vec3 pos;
    glm::vec3 lookAt;
    glm::vec3 up;
    float viewAngle, ratio, near, far;

    Camera2D(
        float = 0.f, float = 0.f, float = 2.f,                        // Camera position
        float = 0.f, float = 0.f, float = 0.f,                        // Look at position
        float = 0.f, float = 1.f, float = 0.f,                        // Up vector
        float = 50.f, float = 4.f / 3.f, float = 0.1f, float = 100.f, // View angle, ratio, near, far
        unsigned int = PERSPECTIVE_PROJECTION                         // Projection type
    );
    ~Camera2D();
    glm::mat4 GetLook();
    void SetLook(Shader *);
    void SetRatio(float);
    void Update();
};

#endif