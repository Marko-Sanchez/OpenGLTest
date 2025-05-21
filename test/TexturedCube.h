#pragma once

#include "Test.h"

#include "GL/glew.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "glm/glm.hpp"

#include "Shaders.h"
#include "Texture.h"

namespace tests
{
class TexturedCube: public Test
{
private:
    struct Vertex
    {
        glm::vec3 pos;
        glm::vec2 tcords;
    };

    int m_numIndices;

    Shaders m_shader;
    Texture m_texture;

    VertexArray m_vao;
    VertexBuffer m_vbo;
    IndexBuffer m_ibo;

    glm::mat4 projectionMatrix;
    glm::mat4 modelMatrix;

public:
    TexturedCube();

    void OnRender() override;
};
}
