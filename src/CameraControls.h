#pragma once

#include "glm/glm.hpp"
#include "glm/fwd.hpp"

#include <GLFW/glfw3.h>

// Friendlier way to convey where user wants to move camera, avoids invoking window-context in Camera class.
enum CAMERA_MOVEMENT
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

/*
* Camera is a fly style camera allowing us to move around in a 3D scene using the arrow keys
* on the keyboard.
*
* The different vertex coordinates that represent the camera / view space are:
* m_cameraPosition: the position of the camera in 3D space.
* m_cameraFront:
* m_cameraRight:
* m_cameraUp:
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

    // will be a constant (0, 1, 0) unlike m_cameraUp that changes based on right and front.
    glm::vec3 m_worldUp;

    glm::vec3 m_cameraPosition;
    glm::vec3 m_cameraFront; // calculated using pitch and yaw.
    glm::vec3 m_cameraRight; // cross(camerafront, cameraup).
    glm::vec3 m_cameraUp;    // cross(cameraRight, camerafront).

    void UpdateCameraVectors();
public:
    Camera();

    void ProcessKeyboardInput(CAMERA_MOVEMENT cameramovement, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset);

    glm::mat4 GetViewMatrix() const;
};
