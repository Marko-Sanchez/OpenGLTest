#include "ClearColor.h"

#include <GL/glew.h>
#include <imgui.h>

namespace tests
{
namespace
{
    constexpr std::string_view k_TestName {"Clear Color"};
}// anonymous namespace

ClearColor::ClearColor(std::shared_ptr<void> window)
:m_clearColor{0.2f, 0.3f, 0.8f, 1.0f}
{}

std::string_view ClearColor::GetName() const
{
    return k_TestName;
}

void ClearColor::OnRender()
{
    glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]);
    glClear(GL_COLOR_BUFFER_BIT);
}

void ClearColor::OnImGuiRender()
{
    ImGui::ColorEdit4("Color Picker", m_clearColor.data());
}
}// namespace tests
