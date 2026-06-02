cmake_minimum_required(VERSION 3.25.0)

project(opengltest
    LANGUAGES CXX
    DESCRIPTION "A Desktop Application demonstrating OpenGL examples."
    HOMEPAGE_URL "https://github.com/Marko-Sanchez/OpenGLTest"
)

include(CTest)

# Set CPP Version and config file for LSP.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

find_package(GLEW QUIET)
if (NOT GLEW_FOUND)
    message(FATAL_ERROR
        "\n[Missing dependency] GLEW not found.\n"
        "Install it with:\n"
        "  Ubuntu/Debian : sudo apt install libglew-dev\n"
        "  Fedora/RHEL   : sudo dnf install glew-devel\n"
        "  macOS         : brew install glew\n"
    )
endif()

find_package(glfw3 QUIET)
if (NOT glfw3_FOUND)
    message(FATAL_ERROR
        "\nTo install GLFW\n"
         "\tsudo apt install libglfw3-dev\n"
         "To compile GLFW for X11, you need to have the X11 development packages installed.\n"
         "\tsudo apt install xorg-dev\n"
    )
endif()

if (NOT EXISTS "${CMAKE_SOURCE_DIR}/vendors/imgui/imgui.h")
    message(FATAL_ERROR
        "ImGui submodule missing.\n"
        "Run: git submodule update --init --recursive"
    )
endif()

if (NOT EXISTS "${CMAKE_SOURCE_DIR}/vendors/stb/stb_image.h")
    message(FATAL_ERROR
        "Stb submodule missing.\n"
        "Run: git submodule update --init --recursive"
    )
endif()


include(FetchContent)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0af55ccecd98d4e5a8d1fad7de25ba429d60e863 # refs/tags/1.0.1
    FIND_PACKAGE_ARGS 1.0.1
)

FetchContent_MakeAvailable(glm)

add_subdirectory(cmake/imgui.cmake ${CMAKE_BINARY_DIR}/imgui)
add_subdirectory(cmake/stb.cmake ${CMAKE_BINARY_DIR}/stb)
add_subdirectory(src)
