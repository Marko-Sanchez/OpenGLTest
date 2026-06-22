#include "Texture.h"

#include <cstdio>
#include <cstring>
#include <iostream>
#include <filesystem>
#include <memory>
#include <ostream>

#include "GL/glew.h"

#include <stb_image.h>

Texture::Texture()
{}

Texture::~Texture()
{
    for(const auto&[textureName, textureID]: m_textures)
    {
        glDeleteTextures(1, &textureID);
    }
}

GLuint Texture::UploadCubeMap(const std::string& name, const std::array<std::filesystem::path, 6>& faces)
{
    stbi_set_flip_vertically_on_load(false);

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

    m_textures[name] = textureID;
    return textureID;
}

GLuint Texture::UploadTexture(const std::string& name, const std::filesystem::path& imagePath)
{
    stbi_set_flip_vertically_on_load(true);

    if (!std::filesystem::exists(imagePath))
    {
        std::cerr << "Filepath " << imagePath << "\ndoes not exist." << std::endl;
        return GLuint(-1);
    }
    else if (auto iter = m_textures.find(name); iter != m_textures.end())
    {
        return iter->second;
    }

    int width{0}, height{0}, bpp{0};
    std::unique_ptr<unsigned char[], decltype(stbi_image_free) *> buffer(stbi_load(imagePath.c_str(), &width, &height, &bpp, 4), stbi_image_free);
    if (buffer)
    {
        unsigned int textureID{0};
        glGenTextures(1, &textureID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);

        // must specify these or will get black screen.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // send data to opengl.
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());
        glGenerateMipmap(GL_TEXTURE_2D);

        m_textures[name] = textureID;

        return textureID;
    }
    else
    {
        std::cerr << "Failed to load texture" << std::endl;
        std::cerr << stbi_failure_reason() << std::endl;
        return GLuint(-1);
    }
}

GLuint Texture::UploadBMP(const std::filesystem::path& imagePath)
{
    const int BMP_HEADER_SIZE{54};

    unsigned char header[BMP_HEADER_SIZE];
    unsigned int dataPos, width, height, imageSize;

    // hidden attributes attached to std::fclose() by gcc are passed to unique_ptr, compiler does not understand
    // how to intepret those attributes; therfore, drops them and throws a warning.
    auto filedeleter = [](FILE* file){if (file) std::fclose(file);};
    std::unique_ptr<FILE, decltype(filedeleter)> uptr_file(std::fopen(imagePath.c_str(), "rb"), filedeleter);
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

    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data.get());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // ... which requires mipmaps. Generate them automatically.
    glGenerateMipmap(GL_TEXTURE_2D);

    m_textures["BMP"] = textureID;

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        std::cerr << "UploadBMP(): OpenGL Error ->" << err << std::endl;
    }

    return textureID;
}

/*
* Select which texture unit the subsequent state calls will affect.
*/
void Texture::Bind(const std::string& name, GLenum target)
{

    if(auto it = m_textures.find(name); it != m_textures.end())
    {
        glBindTexture(target, it->second);
    }
    else
    {
        std::cout << "Texture Name: " << name << " Not Found." << std::endl;
    }
}

void Texture::UnBind(GLenum target) const
{
    glBindTexture(target, 0);
}

/*
* Finds texture name in unordered_map returns unsigned int 'texturename'.
*
* @param:
* {const std::string& name}: name of texture.
*/
GLuint Texture::GetTextureName(const std::string& name) const
{
    if (auto it = m_textures.find(name); it != m_textures.end())
    {
        return it->second;
    }

    std::cerr << "Name: " << name << " Not Found." << std::endl;
    return GLuint(-1);
}
