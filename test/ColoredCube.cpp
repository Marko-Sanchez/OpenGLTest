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
    :m_shader("../res/Shaders/Rectangle.vertex", "../res/Shaders/Rectangle.fragment")
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
        m_vao.AddAttribute(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), reinterpret_cast<void*>(sizeof(float) * 3));

        m_ibo.Bind();
        m_ibo.CreateBuffer(indices);


        translationmatrix = glm::vec3(0.0f, 0.0f, 0.0f);

        glm::mat4 modelmatrix = glm::mat4(1.0f);
        camera = glm::vec3(0, 0, 3);
        /* viewmatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); */
        viewmatrix = glm::lookAt(
                                glm::vec3(0,0,3), // Camera is at (4,3,-3), in World Space
                                glm::vec3(0,0,0), // and looks at the origin
                                glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                           );
        projectionmatrix = glm::perspective(glm::radians(45.0f), static_cast<float>(800.0f / 600.0f), 0.1f, 20.0f);
        glm::mat4 mvp = projectionmatrix * viewmatrix * modelmatrix;

        m_shader.CreateShader();
        m_shader.Bind();
        m_shader.SetUniformMat4f("u_MVP", mvp);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }

    void ColoredCube::OnRender()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_shader.Bind();
        m_vao.Bind();
        {
            glm::mat4 modelmatrix = glm::translate(glm::mat4(1.0f), translationmatrix);
            viewmatrix = glm::lookAt(
                                    camera,
                                    glm::vec3(0,0,0), // and looks at the origin
                                    glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                               );
            glm::mat4 mvp = projectionmatrix * viewmatrix * modelmatrix;
            m_shader.SetUniformMat4f("u_MVP", mvp);

            glDrawElements(GL_TRIANGLES, 6 * 6 /*numfaces*/, GL_UNSIGNED_INT, nullptr);
        }
    }

    void ColoredCube::OnImGuiRender()
    {
        ImGui::SliderFloat3("Translation", &translationmatrix.x, -4.0f, 4.0f);
        ImGui::SliderFloat3("Camera", &camera.x, -4.0f, 4.0f);
    }
}
