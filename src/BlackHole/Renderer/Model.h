#pragma once
#include "Mesh.h"
#include "BlackHole/OpenGL/Texture2D.h"

class Model
{
public:
    explicit Model(const std::string& path);

    const std::vector<Ref<Mesh>>& GetMeshes() const { return m_Meshes; }
    const std::unordered_map<std::string, std::pair<Ref<Texture2D>, TextureType>>& GetTextures() const { return m_Textures; }
private:
    void CollectMaterialInfo(const aiScene* scene);
    void LoadMaterialTexture(const aiMaterial* material, TextureType type);
    void CollectNodeInfo(const aiNode* node, const aiScene* scene);
private:
    std::vector<Ref<Mesh>> m_Meshes;
    std::unordered_map<std::string, std::pair<Ref<Texture2D>, TextureType>> m_Textures;
    std::string m_ModelDirectory;
};