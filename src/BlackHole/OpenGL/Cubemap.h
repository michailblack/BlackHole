#pragma once

struct CubemapSpecification
{
    std::string Right;
    std::string Left;
    std::string Top;
    std::string Bottom;
    std::string Back;
    std::string Front;
};

class Cubemap
{
public:
    Cubemap(const std::string& path);
    Cubemap(const CubemapSpecification& spec);
    ~Cubemap();

    void Bind(uint32_t slot = 0) const;
private:
    uint32_t m_RendererID;
    uint32_t m_Length;
};