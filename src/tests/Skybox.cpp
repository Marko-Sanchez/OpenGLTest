#include "Skybox.h"

#include <filesystem>
#include <string_view>
#include <array>
#include <string>

#include "gtx/Texture.h"
#include "gtx/VertexBuffer.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <imgui.h>

namespace tests
{
namespace
{
    constexpr std::string_view k_TestName   {"Sky Box"};
    constexpr std::string_view k_SkyBoxName {"skybox"};

    const std::filesystem::path k_CubeVertexShader     {"res/Shaders/SkyboxCube.vert"};
    const std::filesystem::path k_CubeFragmentShader   {"res/Shaders/SkyboxCube.frag"};
    const std::filesystem::path k_SkyBoxVertexShader   {"res/Shaders/Skybox.vert"};
    const std::filesystem::path k_SkyBoxFragmentShader {"res/Shaders/Skybox.frag"};

    // note: the order matters, since texture targets are mapped to orientation and then incremented
    // by one in loop: such that, GL_TEXTURE_CUBE_MAP_POSITIVE_X == right.jpg, etc.
    const std::array<std::filesystem::path, 6> k_CubeFaces
    {
        "res/images/skybox/right.jpg",
        "res/images/skybox/left.jpg",
        "res/images/skybox/top.jpg",
        "res/images/skybox/bottom.jpg",
        "res/images/skybox/front.jpg",
        "res/images/skybox/back.jpg"
    };

    constexpr std::array<float, 216> k_CubeVertices
    {
        // positions          // normals
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    constexpr std::array<float, 108> k_SkyBoxVertices
    {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
}// anonymous namespace

Skybox::Skybox(std::shared_ptr<void> window)
    :m_lastFrameTime(0.0f),
    m_firstMouseMovement(true),
    m_window(std::reinterpret_pointer_cast<GLFWwindow>(window)),
    m_cubeShader( k_CubeVertexShader, k_CubeFragmentShader),
    m_skyboxShader( k_SkyBoxVertexShader, k_SkyBoxFragmentShader)
{
    // ---- CUBE ----
    m_cubeVAO.Bind();

    m_cubeVBO.Bind();
    m_cubeVBO.CreateBuffer(k_CubeVertices);

    m_cubeVAO.AddAttribute(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
    m_cubeVAO.AddAttribute(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));

    m_cubeShader.CreateShader();
    m_cubeShader.Bind();

    // ---- SKYBOX ----
    m_skyboxVAO.Bind();

    m_skyboxVBO.Bind();
    m_skyboxVBO.CreateBuffer(k_SkyBoxVertices);

    m_skyboxVAO.AddAttribute(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));

    m_skyboxShader.CreateShader();
    m_skyboxShader.Bind();

    m_cubeTexture.UploadCubeMap(k_SkyBoxName.data(), k_CubeFaces);

    // TODO: Note once cursor is diabled it is unable to interact with ImGui. fix later.
    // set mouse callback.
    glfwSetWindowUserPointer(m_window.get(), this);
    glfwSetCursorPosCallback(m_window.get(), [](GLFWwindow* window, double xPosIn, double yPosIn)
    {
        reinterpret_cast<Skybox*>(glfwGetWindowUserPointer(window))->MouseCallback(window, xPosIn, yPosIn);
    });
    glfwSetScrollCallback(m_window.get(), [](GLFWwindow* window, double xPosIn, double yPosIn)
    {
        reinterpret_cast<Skybox*>(glfwGetWindowUserPointer(window))->ScrollWheelCallback(window, xPosIn, yPosIn);
    });
    // tell glfw to capture mouse.
    glfwSetInputMode(m_window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glEnable(GL_DEPTH_TEST);
}

std::string_view Skybox::GetName() const
{
    return k_TestName;
}

void Skybox::OnRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw cube.
    m_cubeShader.Bind();
    m_cubeVAO.Bind();

    glActiveTexture(GL_TEXTURE0);
    m_cubeTexture.Bind("cubemap", GL_TEXTURE_CUBE_MAP);

    m_cubeShader.SetUniformMat4f("u_model", glm::mat4(1.0f));
    m_cubeShader.SetUniformMat4f("u_view", m_camera.GetViewMatrix());
    m_cubeShader.SetUniformMat4f("u_projection", glm::perspective(glm::radians(m_camera.GetZoom()), static_cast<float>(4.0f/4.0f), 0.1f, 100.0f));
    m_cubeShader.SetUniform3fv("u_cameraPos", 1, glm::value_ptr(m_camera.GetCameraPos()));

    glDrawArrays(GL_TRIANGLES, 0, 36);

    // skybox will always be drawn at the background of all the other objects.
    // No longer needed as we draw the skybox last.
    /* glDepthMask(GL_FALSE); */

    // sky box depth buffer is 1, we only have to render skybox fragments where the early depth buffer passes.
    // since the max depth buffer value is 1 and we set it to 1 in the vertex buffer we use GL_LEQUAL.
    glDepthFunc(GL_LEQUAL);
    m_skyboxShader.Bind();
    m_skyboxVAO.Bind();

    float currentTime{static_cast<float>(glfwGetTime())};
    ProcessKeyboardInput(currentTime - m_lastFrameTime);
    m_lastFrameTime = currentTime;

    /*
        | R R R Tx |
        | R R R Ty |
        | R R R Tz |
        | 0 0 0  1 |
     */
    // remove translation section (upper-left) of view matrix.
    m_skyboxShader.SetUniformMat4f("u_view", glm::mat4(glm::mat3(m_camera.GetViewMatrix())));
    m_skyboxShader.SetUniformMat4f("u_projection", glm::perspective(glm::radians(m_camera.GetZoom()), static_cast<float>(4.0f/4.0f), 0.1f, 100.0f));
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthFunc(GL_LESS);
}

/*
* Handles Keyboard input.
* @note: may be moved to a new class called camera interface since
*        function generalized.
*/
void Skybox::ProcessKeyboardInput(float deltaTime)
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

/*
* Handles mouse position, takes the difference from last position to change
* camera view matrix pitch and yaw.
*/
void Skybox::MouseCallback(GLFWwindow *window, double xposIn, double yposIn)
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
}

/*
* Handles Zoom / FOV via scroll wheel callback.
*/
void Skybox::ScrollWheelCallback(GLFWwindow *window, double xposIn, double yposIn)
{
    m_camera.ProcessMouseScroll(static_cast<float>(yposIn));
}
}// namespace tests
