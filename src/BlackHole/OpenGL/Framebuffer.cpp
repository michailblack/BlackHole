#include "Framebuffer.h"

static constexpr uint32_t s_MaxFramebufferSize = 8192u;

Framebuffer::Framebuffer(const FramebufferSpecification& spec)
    : m_Specification(spec)
{
    Invalidate();
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &m_RendererID);
    glDeleteTextures(1, &m_ColorAttachment);
    glDeleteRenderbuffers(1, &m_DepthStencilAttachment);
}

void Framebuffer::Invalidate()
{
    if (m_RendererID)
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(1, &m_ColorAttachment);
        glDeleteRenderbuffers(1, &m_DepthStencilAttachment);

        m_ColorAttachment = 0;
        m_DepthStencilAttachment = 0;
    }

    glCreateFramebuffers(1, &m_RendererID);

    // Attaching color texture
    glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
    
    glTextureStorage2D(m_ColorAttachment, 1, GL_RGBA8, m_Specification.Width, m_Specification.Height);

    glTextureParameteri(m_ColorAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_ColorAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(m_ColorAttachment, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_ColorAttachment, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glNamedFramebufferTexture(m_RendererID, GL_COLOR_ATTACHMENT0, m_ColorAttachment, 0);

    // Attaching Renderbuffer object for depth and stencil buffers
    glCreateRenderbuffers(1, &m_DepthStencilAttachment);
    
    glNamedRenderbufferStorage(m_DepthStencilAttachment, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);

    glNamedFramebufferRenderbuffer(m_RendererID, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilAttachment);

    BH_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Failed to complete Framebuffer!");
}

void Framebuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
}

void Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(uint32_t width, uint32_t height)
{
    if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
    {
        BH_LOG_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
        return;
    }

    m_Specification.Width = width;
    m_Specification.Height = height;

    Invalidate();
}