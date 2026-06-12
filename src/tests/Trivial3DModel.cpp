#include "Trivial3DModel.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>

#include "gtx/VertexBuffer.h"
#include "gtx/ModelLoader.h"
#include "gtx/Debugger.h"
#include "imgui.h"

#include <GLFW/glfw3.h>
#include <string_view>

namespace tests
{
namespace
{
    constexpr std::string_view k_TestName        {"3D File Model"};
    const std::filesystem::path k_VertexShader   {"res/Shaders/3DModelTrivial.vertex"};
    const std::filesystem::path k_FragmentShader {"res/Shaders/3DModelTrivial.fragment"};
    const std::filesystem::path k_ModelFile      {"res/3dmodels/cube.obj"};
}// anonymous namespace

Trivial3DModel::Trivial3DModel(std::shared_ptr<void> window)
    :m_shader(k_VertexShader, k_FragmentShader),
    m_wireFrame(false)
{
    // load shaders.
    m_shader.CreateShader();
    m_shader.Bind();

    // load model.
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    ModelLoader mdl;
    mdl.LoadOBJ(k_ModelFile, vertices, uvs, normals);
    m_numvertices = vertices.size();

    m_vao.Bind();

    m_vbo.Bind();
    m_vbo.CreateBuffer(vertices);
    m_vao.AddAttribute(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));

    m_uvs.Bind();
    m_uvs.CreateBuffer(uvs);
    m_vao.AddAttribute(1, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void*>(0));

    GLCHECKERROR();

    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::perspective(glm::radians(65.0f), 4.0f / 3.0f, 0.1f, 100.0f);

    glm::mat4 viewmatrix = glm::lookAt(
        glm::vec3(0, 0, 3),// position
        glm::vec3(0, 0, 0),// target
        glm::vec3(0, 1, 0) // up
    );
    m_shader.SetUniformMat4f("MVP", glm::mat4(projectionMatrix * viewmatrix * modelMatrix));

    GLCHECKERROR();

    // enable the storing z-buffer.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    // cull clock wise (winding order) back faces (triangles).
    glEnable(GL_CULL_FACE);
}

Trivial3DModel::~Trivial3DModel()
{
    glDepthFunc(GL_LESS);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

std::string_view Trivial3DModel::GetName() const
{
    return k_TestName;
}

void Trivial3DModel::OnRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, m_wireFrame ? GL_LINE : GL_FILL);

    m_shader.Bind();

    const float radius{10.0f};
    float camX = glm::sin(glfwGetTime()) * radius;
    float camY = glm::cos(glfwGetTime()) * radius;
    glm::mat4 view = glm::lookAt(glm::vec3(camX, 0.0f, camY), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    m_shader.SetUniformMat4f("MVP", glm::mat4(projectionMatrix * view * modelMatrix));

    glDrawArrays(GL_TRIANGLES, 0, m_numvertices);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Trivial3DModel::OnImGuiRender()
{
    ImGui::Checkbox("wire frame", &m_wireFrame);
}
}// namespace tests
