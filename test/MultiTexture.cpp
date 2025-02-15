#include "MultiTexture.h"

#include <iterator>
#include <string>

#include "GL/glew.h"
#include "imgui/imgui.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"


#include "VertexBufferLayout.h"

namespace tests
{
    MultiTexture::MultiTexture()
    :m_shader("../res/Shaders/Multiple.vertex", "../res/Shaders/Multiple.fragment")
    {
        float positions[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0,// 0: vertex position (x,y), texture position (a, b), texIndex
            0.5f, -0.5f, 1.0f, 0.0f,  0,// 1
            0.5f, 0.5f,  1.0f, 1.0f,  0,// 2
            -0.5f, 0.5f, 0.0f, 1.0f,  0,// 3

            0.5f, -0.5f, 0.0f, 0.0f,  1,// 0: vertex position (x,y), texture position (a, b), texIndex
            1.0f, -0.5f, 1.0f, 0.0f,  1,// 1
            1.0f, 0.5f,  1.0f, 1.0f,  1,// 2
            0.5f, 0.5f, 0.0f,  1.0f,  1// 3
        };

        unsigned int indexBuffer[] = {
            0, 1, 2,
            2, 3, 0,

            4, 5, 6,
            6, 7, 4
        };

        VertexBufferLayout vbl;

        m_vertexBuffer.Bind();
        m_vertexBuffer.AddBuffer(std::size(positions), positions);

        vbl.AddFloat(2); // vertex position.
        vbl.AddFloat(2); // texture position.
        vbl.AddInt(1); // texture index.
        m_vertexArray.AddBuffer(m_vertexBuffer, vbl);

        m_indexBuffer.Bind();
        m_indexBuffer.AddBuffer(std::size(indexBuffer), indexBuffer);

        m_shader.CreateShader();
        m_shader.Bind();

        // in an orthographic projection, objects remain the same size regardless of depth.
        // if we modify the model matrix 'z' variable past (bottom: -1.0f, top: 1.0f) the texture will clip
        // out of existence.
        // https://www.youtube.com/watch?v=V87lLvKscIY (ortho visual example).
        m_projectionMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f);
        // we are not modifying the 'camera' so we create a identy matrix as a placeholder.
        m_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0, 0));


        int textureSampler[] = {0 , 1};
        m_shader.SetUniform1iv("u_Textures", 2, textureSampler);

        // TODO: Solve issue; Only one texture displays (cheese) should be showing two different textures.
        AddTexture("cheese", "../res/images/cheese.png", glm::vec3(0.5f, 0.5f, 0.0f), 0);
        AddTexture("nacho", "../res/images/nachowink.png", glm::vec3(0.0f, 0.0f, 0.0f), 1);
    }

    void MultiTexture::OnRender()
    {
        const std::vector<std::string> names = {"cheese", "nacho"};
        m_shader.Bind();

        for(int i{0}; i < m_textureTranslations.size(); ++i)
        {
            m_texture.Bind(names[i]);

            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_textureTranslations[i]);
            glm::mat4 mvp = m_projectionMatrix * m_viewMatrix * model;

            // sends modified mvp value to shader.
            m_shader.SetUniformMat4f("u_MVP", mvp);
            m_renderer.Draw(m_vertexArray, m_shader);
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

    // Adds texture and translation (model) matrix into vectors, to modify on draw calls.
    // bind() links (glActiveTexture), shader then lets fragment know which texture to get
    // pixel coloring from.
    void MultiTexture::AddTexture(const std::string& name, const std::string& texturePath, glm::vec3 translation, int texturePosition)
    {
        m_texture.UploadTexture(name, texturePath, texturePosition);
        m_texture.Bind(name);

        m_textureTranslations.emplace_back(translation);
    }
}
