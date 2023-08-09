#pragma once

class VertexBuffer;
class IndexBuffer;

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void Bind() const;
    static void Unbind();

    void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);
    void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer);

    std::vector<Ref<VertexBuffer>> GetVertexBuffers() { return m_VertexBuffers; }
    Ref<IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }
private:
    uint32_t m_RendererID;
    std::vector<Ref<VertexBuffer>> m_VertexBuffers;
    Ref<IndexBuffer> m_IndexBuffer;
};