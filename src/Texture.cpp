#include "Texture.h"

#include <iostream>
#include <filesystem>
#include <memory>
#include <ostream>
#include <utility>

#include "GL/glew.h"

#include "vendors/stb_image/stb_image.h"

Texture::Texture()
{}

Texture::~Texture()
{
    for(auto& i: m_textures)
    {
        glDeleteTextures(1, &i.second.first);
    }
}

unsigned int Texture::UploadTexture(const std::string& name, const std::string& imagePath, unsigned int textureSlot = 0)
{
    std::filesystem::path textpath{imagePath};
    if (!std::filesystem::exists(textpath))
    {
        std::cerr << "Filepath " << imagePath << "\ndoes not exist." << std::endl;
        return GLuint(-1);
    }
    else if (m_textures.find(name) != m_textures.end())
    {
        std::cerr << "Texture with the name: (" << name << ") Already Exist !" << std::endl;
        return GLuint(-1);
    }

    // sometimes image might be flipped to setting this fixes it.
    stbi_set_flip_vertically_on_load(1);

    int width{0}, height{0}, bpp{0};
    std::unique_ptr<unsigned char, decltype(&stbi_image_free)> buffer(stbi_load(imagePath.c_str(), &width, &height, &bpp, 4), &stbi_image_free);
    if (buffer)
    {
        unsigned int textureName{0};
        glGenTextures(1, &textureName);
        glActiveTexture(GL_TEXTURE0 + textureSlot);
        glBindTexture(GL_TEXTURE_2D, textureName);

        // must specify these or will get black screen.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // send data to opengl.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());

        m_textures[name] = std::make_pair(textureName, textureSlot);

        return textureName;
    }
    else
    {
        std::cerr << "Failed to load texture" << std::endl;
        std::cerr << stbi_failure_reason() << std::endl;
        return GLuint(-1);
    }
}

/*
* Select which texture unit the subsequent state calls will affect.
* There are 0 -> (GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS - 1 ) texture slots available.
*/
void Texture::Bind(const std::string& name)
{
    if(auto it = m_textures.find(name); it != m_textures.end())
    {
        glActiveTexture(GL_TEXTURE0 + it->second.second);
        glBindTexture(GL_TEXTURE_2D, it->second.first);
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

/*
* Finds texture name in unordered_map returns unsigned int 'texturename'.
*
* @param:
* {const std::string& name}: name of texture.
*/
unsigned int Texture::GetTextureName(const std::string& name) const
{
    if (auto it = m_textures.find(name); it != m_textures.end())
    {
        return it->second.first;
    }

    std::cerr << "Name: " << name << " Not Found." << std::endl;
    return GLuint(-1);
}

/*
* Returns a single value indicating the current multitexture unit. Initial value is 0.
*/
int Texture::GetActiveTexture() const
{
    int texture;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &texture);

    return (texture - GL_TEXTURE0);
}
