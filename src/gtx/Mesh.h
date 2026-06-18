#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

#include <vector>

class Mesh
{
    private:

        GLuint m_vao;
        GLuint m_vbo;
        GLuint m_ebo;

        void SetupMesh();

    public:

        struct Vertex
        {
            glm::vec3 position;
            glm::vec3 normal    = glm::vec3{0.f};
            glm::vec2 texcoords = glm::vec2{0.f};
        };

        std::vector<Vertex>  m_vertices;
        std::vector<GLuint>  m_indices;

        Mesh(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices);
        ~Mesh();

        void Draw();
};
