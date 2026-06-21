#include "Trivial3DModel.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <imgui.h>

#include "glm/trigonometric.hpp"
#include "gtx/ModelLoader.h"

#include <GLFW/glfw3.h>
#include <string_view>

namespace tests
{
namespace
{
    constexpr std::string_view k_TestName        {"3D Model"};
    const std::filesystem::path k_VertexShader   {"res/Shaders/3DModelTrivial.vert"};
    const std::filesystem::path k_FragmentShader {"res/Shaders/3DModelTrivial.frag"};
    const std::filesystem::path k_ModelFile      {"res/3dmodels/boxcar/11813_Box_car_v1_l1.obj"};
}// anonymous namespace

Trivial3DModel::Trivial3DModel(std::shared_ptr<void> window)
    :m_shader(k_VertexShader, k_FragmentShader),
    m_modelLoader(k_ModelFile),
    m_wireFrame(false)
{
    m_shader.CreateShader();
    m_shader.Bind();

    m_modelMatrix = glm::mat4(1.0f);
    // default gives us a top view of the object, rotate on the x-axis so we get a side view of it.
    m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
    m_projectionMatrix = glm::perspective(glm::radians(65.0f), 4.0f / 3.0f, 0.1f, 2000.0f);

    glm::vec3 cameraPosition {0, 0, 1000};

    glm::mat4 viewmatrix = glm::lookAt(
        cameraPosition,// camera position
        glm::vec3(0, 0, 0),// target: where we want the camera to look at.
        glm::vec3(0, 1, 0) // up: what direction is up in our world.
    );
    m_shader.SetUniformMat4f("u_model", m_modelMatrix);
    m_shader.SetUniformMat4f("u_view", viewmatrix);
    m_shader.SetUniformMat4f("u_projection", m_projectionMatrix);

    m_shader.SetUniform3fv("u_viewpos", 1, &cameraPosition.x);

    // enable the storing z-buffer.
    // cull clock wise (winding order) back faces (triangles).
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
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

    m_modelLoader.Draw(m_shader);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Trivial3DModel::OnImGuiRender()
{
    ImGui::Checkbox("wire frame", &m_wireFrame);
}
}// namespace tests
