#include "Skybox.h"
#include "VertexBuffer.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

#include <vector>
#include <string>

namespace tests
{
Skybox::Skybox(std::shared_ptr<void> window):
m_lastFrameTime(0.0f),
m_firstMouseMovement(true),
m_window(std::reinterpret_pointer_cast<GLFWwindow>(window)),
m_cubeshader("../res/Shaders/Cubemap.vertex", "../res/Shaders/Cubemap.fragment")
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

    const std::vector<float> vertices =
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
    m_cubeVBA.Bind();

    m_cubeVBO.Bind();
    m_cubeVBO.CreateBuffer(vertices);

    m_cubeVBA.AddAttribute(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));

    m_cubeshader.CreateShader();
    m_cubeshader.Bind();

    m_texture.UploadCubeMap(faces);
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), static_cast<float>(4.0f/4.0f), 0.1f, 100.0f);
    glm::mat4 viewMatrix = glm::lookAt
        (
            glm::vec3(0, 0, 0), // cam position.
            glm::vec3(0, 0, 1),// cam target.
            glm::vec3(0, 1, 0) // up vector.
        );
    m_cubeshader.SetUniformMat4f("u_projection", projectionMatrix);
    m_cubeshader.SetUniformMat4f("u_view", viewMatrix);


    // set mouse callback.
    glfwSetWindowUserPointer(m_window.get(), this);
    glfwSetCursorPosCallback(m_window.get(), [](GLFWwindow* window, double xPosIn, double yPosIn){
        reinterpret_cast<Skybox*>(glfwGetWindowUserPointer(window))->MouseCallback(window, xPosIn, yPosIn);
    });
    // tell glfw to capture mouse.
    glfwSetInputMode(m_window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // TODO: Note once cursor is diabled it is unable to interact with ImGui. fix later.
}

void Skybox::OnRender()
{
    // skybox will always be drawn at the background of all the other objects.
    glDepthMask(GL_FALSE);
    m_cubeshader.Bind();
    m_cubeVBA.Bind();

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
    m_cubeshader.SetUniformMat4f("u_view", glm::mat4(glm::mat3(m_camera.GetViewMatrix())));
    glDrawArrays(GL_TRIANGLES, 0, 36);
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
}
