#include "Texture.h"

#include <iostream>
#include <filesystem>
#include <ostream>

#include "GL/glew.h"

#include "vendors/stb_image/stb_image.h"

Texture::Texture(const std::string& path)
    :m_rendererID(0), m_FilePath(path), m_Width(0), m_Height(0), m_BPP(0), m_LocalBuffer(nullptr)
{
    // On error, continue; allowing no texture to appear on screen.
    std::filesystem::path textpath = path;
    if (!std::filesystem::exists(textpath))
    {
        std::cout << "Filepath " << path << "/ndoes not exist." << std::endl;
    }

    // sometimes image might be flipped to setting this fixes it.
    stbi_set_flip_vertically_on_load(1);
    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);// 4 since we want rgba
    if (!m_LocalBuffer)
    {
        // if m_LocalBuffer is nullptr and passed to glTexImage2D() memory is
        // allocated to accomadate height and width.
        std::cout << "Failed to load texture" << std::endl;
        std::cout << stbi_failure_reason() << std::endl;
    }

    glGenTextures(1, &m_rendererID);
    glBindTexture(GL_TEXTURE_2D, m_rendererID);

    // must specify these or will get black screen.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // send data to opengl.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
    glBindTexture(GL_TEXTURE_2D, m_rendererID);

    // frees image data.
    if (m_LocalBuffer)
        stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_rendererID);
}

void Texture::Bind(unsigned int slot) const
{
    // select which texture unit the subsequent state calls wil affect.
    // There are 0 -> (GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS - 1 ) available.
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_rendererID);
}

void Texture::UnBind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
