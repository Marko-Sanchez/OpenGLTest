#pragma once

#include <GL/gl.h>

GLenum glCheckError_(const char* file, int line);

#define GLCHECKERROR() glCheckError_(__FILE__, __LINE__)
