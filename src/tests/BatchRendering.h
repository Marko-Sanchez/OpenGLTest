#pragma once

#include "Test.h"

#include <glm/fwd.hpp>

#include "gtx/Shaders.h"
#include "gtx/VertexArray.h"
#include "gtx/VertexBuffer.h"
#include "gtx/IndexBuffer.h"

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

        BatchRendering(std::shared_ptr<void> window);

        void OnRender() override;
        void OnImGuiRender() override;
};
}
