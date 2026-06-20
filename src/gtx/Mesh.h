#pragma once

#include "gtx/Shaders.h"
#include <glm/glm.hpp>
#include <GL/glew.h>

#include <vector>
#include <string>

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

        struct Texture
        {
            GLuint texID;
            std::string name;
            std::string type;
        };

        std::vector<Vertex>  m_vertices;
        std::vector<GLuint>  m_indices;
        std::vector<Texture> m_textures;

        Mesh(std::vector<Vertex>&& vertices, std::vector<GLuint>&& indices, std::vector<Texture>&& textures);
        Mesh(Mesh&& other);

        Mesh(const Mesh& other)      = delete;
        Mesh& operator=(const Mesh&) = delete;
        Mesh& operator=(Mesh&&)      = delete;

        ~Mesh();

        void Draw(Shaders& shader);
};
