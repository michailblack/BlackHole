#pragma once
#include <glad/glad.h>
#include <glm/vec4.hpp>

enum class FramebufferTextureFormat
{
    None = 0,

    // Color
    RGBA8,
    RGBA16F,

    // Depth
    DEPTH24,

    // Depth/stencil
    DEPTH24STENCIL8
};

enum class FramebufferTextureParameterValue
{
    None = 0,

    FILTER_TYPE_NEAREST,
    FILTER_TYPE_LINEAR,

    WRAP_TYPE_CLAMP_TO_EDGE,
    WRAP_TYPE_CLAMP_TO_BORDER,
    WRAP_TYPE_REPEAT
};

enum class FramebufferTextureParameterName
{
    None = 0,

    FILTER_MIN,
    FILTER_MAG,

    WRAP_S,
    WRAP_T
};

namespace Utils
{
    static uint32_t GetGLDataFormatFromTextureType(FramebufferTextureFormat textureFormat)
    {
        switch (textureFormat)
        {
            case FramebufferTextureFormat::RGBA8:
            case FramebufferTextureFormat::RGBA16F: return GL_RGBA;
            case FramebufferTextureFormat::None:
            default: BH_ASSERT(false, "Unknown texture format!"); return 0;
        }
    }
}

struct FramebufferTextureAttachmentSpecification
{
    FramebufferTextureAttachmentSpecification() = default;
    FramebufferTextureAttachmentSpecification(FramebufferTextureFormat format,
        std::initializer_list<std::pair<FramebufferTextureParameterName, FramebufferTextureParameterValue>> parameters)
        : Format(format), Parameters(parameters) {}

    FramebufferTextureFormat Format = FramebufferTextureFormat::None;
    std::vector<std::pair<FramebufferTextureParameterName, FramebufferTextureParameterValue>> Parameters;
};

struct FramebufferAttachmentSpecification
{
    FramebufferAttachmentSpecification() = default;
    FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureAttachmentSpecification> textureSpecs)
        : TextureAttachments(textureSpecs) {}

    std::vector<FramebufferTextureAttachmentSpecification> TextureAttachments;
};

struct FramebufferSpecification
{
    uint32_t Width = 0, Height = 0;
    FramebufferAttachmentSpecification Attachments;
    uint8_t Samples = 1;
};

class Framebuffer
{
public:
    Framebuffer(FramebufferSpecification specification);
    ~Framebuffer();

    void Invalidate();

    void Bind() const;
    static void Unbind();

    void Resize(uint32_t width, uint32_t height);
    void BlitFramebuffer(const Ref<Framebuffer>& framebuffer) const;

    uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const
    {
        BH_ASSERT(index < m_ColorAttachments.size(), "Color attachment index out of range!");
        return m_ColorAttachments[index];
    }
    uint32_t GetDepthAttachmentRendererID() const { return m_DepthAttachment; }

    template<typename T>
    void ClearColorAttachment(uint32_t attachmentIndex, const T* value) const;

    const FramebufferSpecification& GetSpecification() const { return m_Specification; }
private:
    uint32_t m_RendererID = 0;
    FramebufferSpecification m_Specification;

    std::vector<FramebufferTextureAttachmentSpecification> m_ColorAttachmentSpecifications;
    FramebufferTextureAttachmentSpecification m_DepthAttachmentSpecification;

    std::vector<uint32_t> m_ColorAttachments;
    uint32_t m_DepthAttachment = 0;
};

template<>
inline void Framebuffer::ClearColorAttachment<float>(uint32_t attachmentIndex, const float* value) const
{
    BH_ASSERT(attachmentIndex < m_ColorAttachments.size(), "Color attachment index out of range!");
    glClearTexImage(m_ColorAttachments[attachmentIndex], 0,
        Utils::GetGLDataFormatFromTextureType(m_ColorAttachmentSpecifications[attachmentIndex].Format),
        GL_FLOAT,
        value
    );
}
