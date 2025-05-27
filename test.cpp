#include <GL/glew.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <string>
#include <memory>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"

#include "ClearColor.h"
#include "MultiTexture.h"
#include "BatchRendering.h"
#include "RawTexture.h"
#include "ColoredCube.h"
#include "TexturedCube.h"
#include "Trivial3DModel.h"

int main(void)
{
    constexpr int WINDOW_WIDTH{1280};
    constexpr int WINDOW_HEIGHT{1280};

    // Initialize the library.
    if (!glfwInit())
    {
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context.
    std::shared_ptr<GLFWwindow> window(
        glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Test World", NULL, NULL), glfwDestroyWindow);

    if (!window)
    {
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window.get());
    glfwSwapInterval(1);
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    GLenum glewError = glewInit();
    if (GLEW_OK != glewError)
    {
        std::cerr << "Glew not Initialized" << std::endl;
        return EXIT_FAILURE;
    }

    // enable blending of the alpha value (rgba: controls transparency) and how it should be blended;
    // artifacts appear if not enabled on transparent textures.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window.get(), true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();

    std::shared_ptr<tests::Test> currentTest;
    std::shared_ptr<tests::TestMenu> testMenu = std::make_shared<tests::TestMenu>(window, currentTest);
    currentTest = testMenu;

    testMenu->RegisterTest<tests::ClearColor>("Clear Color");
    testMenu->RegisterTest<tests::MultiTexture>("Multiple Texture");
    testMenu->RegisterTest<tests::BatchRendering>("Batch Rendering");
    testMenu->RegisterTest<tests::RawTexture>("Raw Texture");
    testMenu->RegisterTest<tests::ColoredCube>("Colored Cube");
    testMenu->RegisterTest<tests::TexturedCube>("Textured Cube");
    testMenu->RegisterTest<tests::Trivial3DModel>("3D Model");

    // Loop until the user closes the window.
    while (glfwGetKey(window.get(), GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window.get()))
    {
        // clear and reset background.
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        // sets bitplane area (region of memory rendered as pixels) to the previous values selected by glclearcolor
        glClear(GL_COLOR_BUFFER_BIT);

        currentTest->OnRender();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("Test Menu");
            if (currentTest != testMenu && ImGui::Button("<-"))
            {
                currentTest = testMenu;
            }
            currentTest->OnImGuiRender();
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }// while

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return EXIT_SUCCESS;
}
