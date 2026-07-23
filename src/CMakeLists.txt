add_subdirectory(gtx)
add_subdirectory(core)
add_subdirectory(tests)
add_subdirectory(app)

add_executable(opengltest
    main.cpp
)

target_include_directories(opengltest PRIVATE
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(opengltest PRIVATE
    opengltest-tests
    opengltest-app
    glfw
)
