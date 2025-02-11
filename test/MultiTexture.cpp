#include "MultiTexture.h"

#include <string>
#include <utility>

#include "GL/glew.h"
#include "imgui/imgui.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"


#include "VertexBufferLayout.h"

namespace tests
{
    MultiTexture::MultiTexture()
    :  m_shader("../res/Shaders/Shader.vertex", "../res/Shaders/Shader.fragment")
    {
        float positions[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, // 0: vertex position (x,y), texture position (a, b)
            0.5f, -0.5f, 1.0f, 0.0f, // 1
            0.5f, 0.5f,  1.0f, 1.0f, // 2
            -0.5f, 0.5f, 0.0f, 1.0f // 3
        };

        unsigned int indexBuffer[] = {
            0, 1, 2,
            2, 3, 0
        };

        VertexBufferLayout vbl;

        m_vertexBuffer.Bind();
        m_vertexBuffer.AddBuffer(8/*a, b*/ + 8/* c, d*/, positions);

        vbl.AddFloat(2); // vertex position.
        vbl.AddFloat(2); // texture position.
        m_vertexArray.AddBuffer(m_vertexBuffer, vbl);

        m_indexBuffer.Bind();
        m_indexBuffer.AddBuffer(6, indexBuffer);

        m_shader.CreateShader();
        m_shader.Bind();

        // in an orthographic projection, objects remain the same size regardless of depth.
        // if we modify the model matrix 'z' variable past (bottom: -1.0f, top: 1.0f) the texture will clip
        // out of existence.
        // https://www.youtube.com/watch?v=V87lLvKscIY (ortho visual example).
        m_projectionMatrix = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, -1.0f, 1.0f);
        // we are not modifying the 'camera' so we create a identy matrix as a placeholder.
        m_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0, 0));

        UpdateTexture("../res/images/cheese.png", glm::vec3(0.5f, 0.5f, 0.0f));
        UpdateTexture("../res/images/cheese.png", glm::vec3(1.0f, 1.0f, 0.0f));
        UpdateTexture("../res/images/nachowink.png", glm::vec3(0.0f, 0.0f, 0.0f), 1);
    }

    void MultiTexture::OnRender()
    {
        m_shader.Bind();

        for(auto translation: m_textureTranslations)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
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
    void MultiTexture::UpdateTexture(const std::string& texture, glm::vec3 translation, int texturePosition)
    {
        m_textureTranslations.push_back(translation);

        // bind() links (glActiveTexture), shader then lets fragment know which texture to use to get
        // pixel coloring from.
        m_textures.emplace_back(texture);
        m_textures.back().Bind(texturePosition);
        m_shader.SetUniform1f("u_Texture", texturePosition);
    }
}
