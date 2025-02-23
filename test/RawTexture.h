#pragma once

#include <Test.h>

#include <GL/glew.h>
#include "Shaders.h"

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

        Shaders m_shader;
        GLuint m_ibo;
        GLuint m_vbo;
        GLuint m_vao;

        public:
        RawTexture();
        ~RawTexture();

        void OnRender() override;
    };
}
