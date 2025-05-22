#pragma once

#include "glm/glm.hpp"
#include "glm/fwd.hpp"

class Camera
{
private:
    float horizontalAngle_m;
    float verticalAngle_m;
    float intialFov_m;
    float speed_m;
    float mouseSpeed_m;

    glm::mat4 viewMatrix_m;
    glm::mat4 projectionMatrix_m;

    glm::vec3 position_m;
public:
    Camera() noexcept;

    void ComputeMatricesFromInputs();

    glm::mat4 GetProjectionMatrix() const noexcept;
    glm::mat4 GetViewMatrix() const noexcept;
};
