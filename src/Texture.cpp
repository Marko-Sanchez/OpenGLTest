#include "Texture.h"

#include <iostream>
#include <filesystem>
#include <ostream>

#include "GL/glew.h"

#include "vendors/stb_image/stb_image.h"

Texture::Texture()
{}

Texture::~Texture()
{
    for(auto& i: m_textureNames)
    {
        glDeleteTextures(1, &i.first);
    }
}

unsigned int Texture::UploadTexture(const std::string& name, const std::string& imagePath, unsigned int slot = 0)
{
    unsigned char* buffer;
    int width{0}, height{0}, bpp{0};

    // On error, continue; allowing no texture to appear on screen.
    std::filesystem::path textpath = imagePath;
    if (!std::filesystem::exists(textpath))
    {
        std::cout << "Filepath " << imagePath << "/ndoes not exist." << std::endl;
    }

    // sometimes image might be flipped to setting this fixes it.
    stbi_set_flip_vertically_on_load(1);
    buffer = stbi_load(imagePath.c_str(), &width, &height, &bpp, 4);// 4 since we want rgba
    if (!buffer)
    {
        // if m_LocalBuffer is nullptr and passed to glTexImage2D() memory is
        // allocated to accomadate height and width.
        std::cout << "Failed to load texture" << std::endl;
        std::cout << stbi_failure_reason() << std::endl;
    }

    unsigned int textureName{0};
    glGenTextures(1, &textureName);
    glBindTexture(GL_TEXTURE_2D, textureName);

    // must specify these or will get black screen.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // send data to opengl.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    glBindTexture(GL_TEXTURE_2D, textureName);

    // frees image data.
    if (buffer)
        stbi_image_free(buffer);

    m_tMap[name] = m_textureNames.size();
    m_textureNames.emplace_back(textureName, slot);

    return textureName;
}

// select which texture unit the subsequent state calls will affect.
// There are 0 -> (GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS - 1 ) available.
void Texture::Bind(const std::string& name)
{
    if(m_tMap.find(name) != m_tMap.end())
    {
        auto tIndex = m_tMap[name];
        auto tName = m_textureNames[tIndex].first;
        auto tSlot = m_textureNames[tIndex].second;

        glActiveTexture(GL_TEXTURE0 + tSlot);
        glBindTexture(GL_TEXTURE_2D, tName);
    }
    else
    {
        std::cout << "Texture Name: " << name << " Not Found." << std::endl;
    }
}

void Texture::UnBind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Texture::GetTextureName(const std::string& name) const
{
    if (m_tMap.find(name) == m_tMap.end())
    {
        std::cout << "Name: " << name << " Not Found." << std::endl;
        return 0;
    }

    auto tIndex = m_tMap.at(name);
    auto tName = m_textureNames[tIndex].first;

    return tName;
}

int Texture::GetActiveTexture() const
{
    int texture;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &texture);

    return (texture - GL_TEXTURE0);
}
