#include "Renderer.h"

void Renderer::Draw(const VertexArray& va, const Shaders& shader, int numberOfElements /* = 6*/) const
{
        shader.Bind();
        va.Bind();

        glDrawElements(GL_TRIANGLES, numberOfElements, GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear() const
{
        glClear(GL_COLOR_BUFFER_BIT);
}
