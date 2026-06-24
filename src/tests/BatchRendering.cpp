#include "BatchRendering.h"

#include <random>
#include <string_view>
#include <filesystem>
#include <array>

#include <glm/ext/matrix_clip_space.hpp>
#include <imgui.h>

namespace tests
{
namespace
{
    constexpr std::string_view k_TestName        {"Batch Rendering"};
    const std::filesystem::path k_VertexShader   {"res/Shaders/BatchRendering.vert"};
    const std::filesystem::path k_FragmentShader {"res/Shaders/BatchRendering.frag"};

    std::array<float, 4> RandomColor()
    {
        static std::mt19937 s_engine {std::random_device{}()};
        static std::uniform_real_distribution<float> s_dist{0.0f, 1.0f};
        const float alpha {1.0f};

        return {s_dist(s_engine), s_dist(s_engine), s_dist(s_engine), alpha};
    }
}// anonymous namespace

BatchRendering::BatchRendering(std::shared_ptr<void> window)
    :m_shader(k_VertexShader, k_FragmentShader),
    m_wireFrame(false)
{
    // Could be any size, but should be large enough to fit desired data; otherwise, will need
    // way to track when store reaches capacity to flush data (draw).
    const float bufferStoreSize {16/*Rows*/ * 8/*Cols*/ * 6/*# Vertices in Rect.*/};
    m_vertices.reserve(bufferStoreSize);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);// vao is now active.

    // generate vertex buffer and allocate memory (1).
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);// vbo is now active on GL_ARRAY_BUFFER.
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * bufferStoreSize, nullptr, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
    glEnableVertexAttribArray(1);

    this->GeneratePositions();

    m_shader.CreateShader();
    m_shader.Bind();

    // ortho helps normalize our data between [-1,1]; (0, 0) -> (-1, -1)
    m_shader.SetUniformMat4f("u_MVP", glm::ortho(0.f, 512.f, 0.f, 1024.f, -1.f, 1.f));

    glBindVertexArray(0);
}

BatchRendering::~BatchRendering()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
}

std::string_view BatchRendering::GetName() const
{
    return k_TestName;
}

/*
 * Without batch rendering we would/might have had to make a draw call for every rectangle created, which would
 * be expensive. Batch-rendering avoids making draw calls by creating a large buffer (1) and making a singular call (2)
 * for the entire buffer that could contain multiple rectangles (3).
 */
void BatchRendering::OnRender()
{
    glPolygonMode(GL_FRONT_AND_BACK, m_wireFrame ? GL_LINE: GL_FILL);

    m_shader.Bind();
    glBindVertexArray(m_vao);

    // Batch Render all the vertices (2).
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

    glBindVertexArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void BatchRendering::OnImGuiRender()
{
    ImGui::Checkbox("View Vertices", &m_wireFrame);
}

void BatchRendering::GeneratePositions()
{
    // (3).
    const size_t totalRows {16};
    const size_t totalCols {8};
    const float  size      {64};

    float yPosition {};
    for (size_t row {0}; row < totalRows; ++row)
    {
        float xPosition {};
        for (size_t col {0}; col < totalCols; ++col)
        {
            const auto color {RandomColor()};

            m_vertices.push_back({{xPosition, yPosition}, color});
            m_vertices.push_back({{xPosition, yPosition + size}, color});
            m_vertices.push_back({{xPosition + size, yPosition + size}, color});

            m_vertices.push_back({{xPosition, yPosition}, color});
            m_vertices.push_back({{xPosition + size, yPosition}, color});
            m_vertices.push_back({{xPosition + size, yPosition + size}, color});

            xPosition += size;
        }
        yPosition += size;
    }

    // remember to bind vbo (if not done so already).
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    // updates a subset of buffer object store in this case 0 -> size of m_vertices<Vertex>
    // buffer object store should be big enough to fit this data, else error is thrown.
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * m_vertices.size(), m_vertices.data());
}
}// namespace tests
