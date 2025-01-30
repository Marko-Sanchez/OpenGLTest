# pragma once

#include <GL/glew.h>

// Vertex Buffer creates a Vertex Buffer Object (VBO).
//
// glgenbuffers() returns an unused buffer name(s) (id), then
// glbindbuffer() associates that name with a buffer object in the
// gpu, if it does not exist gl creates a new buffer object and binds it
// to the name. glbufferdata() uploads that data to the gpu.
class VertexBuffer
{
    private:
    unsigned int m_renderID;// store buffer object name.

    public:
    VertexBuffer();
    ~VertexBuffer();

    void Bind() const;
    void UnBind() const;

    template<typename T>
    void AddBuffer(unsigned int count, void* data);
};
