#include "Instancing.h"
#include "imgui.h"

#include <array>
#include <cstddef>

#include <GL/glew.h>

namespace tests
{
namespace
{
    constexpr std::string_view k_TestName        {"Instancing"};
    const std::filesystem::path k_VertexShader   {"res/Shaders/Instancing.vert"};
    const std::filesystem::path k_FragmentShader {"res/Shaders/Instancing.frag"};

    constexpr int k_InstanceCount {100};

    constexpr std::array<Instancing::Vertex, 6> k_QuadVertices =
    {{
         {{-0.05f, 0.05f},  {1.0f, 0.0f, 0.0f, 1.0f}},// top-left
         {{-0.05f, -0.05f}, {0.0f, 1.0f, 0.0f, 1.0f}},// bot-left
         {{0.05f, -0.05f},  {0.0f, 1.0f, 0.0f, 1.0f}},// bot-right
         {{0.05f, 0.05f},   {0.0f, 0.0f, 1.0f, 1.0f}},// top-right
    }};

    constexpr std::array<GLuint, 6> k_QuadIndices =
    {
        0, 1, 2, 2, 3, 0
    };
}// anonymous namespace

Instancing::Instancing(std::shared_ptr<void> _)
    :m_shader(k_VertexShader, k_FragmentShader)
{
    glGenVertexArrays(1, &m_quadVAO);
    glBindVertexArray(m_quadVAO);

    glGenBuffers(1, &m_quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * k_QuadVertices.size(), k_QuadVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &m_quadEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, k_QuadIndices.size() * sizeof(GLuint), k_QuadIndices.data(), GL_STATIC_DRAW);

    this->GenerateTranslations();

    // instance buffer.
    glGenBuffers(1, &m_instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Instances) * m_translations.size(), m_translations.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Instances), reinterpret_cast<void*>(offsetof(Instances, translation)));
    glEnableVertexAttribArray(2);
    // (1) Tell GPU to advance to the next vec2, in the buffer, once per 'instance' and not per vertex.
    // This ensures all 4 vertices of 'this'(current quad/instance being drawn) quad share the same translation (vec2).
    // esentially converting a vertex attribute to a instance attribute.
    glVertexAttribDivisor(2, 1);

    m_shader.CreateShader();
    m_shader.Bind();
}

Instancing::~Instancing()
{
    glDeleteVertexArrays(1, &m_quadVAO);
    glDeleteBuffers(1, &m_quadEBO);
    glDeleteBuffers(1, &m_quadVBO);
    glDeleteBuffers(1, &m_instanceVBO);
}

void Instancing::GenerateTranslations()
{
    m_translations.reserve(k_InstanceCount);

    const float offset {0.1f};
    for (int y {-10}; y < 10; y +=2)
    {
        for (int x {-10}; x < 10; x +=2)
        {
            m_translations.push_back(Instances{glm::vec2{static_cast<float>(x) / 10.0f + offset, static_cast<float>(y) / 10.0f + offset}});
        }
    }
}

std::string_view Instancing::GetName() const
{
    return k_TestName;
}

void Instancing::OnRender()
{
    m_shader.Bind();
    glBindVertexArray(m_quadVAO);
    // (1) Draw the quad k_InstanceCount times in a single call, using different translations from the instance buffer (instanceVBO).
    glDrawElementsInstanced(GL_TRIANGLES, k_QuadIndices.size(), GL_UNSIGNED_INT, nullptr, k_InstanceCount);
    glBindVertexArray(0);
}

void Instancing::OnImGuiRender()
{
    ImGui::TextWrapped
        (
         "Instancing helps when you have lots of models "
          "sharing the same set of vertex data "
          "but with different world translations / positions."
        );
}
}// namespace tests
