#include "Skybox.h"
#include "VertexBuffer.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include <GL/gl.h>
#include <vector>
#include <string>

namespace tests
{
Skybox::Skybox(std::shared_ptr<void> window):
m_lastFrameTime(0.0f),
m_firstMouseMovement(true),
m_window(std::reinterpret_pointer_cast<GLFWwindow>(window)),
m_cubeShader("../res/Shaders/SkyboxCube.vertex", "../res/Shaders/SkyboxCube.fragment"),
m_skyboxShader("../res/Shaders/Skybox.vertex", "../res/Shaders/Skybox.fragment")
{
    const std::vector<std::string> faces
    {
        "../res/images/skybox/right.jpg",
        "../res/images/skybox/left.jpg",
        "../res/images/skybox/top.jpg",
        "../res/images/skybox/bottom.jpg",
        "../res/images/skybox/front.jpg",
        "../res/images/skybox/back.jpg"
    };

    const std::vector<float> cubeVertices =
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
    const std::vector<float> skyboxVertices =
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
    // ---- CUBE ----
    m_cubeVAO.Bind();

    m_cubeVBO.Bind();
    m_cubeVBO.CreateBuffer(cubeVertices);

    m_cubeVAO.AddAttribute(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
    m_cubeVAO.AddAttribute(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));

    m_cubeShader.CreateShader();
    m_cubeShader.Bind();

    // ---- SKYBOX ----
    m_skyboxVAO.Bind();

    m_skyboxVBO.Bind();
    m_skyboxVBO.CreateBuffer(skyboxVertices);

    m_skyboxVAO.AddAttribute(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));

    m_skyboxShader.CreateShader();
    m_skyboxShader.Bind();

    m_texture.UploadCubeMap(faces);


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

void Skybox::OnRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw cube.
    m_cubeShader.Bind();
    m_cubeVAO.Bind();
    m_texture.Bind("cubemap");

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
    m_texture.Bind("cubemap");

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
