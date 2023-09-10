#pragma once
#include <filesystem>
#include <unordered_set>

#include "BlackHole/Renderer/Mesh.h"
#include "Platform/OpenGL/Texture.h"

class Model
{
public:
    explicit Model(const std::filesystem::path& path);

    const std::filesystem::path& GetModelDirectory() const { return m_ModelDirectory; }
    const std::vector<Ref<Mesh>>& GetMeshes() const { return m_Meshes; }
    const Ref<TextureArray2D>& GetDiffuseMapArray() const { return m_DiffuseMaps; }
    const Ref<TextureArray2D>& GetSpecularMapArray() const { return m_SpecularMaps; }
    const Ref<TextureArray2D>& GetNormalMapArray() const { return m_NormalMaps; }
    const Ref<TextureArray2D>& GetDisplacementMapArray() const { return m_DisplacementMaps; }
private:
    void CollectMaterialInfo(const aiScene* scene);
    void LoadMaterialTextures(const aiMaterial* material, aiTextureType type, std::unordered_set<std::filesystem::path>& texturesSet) const;
    void CollectNodeInfo(const aiNode* node, const aiScene* scene);
private:
    std::vector<Ref<Mesh>> m_Meshes;
    Ref<TextureArray2D> m_DiffuseMaps;
    Ref<TextureArray2D> m_SpecularMaps;
    Ref<TextureArray2D> m_NormalMaps;
    Ref<TextureArray2D> m_DisplacementMaps;
    std::filesystem::path m_ModelDirectory;
};
