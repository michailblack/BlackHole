#pragma once

struct FramebufferSpecification
{
    uint32_t Width = 0, Height = 0;
};

class Framebuffer
{
public:
    Framebuffer(const FramebufferSpecification& spec);
    ~Framebuffer();

    void Invalidate();

    void Bind() const;
    static void Unbind();

    void Resize(uint32_t width, uint32_t height);

    uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; }
private:
    uint32_t m_RendererID = 0;

    FramebufferSpecification m_Specification;
    
    uint32_t m_DepthStencilAttachment = 0;
    uint32_t m_ColorAttachment = 0;
};