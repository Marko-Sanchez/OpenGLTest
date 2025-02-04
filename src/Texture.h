#pragma once

#include <string>

// Reads texture and sends the data to gpu.
// stb_image.h helps us with reading and loading the texture onto a local buffer,
// of which then glTexImage2D() loads onto gpu memory.
class Texture
{
    private:
    unsigned int m_rendererID;
    int m_Width;
    int m_Height;
    int m_BPP;

    unsigned char* m_LocalBuffer;
    std::string m_FilePath;

    public:
    Texture(const std::string& path);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void UnBind() const;

    inline int GetHeight() const {return m_Height;}
    inline int GetWidth() const {return m_Width;}
};
