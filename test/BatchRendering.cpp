#include "BatchRendering.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include "imgui/imgui.h"
#include <iterator>

#include "VertexBufferLayout.h"

namespace tests
{
    BatchRendering::BatchRendering()
    :m_shader("../res/Shaders/Rectangle.vertex", "../res/Shaders/Rectangle.fragment")
    {
        float positions[] =
        {
            // first square.    colors (rgba)
            -1.5f, -0.5f, -10.0f, 0.22f, 1.05f, 0.25f, 1.0f,
            -0.5f, -0.5f, -10.0f, 0.22f, 1.05f, 0.25f, 1.0f,
            -0.5f,  0.5f, -10.0f,  0.22f, 1.05f, 0.25f, 1.0f,
            -1.5f,  0.5f, -10.0f,  0.22f, 1.05f, 0.25f, 1.0f,

            // second square.
            0.5f, -0.5f, -10.0f, 2.27f, 1.93f, 0.0f, 1.0f,
            1.5f, -0.5f, -10.0f, 2.27f, 1.93f, 0.0f, 1.0f,
            1.5f,  0.5f, -10.0f, 2.27f, 1.93f, 0.0f, 1.0f,
            0.5f,  0.5f, -10.0f, 2.27f, 1.93f, 0.0f, 1.0f
        };

        // Indexes help reduce redudant calls to draw vertices that share the same 'position.'
        unsigned int indexes[] =
        {
            0, 1, 2,
            2, 3, 0,

            4, 5, 6,
            6, 7, 4
        };

        VertexBufferLayout vbl;

        m_vb.Bind();
        m_vb.AddBuffer(std::size(positions), positions);
        vbl.AddFloat(3); // positions: 3 floats (x, y, z)
        vbl.AddFloat(4); // color: 4 floats (r, g, b, a)

        m_va.Bind();
        m_va.AddBuffer(m_vb,vbl);

        m_ib.Bind();
        m_ib.AddBuffer(std::size(indexes), indexes);

        // glm::perspective() simulates distance of object to us, making it appear bigger/smaller.
        // first argument, controls the angle at which the camera sees the world; think of it
        // as stretching your hands out to your monitor and making a < symbol.
        // "Zooming in games is often accomplished by decreasing this angle as opposed
        // to moving the camera closer, because it more closely resembles real life."
        // https://open.gl/transformations
        m_translationMatrix = glm::vec3(0.0f, 0.0f, 0.0f);
        m_projectionMatrix = glm::perspective(glm::radians(45.0f), static_cast<float>(800.0f / 600.0f), 0.1f, 20.0f);
        m_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

        m_shader.CreateShader();
        m_shader.Bind();
    }

    void BatchRendering::OnRender()
    {
        m_shader.Bind();
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_translationMatrix);
            glm::mat4 mvp = m_projectionMatrix * m_viewMatrix * model;
            m_shader.SetUniformMat4f("u_MVP", mvp);

            m_va.Bind();
            // batch render both rectangles: 12 = 2 rectangles, each made up of 2 triangles, each triangle has 3 positions/elements; 1 rectangle = 3 elements
            glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
        }
    }

    void BatchRendering::OnImGuiRender()
    {
        ImGui::SliderFloat3("Translation", &m_translationMatrix.x, -4.0f, 4.0f);
    }
}
