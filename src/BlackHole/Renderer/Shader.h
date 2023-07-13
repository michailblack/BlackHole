#pragma once

#include <glad/glad.h>

class Shader
{
public:
    explicit Shader(const std::string& filepath);
    explicit Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~Shader();

    void Bind() const;
    static void Unbind();

    void UploadInt(const std::string& name, int value) const;
    void UploadFloat(const std::string& name, float value) const;
    void UploadFloat3(const std::string& name, const glm::vec3& vector) const;
    void UploadMat4(const std::string& name, const glm::mat4& matrix) const;

private:
    static std::string ReadFile(const std::string& filepath);
    static GLenum ShaderTypeFromStringKeyword(const std::string& keyword);
    void ProcessShaderFile(const std::string& shaderSources);
    void CreateProgram();
private:
    uint32_t m_RendererID;
    std::unordered_map<GLenum, std::string> m_ShaderSourceCode;
};