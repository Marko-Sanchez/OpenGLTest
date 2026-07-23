#pragma once

#include "core/Window.h"

class Application
{
private:

    std::shared_ptr<Core::Window> m_window;

public:

    Application();
    ~Application();

    Application(const Application&)            = delete;
    Application(Application&&)                 = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&)      = delete;

    void Run();
};
