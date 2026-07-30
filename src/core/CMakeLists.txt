add_library(opengltest-core
    WindowSystem.h
    Window.h
    Window.cpp
    Test.h
    Command.h
)

target_include_directories(opengltest-core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(opengltest-core
    PUBLIC
        GL
        GLEW::GLEW
        glfw
)
