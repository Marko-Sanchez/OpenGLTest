#pragma once

#include "Test.h"
#include "gtx/ModelLoader.h"
#include "gtx/Shaders.h"

#include <memory>
#include <string_view>

#include <glm/fwd.hpp>

namespace tests
{
class TextureInstancing final: public Test
{
    private:

        ModelLoader m_planetModel;
        ModelLoader m_asteriodModel;
        Shaders     m_planetShader;
        Shaders     m_asteriodShader;

        GLuint m_instanceVBO;

        std::vector<glm::mat4> m_translations;

        void GenerateTranslations();

    public:

        TextureInstancing(std::shared_ptr<void> window);
        ~TextureInstancing();

        std::string_view GetName() const override;
        void OnRender() override;
};
}// namespace tests
