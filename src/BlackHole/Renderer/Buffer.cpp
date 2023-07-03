#include "bhpch.h"
#include "Buffer.h"

#include <glad/glad.h>

static GLint GetComponentCount(ShaderDataType type)
{
    switch (type)
    {
        case ShaderDataType::Float:  return 1;
        case ShaderDataType::Float2: return 2;
        case ShaderDataType::Float3: return 3;
        case ShaderDataType::Float4: return 4;
        case ShaderDataType::Int:    return 1;
        case ShaderDataType::Int2:   return 2;
        case ShaderDataType::Int3:   return 3;
        case ShaderDataType::Int4:   return 4;
        case ShaderDataType::Mat3:   return 3 * 3;
        case ShaderDataType::Mat4:   return 4 * 4;
        case ShaderDataType::Bool:   return 1;
        default: BH_ASSERT(false, "Unknown ShaderDataType!"); return 0;
    }
}

static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
    switch (type)
    {
        case ShaderDataType::Float:   return GL_FLOAT;
        case ShaderDataType::Float2:  return GL_FLOAT;
        case ShaderDataType::Float3:  return GL_FLOAT;
        case ShaderDataType::Float4:  return GL_FLOAT;
        case ShaderDataType::Int:     return GL_INT;
        case ShaderDataType::Int2:    return GL_INT;
        case ShaderDataType::Int3:    return GL_INT;
        case ShaderDataType::Int4:    return GL_INT;
        case ShaderDataType::Mat3:    return GL_FLOAT;
        case ShaderDataType::Mat4:    return GL_FLOAT;
        case ShaderDataType::Bool:    return GL_BOOL;
        default: BH_ASSERT(false, "Unknown ShaderDataType!"); return 0;
    }
}

// Vertex Buffer

VertexBuffer::VertexBuffer(const float* vertices, uint32_t size)
    : m_Layout({})
{
    glCreateBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::Unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Index Buffer

IndexBuffer::IndexBuffer(const uint32_t* indices, uint32_t count)
    : m_Count(count)
{
    glCreateBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::Unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Vertex Array

VertexArray::VertexArray()
{
    glCreateVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind() const
{
    glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
    BH_ASSERT(!vertexBuffer->GetLayout().GetElements().empty(), "Vertex Buffer has no layout!");

    glBindVertexArray(m_RendererID);
    vertexBuffer->Bind();
    const auto& layout = vertexBuffer->GetLayout();

    uint32_t index = 0;
    for (const auto& element : layout)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index,
            GetComponentCount(element.type),
            ShaderDataTypeToOpenGLBaseType(element.type),
            element.isNormalized ? GL_TRUE : GL_FALSE,
            layout.GetStride(),
            reinterpret_cast<const void*>(element.offset)
        );
        ++index;
    }

    m_VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
    glBindVertexArray(m_RendererID);
    indexBuffer->Bind();
    m_IndexBuffer = indexBuffer;
}
