#pragma once

#include "Test.h"

#include "Shaders.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "glm/fwd.hpp"

namespace tests
{
    // Batch Rendering involves decreasing the calls to glDrawElements(), by increasing how many elements in draws.
    // One Quad ("square/rectangle") takes 6 vertices to draw; therefore, if we increase the 'count' from 6 -> 12
    // we can draw two quads for the price of one draw call; hence batch rendering.
    // Also includes an example of using glm::perspective().
    class BatchRendering: public Test
    {
        private:
        Shaders     m_shader;
        VertexArray m_va;
        IndexBuffer m_ib;
        VertexBuffer m_vb;

        glm::mat4 m_projectionMatrix;
        glm::mat4 m_viewMatrix;
        glm::vec3 m_translationMatrix;

        public:
        BatchRendering();

        void OnRender() override;
        void OnImGuiRender() override;
    };
}
