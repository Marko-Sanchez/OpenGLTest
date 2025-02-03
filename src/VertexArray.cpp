#include "VertexArray.h"
#include "VertexBuffer.h"

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_renderID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_renderID);
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_renderID);
}

void VertexArray::UnBind() const
{
    glBindVertexArray(0);
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& vbl)
{
    Bind();
    vb.Bind();

    unsigned int offset{0};
    const auto& elements = vbl.GetElements();

    for(unsigned int i{0}; i < elements.size(); ++i)
    {
        glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, vbl.GetStride(), reinterpret_cast<void*>(offset));
        glEnableVertexAttribArray(i);

        offset += elements[i].count * VertexBufferElement::GetSizeOfType(elements[i].type);
    }
}
