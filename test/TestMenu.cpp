#include "Test.h"
#include "imgui/imgui.h"

namespace tests
{
    TestMenu::TestMenu(std::shared_ptr<Test>& currentTestPointer)
    : m_currentTest(currentTestPointer)
    {}

    void TestMenu::OnImGuiRender()
    {
        for (auto& test: m_tests)
        {
            if (ImGui::Button(test.first.c_str()))
                m_currentTest = test.second();
        }
    }
}
