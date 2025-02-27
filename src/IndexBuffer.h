#pragma once

#include <vector>

#include "GL/glew.h"
// Index Buffer Object (IBO) store indices that reference vertices in a Vertex Buffer Object (VBO).
// Indices can be represented in any using any primitive ideally uint, ushort or uchar. What these indices
// are, is the position of the vertex in the VBO.
//
// IBO lets us re-use vertices more then one to draw multiple triangles (ex. to draw an octagon all triangles would
// share the middle vertex) saving memory and improving cache efficiency (reuse prev. transformed vertices).
class IndexBuffer
{
    private:
    unsigned int m_objectName;

    public:
    IndexBuffer();
    ~IndexBuffer();

    void Bind();
    void UnBind();

    // Before I was trying to use explicit function specilization.
    // Decided against it, because I had no use for the base function template
    // and explicit specilization do not overload; unlike base function templates
    // and normal functions as those below. It simply complicated the code more.
    // Info on why explicit specilization don't overload: http://www.gotw.ca/publications/mill17.htm
    /* Above comments are in-reference to previous attempts at creating this class, will kept in for education purposes */

    template<typename T>
    void CreateBuffer(const std::vector<T>& data)
    {
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), GL_STATIC_DRAW);
    }
};
