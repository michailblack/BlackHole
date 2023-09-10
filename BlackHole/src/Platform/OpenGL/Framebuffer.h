#pragma once
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

enum class FramebufferTextureFilteringMethod
{
    None = 0,

    FILTER_MIN_TYPE_NEAREST,
    FILTER_MAG_TYPE_NEAREST,
    FILTER_MIN_TYPE_LINEAR,
    FILTER_MAG_TYPE_LINEAR,
};

enum class FramebufferTextureWrappingMethod
{
    None = 0,

    WRAP_S_TYPE_CLAMP_TO_EDGE,
    WRAP_T_TYPE_CLAMP_TO_EDGE,
    WRAP_S_TYPE_CLAMP_TO_BORDER,
    WRAP_T_TYPE_CLAMP_TO_BORDER,
    WRAP_S_TYPE_REPEAT,
    WRAP_T_TYPE_REPEAT
};

enum class FramebufferTextureParameterType
{
    None = 0,

    Int,
    Float,
    IntPtr,
    FloatPtr
};

enum class FramebufferTextureParameterName
{
    None = 0,

    DepthStencilTextureMode,
    TextureBorderColor
};

struct FramebufferTextureFormatSpecification
{
    FramebufferTextureFormatSpecification() = default;
    FramebufferTextureFormatSpecification(FramebufferTextureFormat format,
        std::initializer_list<FramebufferTextureFilteringMethod> filtering,
        std::initializer_list<FramebufferTextureWrappingMethod> wrapping)
        : Format(format), Filtering(filtering), Wrapping(wrapping) {}

    FramebufferTextureFormat Format = FramebufferTextureFormat::None;
    std::vector<FramebufferTextureFilteringMethod> Filtering;
    std::vector<FramebufferTextureWrappingMethod> Wrapping;
};

struct FramebufferAttachmentSpecification
{
    FramebufferAttachmentSpecification() = default;
    FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureFormatSpecification> attachments)
        : Attachments(attachments) {}

    std::vector<FramebufferTextureFormatSpecification> Attachments;
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
    Framebuffer(const FramebufferSpecification& specification);
    ~Framebuffer();

    void Invalidate();

    void Bind() const;
    static void Unbind();

    void Resize(uint32_t width, uint32_t height);
    void BlitFramebuffer(const Ref<Framebuffer>& framebuffer) const;

    uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const { BH_ASSERT(index < m_ColorAttachments.size(), "Color attachment index out of range!"); return m_ColorAttachments[index]; }
    uint32_t GetDepthAttachmentRendererID() const { return m_DepthAttachment; }

    const FramebufferSpecification& GetSpecification() const { return m_Specification; }
private:
    uint32_t m_RendererID = 0;
    FramebufferSpecification m_Specification;

    std::vector<FramebufferTextureFormatSpecification> m_ColorAttachmentSpecifications;
    FramebufferTextureFormatSpecification m_DepthAttachmentSpecification;

    std::vector<uint32_t> m_ColorAttachments;
    uint32_t m_DepthAttachment = 0;
};