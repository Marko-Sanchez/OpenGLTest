#include "VertexArray.h"
#include "VertexBuffer.h"
#include <iostream>

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
    vb.Bind();
    Bind();

    unsigned int offset{0};
    const auto& elements = vbl.GetElements();

    for(unsigned int i{0}; i < elements.size(); ++i)
    {
        if ( elements[i].type == GL_INT || elements[i].type == GL_UNSIGNED_INT)
        {
            glVertexAttribIPointer(i, elements[i].count, elements[i].type, vbl.GetStride(), reinterpret_cast<void*>(offset));
        }
        else
        {
            glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, vbl.GetStride(), reinterpret_cast<void*>(offset));
        }

        glEnableVertexAttribArray(i);

        std::cout << i << ": " << "Offset: " << offset << ", Count: " << elements[i].count << " , Size: " << VertexBufferElement::GetSizeOfType(elements[i].type) << std::endl;
        offset += elements[i].count * VertexBufferElement::GetSizeOfType(elements[i].type);
    }
}
