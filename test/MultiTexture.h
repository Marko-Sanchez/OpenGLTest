#pragma once

#include "Test.h"

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "VertexBuffer.h"
#include "glm/glm.hpp"

#include "Renderer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shaders.h"
#include "Texture.h"

namespace tests
{
    // MultiTexture displays multiple textures (2) using an orthographic projection.
    class MultiTexture: public Test
    {
        private:
            glm::mat4 m_projectionMatrix;
            glm::mat4 m_viewMatrix;

            VertexArray m_vertexArray;
            IndexBuffer m_indexBuffer;
            VertexBuffer m_vertexBuffer;
            Texture m_texture;

            Shaders m_shader;
            Renderer m_renderer;

            std::vector<glm::vec3> m_textureTranslations;
            std::vector<Texture> m_textures;

        public:
            MultiTexture();

            void OnRender() override;
            void OnImGuiRender() override;

            void AddTexture(const std::string& name, const std::string& texturePath, glm::vec3 translation, int texturePosition = 0);
    };
}
