#include "Mesh.h"

#include <cstddef>

#include <GL/gl.h>

Mesh::Mesh(std::vector<Vertex>&& verticez, std::vector<GLuint>&& indicez, std::vector<Texture>&& texturez)
    :vertices(std::move(verticez)),
    indices(std::move(indicez)),
    textures(std::move(texturez))
{
    this->SetupMesh();
}

Mesh::Mesh(Mesh&& other)
    :vertices(std::move(other.vertices)),
    indices(std::move(other.indices)),
    textures(std::move(other.textures)),
    m_vao(other.m_vao),
    m_vbo(other.m_vbo),
    m_ebo(other.m_ebo)
{
    other.m_vao = 0;
    other.m_vbo = 0;
    other.m_ebo = 0;
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
    glDeleteVertexArrays(1, &m_vao);
}

void Mesh::SetupMesh()
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // VBO must be bound before glVertexAttribPointer so the VAO
    // records a valid buffer reference for each attribute
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // EBO is stored directly in the VAO — must be bound while VAO is active
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texcoords)));

    glBindVertexArray(0);
}

void Mesh::Draw(Shaders& shader)
{
    // bind textures.
    GLuint diffuseNr {1}, specularNr {1}, normalNr {1}, heightNr {1};
    for (GLuint i {0}; i < textures.size(); ++i)
    {
        std::string name {textures[i].type};

        if (name == "texture_diffuse")
            name += std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            name += std::to_string(specularNr++);
        else if (name == "texture_normal")
            name += std::to_string(normalNr++);
        else if (name == "texture_height")
            name += std::to_string(heightNr++);

        glActiveTexture(GL_TEXTURE0 + i);
        shader.SetUniform1i(name, i);
        glBindTexture(GL_TEXTURE_2D, textures[i].texID);
    }

    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}

GLuint Mesh::GetVAO() const
{
    return m_vao;
}
