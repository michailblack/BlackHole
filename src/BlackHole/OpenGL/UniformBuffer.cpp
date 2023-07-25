#include "UniformBuffer.h"

UniformBuffer::UniformBuffer(uint64_t size, uint32_t binding)
{
    glCreateBuffers(1, &m_RendererID);
    glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID);
}

UniformBuffer::~UniformBuffer()
{
    glDeleteBuffers(1, &m_RendererID);
}

void UniformBuffer::SetData(uint32_t offset, uint32_t size, const void* data)
{
    glNamedBufferSubData(m_RendererID, offset, size, data);
}