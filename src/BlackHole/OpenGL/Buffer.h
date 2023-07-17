#pragma once

enum class ShaderDataType : uint16_t
{
    None = 0,
    Float, Float2, Float3, Float4,
    Int,   Int2,   Int3,   Int4,
    Mat3,  Mat4,
    Bool
};

namespace Utils
{
    static uint32_t GetShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:   return sizeof(float);
        case ShaderDataType::Float2:  return sizeof(float) * 2;
        case ShaderDataType::Float3:  return sizeof(float) * 3;
        case ShaderDataType::Float4:  return sizeof(float) * 4;
        case ShaderDataType::Int:     return sizeof(int);
        case ShaderDataType::Int2:    return sizeof(int) * 2;
        case ShaderDataType::Int3:    return sizeof(int) * 3;
        case ShaderDataType::Int4:    return sizeof(int) * 4;
        case ShaderDataType::Mat3:    return sizeof(float) * 3 * 3;
        case ShaderDataType::Mat4:    return sizeof(float) * 4 * 4;
        case ShaderDataType::Bool:    return sizeof(bool);
        default: BH_ASSERT(false, "Unknown ShaderDatatType!"); return 0;
        }
    }
}

struct BufferElement
{
    std::string Name;
    uint64_t Offset;
    uint32_t Size;
    ShaderDataType Type;
    bool IsNormalized;

    BufferElement(ShaderDataType type, const std::string& name, bool isNormalized = false)
        : Name(name), Offset(0), Size(0), Type(type), IsNormalized(isNormalized)
    {
        Size = Utils::GetShaderDataTypeSize(Type);
    }
};

class BufferLayout
{
public:
    BufferLayout(const std::initializer_list<BufferElement>&  elements)
        : m_Elements(elements)
    {
        CalculateOffsetsAndStride();
    }

    const std::vector<BufferElement>& GetElements() const { return m_Elements; };
    uint32_t GetStride() const { return m_Stride; }

    std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
    std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

    std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
    std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
private:
    void CalculateOffsetsAndStride()
    {
        uint32_t offset = 0;
        m_Stride = 0;
        for (auto& element : m_Elements)
        {
            element.Offset = offset;
            offset += element.Size;
            m_Stride += element.Size;
        }
    }

    std::vector<BufferElement> m_Elements;
    uint32_t m_Stride = 0;
};

class VertexBuffer
{
public:
    explicit VertexBuffer(const float* vertices, int64_t size);
    ~VertexBuffer();

    void Bind() const;
    static void Unbind();

    const BufferLayout& GetLayout() const { return m_Layout; }
    void SetLayout(const BufferLayout& layout) { m_Layout = layout; }
private:
    uint32_t m_RendererID;
    BufferLayout m_Layout;
};

class IndexBuffer
{
public:
    explicit IndexBuffer(const uint32_t* indices, uint32_t count);
    ~IndexBuffer();

    void Bind() const;
    static void Unbind();

    uint32_t GetCount() const { return m_Count; }
private:
    uint32_t m_RendererID;
    uint32_t m_Count;
};