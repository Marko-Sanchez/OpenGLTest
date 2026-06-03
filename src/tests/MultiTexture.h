#pragma once

#include "Test.h"

#include <string_view>
#include <vector>

#include <glm/glm.hpp>

#include "gtx/VertexArray.h"
#include "gtx/VertexBuffer.h"
#include "gtx/IndexBuffer.h"
#include "gtx/Shaders.h"
#include "gtx/Texture.h"

namespace tests
{
// MultiTexture displays multiple textures using an orthographic projection.
class MultiTexture final: public Test
{
    public:

        struct TextureInfo
        {
            std::string name;
            glm::vec3 translation;
            TextureInfo(std::string s_name, glm::vec3 s_trans)
            : name(s_name), translation(s_trans){}
        };

        struct Vertex
        {
            float position[2];
            float texcords[2];
            int   texID;
        };

    private:

        glm::mat4 m_projectionMatrix;
        glm::mat4 m_viewMatrix;

        Shaders m_shader;

        VertexArray  m_vao;
        IndexBuffer  m_ibo;
        VertexBuffer m_vbo;
        Texture      m_texture;

        std::vector<TextureInfo> m_textureTranslations;

    public:

        MultiTexture(std::shared_ptr<void> window);

        std::string_view GetName() const override;
        void OnRender() override;
        void OnImGuiRender() override;
};
}// namespace tests
