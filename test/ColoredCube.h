#pragma once

#include "Test.h"


#include "Shaders.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <GLFW/glfw3.h>

#include "CameraControls.h"

#include <memory>

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
