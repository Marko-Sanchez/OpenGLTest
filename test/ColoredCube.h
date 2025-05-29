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
        Shaders      m_shader;
        VertexArray  m_vao;
        IndexBuffer  m_ibo;
        VertexBuffer m_vbo;
        Camera       m_camera;

        std::shared_ptr<GLFWwindow> m_window;

        glm::mat4 viewmatrix;
        glm::mat4 projectionmatrix;
        glm::vec3 translationmatrix;

        glm::vec3 cameraPosition;
        glm::vec3 cameraTarget;
        glm::vec3 upVector;

        public:
        ColoredCube(std::shared_ptr<void> window);
        ~ColoredCube();

        void OnRender() override;
        void OnImGuiRender() override;
    };
}
