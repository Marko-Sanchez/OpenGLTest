#pragma once

#include <GL/gl.h>
struct Debugger
{
    GLenum glCheckError_(const char* file, int line);
};

#define GLCHECKERROR(d) d.glCheckError_(__FILE__, __LINE__)
