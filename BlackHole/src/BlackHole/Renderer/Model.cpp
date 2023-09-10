#include "bhpch.h"
#include "BlackHole/Renderer/Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

Model::Model(const std::filesystem::path& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.string(), 
        aiProcess_Triangulate
        | aiProcess_FlipUVs
        | aiProcess_CalcTangentSpace);

    BH_ASSERT(scene || scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE, "Falied to load model!");

    m_ModelDirectory = path.parent_path();

    m_Meshes.reserve(scene->mNumMeshes);
    m_DiffuseMaps.reset();
    m_SpecularMaps.reset();

    CollectMaterialInfo(scene);
    CollectNodeInfo(scene->mRootNode, scene);
}

void Model::CollectMaterialInfo(const aiScene* scene)
{
    std::unordered_set<std::filesystem::path> diffuseTextures, specularTextures, normalTextures, displacementTextures;
    for (size_t i = 0; i < scene->mNumMaterials; ++i)
    {
        const aiMaterial* material = scene->mMaterials[i];
        LoadMaterialTextures(material, aiTextureType_DIFFUSE, diffuseTextures);
        LoadMaterialTextures(material, aiTextureType_SPECULAR, specularTextures);
        LoadMaterialTextures(material, aiTextureType_NORMALS, normalTextures);
        LoadMaterialTextures(material, aiTextureType_DISPLACEMENT, displacementTextures);
    }

    if (!diffuseTextures.empty())
    {
        m_DiffuseMaps = CreateRef<TextureArray2D>(diffuseTextures.extract(diffuseTextures.begin()).value(), static_cast<uint32_t>(diffuseTextures.size()), true);
    
        for (const auto& path : diffuseTextures)
            m_DiffuseMaps->PushBack(path);
    }

    if (!specularTextures.empty())
    {
        m_SpecularMaps = CreateRef<TextureArray2D>(specularTextures.extract(specularTextures.begin()).value(), static_cast<uint32_t>(specularTextures.size()));

        for (const auto& path : specularTextures)
            m_SpecularMaps->PushBack(path);
    }

    if (!normalTextures.empty())
    {
        m_NormalMaps = CreateRef<TextureArray2D>(normalTextures.extract(normalTextures.begin()).value(), static_cast<uint32_t>(normalTextures.size()));

        for (const auto& path : normalTextures)
            m_NormalMaps->PushBack(path);
    }

    if (!displacementTextures.empty())
    {
        m_DisplacementMaps = CreateRef<TextureArray2D>(displacementTextures.extract(displacementTextures.begin()).value(), static_cast<uint32_t>(displacementTextures.size()));

        for (const auto& path : displacementTextures)
            m_DisplacementMaps->PushBack(path);
    }
}

void Model::LoadMaterialTextures(const aiMaterial* material, aiTextureType type, std::unordered_set<std::filesystem::path>& texturesSet) const
{
    for (size_t i = 0; i < material->GetTextureCount(type); ++i)
    {
        aiString relativeTexturePath;
        material->GetTexture(type, i, &relativeTexturePath);

        texturesSet.emplace(m_ModelDirectory / relativeTexturePath.C_Str());
    }
}

void Model::CollectNodeInfo(const aiNode* node, const aiScene* scene)
{
    for (size_t i = 0; i < node->mNumMeshes; ++i)
    {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.emplace_back(CreateRef<Mesh>(mesh, scene, this));
    }

    for (size_t i = 0; i < node->mNumChildren; ++i)
        CollectNodeInfo(node->mChildren[i], scene);
}
