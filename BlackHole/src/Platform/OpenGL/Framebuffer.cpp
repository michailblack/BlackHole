#include "bhpch.h"
#include "Platform/OpenGL/Framebuffer.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

static constexpr uint32_t s_MaxFramebufferSize = 8192u;

namespace Utils
{
    static GLenum GetTextureType(bool useMultisample)
    {
        return useMultisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    static void CreateTextures(bool useMultisample, uint32_t* id, uint32_t count)
    {
        glCreateTextures(GetTextureType(useMultisample), static_cast<int32_t>(count), id);
    }

    static std::pair<uint32_t, int32_t> GetGLFilteringParametersNames(FramebufferTextureFilteringMethod filtering)
    {
        switch (filtering)
        {
            case FramebufferTextureFilteringMethod::FILTER_MIN_TYPE_NEAREST: return std::make_pair(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            case FramebufferTextureFilteringMethod::FILTER_MAG_TYPE_NEAREST: return std::make_pair(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            case FramebufferTextureFilteringMethod::FILTER_MIN_TYPE_LINEAR:  return std::make_pair(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            case FramebufferTextureFilteringMethod::FILTER_MAG_TYPE_LINEAR:  return std::make_pair(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            default: BH_ASSERT(false, "Unknown filtering type!"); return std::make_pair(0, 0);
        }
    }

    static std::pair<uint32_t, int32_t> GetGLWrappingParametersNames(FramebufferTextureWrappingMethod wrapping)
    {
        switch (wrapping)
        {
            case FramebufferTextureWrappingMethod::WRAP_S_TYPE_CLAMP_TO_EDGE:    return std::make_pair(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            case FramebufferTextureWrappingMethod::WRAP_T_TYPE_CLAMP_TO_EDGE:    return std::make_pair(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            case FramebufferTextureWrappingMethod::WRAP_S_TYPE_CLAMP_TO_BORDER:  return std::make_pair(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            case FramebufferTextureWrappingMethod::WRAP_T_TYPE_CLAMP_TO_BORDER:  return std::make_pair(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            case FramebufferTextureWrappingMethod::WRAP_S_TYPE_REPEAT:           return std::make_pair(GL_TEXTURE_WRAP_S, GL_REPEAT);
            case FramebufferTextureWrappingMethod::WRAP_T_TYPE_REPEAT:           return std::make_pair(GL_TEXTURE_WRAP_T, GL_REPEAT);
            default: BH_ASSERT(false, "Unknown wrapping type!"); return std::make_pair(0, 0);
        }
    }

    static uint32_t GetGLTextureParameterName(FramebufferTextureParameterName textureParameterName)
    {
        switch (textureParameterName)
        {
            case FramebufferTextureParameterName::DepthStencilTextureMode: return GL_DEPTH_STENCIL_TEXTURE_MODE;
            case FramebufferTextureParameterName::TextureBorderColor:      return GL_TEXTURE_BORDER_COLOR;
            default: BH_ASSERT(false, "Unknown texture parameter name!"); return 0;
        }
    }

    static void CreateTextureStorage(uint32_t samples, uint32_t attachmentID, GLenum internalFormat, uint32_t width, uint32_t height, const std::vector<FramebufferTextureFilteringMethod>& filtering, const std::vector<FramebufferTextureWrappingMethod>& wrapping)
    {
        if (samples > 1)
        {
            glTextureStorage2DMultisample(attachmentID, static_cast<int32_t>(samples), internalFormat, static_cast<int32_t>(width), static_cast<int32_t>(height), GL_TRUE);
        }
        else
        {
            glTextureStorage2D(attachmentID, 1, internalFormat, static_cast<int32_t>(width), static_cast<int32_t>(height));

            for (const auto& filteringFormat : filtering)
            {
                const auto [ parameterName, value ] = GetGLFilteringParametersNames(filteringFormat);
                glTextureParameteri(attachmentID, parameterName, value);
            }

            for (const auto& wrappingFormat : wrapping)
            {
                const auto [ parameterName, value ] = GetGLWrappingParametersNames(wrappingFormat);
                glTextureParameteri(attachmentID, parameterName, value);
            }
        }
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
}

Framebuffer::Framebuffer(const FramebufferSpecification& specification)
    : m_Specification(specification)
{
    for (const auto& attachment : specification.Attachments.Attachments)
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

    const bool useMultisample = m_Specification.Samples > 1;

    glCreateFramebuffers(1, &m_RendererID);

    if (!m_ColorAttachmentSpecifications.empty())
    {
        m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());

        Utils::CreateTextures(useMultisample, m_ColorAttachments.data(), m_ColorAttachments.size());

        for (int32_t i = 0; i < m_ColorAttachmentSpecifications.size(); ++i)
        {
            switch (m_ColorAttachmentSpecifications[i].Format)
            {
                case FramebufferTextureFormat::RGBA8:
                {
                    Utils::CreateTextureStorage(m_Specification.Samples, m_ColorAttachments[i], GL_RGBA8, m_Specification.Width, m_Specification.Height, m_ColorAttachmentSpecifications[i].Filtering, m_ColorAttachmentSpecifications[i].Wrapping);
                    glNamedFramebufferTexture(m_RendererID, GL_COLOR_ATTACHMENT0 + i, m_ColorAttachments[i], 0);
                    break;
                }
                case FramebufferTextureFormat::RGBA16F:
                {
                    Utils::CreateTextureStorage(m_Specification.Samples, m_ColorAttachments[i], GL_RGBA16F, m_Specification.Width, m_Specification.Height, m_ColorAttachmentSpecifications[i].Filtering, m_ColorAttachmentSpecifications[i].Wrapping);
                    glNamedFramebufferTexture(m_RendererID, GL_COLOR_ATTACHMENT0 + i, m_ColorAttachments[i], 0);
                    break; 
                }
                default: BH_ASSERT(false, "Unknown color attachment format");
            }
        }
    }

    if (m_DepthAttachmentSpecification.Format != FramebufferTextureFormat::None)
    {
        glCreateTextures(Utils::GetTextureType(useMultisample), 1, &m_DepthAttachment);

        switch (m_DepthAttachmentSpecification.Format)
        {
            case FramebufferTextureFormat::DEPTH24STENCIL8:
            {
                    Utils::CreateTextureStorage(m_Specification.Samples, m_DepthAttachment, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, m_DepthAttachmentSpecification.Filtering, m_DepthAttachmentSpecification.Wrapping);
                    glNamedFramebufferTexture(m_RendererID, GL_DEPTH_STENCIL_ATTACHMENT, m_DepthAttachment, 0);
                    break;
            }
            case FramebufferTextureFormat::DEPTH24:
            {
                    Utils::CreateTextureStorage(m_Specification.Samples, m_DepthAttachment, GL_DEPTH_COMPONENT24, m_Specification.Width, m_Specification.Height, m_DepthAttachmentSpecification.Filtering, m_DepthAttachmentSpecification.Wrapping);
                    glNamedFramebufferTexture(m_RendererID, GL_DEPTH_ATTACHMENT, m_DepthAttachment, 0);
                    break;
            }
            default: BH_ASSERT(false, "Unknown depth/stencil attachment format");
        }
    }

    if (m_ColorAttachments.size() > 1)
    {
        BH_ASSERT(m_ColorAttachments.size() < 4, "Usupported amount of color attachemts specified!");
        constexpr GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
        glNamedFramebufferDrawBuffers(m_RendererID, m_ColorAttachments.size(), buffers);
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

void Framebuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
    glViewport(0, 0, static_cast<int32_t>(m_Specification.Width), static_cast<int32_t>(m_Specification.Height));
}

void Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}