#pragma once

#include "Test.h"

#include <string_view>

#include "gtx/ModelLoader.h"
#include "gtx/Shaders.h"

namespace tests
{
class Trivial3DModel final: public Test
{
    private:

        Shaders      m_shader;
        ModelLoader  m_modelLoader;

        glm::mat4 m_projectionMatrix;
        glm::mat4 m_modelMatrix;

        bool m_wireFrame;

    public:

        Trivial3DModel(std::shared_ptr<void> window);
        ~Trivial3DModel();

        std::string_view GetName() const override;
        void OnRender() override;
        void OnImGuiRender() override;
};
}// namespace tests
