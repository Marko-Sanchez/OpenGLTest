#pragma once

#include "Command.h"
#include "Receiver.h"

#include <memory>

namespace Core
{
template<class TestType>
class BasicCommand final: public Command
{
private:

    Receiver* _receiver;

public:

    BasicCommand(Receiver* r):
    _receiver(r)
    {}

    void Execute() override
    {
        _receiver->SetActiveTest(std::make_shared<TestType>());
    }
};
}// namespace Core
