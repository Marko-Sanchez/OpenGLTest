#include "Shaders.h"

#include <GL/glew.h>

#include <climits>
#include <cstddef>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <ostream>
#include <string>

Shaders::Shaders(const std::string& vertexFile, const std::string& fragmentFile)
    :m_programID(0)
{
    m_vertexshader = ParseShaderFile(vertexFile);
    m_fragmentshader = ParseShaderFile(fragmentFile);
}

Shaders::~Shaders()
{
    glDeleteProgram(m_programID);
}

unsigned int Shaders::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // check if shader compiled successfully, else print log error.
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        char *message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        std::cout << "Failed to Compile "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << " shader\n" << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

std::string Shaders::ParseShaderFile(const std::string& filepath)
{
    std::ifstream ifs(filepath);

    if (!std::filesystem::exists(filepath))
    {
        std::cout << "File " << filepath << " does not exits..." << std::endl;
        return "";
    }
    else if (!ifs.is_open())
    {
        std::cout << "Failed to Open file: " << filepath << std::endl;
        return "";
    }

    std::string line;
    std::string contents;

    auto size = ifs.tellg();
    if (size > 0)
        contents.reserve(static_cast<size_t>(size));

    while (getline(ifs, line))
    {
        contents.append(line + '\n');
    }

    return contents;
}

void Shaders::Bind() const
{
   glUseProgram(m_programID);
}

void Shaders::UnBind() const
{
    glUseProgram(0);
}

void Shaders::CreateShader()
{
    m_programID = glCreateProgram();
    if (m_programID == 0)
        std::cout << "Error occured creating program" << std::endl;

    unsigned int vs{CompileShader(GL_VERTEX_SHADER, m_vertexshader)};
    unsigned int fs{CompileShader(GL_FRAGMENT_SHADER, m_fragmentshader)};

    glAttachShader(m_programID, vs);
    glAttachShader(m_programID, fs);
    glLinkProgram(m_programID);
    glValidateProgram(m_programID);

    int linkresult, validateresult;
    glGetProgramiv(m_programID, GL_LINK_STATUS, &linkresult);
    glGetProgramiv(m_programID, GL_VALIDATE_STATUS, &validateresult);// debugging, heavy cost.
    if (linkresult == GL_FALSE || validateresult == GL_FALSE)
    {
        int loglength;
        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &loglength);

        char *message = (char*) alloca(loglength * sizeof(char));
        glGetProgramInfoLog(m_programID, loglength, &loglength, message);

        std::cout << "Program failed to link/validate:\n\n"
                  << message << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
}

// looks up uniform in cache, if not found does a expensive glGetUniformLocation() look up.
int Shaders::GetUniformLocation(const std::string& name)
{
    auto it = m_uniformLocationCache.find(name);
    if (it != m_uniformLocationCache.end())
        return it->second;

    int uLocation{glGetUniformLocation(m_programID, name.c_str())};
    if (uLocation != -1)
        m_uniformLocationCache[name] = uLocation;

    return uLocation;
}

void Shaders::SetUniform1i(const std::string& name, int value)
{
    int var_location{GetUniformLocation(name)};
    if (var_location <= -1)
        std::cout << "SetUniform1i: Variable Location Not Found" << std::endl;


    glUniform1i(var_location, value);
}

void Shaders::SetUniform1f(const std::string& name, float value)
{
    int var_location{GetUniformLocation(name)};
    if (var_location <= -1)
        std::cout << "SetUniform1f: Variable Location Not Found" << std::endl;

    glUniform1f(var_location, value);
}

void Shaders::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    int var_location{GetUniformLocation(name)};
    if (var_location <= -1)
        std::cout << "SetUniform4f: Variable Location Not Found" << std::endl;

    glUniform4f(var_location, v0, v1, v2, v3);
}

void Shaders::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    int var_location{GetUniformLocation(name)};
    if (var_location <= -1)
        std::cout << "SetUniformMat4f: Variable Location Not Found" << std::endl;

    glUniformMatrix4fv(var_location, 1, GL_FALSE, &matrix[0][0]);
}
