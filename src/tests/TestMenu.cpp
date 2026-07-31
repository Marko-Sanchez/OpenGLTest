#include "TestMenu.h"

#include "core/Command.h"

#include <imgui.h>

namespace tests
{
namespace
{
    constexpr std::string_view k_TestName {"Test Menu"};
    constexpr ImVec2 k_ButtonSize         {256, 24};
}// anonymous namespace

std::string_view TestMenu::GetName() const
{
    return k_TestName;
}

void TestMenu::OnImGuiRender()
{
    for (auto& command: m_commands)
    {
        if (ImGui::Button(command->GetLabel().data(), k_ButtonSize))
        {
            command->Execute();
        }
    }
}

void TestMenu::MakeCommand(std::unique_ptr<Core::Command> command)
{
    m_commands.push_back(std::move(command));
}
}// namespace tests
