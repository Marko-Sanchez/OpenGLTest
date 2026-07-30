#pragma once

#include "core/Window.h"
#include "core/Test.h"
#include "tests/TestMenu.h"

class Application
{
private:

    Core::Window& m_window;
    std::shared_ptr<tests::Test>     m_activeTest;
    std::shared_ptr<tests::TestMenu> m_menu;

public:

    Application(Core::Window&);
    ~Application() = default;

    Application(const Application&)            = delete;
    Application(Application&&)                 = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&)      = delete;

    void Run();
    void SetActiveTest(std::shared_ptr<tests::Test>);
};
