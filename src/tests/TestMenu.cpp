#include "TestMenu.h"

#include <imgui.h>

namespace tests
{
namespace
{
    constexpr std::string_view k_TestName {"Test Menu"};
    constexpr ImVec2 k_ButtonSize         {256, 24};
}// anonymous namespace

TestMenu::TestMenu(std::shared_ptr<void> window, OnTestSelected onTestSelected):
    g_window(window),
    m_onSelectedTest(onTestSelected)
{}

std::string_view TestMenu::GetName() const
{
    return k_TestName;
}

// factory() returns a shared_ptr of the test selected, of which m_onSelectedTest then assigns
// to the current test running in main.cpp
void TestMenu::OnImGuiRender()
{
    for (auto& [name, factory]: m_tests)
    {
        if (ImGui::Button(name.c_str(), k_ButtonSize))
        {
            m_onSelectedTest(factory(g_window));
        }
    }
}
}// namespace tests
