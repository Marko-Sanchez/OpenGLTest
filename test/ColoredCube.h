#pragma once

#include "Test.h"

#include "Shaders.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace tests
{
    class ColoredCube: public Test
    {
        private:
        Shaders      m_shader;
        VertexArray  m_vao;
        IndexBuffer  m_ibo;
        VertexBuffer m_vbo;

        glm::mat4 viewmatrix;
        glm::mat4 projectionmatrix;
        glm::vec3 translationmatrix;
        glm::vec3 camera;

        public:
        ColoredCube();

        void OnRender() override;
        void OnImGuiRender() override;
    };
}
