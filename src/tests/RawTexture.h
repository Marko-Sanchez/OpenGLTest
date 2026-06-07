#pragma once

#include "Test.h"

#include <GL/glew.h>
#include <string_view>

#include "gtx/Shaders.h"
#include "gtx/Texture.h"

namespace tests
{
// Example demonstrating drawing two textures.
class RawTexture final: public Test
{
    public:

        struct Vertex
        {
            float position[3];
            float texturecords[2];
            int textureid;
        };

    private:

        Texture m_texture;
        Shaders m_shader;
        GLuint m_ibo;
        GLuint m_vbo;
        GLuint m_vao;

    public:

        RawTexture(std::shared_ptr<void> window);
        ~RawTexture();

        std::string_view GetName() const override;
        void OnRender() override;
};
}// namespace tests
