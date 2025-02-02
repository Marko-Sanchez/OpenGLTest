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

void VertexBuffer::AddBuffer(unsigned int count, float *data)
{
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, GL_STATIC_DRAW);
}

void VertexBuffer::AddBuffer(unsigned int count, int *data)
{
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(int), data, GL_STATIC_DRAW);
}
