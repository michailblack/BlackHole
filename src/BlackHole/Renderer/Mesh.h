#pragma once
#include "Buffer.h"

#include <assimp/scene.h>

class Mesh
{
public:
    explicit Mesh(const aiMesh* mesh, const aiScene* scene);

    const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }
    const std::vector<std::string>& GetTextureKeys() const { return m_TextureKeys; }
    uint64_t GetPointsIndicesCount() const { return m_CountPointsIndices; }
    uint64_t GetLinesIndicesCount() const { return m_CountLinesIndices; }
    uint64_t GetTrianglesIndicesCount() const { return m_CountTrianglesIndices; }
private:
    void CollectMeshInfo(const aiMesh* mesh, const aiScene* scene);
    void CollectMaterialTextureKeys(const aiMaterial* material, aiTextureType type);
private:
    Ref<VertexArray> m_VertexArray;
    std::vector<std::string> m_TextureKeys;
    uint64_t m_CountPointsIndices;
    uint64_t m_CountLinesIndices;
    uint64_t m_CountTrianglesIndices;
};