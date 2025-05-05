#include "Trivial3DModel.h"
#include "VertexBuffer.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

#include <iostream>

namespace tests
{

Trivial3DModel::Trivial3DModel()
:m_shader("../res/Shaders/3DModelTrivial.vertex", "../res/Shaders/3DModelTrivial.fragment")
{
    // load shaders.
    m_shader.CreateShader();
    m_shader.Bind();

    // load model.
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    ModelLoader mdl;
    auto res = mdl.LoadOBJ("../res/3dmodels/dog.obj", vertices, uvs, normals);

    m_vao.Bind();

    m_vbo.Bind();
    m_vbo.CreateBuffer(vertices);
    m_vao.AddAttribute(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));

    m_uvs.Bind();
    m_uvs.CreateBuffer(uvs);
    m_vao.AddAttribute(1, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));

    m_numvertices = vertices.size();
    std::cout << "Num Vertices " << m_numvertices << std::endl;
    std::cout << "Num UVs " << uvs.size() << std::endl;

    glm::mat4 model(1.0f);
    /* glm::mat4 viewmatrix = glm::mat4(1.0f); */
    glm::mat4 viewmatrix = glm::lookAt(
        glm::vec3(0, 0, 3),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    );
    glm::mat4 projectionmatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    m_shader.SetUniformMat4f("MVP", glm::mat4(projectionmatrix * viewmatrix * model));


    /* glEnable(GL_DEPTH_TEST); */
    /* glDepthFunc(GL_LESS); */
    /* glEnable(GL_CULL_FACE); */

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Trivial3DModel::OnRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_shader.Bind();

    glDrawArrays(GL_TRIANGLES, 0, m_numvertices);
}

}
