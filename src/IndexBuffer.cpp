#include "IndexBuffer.h"

#include "GL/glew.h"
#include <stdexcept>

IndexBuffer::IndexBuffer()
:m_count(0)
{
    glGenBuffers(1, &m_renderID);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_renderID);
}

void IndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderID);
}

void IndexBuffer::UnBind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::AddBuffer(unsigned int count, unsigned int *data)
{
    if (data == nullptr)
        throw std::invalid_argument("Index Buffer Data Pointer is null");

    m_count = count;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

void IndexBuffer::AddBuffer(unsigned int count, unsigned short *data)
{
    if (data == nullptr)
        throw std::invalid_argument("Index Buffer Data Pointer is null");

    m_count = count;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned short), data, GL_STATIC_DRAW);
}

void IndexBuffer::AddBuffer(unsigned int count, unsigned char *data)
{
    if (data == nullptr)
        throw std::invalid_argument("Index Buffer Data Pointer is null");

    m_count = count;
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned char), data, GL_STATIC_DRAW);
}

