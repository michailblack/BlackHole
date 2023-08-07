#include "Texture.h"

#include <stb_image.h>
#include <glad/glad.h>
#include <glm/common.hpp>
#include <glm/exponential.hpp>

// Texture2D

Texture2D::Texture2D(const std::filesystem::path& texturePath)
    : m_RendererID(0)
{
    int width, height, channels;
    stbi_uc* data = stbi_load(texturePath.string().c_str(), &width, &height, &channels, 0);
    BH_ASSERT(data, "Failed to load image!");
    
    if (data)
    {
        m_Width = width;
        m_Height = height;

        GLenum internalFormat = 0, dataFormat = 0;

        switch (channels)
        {
        case 1:
            internalFormat = GL_R8;
            dataFormat = GL_RED;
            break;
        case 2:
            internalFormat = GL_RG8;
            dataFormat = GL_RG;
            break;
        case 3:
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
            break;
        case 4:
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
            break;
        }

        BH_ASSERT(internalFormat && dataFormat, "Image format is not supported!");

        m_InternalFormat = internalFormat;
        m_DataFormat = dataFormat;

        glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, static_cast<int32_t>(glm::log2(static_cast<float>(glm::max(m_Width, m_Height))) + 1), m_InternalFormat, static_cast<int32_t>(m_Width), static_cast<int32_t>(m_Height));

        glTextureSubImage2D(m_RendererID, 0, 0, 0, static_cast<int32_t>(m_Width), static_cast<int32_t>(m_Height), m_DataFormat, GL_UNSIGNED_BYTE, data);

        glGenerateTextureMipmap(m_RendererID);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        stbi_image_free(data);
    }
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &m_RendererID);
}

void Texture2D::Bind(uint32_t slot) const
{
    glBindTextureUnit(slot, m_RendererID);
}

// Texture2D Array

TextureArray2D::TextureArray2D(const std::filesystem::path& texturePath, uint32_t layers)
    : m_RendererID(0)
{
    m_TextureKeys.reserve(layers);

    int width, height, channels;
    stbi_uc* data = stbi_load(texturePath.string().c_str(), &width, &height, &channels, 0);
    BH_ASSERT(data, "Failed to load image!");
    
    if (data)
    {
        m_TextureKeys.push_back(texturePath.filename().string());

        m_Width = width;
        m_Height = height;

        GLenum internalFormat = 0, dataFormat = 0;
        switch (channels)
        {
        case 1:
            internalFormat = GL_R8;
            dataFormat = GL_RED;
            break;
        case 2:
            internalFormat = GL_RG8;
            dataFormat = GL_RG;
            break;
        case 3:
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
            break;
        case 4:
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
            break;
        }

        BH_ASSERT(internalFormat && dataFormat, "Image format is not supported!");

        m_InternalFormat = internalFormat;
        m_DataFormat = dataFormat;

        glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_RendererID);
        glTextureStorage3D(m_RendererID, static_cast<int32_t>(glm::log2(static_cast<float>(glm::max(m_Width, m_Height))) + 1), m_InternalFormat, static_cast<int32_t>(m_Width), static_cast<int32_t>(m_Height), static_cast<int32_t>(layers));

        glTextureSubImage3D(m_RendererID, 0, 0, 0, 0, static_cast<int32_t>(m_Width), static_cast<int32_t>(m_Height), 1, m_DataFormat, GL_UNSIGNED_BYTE, data);

        glGenerateTextureMipmap(m_RendererID);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        stbi_image_free(data);
    }
}

TextureArray2D::~TextureArray2D()
{
    glDeleteTextures(1, &m_RendererID);
}

void TextureArray2D::Bind(uint32_t slot)
{
    glBindTextureUnit(slot, m_RendererID);
}

void TextureArray2D::PushBack(const std::filesystem::path& texturePath)
{
    int width, height, channels;
    stbi_uc* data = stbi_load(texturePath.string().c_str(), &width, &height, &channels, 0);
    BH_ASSERT(data, "Failed to load image!");
    
    if (data)
    {
        m_TextureKeys.push_back(texturePath.filename().string());

        glTextureSubImage3D(m_RendererID, 0, 0, 0, static_cast<int32_t>(m_TextureKeys.size() - 1), static_cast<int32_t>(m_Width), static_cast<int32_t>(m_Height), 1, m_DataFormat, GL_UNSIGNED_BYTE, data);

        glGenerateTextureMipmap(m_RendererID);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        stbi_image_free(data);
    }
}