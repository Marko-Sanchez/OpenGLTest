#pragma once

#include "Shaders.h"
#include "Test.h"

#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "CameraControls.h"

namespace tests
{
class Skybox: public Test
{
    private:
    float m_lastFrameTime;
    float m_lastX;
    float m_lastY;
    bool m_firstMouseMovement;

    std::shared_ptr<GLFWwindow> m_window;

    Shaders m_skyboxShader;
    Texture m_texture;
    VertexArray m_cubeVBA;
    VertexBuffer m_cubeVBO;

    Camera m_camera;

    public:
    Skybox(std::shared_ptr<void> window);

    void OnRender() override;

    void ProcessKeyboardInput(float deltaTime);
    void MouseCallback(GLFWwindow *window, double xposIn, double yposIn);

};
}
