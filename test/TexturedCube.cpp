#include "TexturedCube.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"

#include <GLFW/glfw3.h>
#include <cmath>
#include <cstddef>
#include <vector>

namespace tests
{
TexturedCube::TexturedCube()
:m_shader("../res/Shaders/TexturedCube.vertex", "../res/Shaders/TexturedCube.fragment")
{
    // higher the -z the further / smaller from the screen the square is.
    const std::vector<Vertex> vertices =
    {
            //front-face
            {{-0.5, -0.5, -5.0}, {0.0f, 0.0f}}, // 0: bottom-left
            {{0.5, -0.5, -5.0}, {1.0f, 0.0f}},  // 1
            {{0.5, 0.5, -5.0}, {1.0f, 1.0f}},   // 2
            {{-0.5, 0.5, -5.0}, {0.0f, 1.0f}},  // 3
            // back-face
            {{-0.5, -0.5, -6.0}, {0.0f, 0.0f}}, // 4: bottom-left
            {{0.5, -0.5, -6.0}, {1.0f, 0.0f}},  // 5
            {{0.5, 0.5, -6.0}, {1.0f, 1.0f}},   // 6
            {{-0.5, 0.5, -6.0}, {0.0f, 1.0f}},  // 7
    };

    const std::vector<unsigned int> indices =
    {
        // front face
        0, 1, 2, 2, 3, 0,
        // back face
        4, 5, 6, 6, 7, 4,
        // bottom face
        0, 1, 5, 5, 4, 0,
        // top face
        3, 2, 6, 6, 7, 3,
        // right face
        1, 5, 6, 6, 2, 1,
        // left face
        0, 4, 7, 7, 3, 0
    };

    m_numIndices = indices.size();

    m_vao.Bind();
    m_vbo.Bind();
    m_vbo.CreateBuffer(vertices);

    m_ibo.Bind();
    m_ibo.CreateBuffer(indices);

    // Define how the data is formatted
    m_vao.AddAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, pos)));
    m_vao.AddAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tcords)));

    m_shader.CreateShader();
    m_shader.Bind();

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), static_cast<float>(4.0f/4.0f), 0.1f, 100.0f);

    glm::vec3 cameraPosition(0.0f, 0.0f, 10.0f);
    glm::vec3 cameraTarget(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);

    glm::mat4 view = glm::lookAt(cameraPosition, cameraTarget, cameraUp);
    m_shader.SetUniformMat4f("u_viewproj", proj * view);
    m_shader.SetUniform1i("u_texture", 1);

    m_texture.UploadTexture("dirt", "../res/images/mc.png", 1);
    m_texture.Bind("dirt");
    m_texture.UnBind();

    glEnable(GL_DEPTH_TEST);
    /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
    /* glEnable(GL_CULL_FACE); */
    /* glFrontFace(GL_CW); */
    /* glCullFace(GL_BACK); */
}

glm::mat4 TexturedCube::CalculateMVP()
{
    float angle{static_cast<float>(glfwGetTime())};

    glm::mat4 model(glm::mat4(1.0f));
    model = glm::rotate(model, glm::radians(angle * 50.0f), glm::vec3(0.0f, 1.0f, 1.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

    return model;
}

void TexturedCube::OnRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader.Bind();
    m_vao.Bind();

    m_texture.Bind("dirt");
    m_shader.SetUniformMat4f("u_model", CalculateMVP());

    glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, nullptr);
    if (GLenum err = glGetError(); err != GL_NO_ERROR)
        std::cerr << "OpenGL Error: " << err << std::endl;
}
}// namespace tests
