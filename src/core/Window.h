#pragma once

#include <memory>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Core
{
class Window
{
private:

    int _width;
    int _height;
    std::string _title;
    std::shared_ptr<GLFWwindow> _window;

public:

    Window(int width = 1280, int height = 1280, std::string title = "OpenGL Test World");
    ~Window();

    Window(const Window&)            = delete;
    Window(Window&&)                 = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&)      = delete;

    void CreateWindow();
    void DestroyWindow();
    void UpdateBuffers();

    bool IsOpen();
    bool ExitKey();

    std::shared_ptr<GLFWwindow> Get();
};
}
