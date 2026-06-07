#pragma once

#include "Test.h"

#include <string_view>
#include <array>

#include <glm/fwd.hpp>

#include "gtx/Shaders.h"
#include "gtx/VertexArray.h"

namespace tests
{
// Batch Rendering involves decreasing the calls to glDrawElements(), by increasing how many elements are drawn at the same time.
class BatchRendering final: public Test
{
    public:

        struct Vertex
        {
            std::array<float, 2> position;
            std::array<float, 4> color;
        };

    private:

        Shaders      m_shader;
        VertexArray  m_va;
        unsigned int m_vertexBuffer;
        bool         m_wireFrame;

        std::vector<Vertex> m_vertices;

        void GeneratePositions();

    public:

        BatchRendering(std::shared_ptr<void> window);
        ~BatchRendering();

        std::string_view GetName() const override;
        void OnRender() override;
        void OnImGuiRender() override;
};
}// namespace tests
