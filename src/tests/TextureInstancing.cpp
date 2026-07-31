#include "TextureInstancing.h"

#include <imgui.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/trigonometric.hpp>

#include "glm/ext/matrix_clip_space.hpp"
#include "gtx/Mesh.h"

#include <filesystem>
#include <random>
#include <string_view>

namespace tests
{
namespace
{
    constexpr std::string_view k_TestName {"Texture Instancing"};
    constexpr GLuint k_InstanceCount      {5000};

    const std::filesystem::path k_PlanetVertShader   {"res/Shaders/TextureInstancingPlanet.vert"};
    const std::filesystem::path k_PlanetFragShader   {"res/Shaders/TextureInstancingPlanet.frag"};
    const std::filesystem::path k_AsteriodVertShader {"res/Shaders/TextureInstancingAsteroid.vert"};
    const std::filesystem::path k_AsteriodFragShader {"res/Shaders/TextureInstancingAsteroid.frag"};

    const std::filesystem::path k_PlanetModel   {"res/3dmodels/planet/planet.obj"};
    const std::filesystem::path k_AsteriodModel {"res/3dmodels/rock/rock.obj"};
}// anonymous namespace

TextureInstancing::TextureInstancing():
    m_planetModel(k_PlanetModel),
    m_asteriodModel(k_AsteriodModel),
    m_planetShader(k_PlanetVertShader, k_PlanetFragShader),
    m_asteriodShader(k_AsteriodVertShader, k_AsteriodFragShader)
{
    this->GenerateTranslations();

    glGenBuffers(1, &m_instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * k_InstanceCount, m_translations.data(), GL_STATIC_DRAW);

    // set attributes for meshes.
    for (size_t i {0}; i < m_asteriodModel.GetMeshes().size(); ++i)
    {
        auto vao = m_asteriodModel.GetMeshes()[i].GetVAO();
        glBindVertexArray(vao);

        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(sizeof(glm::vec4) * 0));
        glEnableVertexAttribArray(3);
        glVertexAttribDivisor(3, 1);

        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(sizeof(glm::vec4) * 1));
        glEnableVertexAttribArray(4);
        glVertexAttribDivisor(4, 1);

        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(sizeof(glm::vec4) * 2));
        glEnableVertexAttribArray(5);
        glVertexAttribDivisor(5, 1);

        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<void*>(sizeof(glm::vec4) * 3));
        glEnableVertexAttribArray(6);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }

    auto view = glm::lookAt
        (
         glm::vec3(0.0f, 300.0f, 200.0f), // camera position.
         glm::vec3(0.0f, 0.0f, 0.0f), // camera target.
         glm::vec3(0.0f, 1.0f, 0.0f)    // camera up.
         );
    auto projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 1000.0f);

    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));

    m_planetShader.CreateShader();
    m_planetShader.Bind();
    m_planetShader.SetUniformMat4f("u_Model", model);
    m_planetShader.SetUniformMat4f("u_View", view);
    m_planetShader.SetUniformMat4f("u_Projection", projection);
    m_planetShader.UnBind();

    m_asteriodShader.CreateShader();
    m_asteriodShader.Bind();
    m_asteriodShader.SetUniformMat4f("u_View", view);
    m_asteriodShader.SetUniformMat4f("u_Projection", projection);
    m_asteriodShader.SetUniform1i("texture_normal1", 0);
    m_asteriodShader.UnBind();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
}

TextureInstancing::~TextureInstancing()
{
    glDeleteBuffers(1, &m_instanceVBO);

    glDepthFunc(GL_LESS);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
}

std::string_view TextureInstancing::GetName() const
{
    return k_TestName;
}

void TextureInstancing::OnRender()
{
    float camSpeed  {0.1f};
    float camAngle  {static_cast<float>(glfwGetTime() * camSpeed)};
    float camRadius {360.0f};

    auto view = glm::lookAt
    (
     glm::vec3(glm::sin(camAngle) * camRadius, 300.0f, glm::cos(camAngle) * camRadius), // camera position.
     glm::vec3(0.0f, 0.0f, 0.0f), // camera target.
     glm::vec3(0.0f, 1.0f, 0.0f)    // camera up.
    );


    m_planetShader.Bind();
    m_planetShader.SetUniformMat4f("u_View", view);
    m_planetModel.Draw(m_planetShader);
    m_planetShader.UnBind();

    m_asteriodShader.Bind();
    m_asteriodShader.SetUniformMat4f("u_View", view);

    // name derived from rock.mtl
    auto texID = m_asteriodModel.GetTexture().GetTextureName("rock.png");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);

    for (size_t i {0}; i < m_asteriodModel.GetMeshes().size(); ++i)
    {
        glBindVertexArray(m_asteriodModel.GetMeshes()[i].GetVAO());
        glDrawElementsInstanced(GL_TRIANGLES, m_asteriodModel.GetMeshes()[i].indices.size(), GL_UNSIGNED_INT, 0, k_InstanceCount);
        glBindVertexArray(0);
    }
}

void TextureInstancing::OnImGuiRender()
{
    ImGui::TextWrapped
        (
         "Demontrating %d 3D textures displayed using instancing.", k_InstanceCount
        );
}

void TextureInstancing::GenerateTranslations()
{
    m_translations.reserve(k_InstanceCount);

    float radius {100.0f};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> displacementDist (-25.0f, 25.0f);
    std::uniform_real_distribution<float> scaleDist (0.45f, 0.75f);
    std::uniform_real_distribution<float> rotationDist (0.0f, 360.0f);

    for (unsigned int i {0}; i < k_InstanceCount; ++i)
    {
        glm::mat4 model = glm::mat4(1.0f);
        float fraction { static_cast<float>(i) / static_cast<float>(k_InstanceCount)};
        float angle    { glm::radians(fraction * 360.0f)};

        float x {glm::sin(angle) * radius + displacementDist(gen)};
        float y {displacementDist(gen)};
        float z {glm::cos(angle) * radius + displacementDist(gen)};

        // position.
        model = glm::translate(model, glm::vec3(x, y, z));

        // scale.
        float scale {scaleDist(gen)};
        model = glm::scale(model, glm::vec3(scale));

        // rotation.
        float rotAngle {rotationDist(gen)};
        model = glm::rotate(model, glm::radians(rotAngle), glm::vec3(0.4f, 0.6f, 0.8f));

        m_translations.emplace_back(std::move(model));
    }
}
}// namespace tests
