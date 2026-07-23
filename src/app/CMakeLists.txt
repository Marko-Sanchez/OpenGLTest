add_library(opengltest-app
    Application.h
    Application.cpp
)

target_include_directories(opengltest-app
    PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(opengltest-app
    PUBLIC
        opengltest-core
        opengltest-tests
        imgui
)
