#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
{
    glGenBuffers(1, &m_renderID);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_renderID);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_renderID);
}

void VertexBuffer::UnBind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template<>
void VertexBuffer::AddBuffer<float>(unsigned int count, void *data)
{
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, GL_STATIC_DRAW);
}

template<>
void VertexBuffer::AddBuffer<int>(unsigned int count, void *data)
{
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(int), data, GL_STATIC_DRAW);
}
