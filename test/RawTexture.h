#pragma once

#include <Test.h>

#include <GL/glew.h>
#include "Shaders.h"
#include "Texture.h"

namespace tests
{
    class RawTexture: public Test
    {
        private:
        struct Vertex
        {
            float position[3];
            float texturecords[2];
            int textureid;
        };

        Texture m_texture;
        Shaders m_shader;
        GLuint m_ibo;
        GLuint m_vbo;
        GLuint m_vao;

        public:
        RawTexture(std::shared_ptr<void> window);
        ~RawTexture();

        void OnRender() override;
    };
}
