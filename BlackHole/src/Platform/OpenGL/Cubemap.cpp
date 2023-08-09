#include "bhpch.h"
#include "Platform/OpenGL/Cubemap.h"

#include <stb_image.h>
#include <glad/glad.h>
#include <glm/common.hpp>
#include <glm/exponential.hpp>

Cubemap::Cubemap(const CubemapSpecification& specification)
{
    std::array<std::string, 6> faces;

    faces[0] = specification.Right.string();
    faces[1] = specification.Left.string();
    faces[2] = specification.Top.string();
    faces[3] = specification.Bottom.string();
    faces[4] = specification.Front.string();
    faces[5] = specification.Back.string();

    int width, height, channels;
    stbi_uc* rightFacePixels = stbi_load(faces[0].c_str() , &width, &height, &channels, 0);
    BH_ASSERT(rightFacePixels, "Failed to load image!");

    if (rightFacePixels)
    {
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

        m_Length = static_cast<uint32_t>(glm::min(width, height));

        glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);
        glTextureStorage2D(m_RendererID, static_cast<int32_t>(glm::log2(static_cast<float>(static_cast<int32_t>(m_Length))) + 1), m_InternalFormat, static_cast<int32_t>(m_Length), static_cast<int32_t>(m_Length));
        glTextureSubImage3D(m_RendererID, 0, 0, 0, 0, static_cast<int32_t>(m_Length), static_cast<int32_t>(m_Length), 1, m_DataFormat, GL_UNSIGNED_BYTE, rightFacePixels);

        stbi_image_free(rightFacePixels);

        for (int32_t i = 1; i < 6; ++i)
        {
            stbi_uc* pixels = stbi_load(faces[i].c_str() , &width, &height, &channels, 0);
            BH_ASSERT(pixels, "Failed to load image!");
            if (pixels)
            {
                glTextureSubImage3D(m_RendererID, 0, 0, 0, i, static_cast<int32_t>(m_Length), static_cast<int32_t>(m_Length), 1, m_DataFormat, GL_UNSIGNED_BYTE, pixels);
                stbi_image_free(pixels);
            }
        }

        glGenerateTextureMipmap(m_RendererID);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }
}

Cubemap::~Cubemap()
{
    glDeleteTextures(1, &m_RendererID);
}

void Cubemap::Bind(uint32_t slot) const
{
    glBindTextureUnit(slot, m_RendererID);
}