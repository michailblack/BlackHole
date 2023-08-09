#include "bhpch.h"
#include "Platform//OpenGL/Buffer.h"

#include <glad/glad.h>

static constexpr GLbitfield 
	    s_MappingFlags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT,
	    s_StorageFlags = GL_DYNAMIC_STORAGE_BIT | s_MappingFlags;

Buffer::Buffer(uint64_t size)
    : m_RendererID(0)
{
    glCreateBuffers(1, &m_RendererID);
    glNamedBufferStorage(m_RendererID, static_cast<int64_t>(size), nullptr, s_StorageFlags | GL_MAP_READ_BIT);
}

Buffer::Buffer(uint64_t size, const void* data)
    : m_RendererID(0)
{
    glCreateBuffers(1, &m_RendererID);
    glNamedBufferStorage(m_RendererID, static_cast<int64_t>(size), data, s_StorageFlags | GL_MAP_READ_BIT);
}

Buffer::~Buffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void* Buffer::Map(uint64_t offset, uint64_t length) const
{
    void* range = glMapNamedBufferRange(m_RendererID, static_cast<int64_t>(offset), static_cast<int64_t>(length), s_MappingFlags | GL_MAP_UNSYNCHRONIZED_BIT);
    BH_ASSERT(range != nullptr, "Pointer to a buffer memory was nullptr!");
    return range;
}

void Buffer::Unmap() const
{
    glUnmapNamedBuffer(m_RendererID);
}

void Buffer::GetBufferParameterInt(uint32_t paramName, int32_t* params) const
{
    glGetNamedBufferParameteriv(m_RendererID, paramName, params);
}

void Buffer::GetBufferParameterInt64(uint32_t paramName, int64_t* params) const
{
    glGetNamedBufferParameteri64v(m_RendererID, paramName, params);
}

// Vertex Buffer

VertexBuffer::VertexBuffer(uint64_t size)
    : Buffer(size)
    , m_Layout({})
{
}

VertexBuffer::VertexBuffer(uint64_t size, const float* vertices)
    : Buffer(size, vertices)
    , m_Layout({})
{
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void VertexBuffer::BindToBindingPoint(uint32_t vaObj, uint32_t bindingIndex, uint64_t offset) const
{
    BH_ASSERT(!m_Layout.GetElements().empty(), "Vertex Buffer has no layout!");
    glVertexArrayVertexBuffer(vaObj, bindingIndex, m_RendererID, static_cast<int64_t>(offset), static_cast<int32_t>(m_Layout.GetStride()));
}

// Index Buffer

IndexBuffer::IndexBuffer(uint64_t count)
    : Buffer(count * sizeof(uint32_t))
    , m_Count(count)
{
}

IndexBuffer::IndexBuffer(const uint32_t* indices, uint64_t count)
    : Buffer(count * sizeof(uint32_t), indices)
    , m_Count(count)
{
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void IndexBuffer::BindToVAO(uint32_t vaObj) const
{
    glVertexArrayElementBuffer(vaObj, m_RendererID);
}

// Uniform Buffer

UniformBuffer::UniformBuffer(uint64_t size, uint32_t binding)
    : Buffer(size)
{
    glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
}