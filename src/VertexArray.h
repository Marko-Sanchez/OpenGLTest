#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

// Vertex Array or Vertex Array Object (VAO) stores all the data
// needed to needed to suply vertex information. It stores the format of
// the vertex data and references the VBO (it does not copy the VBO, if the
// data is modified changes will be seen in VAO).
// ref: https://www.khronos.org/opengl/wiki/vertex_Specification
class VertexArray
{
    private:
    unsigned int m_renderID;

    public:
    VertexArray();
    ~VertexArray();

    void Bind() const;
    void UnBind() const;

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& vbl);
};
