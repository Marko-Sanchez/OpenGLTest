#pragma once

#include "Test.h"

#include <glm/glm.hpp>
#include <GL/glew.h>

#include "gtx/VertexArray.h"
#include "gtx/VertexBuffer.h"
#include "gtx/IndexBuffer.h"
#include "gtx/Shaders.h"
#include "gtx/Texture.h"

namespace tests
{
class TexturedCube final: public Test
{
    public:

        struct Vertex
        {
            glm::vec3 pos;
            glm::vec2 tcords;
        };

    private:

        Shaders m_shader;
        Texture m_texture;

        VertexArray m_vao;
        VertexBuffer m_vbo;
        IndexBuffer m_ibo;

        glm::mat4 m_projMatrix;
        glm::mat4 m_modelMatrix;

    public:

        TexturedCube(std::shared_ptr<void> window);

        void OnRender() override;
};
}// namespace tests
