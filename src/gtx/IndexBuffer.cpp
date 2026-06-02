#include "IndexBuffer.h"

IndexBuffer::IndexBuffer()
{
    glGenBuffers(1, &m_objectName);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_objectName);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_objectName);
}

void IndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_objectName);
}

void IndexBuffer::UnBind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
