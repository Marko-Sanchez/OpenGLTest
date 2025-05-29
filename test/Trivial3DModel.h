#pragma once

#include "Test.h"
#include "ModelLoader.h"

#include "Shaders.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace tests
{

class Trivial3DModel: public Test
{
    private:
    int m_numvertices;

    Shaders m_shader;
    VertexArray m_vao;
    VertexBuffer m_vbo;
    VertexBuffer m_uvs;

    glm::mat4 projectionMatrix;
    glm::mat4 modelMatrix;

    public:
    Trivial3DModel(std::shared_ptr<void> window);

    void OnRender() override;
};

}
