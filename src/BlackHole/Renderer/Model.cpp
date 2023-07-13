#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

Model::Model(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path.c_str(),
        aiProcess_Triangulate |
         aiProcess_GenNormals
    );

    BH_ASSERT(scene || scene->mRootNode || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE, "Falied to load model!");

    m_ModelDirectory = path.substr(0, path.find_last_of('/'));

    m_Meshes.reserve(scene->mNumMeshes);

    CollectMaterialInfo(scene);
    CollectNodeInfo(scene->mRootNode, scene);
}

void Model::CollectMaterialInfo(const aiScene* scene)
{
    m_Textures.reserve(scene->mNumMaterials);
    for (size_t i = 0; i < scene->mNumMaterials; ++i)
    {
        const aiMaterial* material = scene->mMaterials[i];
        LoadMaterialTexture(material, TextureType::Diffuse);
        LoadMaterialTexture(material, TextureType::Specular);
    }
}

void Model::LoadMaterialTexture(const aiMaterial* material, TextureType type)
{
    aiTextureType aiType = aiTextureType_NONE;
    switch (type)
    {
        case TextureType::Diffuse:   aiType = aiTextureType_DIFFUSE; break;
        case TextureType::Specular:  aiType = aiTextureType_SPECULAR; break;
        case TextureType::None:
        default: BH_ASSERT(false, "Unknown texture type!");
    }

    auto& textureManager = TextureManager::Get();

    for (size_t i = 0; i < material->GetTextureCount(aiType); ++i)
    {
        aiString relativeTexturePath;
        material->GetTexture(aiType, i, &relativeTexturePath);
        const std::string absoluteTexturePath = m_ModelDirectory + '/' + std::string(relativeTexturePath.C_Str());

        if (!m_Textures.contains(absoluteTexturePath))
        {
            m_Textures.emplace(relativeTexturePath.C_Str(),
                std::make_pair(textureManager.Load2D(absoluteTexturePath), type));
        }
    }
}

void Model::CollectNodeInfo(const aiNode* node, const aiScene* scene)
{
    for (size_t i = 0; i < node->mNumMeshes; ++i)
    {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.emplace_back(CreateRef<Mesh>(mesh, scene));
    }

    for (size_t i = 0; i < node->mNumChildren; ++i)
        CollectNodeInfo(node->mChildren[i], scene);
}
