#include <GL/glew.h>

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <iostream>
#include <cstdlib>
#include <memory>

#include "tests/TestMenu.h"
#include "tests/ClearColor.h"
#include "tests/MultiTexture.h"
#include "tests/Instancing.h"
#include "tests/BatchRendering.h"
#include "tests/RawTexture.h"
#include "tests/ColoredCube.h"
#include "tests/TextureInstancing.h"
#include "tests/TexturedCube.h"
#include "tests/Trivial3DModel.h"
#include "tests/Skybox.h"

int main ()
{
    constexpr int WINDOW_WIDTH{1280};
    constexpr int WINDOW_HEIGHT{1280};

    // Initialize the library.
    if (!glfwInit())
    {
        return EXIT_FAILURE;
    }

    glfwSetErrorCallback([](int error, const char* description)
    {
        std::cerr << "GLFW Error: " << error << ": " << description << std::endl;
    });


    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window-mode window and its OpenGL context.
    std::shared_ptr<GLFWwindow> window
    (
        glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Test World", NULL, NULL),
        glfwDestroyWindow
    );

    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    // Make the window's context current
    glfwMakeContextCurrent(window.get());
    glfwSwapInterval(1);
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;


    if (GLenum glewError = glewInit(); GLEW_OK != glewError)
    {
        std::cerr << "Glew not Initialized" << std::endl;
        window.reset();
        glfwTerminate();
        return EXIT_FAILURE;
    }
    // tell OpenGL the size of the rendering window so OpenGL knows
    // how we want to display the data and coordinates with respect to the window
    // (0, 0) -> bottom-left coordinates.
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
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

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window.get(), true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();

    {
        std::shared_ptr<tests::Test> currentTest;
        std::shared_ptr<tests::TestMenu> testMenu = std::make_shared<tests::TestMenu>(window, currentTest);
        currentTest = testMenu;

        testMenu->RegisterTest<tests::ClearColor>("Clear Color");
        testMenu->RegisterTest<tests::BatchRendering>("Batch Rendering");
        testMenu->RegisterTest<tests::RawTexture>("Raw Texture");
        testMenu->RegisterTest<tests::MultiTexture>("Multiple Texture");
        testMenu->RegisterTest<tests::ColoredCube>("Colored Cube");
        testMenu->RegisterTest<tests::TexturedCube>("Textured Cube");
        testMenu->RegisterTest<tests::Trivial3DModel>("3D Model");
        testMenu->RegisterTest<tests::Instancing>("Instancing");
        testMenu->RegisterTest<tests::TextureInstancing>("Texture Instancing");
        testMenu->RegisterTest<tests::Skybox>("Skybox");

        std::string imguiTitle;

        // Loop until the user closes the window.
        while (!glfwWindowShouldClose(window.get()))
        {
            if (glfwGetKey(window.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            {
                glfwSetWindowShouldClose(window.get(), true);
            }

            // clear and set background; set bitplane area (region of memory rendered as pixels) to the value selected by glclearcolor
            glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            currentTest->OnRender();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            {
                imguiTitle.clear();
                imguiTitle.append("Test Menu - ").append(currentTest->GetName()).append(" ###WindowTitle");
                ImGui::Begin(imguiTitle.c_str());
                if (currentTest != testMenu && (ImGui::ArrowButton("##left", ImGuiDir_Left) || glfwGetKey(window.get(), GLFW_KEY_Q) == GLFW_PRESS))
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
    }// scope - destroy test shared pointers.

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    window.reset();
    glfwTerminate();
    return EXIT_SUCCESS;
}
