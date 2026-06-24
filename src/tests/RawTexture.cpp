#include "RawTexture.h"

#include <cstddef>
#include <array>

#include <imgui.h>

namespace tests
{
namespace
{
    constexpr std::string_view k_TestName        {"Raw Texture"};

    const std::filesystem::path k_VertexShader   {"res/Shaders/RawTexture.vert"};
    const std::filesystem::path k_FragmentShader {"res/Shaders/RawTexture.frag"};
    const std::filesystem::path k_Image1         {"res/images/cheese.png"};
    const std::filesystem::path k_Image2         {"res/images/nachowink.png"};

    // values are normalized between [-1, 1].
    constexpr std::array<RawTexture::Vertex, 8> k_VertexArray =
    {{
        {{-0.50f, -0.25f, 0.0f}, {0.0f, 0.0f}, 0},
        {{-0.25f, -0.25f, 0.0f}, {1.0f, 0.0f}, 0},
        {{-0.25f,  0.0f , 0.0f}, {1.0f, 1.0f}, 0},
        {{-0.50f,  0.0f , 0.0f}, {0.0f, 1.0f}, 0},


        {{0.25f, -0.25f, 0.0f}, {0.0f, 0.0f}, 1},
        {{0.50f, -0.25f, 0.0f}, {1.0f, 0.0f}, 1},
        {{0.50f,  0.0f , 0.0f}, {1.0f, 1.0f}, 1},
        {{0.25f,  0.0f , 0.0f}, {0.0f, 1.0f}, 1}
    }};

    constexpr std::array<GLuint, 12> k_IndexBuffer =
    {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4
    };
}// anonymous namespace

RawTexture::RawTexture(std::shared_ptr<void> window)
:m_shader(k_VertexShader, k_FragmentShader)
{
    // VAO must be bound before vbo and ebo are bound.
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // allocate vertex buffer object.
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * k_VertexArray.size(), k_VertexArray.data(), GL_STATIC_DRAW);

    // When a buffer is bound to GL_ELEMENT_ARRAY_BUFFER, all drawing commands of the form gl*Draw*Elements* will use indexes from that buffer.
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * k_IndexBuffer.size(), k_IndexBuffer.data(), GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texturecords)));
    glVertexAttribIPointer(2, 1, GL_INT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, textureid)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Use shader class to parse and compile shader.
    // make sure to compile before setting uniform, else
    // you wont be able to find the uniform in the shader and get
    // a 1282:INVALID_OPERATION error.
    m_shader.CreateShader();
    m_shader.Bind();

    // Add textures.
    int textureSampler[2] = {0, 1};
    m_shader.SetUniform1iv("u_Textures", 2, textureSampler);

    GLuint cheese_texturename = m_texture.UploadTexture("cheese", k_Image1);
    GLuint nacho_texturename  = m_texture.UploadTexture("nacho", k_Image2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cheese_texturename);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, nacho_texturename);
}

RawTexture::~RawTexture()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_ebo);
    glDeleteBuffers(1, &m_vbo);
}

std::string_view RawTexture::GetName() const
{
    return k_TestName;
}

void RawTexture::OnRender()
{
    m_shader.Bind();
    glBindVertexArray(m_vao);

    // since the ebo was bound to vao we can set the last parameter to nullptr.
    glDrawElements(GL_TRIANGLES, k_IndexBuffer.size(), GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}

void RawTexture::OnImGuiRender()
{
    ImGui::TextWrapped("Simple example demonstrating 2D textures.");
}
}// namespace tests
