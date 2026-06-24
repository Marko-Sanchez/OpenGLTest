#pragma once

#include <GL/glew.h>

// Vertex Array or Vertex Array Object (VAO) stores all the data
// needed to needed to suply vertex information. It stores the format of
// the vertex data and references the VBO (it does not copy the VBO, if the
// data is modified changes will be seen in VAO).
// ref: https://www.khronos.org/opengl/wiki/vertex_Specification
class VertexArray
{
    private:

        GLuint m_objectName;

    public:

        VertexArray();
        ~VertexArray();

        // Define rule-of-five to avoid bugs.
        VertexArray(const VertexArray&)            = delete;
        VertexArray& operator=(const VertexArray&) = delete;
        VertexArray(VertexArray&&)                 = delete;
        VertexArray& operator=(VertexArray&&)      = delete;

        void Bind() const;
        void UnBind() const;

        GLuint GetName() const;

        void AddAttribute(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* offset);
        void AddIntAttribute(GLuint index, GLint size, GLsizei stride, const void* offset);
};
