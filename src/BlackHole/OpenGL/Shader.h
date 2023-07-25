#pragma once

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Shader
{
public:
    explicit Shader(const std::string& filepath);
    explicit Shader(std::string name, const std::string& vertexSrc, const std::string& fragmentSrc);
    ~Shader();

    void Bind() const;
    static void Unbind();

    void UploadInt(const std::string& name, int value) const;
    void UploadFloat(const std::string& name, float value) const;
    void UploadFloat3(const std::string& name, const glm::vec3& vector) const;
    void UploadMat4(const std::string& name, const glm::mat4& matrix) const;

    const std::string& GetName() const { return m_Name; }
private:
    static std::string ReadFile(const std::string& filepath);
    static GLenum ShaderTypeFromStringKeyword(const std::string& keyword);
    void ProcessShaderFile(const std::string& shaderSources);
    void CreateProgram();

    GLint GetUniformLocation(const std::string& name) const;
private:
    uint32_t m_RendererID;
    std::string m_Name;

    std::unordered_map<GLenum, std::string> m_ShaderSourceCode;
    mutable std::unordered_map<std::string, GLint> m_UniformLocationCache;
};

class ShaderLibrary
{
public:
    void Add(const Ref<Shader>& shader);

    Ref<Shader> Load(const std::string& filepath);
    Ref<Shader> Load(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

    Ref<Shader> Get(const std::string& name) const;

    bool Exists(const std::string& name) const;
private:
    std::unordered_map<std::string, Ref<Shader>> m_Shaders;
};