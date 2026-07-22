#pragma once

namespace Core
{
class Command
{
protected:

    Command()          = default;

public:

    virtual ~Command() = default;

    virtual void Execute() = 0;
};
}// namespace Core
