#include "ClearColor.h"
#include "GL/glew.h"

#include "imgui/imgui.h"

namespace tests
{
    ClearColor::ClearColor()
    :m_ClearColor{0.2f, 0.3f, 0.8f, 1.0f}
    {}

    void ClearColor::OnRender()
    {
        glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    // Updates color via button.
    void ClearColor::OnImGuiRender()
    {
        ImGui::ColorEdit4("Clear Color", m_ClearColor);
    }
}
