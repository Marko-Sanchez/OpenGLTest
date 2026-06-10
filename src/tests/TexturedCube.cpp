#include "TexturedCube.h"

#include <iostream>
#include <cmath>
#include <cstddef>
#include <array>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/trigonometric.hpp>

#include <GLFW/glfw3.h>

namespace tests
{
namespace
{
    const std::filesystem::path k_VertexShader   {"res/Shaders/TexturedCube.vertex"};
    const std::filesystem::path k_FragmentShader {"res/Shaders/TexturedCube.fragment"};
    const std::filesystem::path k_Image          {"res/images/mc.png"};

    // higher the -z the further / smaller from the screen the square is.
    constexpr std::array<TexturedCube::Vertex, 24> k_VertexArray =
    {{
        // Front face
        {{-0.5, -0.5,  0.5}, {0.0f, 0.0f}}, // 0
        {{ 0.5, -0.5,  0.5}, {1.0f, 0.0f}}, // 1
        {{ 0.5,  0.5,  0.5}, {1.0f, 1.0f}}, // 2
        {{-0.5,  0.5,  0.5}, {0.0f, 1.0f}}, // 3

        // Back face
        {{ 0.5, -0.5, -0.5}, {0.0f, 0.0f}}, // 4
        {{-0.5, -0.5, -0.5}, {1.0f, 0.0f}}, // 5
        {{-0.5,  0.5, -0.5}, {1.0f, 1.0f}}, // 6
        {{ 0.5,  0.5, -0.5}, {0.0f, 1.0f}}, // 7

        // Left face
        {{-0.5, -0.5, -0.5}, {0.0f, 0.0f}}, // 8
        {{-0.5, -0.5,  0.5}, {1.0f, 0.0f}}, // 9
        {{-0.5,  0.5,  0.5}, {1.0f, 1.0f}}, // 10
        {{-0.5,  0.5, -0.5}, {0.0f, 1.0f}}, // 11

        // Right face
        {{ 0.5, -0.5,  0.5}, {0.0f, 0.0f}}, // 12
        {{ 0.5, -0.5, -0.5}, {1.0f, 0.0f}}, // 13
        {{ 0.5,  0.5, -0.5}, {1.0f, 1.0f}}, // 14
        {{ 0.5,  0.5,  0.5}, {0.0f, 1.0f}}, // 15

        // Top face
        {{-0.5,  0.5,  0.5}, {0.0f, 0.0f}}, // 16
        {{ 0.5,  0.5,  0.5}, {1.0f, 0.0f}}, // 17
        {{ 0.5,  0.5, -0.5}, {1.0f, 1.0f}}, // 18
        {{-0.5,  0.5, -0.5}, {0.0f, 1.0f}}, // 19

        // Bottom face
        {{-0.5, -0.5, -0.5}, {0.0f, 0.0f}}, // 20
        {{ 0.5, -0.5, -0.5}, {1.0f, 0.0f}}, // 21
        {{ 0.5, -0.5,  0.5}, {1.0f, 1.0f}}, // 22
        {{-0.5, -0.5,  0.5}, {0.0f, 1.0f}}, // 23
    }};

    constexpr std::array<unsigned int, 36> k_IndexArray =
    {
        // Front face
        0, 1, 2, 2, 3, 0,

        // Back face
        4, 5, 6, 6, 7, 4,

        // Left face
        8, 9,10,10,11, 8,

        // Right face
       12,13,14,14,15,12,

        // Top face
       16,17,18,18,19,16,

        // Bottom face
       20,21,22,22,23,20
    };
}// anonymous namespace

TexturedCube::TexturedCube(std::shared_ptr<void> window)
:m_shader(k_VertexShader, k_FragmentShader)
{
    m_vao.Bind();
    m_vbo.Bind();
    m_vbo.CreateBuffer(k_VertexArray);

    m_ibo.Bind();
    m_ibo.CreateBuffer(k_IndexArray);

    // Define how the data is formatted
    m_vao.AddAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, pos)));
    m_vao.AddAttribute(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tcords)));

    m_shader.CreateShader();
    m_shader.Bind();

    m_modelMatrix = glm::mat4(1.0f);
    m_projMatrix = glm::perspective(glm::radians(65.0f), static_cast<float>(4.0f/4.0f), 0.1f, 100.0f);

    glm::vec3 cameraPosition (0.0f, 0.0f, 3.0f);
    glm::vec3 cameraTarget   (0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp       (0.0f, 1.0f, 0.0f);

    glm::mat4 viewMatrix = glm::lookAt(cameraPosition, cameraTarget, cameraUp);

    m_shader.SetUniformMat4f("u_mvp", m_projMatrix * viewMatrix * m_modelMatrix);
    m_shader.SetUniform1i("u_texture", 1);

    m_texture.UploadTexture("dirt", k_Image, 1);
    m_texture.Bind("dirt");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    if (GLenum err = glGetError(); err != GL_NO_ERROR)
        std::cerr << "TexturedCube::Construtor() OpenGL Error: " << err << std::endl;
}

void TexturedCube::OnRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader.Bind();
    m_vao.Bind();

    const float radius{10.0f};
    float camX = glm::sin(glfwGetTime()) * radius;
    float camZ = glm::cos(glfwGetTime()) * radius;
    glm::mat4 viewMatrix = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    m_shader.SetUniformMat4f("u_mvp", m_projMatrix * viewMatrix * m_modelMatrix);

    glDrawElements(GL_TRIANGLES, k_IndexArray.size(), GL_UNSIGNED_INT, nullptr);
}
}// namespace tests
