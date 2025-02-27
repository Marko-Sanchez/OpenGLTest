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
    // MultiTexture displays multiple textures (2) using an orthographic projection.
    class MultiTexture: public Test
    {
        private:
            struct Vertex
            {
                float position[2];
                float texcords[2];
                int   texID;
            };

            glm::mat4 m_projectionMatrix;
            glm::mat4 m_viewMatrix;

            Shaders m_shader;

            VertexArray m_vao;
            IndexBuffer m_ibo;
            VertexBuffer m_vbo;
            Texture m_texture;

            std::vector<glm::vec3> m_textureTranslations;

        public:
            MultiTexture();

            void OnRender() override;
            void OnImGuiRender() override;
    };
}
