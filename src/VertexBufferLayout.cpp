#include "VertexBufferLayout.h"

VertexBufferLayout::VertexBufferLayout()
    :m_stride(0)
{}

template<>
void VertexBufferLayout::Add<float>(unsigned int count)
{
    m_elements.push_back({GL_FLOAT, count, GL_FALSE});
    m_stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}

template<>
void VertexBufferLayout::Add<unsigned int>(unsigned int count)
{
    m_elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    m_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}

template<>
void VertexBufferLayout::Add<unsigned char>(unsigned int count)
{
    m_elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
    m_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}
