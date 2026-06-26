#pragma once

#include "Test.h"
#include "gtx/Shaders.h"

#include <memory>
#include <vector>

#include <glm/fwd.hpp>
#include <GL/glew.h>

namespace tests
{
class Instancing final: public Test
{
    public:

        struct Vertex
        {
            glm::vec2 position;
            glm::vec4 color;
        };

    private:

        struct Instances
        {
            glm::vec2 translation;
        };

        Shaders m_shader;

        GLuint m_quadVAO;
        GLuint m_quadEBO;
        GLuint m_quadVBO;
        GLuint m_instanceVBO;

        std::vector<Instances> m_translations;

        void GenerateTranslations();

    public:

        Instancing(std::shared_ptr<void> window);
        ~Instancing();

        std::string_view GetName() const override;

        void OnRender() override;
        void OnImGuiRender() override;
};
}// namespace tests
