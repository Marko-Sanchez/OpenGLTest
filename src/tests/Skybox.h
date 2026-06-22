#pragma once

#include "Test.h"

#include <string_view>

#include "gtx/Shaders.h"
#include "gtx/Texture.h"
#include "gtx/VertexBuffer.h"
#include "gtx/VertexArray.h"
#include "gtx/CameraControls.h"

namespace tests
{
class Skybox final: public Test
{
    private:

        float m_lastFrameTime;
        float m_lastX;
        float m_lastY;
        bool m_firstMouseMovement;

        std::shared_ptr<GLFWwindow> m_window;

        Texture m_cubeTexture;

        Shaders      m_cubeShader;
        VertexArray  m_cubeVAO;
        VertexBuffer m_cubeVBO;

        Shaders      m_skyboxShader;
        VertexArray  m_skyboxVAO;
        VertexBuffer m_skyboxVBO;

        Camera m_camera;

        void ProcessKeyboardInput(float deltaTime);
        void MouseCallback(GLFWwindow *window, double xposIn, double yposIn);
        void ScrollWheelCallback(GLFWwindow *window, double xposIn, double yposIn);

    public:

        Skybox(std::shared_ptr<void> window);

        std::string_view GetName() const override;
        void OnRender() override;
        void OnImGuiRender() override;
};
}// namespace tests
