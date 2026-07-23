#include "Application.h"

#include <string>
#include <memory>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

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

Application::Application()
{
    m_window = std::make_shared<Core::Window>();
    m_window->CreateWindow();
}

Application::~Application()
{
    m_window->DestroyWindow();
}

void Application::Run()
{
    auto window = m_window->Get();
    if (!window)
    {
        return;
    }

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window.get(), true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();

    std::shared_ptr<tests::Test> currentTest;

    auto testMenu = std::make_shared<tests::TestMenu>(window,
            [&currentTest](std::shared_ptr<tests::Test> newTest)
            {
                currentTest = std::move(newTest);
            });

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


    while (m_window->IsOpen())
    {
        if (m_window->ExitKey()) return;


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

        m_window->UpdateBuffers();
    }


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
