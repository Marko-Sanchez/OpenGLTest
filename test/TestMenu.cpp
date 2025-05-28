#include "Test.h"
#include "imgui/imgui.h"

namespace tests
{
    TestMenu::TestMenu(std::shared_ptr<void> window, std::shared_ptr<Test>& currentTestPointer)
    : g_window(window), m_currentTest(currentTestPointer)
    {}

    void TestMenu::OnImGuiRender()
    {
        for (auto& [name, test]: m_tests)
        {
            if (ImGui::Button(name.c_str()))
                m_currentTest = test(g_window);
        }
    }
}
