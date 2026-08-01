#pragma once

#include "Command.h"
#include "Receiver.h"

#include <GLFW/glfw3.h>
#include <string>

namespace Core
{
template<class TestType>
class WindowedCommand final: public Command
{
private:

    Receiver*    _receiver;
    std::string  _label;
    GLFWwindow*  _window;

public:

    WindowedCommand(Receiver* r, std::string label, GLFWwindow* w):
        _receiver(r),
        _label(std::move(label)),
        _window(w)
    {}

    void Execute() override
    {
        _receiver->SetActiveTest(std::make_shared<TestType>(_window));
    }

    std::string_view GetLabel() const override
    {
        return _label;
    }
};
}// namespace core
