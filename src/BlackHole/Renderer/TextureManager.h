#pragma once
#include "BlackHole/OpenGL/Texture2D.h"

class TextureManager
{
public:
    TextureManager(const TextureManager& other) = delete;
    TextureManager(TextureManager&& other) noexcept = delete;
    TextureManager& operator=(const TextureManager& other) = delete;
    TextureManager& operator=(TextureManager&& other) noexcept = delete;

    static TextureManager& Get();

    Ref<Texture2D> Load2D(const std::string& filepath);
private:
    TextureManager() = default;
private:
    static TextureManager s_Instance;
    std::unordered_map<std::string, Ref<Texture2D>> m_Textures2D;
};