#include <RawTexture.h>

#include <cstddef>
#include <vector>

namespace tests
{

RawTexture::RawTexture()
:m_shader("../res/Shaders/Raw.vertex","../res/Shaders/Raw.fragment")
{
    const std::vector<Vertex> vertices =
    {
        {{-0.50f, -0.25f, 0.0f}, {0.0f, 0.0f}, 0},
        {{-0.25f, -0.25f, 0.0f}, {1.0f, 0.0f}, 0},
        {{-0.25f,  0.0f , 0.0f}, {1.0f, 1.0f}, 0},
        {{-0.50f,  0.0f , 0.0f}, {0.0f, 1.0f}, 0},


        {{0.25f, -0.25f, 0.0f}, {0.0f, 0.0f}, 1},
        {{0.50f, -0.25f, 0.0f}, {1.0f, 0.0f}, 1},
        {{0.50f,  0.0f , 0.0f}, {1.0f, 1.0f}, 1},
        {{0.25f,  0.0f , 0.0f}, {0.0f, 1.0f}, 1}
    };
    const std::vector<unsigned int> ibuffer =
    {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4
    };

    // VAO must be bound before vbo and ibo are bound.
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
    // make sure to compile before setting uniform, else
    // you wont be able to find the uniform in the shader and get
    // a 1282:INVALID_OPERATION error.
    m_shader.CreateShader();
    m_shader.Bind();

    // set vao globally to bind in render.
    m_vao = vao;

    // set globally to delete buffers in destructor.
    m_vbo = vbo;
    m_ibo = ibo;

    // Add textures.
    int textureSampler[2] = {0, 1};
    m_shader.SetUniform1iv("texturez", 2, textureSampler);

    GLuint cheese_texturename = m_texture.UploadTexture("cheese", "../res/images/cheese.png", 0);
    GLuint nacho_texturename = m_texture.UploadTexture("nacho", "../res/images/nachowink.png", 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cheese_texturename);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, nacho_texturename);
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

    // since the ibo was bound to vao we can set the last parameter to nullptr. 6 is based on num elements in ibuffer.
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
}
}
