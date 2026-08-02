#pragma once

#include "core/Command.h"
#include "core/Test.h"

#include <memory>
#include <string>
#include <list>

namespace tests
{

class TestMenu final: public Test
{
private:

    struct Entry
    {
        std::string label;
        std::unique_ptr<Core::Command> command;
        Entry(std::string l, std::unique_ptr<Core::Command> c):
            label(std::move(l)),
            command(std::move(c))
        {}
    };

    std::list<Entry> m_commands;

public:

    TestMenu() = default;

    std::string_view GetName() const override;
    void OnImGuiRender() override;

    void AddCommand(std::string name, std::unique_ptr<Core::Command> command);
};
}//namespace tests
