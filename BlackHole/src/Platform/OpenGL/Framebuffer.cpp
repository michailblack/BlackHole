#include "bhpch.h"
#include "Platform/OpenGL/Framebuffer.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

static constexpr uint32_t s_MaxFramebufferSize = 8192u;

namespace Utils
{
    static uint32_t GetTextureType(bool useMultisampling)
    {
        return useMultisampling ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    static bool IsColorAttachment(FramebufferTextureFormat format)
    {
        switch (format)
        {
            case FramebufferTextureFormat::RGBA8:   return true;
            case FramebufferTextureFormat::RGBA16F: return true;
            default: return false;
        }
    }

    static void CreateTextures(bool useMultisample, uint32_t* id, uint32_t count)
    {
        glCreateTextures(GetTextureType(useMultisample), static_cast<int32_t>(count), id);
    }

    static uint32_t GetGLTextureFormat(FramebufferTextureFormat format)
    {
        switch (format)
        {
            case FramebufferTextureFormat::RGBA8:           return GL_RGBA8;
            case FramebufferTextureFormat::RGBA16F:         return GL_RGBA16F;
            case FramebufferTextureFormat::DEPTH24:         return GL_DEPTH_COMPONENT24;
            case FramebufferTextureFormat::DEPTH24STENCIL8: return GL_DEPTH24_STENCIL8;
            case FramebufferTextureFormat::None:
            default: BH_ASSERT(false, "Unknown texture format!"); return 0;
        }
    }

    static uint32_t GetGLDepthStencilAttachmentType(FramebufferTextureFormat format)
    {
        switch (format)
        {
            case FramebufferTextureFormat::DEPTH24:         return GL_DEPTH_ATTACHMENT;
            case FramebufferTextureFormat::DEPTH24STENCIL8: return GL_DEPTH_STENCIL_ATTACHMENT;
            case FramebufferTextureFormat::None:
            default: BH_ASSERT(false, "Unknown depth/stencil texture format!"); return 0;
        }
    }

    static uint32_t GetGLTextureParameterName(FramebufferTextureParameterName parameterName)
    {
        switch (parameterName)
        {
            case FramebufferTextureParameterName::FILTER_MIN: return GL_TEXTURE_MIN_FILTER;
            case FramebufferTextureParameterName::FILTER_MAG: return GL_TEXTURE_MAG_FILTER;
            case FramebufferTextureParameterName::WRAP_S:     return GL_TEXTURE_WRAP_S;
            case FramebufferTextureParameterName::WRAP_T:     return GL_TEXTURE_WRAP_T;
            case FramebufferTextureParameterName::None:
            default: BH_ASSERT(false, "Unknown texture parameter name!"); return 0;
        }
    }

    static int32_t GetGLTextureParameterValue(FramebufferTextureParameterValue parameterValue)
    {
        switch (parameterValue)
        {
            case FramebufferTextureParameterValue::FILTER_TYPE_NEAREST:      return GL_NEAREST;
            case FramebufferTextureParameterValue::FILTER_TYPE_LINEAR:       return GL_LINEAR;
            case FramebufferTextureParameterValue::WRAP_TYPE_CLAMP_TO_EDGE:  return GL_CLAMP_TO_EDGE;
            case FramebufferTextureParameterValue::WRAP_TYPE_CLAMP_TO_BORDER:return GL_CLAMP_TO_BORDER;
            case FramebufferTextureParameterValue::WRAP_TYPE_REPEAT:         return GL_REPEAT;
            case FramebufferTextureParameterValue::None:
            default: BH_ASSERT(false, "Unknown texture parameter value!"); return 0;
        }
    }

    static void CreateTextureStorage(uint32_t samples, uint32_t attachmentID, GLenum internalFormat, uint32_t width, uint32_t height, const std::vector<std::pair<FramebufferTextureParameterName, FramebufferTextureParameterValue>>& textureParameter)
    {
        if (samples > 1)
        {
            glTextureStorage2DMultisample(attachmentID, static_cast<int32_t>(samples), internalFormat, static_cast<int32_t>(width), static_cast<int32_t>(height), GL_TRUE);
        }
        else
        {
            glTextureStorage2D(attachmentID, 1, internalFormat, static_cast<int32_t>(width), static_cast<int32_t>(height));
        }

        for (const auto& parameter : textureParameter)
        {
            const uint32_t parameterName = GetGLTextureParameterName(parameter.first);
            const int32_t parameterValue = GetGLTextureParameterValue(parameter.second);
            glTextureParameteri(attachmentID, parameterName, parameterValue);
        }
    }
}

Framebuffer::Framebuffer(FramebufferSpecification specification)
    : m_Specification(std::move(specification))
{
    for (const auto& attachment : m_Specification.Attachments.TextureAttachments)
    {
        if (Utils::IsColorAttachment(attachment.Format))
            m_ColorAttachmentSpecifications.emplace_back(attachment);
        else
            m_DepthAttachmentSpecification = attachment;
    }

    Invalidate();
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &m_RendererID);
    glDeleteTextures(static_cast<int32_t>(m_ColorAttachments.size()), m_ColorAttachments.data());
    glDeleteTextures(1, &m_DepthAttachment);
}

void Framebuffer::Invalidate()
{
    if (m_RendererID)
    {
        glDeleteFramebuffers(1, &m_RendererID);
        glDeleteTextures(static_cast<int32_t>(m_ColorAttachments.size()), m_ColorAttachments.data());
        glDeleteTextures(1, &m_DepthAttachment);

        m_ColorAttachments.clear();
        m_DepthAttachment = 0;
    }

    const bool useMultisampling = m_Specification.Samples > 1;

    glCreateFramebuffers(1, &m_RendererID);

    if (!m_ColorAttachmentSpecifications.empty())
    {
        m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());

        Utils::CreateTextures(useMultisampling, m_ColorAttachments.data(), m_ColorAttachments.size());

        for (uint32_t i = 0; i < m_ColorAttachmentSpecifications.size(); ++i)
        {
            Utils::CreateTextureStorage(m_Specification.Samples,
                m_ColorAttachments[i],
                Utils::GetGLTextureFormat(m_ColorAttachmentSpecifications[i].Format),
                m_Specification.Width, m_Specification.Height,
                m_ColorAttachmentSpecifications[i].Parameters
            );
            glNamedFramebufferTexture(m_RendererID, GL_COLOR_ATTACHMENT0 + i, m_ColorAttachments[i], 0);
        }
    }

    if (m_DepthAttachmentSpecification.Format != FramebufferTextureFormat::None)
    {
        glCreateTextures(Utils::GetTextureType(useMultisampling), 1, &m_DepthAttachment);

        Utils::CreateTextureStorage(m_Specification.Samples,
            m_DepthAttachment,
            Utils::GetGLTextureFormat(m_DepthAttachmentSpecification.Format),
            m_Specification.Width, m_Specification.Height,
            m_DepthAttachmentSpecification.Parameters
        );
        glNamedFramebufferTexture(m_RendererID, Utils::GetGLDepthStencilAttachmentType(m_DepthAttachmentSpecification.Format), m_DepthAttachment, 0);
    }

    int32_t maxColorAttachments;
    glGetIntegerv(GL_MAX_DRAW_BUFFERS, &maxColorAttachments);

    if (m_ColorAttachments.size() > 1 && m_ColorAttachments.size() < maxColorAttachments)
    {
        std::vector<uint32_t> buffers(m_ColorAttachments.size());
        for (uint32_t i = 0; i < m_ColorAttachments.size(); ++i)
            buffers[i] = GL_COLOR_ATTACHMENT0 + i;

        BH_ASSERT(m_ColorAttachments.size() < 4, "Usupported amount of color attachemts specified!");
        glNamedFramebufferDrawBuffers(m_RendererID, m_ColorAttachments.size(), buffers.data());
    }

    if (m_ColorAttachmentSpecifications.empty())
    {
        glNamedFramebufferDrawBuffer(m_RendererID, GL_NONE);
    }

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

void Framebuffer::BlitFramebufferDepthAttachment(const Ref<Framebuffer>& framebuffer) const
{
    glBlitNamedFramebuffer(framebuffer->m_RendererID, m_RendererID,
        0, 0, static_cast<int32_t>(framebuffer->m_Specification.Width), static_cast<int32_t>(framebuffer->m_Specification.Height),
        0, 0, static_cast<int32_t>(m_Specification.Width), static_cast<int32_t>(m_Specification.Height),
        m_DepthAttachmentSpecification.Format == FramebufferTextureFormat::DEPTH24 ? GL_DEPTH_BUFFER_BIT : GL_STENCIL_BUFFER_BIT, GL_NEAREST);
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