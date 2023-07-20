#include "Texture2D.h"

#include <stb_image.h>

Texture2D::Texture2D(const std::string& path)
{
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);

    stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
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
        glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

        glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

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