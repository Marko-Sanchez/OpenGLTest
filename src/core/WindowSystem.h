#pragma once

#include <iostream>
#include <GLFW/glfw3.h>

namespace Core
{
/*
 * Intialize GLFW, only needs to be done once for the lifetime of the application.
 */
class WindowSystem
{
public:

    WindowSystem()
    {
        glfwSetErrorCallback([](int error, const char* description)
        {
            std::cerr << "GLFW Error: " << error << ": " << description << std::endl;
        });

        if (!glfwInit())
        {
            throw std::runtime_error("Error intializing GLFW.");
        }
    }

    ~WindowSystem()
    {
        glfwTerminate();
    }

    WindowSystem(const WindowSystem&)            = delete;
    WindowSystem(WindowSystem&&)                 = delete;
    WindowSystem& operator=(const WindowSystem&) = delete;
    WindowSystem& operator=(WindowSystem&&)      = delete;
};
}
