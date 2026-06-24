#pragma once

#include "Test.h"

#include <string_view>
#include <array>
#include <vector>

#include <glm/fwd.hpp>
#include <GL/glew.h>

#include "gtx/Shaders.h"

namespace tests
{
// Batch Rendering involves decreasing the calls to glDrawElements(), by increasing how many elements are drawn at the same time.
class BatchRendering final: public Test
{
    private:

        struct Vertex
        {
            std::array<float, 2> position;
            std::array<float, 4> color;
        };

        Shaders m_shader;
        GLuint  m_vao;
        GLuint  m_vbo;
        bool    m_wireFrame;

        std::vector<Vertex> m_vertices;

        void GeneratePositions();

    public:

        BatchRendering(std::shared_ptr<void> window);
        ~BatchRendering();

        BatchRendering(const BatchRendering&)            = delete;
        BatchRendering& operator=(const BatchRendering&) = delete;
        BatchRendering(BatchRendering&&)                 = delete;
        BatchRendering& operator=(BatchRendering&&)      = delete;

        std::string_view GetName() const override;
        void OnRender() override;
        void OnImGuiRender() override;
};
}// namespace tests
