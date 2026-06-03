#include "TestMenu.h"

#include <imgui.h>

namespace tests
{
namespace
{
    constexpr std::string_view k_TestName {"Test Menu"};
}

TestMenu::TestMenu(std::shared_ptr<void> window, std::shared_ptr<Test>& currentTestPointer)
    :g_window(window),
    m_currentTest(currentTestPointer)
{}

std::string_view TestMenu::GetName() const
{
    return k_TestName;
}

void TestMenu::OnImGuiRender()
{
    for (auto& [name, test]: m_tests)
    {
        if (ImGui::Button(name.c_str()))
            m_currentTest = test(g_window);
    }
}
}// namespace tests
