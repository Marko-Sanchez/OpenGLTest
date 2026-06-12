#pragma once

#include "Test.h"

#include <string_view>

#include "gtx/Shaders.h"
#include "gtx/VertexArray.h"
#include "gtx/VertexBuffer.h"

namespace tests
{
class Trivial3DModel final: public Test
{
    private:

        int m_numvertices;

        Shaders      m_shader;
        VertexArray  m_vao;
        VertexBuffer m_vbo;
        VertexBuffer m_uvs;

        glm::mat4 projectionMatrix;
        glm::mat4 modelMatrix;

        bool m_wireFrame;

    public:

        Trivial3DModel(std::shared_ptr<void> window);
        ~Trivial3DModel();

        std::string_view GetName() const override;
        void OnRender() override;
        void OnImGuiRender() override;
};
}// namespace tests
