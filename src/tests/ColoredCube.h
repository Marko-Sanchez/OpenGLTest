#pragma once

#include "Test.h"

#include <memory>
#include <string_view>

#include "gtx/Shaders.h"
#include "gtx/VertexArray.h"
#include "gtx/VertexBuffer.h"
#include "gtx/IndexBuffer.h"
#include <GLFW/glfw3.h>

#include "gtx/CameraControls.h"

namespace tests
{
class ColoredCube final: public Test
{
    public:

        struct Vertex
        {
            glm::vec3 position;
            glm::vec4 color;
        };

    private:

        std::shared_ptr<GLFWwindow> m_window;

        Shaders      m_shader;
        VertexArray  m_vao;
        IndexBuffer  m_ibo;
        VertexBuffer m_vbo;
        Camera       m_camera;

        glm::mat4 m_viewMatrix;
        glm::mat4 m_projMatrix;

        float m_lastFrameTime;
        bool  m_wireFrame;

        void ProcessKeyboardInput(float deltaTime);

    public:

        ColoredCube(std::shared_ptr<void> window);
        ~ColoredCube();

        std::string_view GetName() const override;
        void OnRender() override;
        void OnImGuiRender() override;
};
}// namespace tests
