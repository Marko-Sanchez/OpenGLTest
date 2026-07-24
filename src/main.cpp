#include "app/Application.h"

#include "core/WindowSystem.h"

#include <cstdlib>

int main ()
{
    try
    {
        Core::WindowSystem ws;
        Core::Window w;

        Application app(w);
        app.Run();
    }
    catch(const std::exception& error)
    {
        std::cerr << "Application Error: " << error.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
