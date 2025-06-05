#pragma once

#include <vector>
#include <string>
#include <unordered_map>

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

    unsigned int UploadCubeMap(const std::vector<std::string>& faces);
    unsigned int UploadTexture(const std::string& name, const std::string& imagePath, unsigned int textureSlot);
    unsigned int UploadBMP(const std::string& imagePath, unsigned int textureSlot);

    void Bind(const std::string& name);
    void UnBind() const;

    unsigned int GetTextureName(const std::string& name) const;
    int GetActiveTexture() const;
};
