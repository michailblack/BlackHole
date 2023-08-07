#include "Filesystem.h"

struct FilesystemData
{
    std::filesystem::path AssetsPath;
    std::filesystem::path ModelsPath;
    std::filesystem::path ShadersPath;
    std::filesystem::path TexturesPath;
    std::filesystem::path FontPath;
} static s_Data;

void Filesystem::Init()
{
    auto workingDir = std::filesystem::current_path();

    bool foundAssetsDir = false;
    while (!foundAssetsDir)
    {
        for (const auto& entry : std::filesystem::directory_iterator(workingDir))
        {
            if (entry.is_directory() && entry.path().filename() == "assets")
            {
                s_Data.AssetsPath = entry;
                foundAssetsDir = true;
                break;
            }
        }
        workingDir = workingDir.parent_path();
    }

    BH_LOG_INFO("Found Assets path: {0}", s_Data.AssetsPath.string());

    s_Data.ModelsPath = s_Data.AssetsPath / "models";
    s_Data.ShadersPath = s_Data.AssetsPath / "shaders";
    s_Data.TexturesPath = s_Data.AssetsPath / "textures";
    s_Data.FontPath = s_Data.AssetsPath / "fonts";
}

const std::filesystem::path& Filesystem::GetAssetsPath()
{
    return s_Data.AssetsPath;
}

const std::filesystem::path& Filesystem::GetModelsPath()
{
    return s_Data.ModelsPath;
}

const std::filesystem::path& Filesystem::GetShadersPath()
{
    return s_Data.ShadersPath;
}

const std::filesystem::path& Filesystem::GetTexturesPath()
{
    return s_Data.TexturesPath;
}

const std::filesystem::path& Filesystem::GetFontsPath()
{
    return s_Data.FontPath;
}
