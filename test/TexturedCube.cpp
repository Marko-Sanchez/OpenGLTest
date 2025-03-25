#include "TexturedCube.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"

#include <cstddef>
#include <memory>
#include <vector>

namespace tests
{
TexturedCube::TexturedCube()
:m_shader("../res/Shaders/TexturedCube.vertex", "../res/Shaders/TexturedCube.fragment")
{
    /* const std::vector<Vertex> vertices = */
    /* { */
    /*     {{-0.5,  0.5, -5.0}, {0, 1}}, */
    /*     {{-0.5, -0.5, -5.0}, {0, 0}}, */
    /*     {{ 0.5, -0.5, -5.0}, {1, 0}}, */
    /*     {{ 0.5,  0.5, -5.0}, {1, 1}}, */


    /*     {{-0.5,  0.5, -10.0}, {0, 1}}, */
    /*     {{-0.5, -0.5, -10.0}, {0, 0}}, */
    /*     {{ 0.5, -0.5, -10.0}, {1, 0}}, */
    /*     {{ 0.5,  0.5, -10.0}, {1, 1}}, */
    /* }; */

    /* const std::vector<GLuint> indices = */
    /* { */
    /*     // first face */
    /*     0, 1, 2, 2, 3, 0, */
    /*     // back face */
    /*     4, 5, 6, 6, 7, 4, */
    /*     // bottom face */
    /*     1, 2, 6, 6, 5, 1, */
    /*     // top face */
    /*     0, 3, 7, 7, 4, 0, */
    /*     // left-side face */
    /*     0, 4, 5, 5, 1, 0, */
    /*     //right-side face */
    /*     3, 7, 6, 6, 2, 3 */
    /* }; */

    const std::vector<Vertex> vertices =
    {
            {{-0.5, 0.5, 0}, {0, 1}}, {{-0.5, -0.5, 0}, {0, 0}},
            {{0.5, -0.5, 0}, {1, 0}}, {{0.5, 0.5, 0}, {1, 1}}
    };

    const std::vector<unsigned int> indices =
    {
        0, 1, 2, 2, 3, 0
        /* 1, 2, 3, 3, 0, 1 */
    };


    GLuint vaoName;
    glGenVertexArrays(1, &vaoName);
    glBindVertexArray(vaoName);

    GLuint vboName;
    glGenBuffers(1, &vboName);
    glBindBuffer(GL_ARRAY_BUFFER, vboName);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    GLuint iboName;
    glGenBuffers(1, &iboName);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboName);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

    m_vao = vaoName;
    m_vbo = vboName;
    m_ibo = iboName;

    // Define how the data is formatted
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, pos)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tcords)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    /* uptrShader = std::make_unique<Shaders>("../res/Shaders/TexturedCube.vertex", "../res/Shaders/TexturedCube.fragment"); */
    /* uptrShader->CreateShader(); */
    /* uptrShader->Bind(); */
    /* uptrShader->SetUniform1i("text", 1); */
    /* uptrShader->SetUniformMat4f("u_mvp", CalculateMVP()); */

    m_shader.CreateShader();
    m_shader.Bind();
    m_shader.SetUniform1i("text", 1);

    /* uptrTexture = std::make_unique<Texture>(); */
    /* auto tName = uptrTexture->UploadTexture("mc", "../res/images/mc.png", 1); */
    /* uptrTexture->Bind("mc"); */
    m_texture.UploadTexture("dirt", "../res/images/mc.png", 1);
    m_texture.Bind("dirt");

    /* glActiveTexture(GL_TEXTURE1); */
    /* glBindTexture(GL_TEXTURE_2D, tName); */

    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        std::cerr << "OpenGL Error: " << err << std::endl;
    }

    // depth test was doing something to not have square show
    /* glEnable(GL_DEPTH_TEST); */
    /* glDepthFunc(GL_LESS); */
    /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
}

TexturedCube::~TexturedCube()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ibo);

    glDisable(GL_DEPTH_TEST);
}

glm::mat4 TexturedCube::CalculateMVP()
{
    glm::mat4 model(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(0,0,3), glm::vec3(0,0,0), glm::vec3(0,5,0));
    glm::mat4 proj = glm::perspective(glm::radians(65.0f), static_cast<float>(4.0f/3.0f), 0.5f, 100.0f);

    return proj * view * model;
}

void TexturedCube::OnRender()
{
    /* glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); */
    /* glEnable(GL_DEPTH_TEST); */
    /* glDisable(GL_CULL_FACE); */

    /* uptrShader->Bind(); */
    m_shader.Bind();
    glBindVertexArray(m_vao);
    {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }

    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        std::cerr << "OpenGL Error: " << err << std::endl;
    }
}
}// namespace tests
