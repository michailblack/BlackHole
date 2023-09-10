#pragma once
#include <assimp/scene.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Platform/OpenGL/VertexArray.h"

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
    glm::vec3 Tangent;
};

class Model;

class Mesh
{
public:
    explicit Mesh(const aiMesh* mesh, const aiScene* scene, const Model* parentModel);

    uint32_t GetDiffuseTextureLayer() const { return m_DiffuseTextureLayer; }
    uint32_t GetSpecularTextureLayer() const { return m_SpecularTextureLayer; }
    uint32_t GetNormalLayer() const { return m_NormalTextureLayer; }
    uint32_t GetDisplacementLayer() const { return m_DisplacementTextureLayer; }

    const Ref<VertexArray>& GetVertexArray() const { return m_VertexArray; }

    uint32_t GetPointIndicesCount() const { return m_PointIndicesCount; }
    uint32_t GetLineIndicesCount() const { return m_LineIndicesCount; }
    uint32_t GetTriangleIndicesCount() const { return m_TriangleIndicesCount; }
private:
    void CollectMeshInfo(const aiMesh* mesh);
    void CollectMaterialTextureKeys(const aiMaterial* material, aiTextureType type);
private:
    const Model* const m_ParentModel;

    Ref<VertexArray> m_VertexArray;

    uint32_t m_PointIndicesCount;
    uint32_t m_LineIndicesCount;
    uint32_t m_TriangleIndicesCount;

    uint32_t m_DiffuseTextureLayer, m_SpecularTextureLayer, m_NormalTextureLayer, m_DisplacementTextureLayer;
};