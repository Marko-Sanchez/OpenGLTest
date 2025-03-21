#pragma once

#include "Test.h"

#include "GL/glew.h"
#include "glm/glm.hpp"

#include "Shaders.h"
#include "Texture.h"
#include <memory>

namespace tests
{
class TexturedCube: public Test
{
private:
    struct Vertex
    {
        glm::vec3 pos;
        glm::vec2 tcords;
    };


    std::unique_ptr<Shaders> uptrShader;
    std::unique_ptr<Texture> uptrTexture;

    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_ibo;

public:
    TexturedCube();
    ~TexturedCube();

    glm::mat4 CalculateMVP();
    void OnRender() override;
};
}
