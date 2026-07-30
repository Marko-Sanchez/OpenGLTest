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

Application::Application(Core::Window& win):
    m_window(win)
{
    m_window.CreateWindow();
}

void Application::Run()
{
    auto window = m_window.Get();
    if (!window)
    {
        return;
    }

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window.get(), true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();

    auto m_menu = std::make_shared<tests::TestMenu>(window,
            [&currentTest = m_activeTest](std::shared_ptr<tests::Test> newTest)
            {
                currentTest = std::move(newTest);
            });

    SetActiveTest(m_menu);

    m_menu->RegisterTest<tests::ClearColor>("Clear Color");
    m_menu->RegisterTest<tests::BatchRendering>("Batch Rendering");
    m_menu->RegisterTest<tests::RawTexture>("Raw Texture");
    m_menu->RegisterTest<tests::MultiTexture>("Multiple Texture");
    m_menu->RegisterTest<tests::ColoredCube>("Colored Cube");
    m_menu->RegisterTest<tests::TexturedCube>("Textured Cube");
    m_menu->RegisterTest<tests::Trivial3DModel>("3D Model");
    m_menu->RegisterTest<tests::Instancing>("Instancing");
    m_menu->RegisterTest<tests::TextureInstancing>("Texture Instancing");
    m_menu->RegisterTest<tests::Skybox>("Skybox");

    std::string imguiTitle;


    while (m_window.IsOpen())
    {
        if (m_window.ExitKey()) return;


        // clear and set background; set bitplane area (region of memory rendered as pixels) to the value selected by glclearcolor
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_activeTest->OnRender();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            imguiTitle.clear();
            imguiTitle.append("Test Menu - ").append(m_activeTest->GetName()).append(" ###WindowTitle");

            ImGui::Begin(imguiTitle.c_str());
            if (m_activeTest != m_menu && (ImGui::ArrowButton("##left", ImGuiDir_Left) || glfwGetKey(window.get(), GLFW_KEY_Q) == GLFW_PRESS))
            {
                SetActiveTest(m_menu);
            }
            m_activeTest->OnImGuiRender();
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        m_window.UpdateBuffers();
    }


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Application::SetActiveTest(std::shared_ptr<tests::Test> test)
{
    m_activeTest = test;
}
