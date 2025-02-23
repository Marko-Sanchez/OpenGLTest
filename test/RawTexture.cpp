#include <RawTexture.h>

#include <cstddef>
#include <vector>

namespace tests
{

RawTexture::RawTexture()
:m_shader("../res/Shaders/Raw.vertex","../res/Shaders/Raw.fragment")
{
    // todo: setting z to -5.0f, was causing the rect. not to appear setting it to 0.0f fixed it,
    // rectangle is squead fix it.
    std::vector<Vertex> vertices =
    {
        {{-0.50f, -0.25f, 0.0f}, {0.0f, 0.0f}, 1},
        {{-0.25f, -0.25f, 0.0f}, {1.0f, 0.0f}, 1},
        {{-0.25f,  0.0f , 0.0f}, {1.0f, 1.0f}, 1},
        {{-0.50f,  0.0f , 0.0f}, {0.0f, 1.0f}, 1}
    };
    std::vector<unsigned int> ibuffer =
    {
        0, 1, 2, 2, 3, 0
    };

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // create vertex buffer object.
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // When a buffer is bound to GL_ELEMENT_ARRAY_BUFFER, all drawing commands of the form gl*Draw*Elements* will use indexes from that buffer.
    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * ibuffer.size(), &ibuffer[0], GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texturecords)));
    glVertexAttribIPointer(2, 1, GL_INT, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, textureid)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // Use shader class to parse and compile shader.
    m_shader.CreateShader();

    // set vao globally to bind in render.
    m_vao = vao;

    // set globally to delete buffers in destructor.
    m_vbo = vbo;
    m_ibo = ibo;
}

RawTexture::~RawTexture()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ibo);
}

void RawTexture::OnRender()
{
    m_shader.Bind();
    glBindVertexArray(m_vao);
    if (m_vao == 0) {
        std::cerr << "Error: VAO is not initialized!" << std::endl;
    }

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error before draw: " << err << std::endl;
    }

    // since the ibo was bound to vao we can set the last parameter to nullptr. 6 is based on num elements in ibuffer.
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error after draw: " << err << std::endl;
    }
}
}
