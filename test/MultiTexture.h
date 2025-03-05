#pragma once

#include "Test.h"

#include <vector>

#include "glm/glm.hpp"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shaders.h"
#include "Texture.h"

namespace tests
{
    // MultiTexture displays multiple textures using an orthographic projection.
    class MultiTexture: public Test
    {
        private:

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

            glm::mat4 m_projectionMatrix;
            glm::mat4 m_viewMatrix;

            Shaders m_shader;

            VertexArray  m_vao;
            IndexBuffer  m_ibo;
            VertexBuffer m_vbo;
            Texture      m_texture;

            std::vector<TextureInfo> m_textureTranslations;

        public:
            MultiTexture();

            void OnRender() override;
            void OnImGuiRender() override;
    };
}
