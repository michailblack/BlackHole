#include "TextureManager.h"

TextureManager TextureManager::s_Instance;

TextureManager& TextureManager::Get()
{
    return s_Instance;
}

Ref<Texture2D> TextureManager::Load2D(const std::string& filepath)
{
    const auto it = m_Textures2D.find(filepath);

    if (it != m_Textures2D.end())
    {
        return it->second;
    }
    else
    {
        const auto [it, success] = m_Textures2D.emplace(filepath, std::make_shared<Texture2D>(filepath));
        BH_ASSERT(success, "Failed to load texture '{}'", filepath);
        return it->second;
    }
}