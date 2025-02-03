#pragma once

#include "Shaders.h"
#include "VertexArray.h"

// Handles draw call
class Renderer
{
    public:
    void Draw(const VertexArray& va, const Shaders& shader, int numberOfElements = 6) const;
    void Clear() const;
};
