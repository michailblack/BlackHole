#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

struct ShaderSpecification
{
    std::filesystem::path VertexPath = "";
    std::filesystem::path FragmentPath = "";
    std::filesystem::path GeometryPath = "";
};

class Shader
{
public:
    explicit Shader(const std::filesystem::path& filepath);
    explicit Shader(std::string name, const ShaderSpecification& specification);
    ~Shader();

    void Bind() const;

    void UploadInt(const std::string& name, int32_t value) const;
    void UploadIntArray(const std::string& name, uint32_t count, const int32_t* values) const;
    void UploadUint(const std::string& name, uint32_t value) const;
    void UploadFloat(const std::string& name, float value) const;
    void UploadFloat3(const std::string& name, const glm::vec3& vector) const;
    void UploadMat4(const std::string& name, const glm::mat4& matrix) const;

    const std::string& GetName() const { return m_Name; }
private:
    struct UniformInfo
    {
        uint32_t ProgramID;
        int32_t Location;
        int32_t Count;
    };
private:
    void ProcessShaderFile(const std::string& shaderSources);
    void CreateProgram();

    UniformInfo GetUniformInfo(const std::string& name) const;
    void CollectUniformLocations(uint32_t programID) const;
private:
    uint32_t m_RendererID;
    std::string m_Name;

    std::unordered_map<uint32_t, uint32_t> m_ProgramIDs;
    std::unordered_map<uint32_t, std::string> m_ShaderSourceCode;
    mutable std::unordered_map<std::string, UniformInfo> m_UniformLocationCache;
};

class ShaderLibrary
{
public:
    void Add(const Ref<Shader>& shader);

    Ref<Shader> Load(const std::filesystem::path& filepath);
    Ref<Shader> Load(const std::string& name, const ShaderSpecification& spec);

    Ref<Shader> Get(const std::string& name) const;

    bool Exists(const std::string& name) const;
private:
    std::unordered_map<std::string, Ref<Shader>> m_Shaders;
};