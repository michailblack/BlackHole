#pragma once
#include <cstdint>
#include <string>

class Shader
{
public:
    explicit Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~Shader();

    void Bind() const;
    void Unbind() const;
private:
    uint32_t m_RendererID;
};