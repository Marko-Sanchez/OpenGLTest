#pragma once

#include "Test.h"

#include <memory>

#include "gtx/Shaders.h"
#include "gtx/VertexArray.h"
#include "gtx/VertexBuffer.h"
#include "gtx/IndexBuffer.h"
#include <GLFW/glfw3.h>

#include "gtx/CameraControls.h"

namespace tests
{
class ColoredCube: public Test
{
    private:

        float m_lastFrameTime;
        float m_lastX;
        float m_lastY;

        std::shared_ptr<GLFWwindow> m_window;

        Shaders      m_shader;
        VertexArray  m_vao;
        IndexBuffer  m_ibo;
        VertexBuffer m_vbo;
        Camera       m_camera;

        glm::mat4 viewmatrix;
        glm::mat4 m_projectionMatrix;

        bool m_firstMouseMovement;

    public:

        ColoredCube(std::shared_ptr<void> window);
        ~ColoredCube();

        void OnRender() override;
        void ProcessKeyboardInput(float deltaTime);
        void MouseCallback(GLFWwindow *window, double xposIn, double yposIn);
};
}
