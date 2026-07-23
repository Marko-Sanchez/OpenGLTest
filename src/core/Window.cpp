#include "Window.h"

#include <iostream>
#include <stdexcept>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

namespace Core
{
Window::~Window()
{
    if (_window)
    {
        DestroyWindow();
    }
}

void Window::CreateWindow(int width, int height)
{
    if (_window)
    {
        throw std::runtime_error("Window already created.");
    }

    glfwSetErrorCallback([](int error, const char* description)
    {
        std::cerr << "GLFW Error: " << error << ": " << description << std::endl;
    });


    if (!glfwInit())
    {
        throw std::runtime_error("Error intializing GLFW.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window-mode window and its OpenGL context.
    std::shared_ptr<GLFWwindow> window
    (
        glfwCreateWindow(width, height, "OpenGL Test World", NULL, NULL),
        glfwDestroyWindow
    );

    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create window.");
    }
    else
    {
        // Make the window's context current
        glfwMakeContextCurrent(window.get());
        glfwSwapInterval(1);
    }

    if (GLenum glewError = glewInit(); GLEW_OK != glewError)
    {
        window.reset();
        glfwTerminate();
        throw std::runtime_error("glew not initialized.");
    }
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    // tell OpenGL the size of the rendering window so OpenGL knows
    // how we want to display the data and coordinates with respect to the window
    // (0, 0) -> bottom-left coordinates.
    glViewport(0, 0, width, height);
    // if user resizes window update viewport.
    auto windowResizeCallback = [](GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    };
    glfwSetFramebufferSizeCallback(window.get(), windowResizeCallback);

    // enable blending of the alpha value (rgba: controls transparency) and how it should be blended;
    // artifacts appear if not enabled on transparent textures.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    _window = window;
}

void Window::DestroyWindow()
{
    _window.reset();
    glfwTerminate();
}

void Window::UpdateBuffers()
{
    glfwSwapBuffers(_window.get());
    glfwPollEvents();
}

bool Window::IsOpen()
{
    return !glfwWindowShouldClose(_window.get());
}

bool Window::ExitKey()
{
    return glfwGetKey(_window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS;
}

std::shared_ptr<GLFWwindow> Window::Get()
{
    return _window;
}
}// namespace Core
