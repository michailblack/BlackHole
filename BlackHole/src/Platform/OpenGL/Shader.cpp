#include "bhpch.h"
#include "Platform/OpenGL/Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Utils
{
    static std::string ReadFile(const std::string& filepath)
    {
        std::ifstream shaderFile(filepath, std::ios::in | std::ios::binary);
        std::string shaderSrc;

        if (shaderFile.is_open())
	    {
		    shaderFile.seekg(0, std::ios::end);
		    const size_t size = shaderFile.tellg();
		    if (size != static_cast<size_t>(-1))
		    {
			    shaderSrc.resize(size);
			    shaderFile.seekg(0, std::ios::beg);
			    shaderFile.read(shaderSrc.data(), static_cast<std::streamsize>(size));
		    }
		    else
		    {
			    BH_LOG_ERROR("Could not read from file '{0}'", filepath);
		    }
	    }
	    else
	    {
		    BH_LOG_ERROR("Could not open file '{0}'", filepath);
	    }

        return shaderSrc;
    }

    static uint32_t ShaderTypeFromStringKeyword(const std::string& keyword)
    {
        if (keyword == "vertex")
            return GL_VERTEX_SHADER;
        if (keyword == "fragment")
            return GL_FRAGMENT_SHADER;
        if (keyword == "geometry")
            return GL_GEOMETRY_SHADER;

        BH_ASSERT(false, "Unknown shader type keyword!");
        return 0;
    }

    static uint32_t ShaderStageFromShaderType(const uint32_t type)
    {
        switch (type)
        {
        case GL_VERTEX_SHADER: return GL_VERTEX_SHADER_BIT;
        case GL_FRAGMENT_SHADER: return GL_FRAGMENT_SHADER_BIT;
        case GL_GEOMETRY_SHADER: return GL_GEOMETRY_SHADER_BIT;
        }

        BH_ASSERT(false, "Unknown Shader type!");
        return 0;
    }
}

Shader::Shader(const std::filesystem::path& filepath)
    : m_RendererID(0)
{
    const std::string shaderFileSrc = Utils::ReadFile(filepath.string());
    ProcessShaderFile(shaderFileSrc);

    CreateProgram();

    m_Name = filepath.filename().stem().string();
}

Shader::Shader(std::string name, const ShaderSpecification& specification)
    : m_RendererID(0)
    , m_Name(std::move(name))
{
    BH_ASSERT(specification.VertexPath.has_filename(), "Can't create shade without Vertex Shader!");
    BH_ASSERT(specification.FragmentPath.has_filename(), "Can't create shade without Fragment Shader!");

    m_ShaderSourceCode[GL_VERTEX_SHADER]   = Utils::ReadFile(specification.VertexPath.string());
    m_ShaderSourceCode[GL_FRAGMENT_SHADER] = Utils::ReadFile(specification.FragmentPath.string());

    if (specification.GeometryPath.has_filename())
        m_ShaderSourceCode[GL_GEOMETRY_SHADER] = Utils::ReadFile(specification.GeometryPath.string());

    CreateProgram();
}

Shader::~Shader()
{
    glDeleteProgramPipelines(1, &m_RendererID);
}

void Shader::Bind() const
{
    glBindProgramPipeline(m_RendererID);
}

void Shader::UploadInt(const std::string& name, int32_t value) const
{
    const auto& uniformInfo = GetUniformInfo(name);
    glProgramUniform1i(uniformInfo.ProgramID, uniformInfo.Location, value);
}

void Shader::UploadIntArray(const std::string& name, uint32_t count, const int32_t* values) const
{
    const auto& uniformInfo = GetUniformInfo(name);
    glProgramUniform1iv(uniformInfo.ProgramID, uniformInfo.Location, count, values);
}

void Shader::UploadUint(const std::string& name, uint32_t value) const
{
    const auto& uniformInfo = GetUniformInfo(name);
    glProgramUniform1ui(uniformInfo.ProgramID, uniformInfo.Location, value);
}

void Shader::UploadFloat(const std::string& name, float value) const
{
    const auto& uniformInfo = GetUniformInfo(name);
    glProgramUniform1f(uniformInfo.ProgramID, uniformInfo.Location, value);
}

void Shader::UploadFloat3(const std::string& name, const glm::vec3& vector) const
{
    const auto& uniformInfo = GetUniformInfo(name);
    glProgramUniform3f(uniformInfo.ProgramID, uniformInfo.Location, vector.x, vector.y, vector.z);
}

void Shader::UploadMat4(const std::string& name, const glm::mat4& matrix) const
{
    const auto& uniformInfo = GetUniformInfo(name);
    glProgramUniformMatrix4fv(uniformInfo.ProgramID, uniformInfo.Location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::ProcessShaderFile(const std::string& shaderSources)
{
    const char* typeToken = "#type";
    const size_t tokenSize = strlen(typeToken);

    size_t pos = shaderSources.find(typeToken);
    while (pos != std::string::npos)
    {
        // Find the end of shader type declaration line
        const size_t eol = shaderSources.find_first_of("\r\n", pos);
        BH_ASSERT(eol != std::string::npos, "Syntax error!");

        const size_t beginOfShaderTypeKeyword = pos + tokenSize + 1;
        std::string shaderType = shaderSources.substr(beginOfShaderTypeKeyword, eol - beginOfShaderTypeKeyword);

        const size_t startOfShaderSourceCode = shaderSources.find_first_not_of("\r\n", eol);
        BH_ASSERT(startOfShaderSourceCode != std::string::npos, "Syntax error!");
        pos = shaderSources.find(typeToken, startOfShaderSourceCode);

        m_ShaderSourceCode[Utils::ShaderTypeFromStringKeyword(shaderType)] = (pos == std::string::npos) ?
            shaderSources.substr(startOfShaderSourceCode) :
            shaderSources.substr(startOfShaderSourceCode, pos - startOfShaderSourceCode);
    }
}

void Shader::CreateProgram()
{
    glCreateProgramPipelines(1, &m_RendererID);

    for (const auto& [shaderType, shaderSource] : m_ShaderSourceCode)
    {
        const GLchar* const source = shaderSource.data();
        const uint32_t shaderProgram = glCreateShaderProgramv(shaderType, 1, &source);

        GLint isLinked = 0;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinked);
        if (isLinked == GL_FALSE)
        {
	        GLint maxLength = 0;
	        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

	        std::vector<GLchar> infoLog(maxLength);
	        glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, infoLog.data());

	        glDeleteProgram(shaderProgram);

            BH_ASSERT(false, std::string("Shader link error: ") + infoLog.data());
	        return;
        }

        m_ProgramIDs[shaderType] = shaderProgram;

        CollectUniformLocations(shaderProgram);

        glUseProgramStages(m_RendererID, Utils::ShaderStageFromShaderType(shaderType), shaderProgram);
    }
}

Shader::UniformInfo Shader::GetUniformInfo(const std::string& name) const
{
    const auto& it = m_UniformLocationCache.find(name);
    if (it != m_UniformLocationCache.end())
        return  it->second;

    BH_LOG_WARN("[Shader] The uniform '{0}' doesn't exist.", name);

    return Shader::UniformInfo{m_ProgramIDs.at(GL_VERTEX_SHADER), -1, 0};
}

void Shader::CollectUniformLocations(uint32_t programID) const
{
    int32_t uniformCount = 0;
    glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &uniformCount);

    if (uniformCount != 0)
    {
	    int32_t maxNameLength = 0;
	    int32_t length = 0;
	    int32_t count = 0;
	    uint32_t type = GL_NONE;

	    glGetProgramiv(programID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);

        auto* const uniformName = new char[maxNameLength];

	    for (GLint i = 0; i < uniformCount; ++i)
	    {
		    glGetActiveUniform(programID, i, maxNameLength, &length, &count, &type, uniformName);

            UniformInfo uniformInfo = {};
            uniformInfo.ProgramID = programID;
            uniformInfo.Location = glGetUniformLocation(programID, uniformName);
            uniformInfo.Count = count;

		    m_UniformLocationCache.emplace(std::make_pair(std::string(uniformName), uniformInfo));
	    }

        delete[] uniformName;
    }
}

void ShaderLibrary::Add(const Ref<Shader>& shader)
{
    const auto& name = shader->GetName();
    BH_ASSERT(!Exists(name), "Shader already exists!");
    m_Shaders[name] = shader;
}

Ref<Shader> ShaderLibrary::Load(const std::filesystem::path& filepath)
{
    const auto& shader = CreateRef<Shader>(filepath);
    Add(shader);
    return shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string& name, const ShaderSpecification& spec)
{
    const auto& shader = CreateRef<Shader>(name, spec);
    Add(shader);
    return shader;
}

Ref<Shader> ShaderLibrary::Get(const std::string& name) const
{
    BH_ASSERT(Exists(name), "Shader not found!");
    return m_Shaders.at(name);
}

bool ShaderLibrary::Exists(const std::string& name) const
{
    return m_Shaders.contains(name);
}