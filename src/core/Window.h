#pragma once

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Core
{
class Window
{
private:

    std::shared_ptr<GLFWwindow> _window;

public:

    Window() = default;
    ~Window();

    Window(const Window&)            = delete;
    Window(Window&&)                 = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&)      = delete;

    void CreateWindow(int width = 1280, int height = 1280);
    void DestroyWindow();
    void UpdateBuffers();

    bool IsOpen();
    bool ExitKey();

    std::shared_ptr<GLFWwindow> Get();
};
}
