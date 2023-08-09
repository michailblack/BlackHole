#pragma once
#include <glm/vec4.hpp>

struct FramebufferSpecification
{
    uint32_t Width = 0, Height = 0;
    uint8_t Samples = 1;
};

class Framebuffer
{
public:
    Framebuffer(const FramebufferSpecification& specification);
    ~Framebuffer();

    void Bind() const;
    static void Unbind();

    void ClearColorAttachment(const glm::vec4& color = glm::vec4(0.0f)) const;
    void ClearDepthAttachment(float value = 1.0f) const;
    void ClearStencilAttachment(int32_t value = 0.0f) const;
    void ClearDepthStencilAttachment(float d = 1.0f, int32_t s = 0.0f) const;

    void Invalidate();
    void Resize(uint32_t width, uint32_t height);

    void BlitFramebuffer(const Ref<Framebuffer>& framebuffer) const;

    uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; }
    const FramebufferSpecification& GetSpecification() const { return m_Specification; }

    static void ClearDefaultFramebufferColorAttachment(const glm::vec4& color = glm::vec4(0.0f));
    static void ClearDefaultFramebufferDepthAttachment(float value = 1.0f);
    static void ClearDefaultFramebufferStencilAttachment(int32_t value = 0.0f);
    static void ClearDefaultFramebufferDepthStencilAttachment(float d = 1.0f, int32_t s = 0.0f);

private:
    uint32_t m_RendererID = 0;

    FramebufferSpecification m_Specification;
    
    uint32_t m_DepthStencilAttachment = 0;
    uint32_t m_ColorAttachment = 0;
};