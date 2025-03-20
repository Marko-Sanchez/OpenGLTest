#include "ColoredCube.h"

#include "VertexBuffer.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"

#include <imgui/imgui.h>

#include <vector>

namespace tests
{
    ColoredCube::ColoredCube()
    :m_shader("../res/Shaders/Cube.vertex", "../res/Shaders/Cube.fragment")
    {
        const std::vector<float> cubevertices =
        {
            // Position (x, y, z)      // Color (r, g, b, a)
            // Front face
            -1.0f, -1.0f, -9.0f,  0.583f,  0.771f,  0.014f, 1.0f,  // 0
             1.0f, -1.0f, -9.0f,  0.609f,  0.115f,  0.436f, 1.0f,  // 1
             1.0f,  1.0f, -9.0f,  0.327f,  0.483f,  0.844f, 1.0f,  // 2
            -1.0f,  1.0f, -9.0f,  0.822f,  0.569f,  0.201f, 1.0f,  // 3

            // Back face
            -1.0f, -1.0f, -13.0f,  0.435f,  0.602f,  0.223f, 1.0f,  // 4
             1.0f, -1.0f, -13.0f,  0.310f,  0.747f,  0.185f, 1.0f,  // 5
             1.0f,  1.0f, -13.0f,  0.597f,  0.770f,  0.761f, 1.0f,  // 6
            -1.0f,  1.0f, -13.0f,  0.559f,  0.436f,  0.730f, 1.0f,  // 7
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


        translationmatrix = glm::vec3(0.0f, 0.0f, 0.0f);

        cameraPosition = glm::vec3(0, 0, 3);
        cameraTarget   = glm::vec3(0, 0, 0);
        upVector       = glm::vec3(0, 1, 0);

        viewmatrix = glm::lookAt(cameraPosition, cameraTarget, upVector);

        projectionmatrix = glm::perspective(glm::radians(65.0f), static_cast<float>(4.0f / 3.0f), 0.1f, 50.0f);

        glm::mat4 modelmatrix = glm::mat4(1.0f);
        glm::mat4 mvp = projectionmatrix * viewmatrix * modelmatrix;

        m_shader.CreateShader();
        m_shader.Bind();
        m_shader.SetUniformMat4f("u_MVP", mvp);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }

    ColoredCube::~ColoredCube()
    {
        glDisable(GL_DEPTH_TEST);
    }

    void ColoredCube::OnRender()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_shader.Bind();
        m_vao.Bind();
        {
            viewmatrix = glm::lookAt(cameraPosition, cameraTarget, upVector);

            glm::mat4 modelmatrix = glm::translate(glm::mat4(1.0f), translationmatrix);
            glm::mat4 mvp = projectionmatrix * viewmatrix * modelmatrix;
            m_shader.SetUniformMat4f("u_MVP", mvp);

            glDrawElements(GL_TRIANGLES, 6 * 6 /*numfaces*/, GL_UNSIGNED_INT, nullptr);
        }
    }

    void ColoredCube::OnImGuiRender()
    {
        ImGui::SliderFloat3("Translation", &translationmatrix.x, -4.0f, 4.0f);
        ImGui::SliderFloat3("CameraPosition", &cameraPosition.x, -4.0f, 4.0f);
        ImGui::SliderFloat3("CameraTarget", &cameraTarget.x, -4.0f, 4.0f);
        ImGui::SliderFloat3("upVector", &upVector.x, -4.0f, 4.0f);
    }
}
