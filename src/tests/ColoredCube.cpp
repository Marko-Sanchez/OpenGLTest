#include "ColoredCube.h"

#include <array>
#include <string_view>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "gtx/CameraControls.h"
#include "gtx/VertexBuffer.h"

#include <GLFW/glfw3.h>

namespace tests
{
namespace
{
    constexpr std::string_view k_TestName        {"Colored Cube"};
    const std::filesystem::path k_VertexShader   {"res/Shaders/Cube.vertex"};
    const std::filesystem::path k_FragmentShader {"res/Shaders/Cube.fragment"};

    constexpr std::array<ColoredCube::Vertex, 8> k_VertexArray =
    {{
        // Position (x, y, z)      // Color (r, g, b, a)
        // Front face
         {{-1.0f, -1.0f, 1.0f},  {0.583f,  0.771f,  0.014f, 1.0f}},  // 0
         {{1.0f, -1.0f, 1.0f},  {0.609f,  0.115f,  0.436f, 1.0f}},  // 1
         {{1.0f,  1.0f, 1.0f},  {0.327f,  0.483f,  0.844f, 1.0f}},  // 2
         {{-1.0f,  1.0f, 1.0f},  {0.822f,  0.569f,  0.201f, 1.0f}},  // 3

        // Back face
         {{-1.0f, -1.0f, -1.0f},  {0.435f,  0.602f,  0.223f, 1.0f}},  // 4
         {{1.0f, -1.0f, -1.0f},  {0.310f,  0.747f,  0.185f, 1.0f}},  // 5
         {{1.0f,  1.0f, -1.0f},  {0.597f,  0.770f,  0.761f, 1.0f}},  // 6
         {{-1.0f,  1.0f, -1.0f},  {0.559f,  0.436f,  0.730f, 1.0f}}  // 7
    }};

    constexpr std::array<unsigned int, 36> k_IndexArray =
    {
        // Front face
        0, 1, 2,  2, 3, 0,
        // Back face
        4, 5, 6,  6, 7, 4,
        // Left face
        0, 3, 7,  7, 4, 0,
        // Right face
        1, 2, 6,  6, 5, 1,
        // Top face
        3, 2, 6,  6, 7, 3,
        // Bottom face
        0, 1, 5,  5, 4, 0
    };
}// anonymous namespace

ColoredCube::ColoredCube(std::shared_ptr<void> window)
    :m_window(std::reinterpret_pointer_cast<GLFWwindow>(window)),
    m_shader(k_VertexShader, k_FragmentShader),
    m_lastFrameTime(0.f),
    m_wireFrame(false)
{
    m_vao.Bind();

    m_vbo.Bind();
    m_vbo.CreateBuffer(k_VertexArray);
    m_vao.AddAttribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
    m_vao.AddAttribute(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));

    m_ibo.Bind();
    m_ibo.CreateBuffer(k_IndexArray);

    m_shader.CreateShader();
    m_shader.Bind();

    m_projMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

    // avoids the back of the cube showing up when looking at the front.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

/*
 * Reset all values to default to avoid messing with other test.
 */
ColoredCube::~ColoredCube()
{
    glDisable(GL_DEPTH_TEST);
}

std::string_view ColoredCube::GetName() const
{
    return k_TestName;
}

void ColoredCube::OnRender()
{
    auto currentTime {static_cast<float>(glfwGetTime())};
    auto modelmatrix {glm::mat4(1.0f)};

    // rotate on xy-axis.
    modelmatrix = glm::rotate(modelmatrix, currentTime, glm::vec3(1.f, 1.f, 0.f));

    this->ProcessKeyboardInput(currentTime - m_lastFrameTime);

    m_lastFrameTime = currentTime;
    m_viewMatrix    = m_camera.GetViewMatrix();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, m_wireFrame ? GL_LINE : GL_FILL);

    m_shader.Bind();
    m_vao.Bind();

    m_shader.SetUniformMat4f("u_MVP", m_projMatrix * m_viewMatrix * modelmatrix);
    glDrawElements(GL_TRIANGLES, k_IndexArray.size(), GL_UNSIGNED_INT, nullptr);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ColoredCube::OnImGuiRender()
{
    ImGui::Text("Cube can be moved using arrow keys.\n\n");
    ImGui::Checkbox("View Vertices", &m_wireFrame);
}

void ColoredCube::ProcessKeyboardInput(float deltaTime)
{
    if (glfwGetKey(m_window.get(), GLFW_KEY_UP) == GLFW_PRESS)
    {
        m_camera.ProcessKeyboardInput(FORWARD, deltaTime);
    }
    if (glfwGetKey(m_window.get(), GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        m_camera.ProcessKeyboardInput(BACKWARD, deltaTime);
    }
    if (glfwGetKey(m_window.get(), GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        m_camera.ProcessKeyboardInput(RIGHT, deltaTime);
    }
    if (glfwGetKey(m_window.get(), GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        m_camera.ProcessKeyboardInput(LEFT, deltaTime);
    }
}
}// namespace tests
