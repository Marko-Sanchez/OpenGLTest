#include "MultiTexture.h"

#include <GL/gl.h>
#include <array>
#include <cstddef>
#include <filesystem>
#include <string>
#include <string_view>

#include <GL/glew.h>
#include <imgui.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace tests
{
namespace
{
    constexpr std::string_view k_TestName        {"Multi-Texture"};
    const std::filesystem::path k_VertexShader   {"res/Shaders/MultiTexture.vert"};
    const std::filesystem::path k_FragmentShader {"res/Shaders/MultiTexture.frag"};

    const std::filesystem::path k_CheeseImage {"res/images/cheese.png"};
    const std::filesystem::path k_NachoImage  {"res/images/nachowink.png"};
    const std::filesystem::path k_DirtImage   {"res/images/mc.png"};

    constexpr std::array<MultiTexture::Vertex, 12> k_VertexArray =
    {{
        {{-0.5f, -0.5f}, {0.0f, 0.0f}, 0},// 0: vertex position (x,y), texture position (a, b), texIndex
        {{0.5f, -0.5f}, {1.0f, 0.0f},  0},// 1
        {{0.5f, 0.5f},  {1.0f, 1.0f},  0},// 2
        {{-0.5f, 0.5f}, {0.0f, 1.0f},  0},// 3


        {{0.5f, -0.5f}, {0.0f, 0.0f},  1},// 4: vertex position (x,y), texture position (a, b), texIndex
        {{1.0f, -0.5f}, {1.0f, 0.0f},  1},// 5
        {{1.0f, 0.5f},  {1.0f, 1.0f},  1},// 6
        {{0.5f, 0.5f}, {0.0f,  1.0f},  1},// 7

        {{-0.5f, -0.5f}, {0.0f, 0.0f}, 2},// 8: vertex position (x,y), texture position (a, b), texIndex
        {{0.5f, -0.5f}, {1.0f, 0.0f},  2},// 9
        {{0.5f, 0.5f},  {1.0f, 1.0f},  2},// 10
        {{-0.5f, 0.5f}, {0.0f, 1.0f},  2},// 11
    }};

    constexpr std::array<unsigned int, 18> k_IndexArray =
    {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8
    };
}// anonymous namespace

MultiTexture::MultiTexture(std::shared_ptr<void> window)
:m_shader(k_VertexShader, k_FragmentShader)
{
    m_vao.Bind();
    m_vbo.CreateBuffer(k_VertexArray);

    m_ibo.Bind();
    m_ibo.CreateBuffer(k_IndexArray);

    m_vao.AddAttribute(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
    m_vao.AddAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texcords)));
    m_vao.AddIntAttribute(2, 1, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texID)));

    m_shader.CreateShader();
    m_shader.Bind();

    // in an orthographic projection, objects remain the same size regardless of depth.
    // if we modify the model matrix 'z' variable past (bottom: -1.0f, top: 1.0f) the texture will clip
    // out of existence.
    // https://www.youtube.com/watch?v=V87lLvKscIY (ortho visual example).
    m_projectionMatrix = glm::ortho(-4.0f, 4.0f, -4.0f, 4.0f, -1.0f, 1.0f);
    // we are not modifying the 'camera' so we create a identity matrix as a placeholder.
    m_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0, 0));


    int textureSampler[] = {0, 1, 2};
    m_shader.SetUniform1iv("u_Textures", 3, textureSampler);

    m_texture.UploadTexture("cheese", k_CheeseImage);
    m_textureTranslations.emplace_back("cheese", glm::vec3(0.5f, 0.5f, 0.0f));

    m_texture.UploadTexture("nacho", k_NachoImage);
    m_textureTranslations.emplace_back("nacho", glm::vec3(0.0f, 0.0f, 0.0f));

    m_texture.UploadTexture("dirt", k_DirtImage);
    m_textureTranslations.emplace_back("dirt", glm::vec3(0.0f, 0.0f, 0.0f));
}

std::string_view MultiTexture::GetName() const
{
    return k_TestName;
}

void MultiTexture::OnRender()
{
    m_shader.Bind();
    m_vao.Bind();

    // number of vertices to draw.
    const auto texturesCount {3};
    const auto indicesPerTexture {k_IndexArray.size() / texturesCount};

    for(size_t i {0}; i < m_textureTranslations.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        m_texture.Bind(m_textureTranslations[i].name);

        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_textureTranslations[i].translation);
        glm::mat4 mvp   = m_projectionMatrix * m_viewMatrix * model;

        // Model-View-Projection is updated and affects current active texture.
        m_shader.SetUniformMat4f("u_MVP", mvp);

        // Draw one texture at a time, to allow for independent movement of texture to be displayed.
        const auto offset {reinterpret_cast<GLvoid*>(i * indicesPerTexture * sizeof(unsigned int))};
        glDrawElements(GL_TRIANGLES, indicesPerTexture, GL_UNSIGNED_INT, offset);
    }
}

void MultiTexture::OnImGuiRender()
{
    ImGui::TextWrapped("Example demonstrating multiple moveable texture objects.");
    ImGui:;ImGui::NewLine();

    for(auto& [name, translation]: m_textureTranslations)
    {
        ImGui::SliderFloat2(name.c_str(), &translation.x, -1.5f, 1.5f);
    }
}
}// namespace tests
