#pragma once

#include "Command.h"
#include "Receiver.h"

#include <GLFW/glfw3.h>

namespace Core
{
template<class TestType>
class WindowedCommand final: public Command
{
private:

    Receiver*    _receiver;
    GLFWwindow*  _window;

public:

    WindowedCommand(Receiver* r, GLFWwindow* w):
        _receiver(r),
        _window(w)
    {}

    void Execute() override
    {
        _receiver->SetActiveTest(std::make_shared<TestType>(_window));
    }
};
}// namespace core
