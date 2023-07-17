#pragma once

enum class TextureType
{
    None = -1,
    Diffuse,
    Specular
};

class Texture2D
{
public:
    explicit Texture2D(const std::string& path);
    ~Texture2D();

    void Bind(uint32_t slot = 0) const;
private:
    uint32_t m_RendererID;
    uint32_t m_Width, m_Height;
    unsigned int m_InternalFormat, m_DataFormat;
};
