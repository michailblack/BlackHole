#pragma once
#include <filesystem>

class Filesystem
{
public:
    static void Init();

    static const std::filesystem::path& GetAssetsPath();
    static const std::filesystem::path& GetModelsPath();
    static const std::filesystem::path& GetShadersPath();
    static const std::filesystem::path& GetTexturesPath();
    static const std::filesystem::path& GetFontsPath();
};