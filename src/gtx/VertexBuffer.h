# pragma once

#include <GL/glew.h>

#include <vector>

// Vertex Buffer creates a Vertex Buffer Object (VBO).
//
// glgenbuffers() returns an unused buffer name(s) (id), then
// glbindbuffer() associates that name with a buffer object in the
// gpu, if it does not exist gl creates a new buffer object and binds it
// to the name. glbufferdata() uploads that data to the gpu.
class VertexBuffer
{
    private:
    unsigned int m_objectName;

    public:
    VertexBuffer();
    ~VertexBuffer();

    void Bind() const;
    void UnBind() const;

    template<typename T>
    void CreateBuffer(const std::vector<T>& data)
    {
        Bind();
        glBufferData(GL_ARRAY_BUFFER, sizeof(T) * data.size(), data.data(), GL_STATIC_DRAW);
    }
};
