#pragma once

#include "Command.h"
#include "Receiver.h"

#include <memory>
#include <string>

namespace Core
{
template<class Type>
class BasicCommand final: public Command
{
private:

    Receiver* _receiver;
    std::string _label;

public:

    BasicCommand(Receiver* r, std::string label):
    _receiver(r),
    _label(std::move(label))
    {}

    void Execute() override
    {
        _receiver->SetActiveTest(std::make_shared<Type>());
    }

    std::string_view GetLabel() const override
    {
        return _label;
    }
};
}// namespace Core
