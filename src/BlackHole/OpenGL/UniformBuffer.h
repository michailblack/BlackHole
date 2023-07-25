#pragma once

class UniformBuffer
{
public:
    UniformBuffer(uint64_t size, uint32_t binding);
    ~UniformBuffer();

    void SetData(uint32_t offset, uint32_t size, const void* data);
private:
    uint32_t m_RendererID;
};