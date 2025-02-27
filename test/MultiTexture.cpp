#include "MultiTexture.h"

#include <cstddef>
#include <iterator>
#include <string>

#include "GL/glew.h"
#include "imgui/imgui.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

namespace tests
{
    MultiTexture::MultiTexture()
    :m_shader("../res/Shaders/Multiple.vertex", "../res/Shaders/Multiple.fragment")
    {
        const std::vector<Vertex> vec =
        {
            {{-0.5f, -0.5f}, {0.0f, 0.0f}, 0},// 0: vertex position (x,y), texture position (a, b), texIndex
            {{0.5f, -0.5f}, {1.0f, 0.0f},  0},// 1
            {{0.5f, 0.5f},  {1.0f, 1.0f},  0},// 2
            {{-0.5f, 0.5f}, {0.0f, 1.0f},  0},// 3


            {{0.5f, -0.5f}, {0.0f, 0.0f},  1},// 0: vertex position (x,y), texture position (a, b), texIndex
            {{1.0f, -0.5f}, {1.0f, 0.0f},  1},// 1
            {{1.0f, 0.5f},  {1.0f, 1.0f},  1},// 2
            {{0.5f, 0.5f}, {0.0f,  1.0f},  1}// 3
        };

        const std::vector<unsigned int> indexBuffer =
        {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4
        };

        m_vao.Bind();
        m_vbo.CreateBuffer(vec);

        m_ibo.Bind();
        m_ibo.CreateBuffer(indexBuffer);

        m_vao.AddAttribute(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
        m_vao.AddAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texcords)));
        m_vao.AddIntAttribute(2, 1, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texID)));

        m_shader.CreateShader();
        m_shader.Bind();

        // in an orthographic projection, objects remain the same size regardless of depth.
        // if we modify the model matrix 'z' variable past (bottom: -1.0f, top: 1.0f) the texture will clip
        // out of existence.
        // https://www.youtube.com/watch?v=V87lLvKscIY (ortho visual example).
        m_projectionMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f);
        // we are not modifying the 'camera' so we create a identity matrix as a placeholder.
        m_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0, 0));


        int textureSampler[] = {0 , 1};
        m_shader.SetUniform1iv("u_Textures", 2, textureSampler);

        m_texture.UploadTexture("cheese", "../res/images/cheese.png", 0);
        m_texture.Bind("cheese");
        m_textureTranslations.emplace_back(glm::vec3(0.5f, 0.5f, 0.0f));

        m_texture.UploadTexture("nacho", "../res/images/nachowink.png", 1);
        m_texture.Bind("nacho");
        m_textureTranslations.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f));
    }

    void MultiTexture::OnRender()
    {
        m_shader.Bind();
        m_vao.Bind();

        const std::vector<std::string> names = {"cheese", "nacho"};
        for(int i{0}; i < m_textureTranslations.size(); ++i)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_textureTranslations[i]);
            glm::mat4 mvp = m_projectionMatrix * m_viewMatrix * model;

            m_texture.Bind(names[i]);

            // sends modified mvp value to shader.
            m_shader.SetUniformMat4f("u_MVP", mvp);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(i * 6 * sizeof(unsigned int)));
        }
    }

    void MultiTexture::OnImGuiRender()
    {
        std::string name{"translation A"};
        for(int i {0}; i < m_textureTranslations.size(); ++i)
        {
            name.back() = static_cast<char>('A' + i);
            ImGui::SliderFloat2(name.c_str(), &m_textureTranslations[i].x, -1.5f, 1.5f);
        }
    }
}
