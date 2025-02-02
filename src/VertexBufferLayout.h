#pragma once

#include <GL/glew.h>
#include <stdexcept>
#include <vector>

// vertex attribute description.
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
            case GL_UNSIGNED_SHORT: return sizeof(unsigned short);
            case GL_UNSIGNED_BYTE: return 1;
        }

        // unsupported type.
        throw std::runtime_error("Unknown type in VertexBufferLayout");
    }
};

// Since OpenGL stores multiple vertex attributes in a single buffer.
// Vertex Buffer Layout helps define how a vertex attribute is structured inside a Vertex buffer object.
//
// A Vertex Buffer Object is simply an array of data, all vertex's attributes are stored sequentially in the VBO
// this data is stored in GPU memory. this data is uploaded to the GPU from the CPU using glBufferData() (which is
// currently done in VertexBuffer.h)
class VertexBufferLayout
{
    private:
    unsigned int m_stride;                      // size of a vertex attribute (attribute can contain: positions, colors, etc.).
    std::vector<VertexBufferElement> m_elements;// array of a descriptions of vertex attribute.

    public:
    VertexBufferLayout();

    void AddFloat(unsigned int count);
    void AddUint(unsigned int count);
    void AddUshort(unsigned int count);
    void AddUchar(unsigned int count);

    inline const std::vector<VertexBufferElement>& GetElements() const {return m_elements;}
    inline unsigned int GetStride () const {return m_stride;}
};
