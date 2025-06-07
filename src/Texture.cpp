#include "Texture.h"

#include <cstdio>
#include <cstring>
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
    for(const auto&[textureName, textureSlot]: m_textures)
    {
        glDeleteTextures(1, &textureSlot.first);
    }
}

unsigned int Texture::UploadCubeMap(const std::vector<std::string>& faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i{0}; i < faces.size(); ++i)
    {
        std::unique_ptr<unsigned char[], decltype(stbi_image_free) *> buffer(stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0), stbi_image_free);
        if (buffer)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer.get());
        }
        else
        {
            std::cerr << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            return GLuint(-1);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_textures["cubemap"] = std::make_pair(textureID, GL_TEXTURE0);
    return textureID;
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
    std::unique_ptr<unsigned char[], decltype(stbi_image_free) *> buffer(stbi_load(imagePath.c_str(), &width, &height, &bpp, 4), stbi_image_free);
    if (buffer)
    {
        unsigned int textureID{0};
        glGenTextures(1, &textureID);
        glActiveTexture(GL_TEXTURE0 + textureSlot);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // must specify these or will get black screen.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // send data to opengl.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());

        m_textures[name] = std::make_pair(textureID, textureSlot);

        return textureID;
    }
    else
    {
        std::cerr << "Failed to load texture" << std::endl;
        std::cerr << stbi_failure_reason() << std::endl;
        return GLuint(-1);
    }
}

unsigned int Texture::UploadBMP(const std::string& imagePath, unsigned int textureSlot)
{
    const int BMP_HEADER_SIZE{54};

    unsigned char header[BMP_HEADER_SIZE];
    unsigned int dataPos, width, height, imageSize;

    std::unique_ptr<FILE, decltype(std::fclose) *> uptr_file(std::fopen(imagePath.c_str(), "rb"), std::fclose);
    if ( (std::fread(header, 1, BMP_HEADER_SIZE, uptr_file.get()) != BMP_HEADER_SIZE)
        and (header[0] != 'B' or header[1] != 'M') )
    {
        std::cerr << "Not a BMP file" << std::endl;
        return uint(-1);
    }

    std::memcpy(&dataPos, &header[0x0A], sizeof(dataPos));
    std::memcpy(&imageSize, &header[0x22], sizeof(imageSize));
    std::memcpy(&width, &header[0x12], sizeof(width));
    std::memcpy(&height, &header[0x16], sizeof(height));

    if (imageSize == 0) imageSize = width * height * 3;
    if (dataPos == 0)  dataPos = 54;

    auto data = std::make_unique<unsigned char[]>(imageSize);
    if (std::fread(data.get(), 1, imageSize, uptr_file.get()) != imageSize)
    {
        std::cerr << "Failed to read BMP image data" << std::endl;
        return uint(-1);
    }

    if (!data)
    {
        std::cerr << "Data is Null" << std::endl;
        return uint(-1);
    }

    unsigned int textureName;
    glGenTextures(1, &textureName);
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_2D, textureName);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data.get());
    /* glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); */
    /* glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); */

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // ... which requires mipmaps. Generate them automatically.
    glGenerateMipmap(GL_TEXTURE_2D);

    m_textures["BMP"] = std::make_pair(textureName, textureSlot);

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL Error: " << err << std::endl;
    }

    return textureName;
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
