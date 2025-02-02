#pragma once

// Index Buffer Object (IBO) store indices that reference vertices in a Vertex Buffer Object (VBO).
// Indices can be represented in any using any primitive ideally uint, ushort or uchar. What these indices
// are, is the position of the vertex in the VBO.
//
// IBO lets us re-use vertices more then one to draw multiple triangles (ex. to draw an octagon all triangles would
// share the middle vertex) saving memory and improving cache efficiency (reuse prev. transformed vertices).
class IndexBuffer
{
    private:
    unsigned int m_renderID;
    unsigned int m_count;

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
    void AddBuffer(unsigned int count, unsigned int *data);
    void AddBuffer(unsigned int count, unsigned short *data);
    void AddBuffer(unsigned int count, unsigned char *data);

    inline unsigned int GetCount() const {return m_count;};
};
