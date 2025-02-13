#include "VertexBufferLayout.h"

VertexBufferLayout::VertexBufferLayout()
    :m_stride(0)
{}

void VertexBufferLayout::AddFloat(unsigned int count)
{
    m_elements.push_back({GL_FLOAT, count, GL_FALSE});
    m_stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
}

void VertexBufferLayout::AddInt(unsigned int count)
{
    m_elements.push_back({GL_INT, count, GL_FALSE});
    m_stride += count * VertexBufferElement::GetSizeOfType(GL_INT);
}

void VertexBufferLayout::AddUint(unsigned int count)
{
    m_elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    m_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
}

void VertexBufferLayout::AddUshort(unsigned int count)
{
    m_elements.push_back({GL_UNSIGNED_SHORT, count, GL_FALSE});
    m_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_SHORT);
}

void VertexBufferLayout::AddUchar(unsigned int count)
{
    m_elements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
    m_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
}
