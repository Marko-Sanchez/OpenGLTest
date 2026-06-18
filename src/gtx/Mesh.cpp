#include "Mesh.h"

#include <cstddef>

#include <GL/gl.h>
#include <iostream>

Mesh::Mesh(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices)
    :m_vertices(std::move(vertices)),
    m_indices(std::move(indices))
{
    this->SetupMesh();
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
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texcoords)));

    // EBO is stored directly in the VAO — must be bound while VAO is active
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), m_indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    if (GLenum err = glGetError(); err != GL_NO_ERROR)
    {
        std::cerr << "OpenGL: Error setting up Mesh:\n" << err << std::endl;
    }
}

void Mesh::Draw()
{
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}
