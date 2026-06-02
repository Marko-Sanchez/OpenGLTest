#include "VertexArray.h"
#include "VertexBuffer.h"

VertexArray::VertexArray()
:m_objectName(0)
{
    glGenVertexArrays(1, &m_objectName);
    glBindVertexArray(m_objectName);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_objectName);
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_objectName);
}

void VertexArray::UnBind() const
{
    glBindVertexArray(0);
}

unsigned int VertexArray::GetName() const
{
    return m_objectName;
}

/*
* Define how OpenGL should interpret the most recently binded buffer objects data.
* Floating point attributes must use glVertexAttribPointer(). Integer values will
* be converted to Floats.
*
* @parameters:
* GLint size: defines the number of components in the vector, can be any number between 1-4.
* GLboolean normalized: floating point types must be GL_FALSE, if the type is int GL_FALSE converts ints
*                       directly to float using c-style casting (255 -> 255.0f); if GL_TRUE int gets converted
*                       via integer normalization.
*/
void VertexArray::AddAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* offset)
{
    glVertexAttribPointer(index, size, type, normalized, stride, offset);
    glEnableVertexAttribArray(index);
}

/*
* Define how OpenGL should interpret the most recently binded buffer objects data.
* Integer (signed and unsigned) point attributes must use glVertexAttribIPointer().
*
* @parameters:
* GLint size: defines the number of components in the vector, can be any number between 1-4.
*/
void VertexArray::AddIntAttribute(GLuint index, GLint size, GLsizei stride, const void* offset)
{
    glVertexAttribIPointer(index, size, GL_INT, stride, offset);
    glEnableVertexAttribArray(index);
}
