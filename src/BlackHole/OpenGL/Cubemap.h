#pragma once

struct CubemapSpecification
{
    std::filesystem::path Right;
    std::filesystem::path Left;
    std::filesystem::path Top;
    std::filesystem::path Bottom;
    std::filesystem::path Back;
    std::filesystem::path Front;
};

class Cubemap
{
public:
    Cubemap(const CubemapSpecification& spec);
    ~Cubemap();

    void Bind(uint32_t slot = 0) const;
private:
    uint32_t m_RendererID;
    uint32_t m_Length;
    uint32_t m_InternalFormat, m_DataFormat;
};