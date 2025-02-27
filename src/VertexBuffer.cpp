#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
:m_objectName(0)
{
    glGenBuffers(1, &m_objectName);
    glBindBuffer(GL_ARRAY_BUFFER, m_objectName);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_objectName);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_objectName);
}

void VertexBuffer::UnBind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
