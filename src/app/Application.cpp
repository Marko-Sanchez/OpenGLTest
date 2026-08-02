#include "Application.h"

#include <string>
#include <memory>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include "core/BasicCommand.h"
#include "core/WindowedCommand.h"

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
    auto glfwWindow = m_window.Get();
    if (glfwWindow == nullptr)
    {
        return;
    }

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(glfwWindow, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();

    m_menu = std::make_shared<tests::TestMenu>();
    SetActiveTest(m_menu);

    m_menu->AddCommand("Clear Color", std::make_unique<Core::BasicCommand<tests::ClearColor>>(this));
    m_menu->AddCommand("Batch Rendering", std::make_unique<Core::BasicCommand<tests::BatchRendering>>(this));
    m_menu->AddCommand("Raw Texture", std::make_unique<Core::BasicCommand<tests::RawTexture>>(this));
    m_menu->AddCommand("Multiple Texture", std::make_unique<Core::BasicCommand<tests::MultiTexture>>(this));
    m_menu->AddCommand("Colored Cube", std::make_unique<Core::WindowedCommand<tests::ColoredCube>>(this, glfwWindow));
    m_menu->AddCommand("Textured Cube", std::make_unique<Core::BasicCommand<tests::TexturedCube>>(this));
    m_menu->AddCommand("3D Model", std::make_unique<Core::BasicCommand<tests::Trivial3DModel>>(this));
    m_menu->AddCommand("Instancing", std::make_unique<Core::BasicCommand<tests::Instancing>>(this));
    m_menu->AddCommand("Texture Instancing", std::make_unique<Core::BasicCommand<tests::TextureInstancing>>(this));
    m_menu->AddCommand("Skybox", std::make_unique<Core::WindowedCommand<tests::Skybox>>(this, glfwWindow));

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
            if (m_activeTest != m_menu && (ImGui::ArrowButton("##left", ImGuiDir_Left) || glfwGetKey(glfwWindow, GLFW_KEY_Q) == GLFW_PRESS))
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
