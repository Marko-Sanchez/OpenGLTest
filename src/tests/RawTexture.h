#pragma once

#include "Test.h"

#include <string_view>
#include <GL/glew.h>

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

        GLuint m_vao;
        GLuint m_vbo;
        GLuint m_ebo;

    public:

        RawTexture(std::shared_ptr<void> window);
        ~RawTexture();

        RawTexture(const RawTexture&)            = delete;
        RawTexture& operator=(const RawTexture&) = delete;
        RawTexture(RawTexture&&)                 = delete;
        RawTexture& operator=(RawTexture&&)      = delete;

        std::string_view GetName() const override;
        void OnRender() override;
        void OnImGuiRender() override;
};
}// namespace tests
