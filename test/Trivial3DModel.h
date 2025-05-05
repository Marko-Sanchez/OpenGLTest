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

    public:
    Trivial3DModel();

    void OnRender() override;
};

}
