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

        // first value: texture name, second: texture slot.
        using nameslot = std::pair<unsigned int, unsigned int>;
        std::unordered_map<std::string, nameslot> m_textures;

    public:

        Texture();
        ~Texture();

        unsigned int UploadCubeMap(const std::array<std::filesystem::path, 6>& faces);
        unsigned int UploadTexture(const std::string& name, const std::filesystem::path& imagePath, unsigned int textureSlot);
        unsigned int UploadBMP(const std::filesystem::path& imagePath, unsigned int textureSlot);

        void Bind(const std::string& name, GLenum target = GL_TEXTURE_2D);
        void UnBind() const;

        unsigned int GetTextureName(const std::string& name) const;
        int GetActiveTexture() const;
};
