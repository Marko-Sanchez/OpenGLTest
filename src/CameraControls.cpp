#include "CameraControls.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

#include <GLFW/glfw3.h>

Camera::Camera() noexcept
:horizontalAngle_m(3.14f), verticalAngle_m(0.0f), initialFov_m(65.0f), speed_m(3.0f), mouseSpeed_m(0.005f),
 position_m(std::move(glm::vec3(0, 0, 5)))
{}

void Camera::ComputeMatricesFromInputs(std::shared_ptr<GLFWwindow> window)
{
    static double lastTime = glfwGetTime();
    double currentTime{glfwGetTime()};

    // helps balance slow (larger delta) and fast fps (smaller delta) computers.
    float deltaTime{static_cast<float>(currentTime - lastTime)};

    int WINDOW_WIDTH, WINDOW_HEIGHT;
    double xpos, ypos;

    glfwGetWindowSize(window.get(), &WINDOW_WIDTH, &WINDOW_HEIGHT);
    glfwGetCursorPos(window.get(), &xpos, &ypos);
    glfwSetCursorPos(window.get(), static_cast<double>(WINDOW_WIDTH) / 2, static_cast<double>(WINDOW_HEIGHT) / 2);

    horizontalAngle_m += mouseSpeed_m * (static_cast<float>(WINDOW_WIDTH) / 2 - xpos);
    verticalAngle_m   += mouseSpeed_m * (static_cast<float>(WINDOW_HEIGHT) / 2 - ypos);

    // Direction: Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction(
        cos(verticalAngle_m) * sin(horizontalAngle_m),
        sin(verticalAngle_m),
        cos(verticalAngle_m) * cos(verticalAngle_m)
    );

   // Right vector
   glm::vec3 right(
        sin(horizontalAngle_m - 3.14f / 2.0f),
        0,
        cos(horizontalAngle_m - 3.14f / 2.0f)
    );

    // Up vector
    glm::vec3 up = glm::cross(right, direction);

    // Move forward
    if (glfwGetKey(window.get(), GLFW_KEY_UP) == GLFW_PRESS)
    {
        position_m += direction * (deltaTime * speed_m);
    }

    // Move backward
    if (glfwGetKey(window.get(), GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        position_m -= direction * (deltaTime * speed_m);
    }

    // Strafe right
    if (glfwGetKey(window.get(), GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        position_m += right * (deltaTime * speed_m);
    }

    // Strafe left
    if (glfwGetKey(window.get(), GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        position_m -= right * (deltaTime * speed_m);
    }

    // Projection matrix: 45* Field of View, 4:3 ratio, display range:
    projectionMatrix_m = glm::perspective(glm::radians(initialFov_m), 4.0f/3.0f, 0.1f, 50.0f);

    viewMatrix_m = glm::lookAt(
        position_m,
        position_m + direction,
        up
    );

    lastTime = currentTime;
}

glm::mat4 Camera::GetProjectionMatrix() const noexcept
{
    return projectionMatrix_m;
}

glm::mat4 Camera::GetViewMatrix() const noexcept
{
    return viewMatrix_m;
}
