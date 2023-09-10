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
    explicit Texture2D(const std::filesystem::path& texturePath, bool useGammaCorrection = false);
    ~Texture2D();

    void Bind(uint32_t slot = 0) const;
private:
    uint32_t m_RendererID;
    uint32_t m_Width, m_Height;
    uint32_t m_InternalFormat, m_DataFormat;
    bool m_NeedToBeGammaCorrected;
};


class TextureArray2D
{
public:
    explicit TextureArray2D(const std::filesystem::path& texturePath, uint32_t layers, bool useGammaCorrection = false);
    ~TextureArray2D();

    void Bind(uint32_t slot = 0);

    void PushBack(const std::filesystem::path& texturePath);

    const std::vector<std::string>& GetTextureKeys() const { return m_TextureKeys; }
private:
    uint32_t m_RendererID;
    uint32_t m_Width, m_Height;
    uint32_t m_InternalFormat, m_DataFormat;
    std::vector<std::string> m_TextureKeys;
    bool m_NeedToBeGammaCorrected;
};