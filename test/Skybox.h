#pragma once

#include "Shaders.h"
#include "Test.h"

#include "Texture.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

namespace tests
{
class Skybox: public Test
{
    private:
    Shaders m_cubeshader;
    Texture m_texture;
    VertexArray m_vba;
    VertexBuffer m_vbo;

    public:
    Skybox(std::shared_ptr<void> window);

    void OnRender() override;

};
}
