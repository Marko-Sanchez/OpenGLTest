#include "ColoredCube.h"

#include "CameraControls.h"
#include "VertexBuffer.h"

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>

#include <vector>

namespace tests
{
ColoredCube::ColoredCube(std::shared_ptr<void> window)
:m_lastFrameTime(0.0f),
 m_window(std::reinterpret_pointer_cast<GLFWwindow>(window)),
 m_shader("../res/Shaders/Cube.vertex", "../res/Shaders/Cube.fragment"),
 m_firstMouseMovement(true)
{
    const std::vector<float> cubevertices =
    {
        // Position (x, y, z)      // Color (r, g, b, a)
        // Front face
        -1.0f, -1.0f, 1.0f,  0.583f,  0.771f,  0.014f, 1.0f,  // 0
         1.0f, -1.0f, 1.0f,  0.609f,  0.115f,  0.436f, 1.0f,  // 1
         1.0f,  1.0f, 1.0f,  0.327f,  0.483f,  0.844f, 1.0f,  // 2
        -1.0f,  1.0f, 1.0f,  0.822f,  0.569f,  0.201f, 1.0f,  // 3

        // Back face
        -1.0f, -1.0f, -1.0f,  0.435f,  0.602f,  0.223f, 1.0f,  // 4
         1.0f, -1.0f, -1.0f,  0.310f,  0.747f,  0.185f, 1.0f,  // 5
         1.0f,  1.0f, -1.0f,  0.597f,  0.770f,  0.761f, 1.0f,  // 6
        -1.0f,  1.0f, -1.0f,  0.559f,  0.436f,  0.730f, 1.0f,  // 7
    };

    const std::vector<unsigned int> indices =
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

    m_vao.Bind();

    m_vbo.Bind();
    m_vbo.CreateBuffer(cubevertices);
    m_vao.AddAttribute(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), reinterpret_cast<void*>(0));
    m_vao.AddAttribute(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));

    m_ibo.Bind();
    m_ibo.CreateBuffer(indices);

    m_shader.CreateShader();
    m_shader.Bind();

    m_projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);


    // avoids the back of the cube showing up when looking at the front.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Camera Controls

    // set mouse callback.
    glfwSetWindowUserPointer(m_window.get(), this);
    glfwSetCursorPosCallback(m_window.get(), [](GLFWwindow* window, double xPosIn, double yPosIn){
        reinterpret_cast<ColoredCube*>(glfwGetWindowUserPointer(window))->MouseCallback(window, xPosIn, yPosIn);
    });

    // tell glfw to capture mouse.
    glfwSetInputMode(m_window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

/*
 * Reset all values to default to avoid messing with other test.
 */
ColoredCube::~ColoredCube()
{
    glDisable(GL_DEPTH_TEST);

    // tell glfw to capture mouse.
    glfwSetInputMode(m_window.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetWindowUserPointer(m_window.get(), nullptr);
    glfwSetCursorPosCallback(m_window.get(), nullptr);

    // TODO: ImGui window no longer responding to mouse interaction.
    ImGui::GetIO().ConfigFlags |= ~ImGuiConfigFlags_NoMouse;
}

void ColoredCube::OnRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader.Bind();
    m_vao.Bind();
    {
        float currentTime{static_cast<float>(glfwGetTime())};
        ProcessKeyboardInput(currentTime - m_lastFrameTime);
        m_lastFrameTime = currentTime;

        glm::mat4 modelmatrix{glm::mat4(1.0f)};
        viewmatrix = m_camera.GetViewMatrix();

        m_shader.SetUniformMat4f("u_MVP", m_projectionMatrix * viewmatrix * modelmatrix);

        glDrawElements(GL_TRIANGLES, 6 * 6 /*numfaces*/, GL_UNSIGNED_INT, nullptr);
    }
}

/*
* Handles Keyboard input.
* @note: may be moved to a new class called camera interface since
*        function generalized.
*/
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

void ColoredCube::MouseCallback(GLFWwindow *window, double xposIn, double yposIn)
{
    float xpos{static_cast<float>(xposIn)};
    float ypos{static_cast<float>(yposIn)};

    if (m_firstMouseMovement)
    {
        m_lastX = xpos;
        m_lastY = ypos;
        m_firstMouseMovement = false;
    }

    float xoffset{xpos - m_lastX};
    float yoffset{m_lastY - ypos};

    m_lastX = xpos;
    m_lastY = ypos;
    m_camera.ProcessMouseMovement(xoffset, yoffset);

    // TODO: may have to remove this not completly sure what this does, was supposed
    // to solve cursor issue with imgui.
    ImGui_ImplGlfw_CursorPosCallback(window, xposIn, yposIn);
}
}
