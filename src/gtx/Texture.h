#pragma once

#include <array>
#include <string>
#include <unordered_map>
#include <filesystem>

#include "GL/glew.h"

// Reads texture and sends the data to gpu.
// stb_image.h helps us with reading and loading the texture onto a local buffer,
// of which then glTexImage2D() loads onto gpu memory, allowing texture to be read by shader.
class Texture
{
    private:

        std::unordered_map<std::string, GLuint> m_textures;

    public:

        Texture();
        ~Texture();

        Texture(const Texture&)            = delete;
        Texture& operator=(const Texture&) = delete;
        Texture(Texture&&)                 = delete;
        Texture& operator=(Texture&&)      = delete;


        GLuint UploadCubeMap(const std::string& name, const std::array<std::filesystem::path, 6>& faces);
        GLuint UploadTexture(const std::string& name, const std::filesystem::path& imagePath);
        GLuint UploadBMP(const std::filesystem::path& imagePath);

        void Bind(const std::string& name, GLenum target = GL_TEXTURE_2D);
        void UnBind(GLenum target = GL_TEXTURE_2D) const;

        GLuint GetTextureName(const std::string& name) const;
};
