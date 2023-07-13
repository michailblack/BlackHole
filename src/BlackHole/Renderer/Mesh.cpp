#include "Mesh.h"

struct Vertex
{
    glm::vec3 Position      = glm::vec3(0.0f);
    glm::vec3 Normal        = glm::vec3(0.0f);
    glm::vec2 TextureCoords = glm::vec2(0.0f);
};

Mesh::Mesh(const aiMesh* mesh, const aiScene* scene)
    : m_VertexArray(CreateRef<VertexArray>())
    , m_CountPointsIndices(0)
    , m_CountLinesIndices(0)
    , m_CountTrianglesIndices(0)
{
    CollectMeshInfo(mesh, scene);
}

void Mesh::CollectMeshInfo(const aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices(mesh->mNumVertices);
    std::vector<uint32_t> pointsIndices, linesIndices, trianglesIndices;

    for (size_t i = 0; i < mesh->mNumVertices; ++i)
    {
        Vertex vertex;
        vertex.Position.x = mesh->mVertices[i].x;
        vertex.Position.y = mesh->mVertices[i].y;
        vertex.Position.z = mesh->mVertices[i].z;

        if (mesh->HasNormals())
        {
            vertex.Normal.x = mesh->mNormals[i].x;
            vertex.Normal.y = mesh->mNormals[i].y;
            vertex.Normal.z = mesh->mNormals[i].z;
        }

        if (mesh->HasTextureCoords(0))
        {
            vertex.TextureCoords.x = mesh->mTextureCoords[0][i].x;
            vertex.TextureCoords.y = mesh->mTextureCoords[0][i].y;
        }

        vertices[i] = vertex;
    }

    for (size_t i = 0; i < mesh->mNumFaces; ++i)
    {
        const aiFace& face = mesh->mFaces[i];

        switch (face.mNumIndices)
        {
            case 1:
                pointsIndices.push_back(face.mIndices[0]);
                break;
            case 2:
                linesIndices.push_back(face.mIndices[0]);
                linesIndices.push_back(face.mIndices[1]);
                break;
            case 3:
                trianglesIndices.push_back(face.mIndices[0]);
                trianglesIndices.push_back(face.mIndices[1]);
                trianglesIndices.push_back(face.mIndices[2]);
                break;
            default: BH_ASSERT(false, "Unsupported mesh primitive"); return;
        }
    }

    m_CountPointsIndices = pointsIndices.size();
    m_CountLinesIndices = linesIndices.size();
    m_CountTrianglesIndices = trianglesIndices.size();

    std::vector<uint32_t> indices;
    indices.reserve(m_CountPointsIndices + m_CountLinesIndices + m_CountTrianglesIndices);
    indices.insert(indices.end(), pointsIndices.begin(), pointsIndices.end());
    indices.insert(indices.end(), linesIndices.begin(), linesIndices.end());
    indices.insert(indices.end(), trianglesIndices.begin(), trianglesIndices.end());

    if (mesh->mMaterialIndex < scene->mNumMaterials)
    {
        const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        m_TextureKeys.reserve(material->GetTextureCount(aiTextureType_DIFFUSE) + material->GetTextureCount(aiTextureType_SPECULAR));
        CollectMaterialTextureKeys(material, aiTextureType_DIFFUSE);
        CollectMaterialTextureKeys(material, aiTextureType_SPECULAR);
    }

    const auto vertexBuffer = CreateRef<VertexBuffer>(reinterpret_cast<float*>(vertices.data()), 
        static_cast<int32_t>(vertices.size() * sizeof(Vertex)));
    const auto indexBuffer = CreateRef<IndexBuffer>(indices.data(), 
        static_cast<uint32_t>(indices.size()));

    vertexBuffer->SetLayout({
        {ShaderDataType::Float3, "a_Position"     },
        {ShaderDataType::Float3, "a_Normal"       },
        {ShaderDataType::Float2, "a_TextureCoords"}
    });

    m_VertexArray->AddVertexBuffer(vertexBuffer);
    m_VertexArray->SetIndexBuffer(indexBuffer);
}

void Mesh::CollectMaterialTextureKeys(const aiMaterial* material, aiTextureType type)
{
    for (size_t i = 0; i < material->GetTextureCount(type); ++i)
    {
        aiString relativeTexturePath;
        material->GetTexture(type, i, &relativeTexturePath);

        m_TextureKeys.emplace_back(relativeTexturePath.C_Str());
    }
}