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

class Buffer
{
public:
    explicit Buffer(uint64_t size);
    explicit Buffer(uint64_t size, const void* data);
    virtual ~Buffer();

    void* Map(uint64_t offset, uint64_t length) const;
    void Unmap() const;

    void GetBufferParameterInt(uint32_t paramName, int32_t* params) const;
    void GetBufferParameterInt64(uint32_t paramName, int64_t* params) const;

    virtual void Bind() const = 0;
protected:
    uint32_t m_RendererID;
};

class VertexBuffer : public Buffer
{
public:
    explicit VertexBuffer(uint64_t size);
    explicit VertexBuffer(uint64_t size, const float* vertices);
    ~VertexBuffer() override = default;

    void Bind() const override;

    void BindToBindingPoint(uint32_t vaObj, uint32_t bindingIndex, uint64_t offset) const;

    void SetLayout(const BufferLayout& layout) { m_Layout = layout; }
    const BufferLayout& GetLayout() const { return m_Layout; }
private:
    BufferLayout m_Layout;
};

class IndexBuffer : public Buffer
{
public:
    explicit IndexBuffer(uint64_t count);
    explicit IndexBuffer(const uint32_t* indices, uint64_t count);
    ~IndexBuffer() override = default;

    void Bind() const override;

    void BindToVAO(uint32_t vaObj) const;

    uint32_t GetCount() const { return m_Count; }
private:
    uint64_t m_Count;
};

class UniformBuffer : public Buffer
{
public:
    UniformBuffer(uint64_t size, uint32_t binding);
    ~UniformBuffer() override = default;

    void Bind() const override {};
};