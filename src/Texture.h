#pragma once

#include <string>
#include <unordered_map>
#include <vector>

// Reads texture and sends the data to gpu.
// stb_image.h helps us with reading and loading the texture onto a local buffer,
// of which then glTexImage2D() loads onto gpu memory, allowing texture to be read by shader.
class Texture
{
    private:
    std::unordered_map<std::string, size_t> m_tMap;
    std::vector<std::pair<unsigned int, unsigned int>> m_textureNames;

    public:
    Texture();
    ~Texture();

    unsigned int UploadTexture(const std::string& name, const std::string& imagePath, unsigned int slot);
    void Bind(const std::string& name);
    void UnBind() const;

    unsigned int GetTextureName(const std::string& name) const;
    int GetActiveTexture() const;
};
