#include "Cubemap.h"

#include <stb_image.h>

Cubemap::Cubemap(const CubemapSpecification& spec)
{
    std::array<std::string, 6> faces;

    int width, height, channels;

    faces[0] = spec.Right;
    faces[1] = spec.Left;
    faces[2] = spec.Top;
    faces[3] = spec.Bottom;
    faces[4] = spec.Front;
    faces[5] = spec.Back;

    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);

    for (size_t i = 0; i < 6; ++i)
    {
        stbi_uc* data = stbi_load(faces[i].c_str() , &width, &height, &channels, 0);
        if (data)
        {
            GLenum dataFormat = 0;

            switch (channels)
            {
            case 1: dataFormat = GL_RED; break;
            case 2: dataFormat = GL_RG; break;
            case 3: dataFormat = GL_RGB; break;
            case 4: dataFormat = GL_RGBA; break;
            }

            BH_ASSERT(dataFormat, "Image format is not supported!");

            if (i == 0)
                glTextureStorage2D(m_RendererID, 1, GL_RGB8, width, height);
            glTextureSubImage3D(m_RendererID, 0, 0, 0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, width, height, 1, dataFormat, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            BH_ASSERT(false, "Failed to load cubemap face!");
        }
    }

    m_Length = width;

    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

Cubemap::~Cubemap()
{
    glDeleteTextures(1, &m_RendererID);
}

void Cubemap::Bind(uint32_t slot) const
{
    glBindTextureUnit(slot, m_RendererID);
}