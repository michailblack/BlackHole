#include "bhpch.h"
#include "BlackHole/Renderer/Mesh.h"

#include "BlackHole/Renderer/Model.h"
#include "Platform/OpenGL/Buffer.h"

Mesh::Mesh(const aiMesh* mesh, const aiScene* scene, const Model* parentModel)
    : m_ParentModel(parentModel)
{
    CollectMeshInfo(mesh);

    if (mesh->mMaterialIndex < scene->mNumMaterials)
    {
        const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        CollectMaterialTextureKeys(material, aiTextureType_DIFFUSE);
        CollectMaterialTextureKeys(material, aiTextureType_SPECULAR);
        CollectMaterialTextureKeys(material, aiTextureType_NORMALS);
        CollectMaterialTextureKeys(material, aiTextureType_DISPLACEMENT);
    }
}

void Mesh::CollectMeshInfo(const aiMesh* mesh)
{
    std::vector<Vertex> vertices;
    vertices.reserve(mesh->mNumVertices);

    for (size_t i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex = {};

        vertex.Position.x = mesh->mVertices[i].x;
        vertex.Position.y = mesh->mVertices[i].y;
        vertex.Position.z = mesh->mVertices[i].z;

        if (mesh->HasNormals())
        {
            vertex.Normal.x = mesh->mNormals[i].x;
            vertex.Normal.y = mesh->mNormals[i].y;
            vertex.Normal.z = mesh->mNormals[i].z;
        }

        if (mesh->HasTangentsAndBitangents())
        {
            vertex.Tangent.x = mesh->mTangents[i].x;
            vertex.Tangent.y = mesh->mTangents[i].y;
            vertex.Tangent.z = mesh->mTangents[i].z;
        }

        if (mesh->HasTextureCoords(0))
        {
            vertex.TexCoord.x = mesh->mTextureCoords[0][i].x;
            vertex.TexCoord.y = mesh->mTextureCoords[0][i].y;
        }

        vertices.push_back(vertex);
    }

    std::vector<uint32_t> pointIndices, lineIndices, triangleIndices;

    for (size_t i = 0; i < mesh->mNumFaces; ++i)
    {
        const aiFace& face = mesh->mFaces[i];

        switch (face.mNumIndices)
        {
            case 1:
                pointIndices.push_back(face.mIndices[0]);
                break;
            case 2:
                lineIndices.push_back(face.mIndices[0]);
                lineIndices.push_back(face.mIndices[1]);
                break;
            case 3:
                triangleIndices.push_back(face.mIndices[0]);
                triangleIndices.push_back(face.mIndices[1]);
                triangleIndices.push_back(face.mIndices[2]);
                break;
            default: BH_ASSERT(false, "Unsupported mesh primitive"); return;
        }
    }

    m_PointIndicesCount = pointIndices.size();
    m_LineIndicesCount = lineIndices.size();
    m_TriangleIndicesCount = triangleIndices.size();

    std::vector<uint32_t> indices;
    indices.reserve(m_PointIndicesCount + m_LineIndicesCount + m_TriangleIndicesCount);

    indices.insert(indices.end(), pointIndices.begin(), pointIndices.end());
    indices.insert(indices.end(), lineIndices.begin(), lineIndices.end());
    indices.insert(indices.end(), triangleIndices.begin(), triangleIndices.end());

    m_VertexArray = CreateRef<VertexArray>();
    const auto& vertexBuffer = CreateRef<VertexBuffer>(vertices.size() * sizeof(Vertex), reinterpret_cast<const float*>(vertices.data()));
    vertexBuffer->SetLayout({
        { ShaderDataType::Float3, "a_Position"  },
        { ShaderDataType::Float3, "a_Normal"    },
        { ShaderDataType::Float2, "a_TexCoord"  },
        { ShaderDataType::Float3, "a_Tangent"   }
    });
    m_VertexArray->AddVertexBuffer(vertexBuffer);
    m_VertexArray->SetIndexBuffer(CreateRef<IndexBuffer>(indices.data(), indices.size()));
}

void Mesh::CollectMaterialTextureKeys(const aiMaterial* material, aiTextureType type)
{
    const Ref<TextureArray2D>* textureArray = nullptr;
    uint32_t* textureIndices = nullptr;

    switch (type)
    {
        case aiTextureType_DIFFUSE:
            textureArray = &m_ParentModel->GetDiffuseMapArray();
            textureIndices = &m_DiffuseTextureLayer;
            break;
        case aiTextureType_SPECULAR:
            textureArray = &m_ParentModel->GetSpecularMapArray();
            textureIndices = &m_SpecularTextureLayer;
            break;
        case aiTextureType_NORMALS:
            textureArray = &m_ParentModel->GetNormalMapArray();
            textureIndices = &m_NormalTextureLayer;
            break;
        case aiTextureType_DISPLACEMENT:
            textureArray = &m_ParentModel->GetDisplacementMapArray();
            textureIndices = &m_DisplacementTextureLayer;
            break;
        default: BH_ASSERT(false, "Unknown texture type!"); return;
    }

    for (size_t i = 0; i < material->GetTextureCount(type); ++i)
    {
        aiString relativeTexturePath;
        material->GetTexture(type, i, &relativeTexturePath);

        std::filesystem::path path = relativeTexturePath.C_Str();

        const auto& it = std::ranges::find((*textureArray)->GetTextureKeys(), path.filename().string());

        if (it == std::ranges::end((*textureArray)->GetTextureKeys()))
            BH_ASSERT(false, "Failed to find texture in model array!");

        *textureIndices = it - std::ranges::begin((*textureArray)->GetTextureKeys());
    }
}