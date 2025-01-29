#pragma once

#include <GL/glew.h>
#include <stdexcept>
#include <vector>

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    bool normalized;

    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch(type)
        {
            case GL_FLOAT: return sizeof(float);
            case GL_UNSIGNED_INT: return sizeof(unsigned int);
            case GL_UNSIGNED_BYTE: return 1;
        }

        // unsupported type.
        throw std::runtime_error("Unknown type in VertexBufferLayout");
    }
};

class VertexBufferLayout
{
    private:
    unsigned int m_stride;
    std::vector<VertexBufferElement> m_elements;

    public:
    VertexBufferLayout();

    void Add(unsigned int count);

    inline const std::vector<VertexBufferElement>& GetElements() const {return m_elements;}
    inline unsigned int GetStride () const {return m_stride;}
};
