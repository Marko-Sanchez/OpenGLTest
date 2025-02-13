#pragma once

#include "vendors/glm/glm.hpp"
#include <string>
#include <unordered_map>

// Shaders are software that run on the GPU, this class takes
// two types of shaders Vertex shader (runs once per vertex) and
// Fragment shader (runs per pixel). This class parses, and compiles the
// shader source code. As well as modifies the value of a uniform variable/array
// within the shaders, all active uniforms are initialized to 0 when the program
// object is linked, the retain there assigned values by glUniform*(), until linked once
// again where there initialized to 0 again.
class Shaders
{
    private:
    // A program (Shader program) is an object that combines multiple shader stages
    // (vertex, fragment, etc) into a single executable that runs on the GPU.
    unsigned int m_programID;

    std::string m_vertexshader;
    std::string m_fragmentshader;

    std::unordered_map<std::string, int> m_uniformLocationCache;

    public:
    Shaders(const std::string& vertex, const std::string& fragment);
    ~Shaders();

    void Bind() const;
    void UnBind() const;

    std::string ParseShaderFile(const std::string& filepath);
    void CreateShader();

    // Set the value of a uniform in current shader.
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1iv(const std::string& name, int count, int* value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

    int GetUniformLocation(const std::string& name);

    private:
    unsigned int CompileShader(unsigned int type, const std::string& source);
};
