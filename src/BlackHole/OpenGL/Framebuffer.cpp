#include "Framebuffer.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

static constexpr uint32_t s_MaxFramebufferSize = 8192u;

namespace Utils
{
    static void AttachColorTexture(uint32_t* colorAttachmentID, FramebufferSpecification spec)
    {
        if (spec.Samples > 1)
        {
            glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, colorAttachmentID);
            glTextureStorage2DMultisample(*colorAttachmentID, spec.Samples, GL_RGBA8, static_cast<int32_t>(spec.Width), static_cast<int32_t>(spec.Height), GL_TRUE);
        }
        else
        {
            glCreateTextures(GL_TEXTURE_2D, 1, &*colorAttachmentID);
            glTextureStorage2D(*colorAttachmentID, 1, GL_RGBA8, static_cast<int32_t>(spec.Width), static_cast<int32_t>(spec.Height));

            glTextureParameteri(*colorAttachmentID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTextureParameteri(*colorAttachmentID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTextureParameteri(*colorAttachmentID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTextureParameteri(*colorAttachmentID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
    }

    static void AttachDepthStencilRenderBuffer(uint32_t* depthStencilAttachmentID, FramebufferSpecification spec)
    {
        glCreateRenderbuffers(1, depthStencilAttachmentID);

        if (spec.Samples > 1)
        {
            glNamedRenderbufferStorageMultisample(*depthStencilAttachmentID, spec.Samples, GL_DEPTH24_STENCIL8, static_cast<int32_t>(spec.Width), static_cast<int32_t>(spec.Height));
        }
        else
        {
            glNamedRenderbufferStorage(*depthStencilAttachmentID, GL_DEPTH24_STENCIL8, static_cast<int32_t>(spec.Width), static_cast<int32_t>(spec.Height));
        }
    }
}

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

    Utils::AttachColorTexture(&m_ColorAttachment, m_Specification);
    glNamedFramebufferTexture(m_RendererID, GL_COLOR_ATTACHMENT0, m_ColorAttachment, 0);

    Utils::AttachDepthStencilRenderBuffer(&m_DepthStencilAttachment, m_Specification);
    glNamedFramebufferRenderbuffer(m_RendererID, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilAttachment);

    BH_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Failed to complete Framebuffer!");
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

void Framebuffer::BlitFramebuffer(const Ref<Framebuffer>& framebuffer) const
{
    glBlitNamedFramebuffer(framebuffer->m_RendererID, m_RendererID,
        0, 0, static_cast<int32_t>(framebuffer->m_Specification.Width), static_cast<int32_t>(framebuffer->m_Specification.Height),
        0, 0, static_cast<int32_t>(m_Specification.Width), static_cast<int32_t>(m_Specification.Height),
        GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void Framebuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glViewport(0, 0, static_cast<int32_t>(m_Specification.Width), static_cast<int32_t>(m_Specification.Height));
}

void Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::ClearColorAttachment(const glm::vec4& color) const
{
    glClearNamedFramebufferfv(m_RendererID, GL_COLOR, 0, glm::value_ptr(color));
}

void Framebuffer::ClearDepthAttachment(float value) const
{
    glClearNamedFramebufferfv(m_RendererID, GL_DEPTH, 0, &value);
}

void Framebuffer::ClearStencilAttachment(int32_t value) const
{
    glClearNamedFramebufferiv(m_RendererID, GL_STENCIL, 0, &value);
}

void Framebuffer::ClearDepthStencilAttachment(float d, int32_t s) const
{
    glClearNamedFramebufferfi(m_RendererID, GL_DEPTH_STENCIL, 0, d, s);
}

void Framebuffer::ClearDefaultFramebufferColorAttachment(const glm::vec4& color)
{
    glClearNamedFramebufferfv(0, GL_COLOR, 0, glm::value_ptr(color));
}

void Framebuffer::ClearDefaultFramebufferDepthAttachment(float value)
{
    glClearNamedFramebufferfv(0, GL_DEPTH, 0, &value);
}

void Framebuffer::ClearDefaultFramebufferStencilAttachment(int32_t value)
{
    glClearNamedFramebufferiv(0, GL_STENCIL, 0, &value);
}

void Framebuffer::ClearDefaultFramebufferDepthStencilAttachment(float d, int32_t s)
{
    glClearNamedFramebufferfi(0, GL_DEPTH_STENCIL, 0, d, s);
}
