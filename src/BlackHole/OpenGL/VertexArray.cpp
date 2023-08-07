#include "VertexArray.h"

#include <glad/glad.h>

namespace Utils
{
    static GLint GetComponentCount(const ShaderDataType type)
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

    static GLenum ShaderDataTypeToOpenGLBaseType(const ShaderDataType type)
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

}

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

void VertexArray::Unbind()
{
    glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
{
    BH_ASSERT(!vertexBuffer->GetLayout().GetElements().empty(), "Vertex Buffer has no layout!");

    const auto& layout = vertexBuffer->GetLayout();
    uint32_t index = 0;
    for (const auto& element : layout)
    {
        const GLint componentCount = Utils::GetComponentCount(element.Type);

        uint8_t attribPointersNeeded = 1;
        if (componentCount > 4)
        {
            attribPointersNeeded = 4;
            while (componentCount % attribPointersNeeded != 0)
                --attribPointersNeeded;
        }

        const uint64_t vecOffset = Utils::GetShaderDataTypeSize(element.Type) / attribPointersNeeded;
        for (uint8_t i = 0; i < attribPointersNeeded; ++i)
        {
            glEnableVertexArrayAttrib(m_RendererID, index);
            glVertexArrayAttribFormat(m_RendererID,
                index,
                componentCount / attribPointersNeeded,
                Utils::ShaderDataTypeToOpenGLBaseType(element.Type),
                element.IsNormalized ? GL_TRUE : GL_FALSE,
                element.Offset + vecOffset * i);
            glVertexArrayAttribBinding(m_RendererID, index, 0);
            vertexBuffer->BindToBindingPoint(m_RendererID, 0, 0);
            ++index;
        }
    }
    m_VertexBuffers.push_back(vertexBuffer);
}

void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
{
    indexBuffer->BindToVAO(m_RendererID);
    m_IndexBuffer = indexBuffer;
}