#pragma once

#include "core/Command.h"
#include "core/Test.h"

#include <memory>
#include <list>

namespace tests
{

class TestMenu final: public Test
{
private:

    std::list<std::unique_ptr<Core::Command>> m_commands;

public:

    TestMenu() = default;

    std::string_view GetName() const override;
    void OnImGuiRender() override;

    void MakeCommand(std::unique_ptr<Core::Command> command);
};
}//namespace tests
