#pragma once

#include "glm/glm.hpp"
#include "glm/fwd.hpp"

#include <GLFW/glfw3.h>

// Freindlier way to convey where user wants to move camera, avoids invoking window-context in Camera class.
enum CAMERA_MOVEMENT
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

/*
*
* @ref: https://learnopengl.com/Getting-started/Camera
*/
class Camera
{
private:
    float m_yawAngle; // default angle -90.0f points towards -z-axis, value closer to 0 points to +x-axis (right).
    float m_pitchAngle;
    float m_cameraSpeed;
    float m_mouseSensitivty;

    glm::vec3 m_worldUp;

    glm::vec3 m_cameraPosition;
    glm::vec3 m_cameraFront;
    glm::vec3 m_cameraUp;
    glm::vec3 m_cameraRight; // camerafront x cameraup

    void UpdateCameraVectors();
public:
    Camera();

    void ProcessKeyboard(CAMERA_MOVEMENT cameramovement, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset);

    glm::mat4 GetViewMatrix() const;
};
