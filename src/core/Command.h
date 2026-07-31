#pragma once

#include <string_view>

namespace Core
{
class Command
{
protected:

    Command() = default;

public:

    virtual ~Command() = default;

    virtual void Execute() = 0;
    virtual std::string_view GetLabel() const = 0;
};
}// namespace Core
