# To install GLFW
# sudo apt install libglfw3
# sudo apt install libglfw3-dev

# To compile GLFW for X11, you need to have the X11 development packages installed.
# sudo apt install xorg-dev

# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and language
project(OpenGLTest CXX)

# Set CPP Version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Collect all source files in the src directory
file(GLOB_RECURSE SRC_FILES src/*.cpp)
file(GLOB_RECURSE TEST_FILES test/*.cpp)

# Set the source file(s)
set(SOURCE test.cpp)

# Enable compile_commands.json generation
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add the executable
add_executable(test ${SOURCE} ${SRC_FILES} ${TEST_FILES})

# Include the src directory for header files
# helps shorten include path: vendors/vendor/somefile.cpp -> vendor/somefile.cpp
target_include_directories(test PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/src/vendors ${CMAKE_SOURCE_DIR}/test)

# Link necessary libraries
# If you're missing certain flags use:
# pkg-config --libs glew
target_link_libraries(test
    GLEW
    glfw
    GLU
    GL
    Xrandr
    Xxf86vm
    Xi
    Xinerama
    X11
    rt
    dl
)

# Enable pthread
find_package(Threads REQUIRED)
target_link_libraries(test Threads::Threads)
