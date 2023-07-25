#include "Renderer.h"

#include <glad/glad.h>

#include "GLFW/glfw3.h"
#include "glm/gtc/type_ptr.hpp"

Renderer::RendererData Renderer::m_Data;

void Renderer::Init()
{
    m_Data.MatricesUB = CreateRef<UniformBuffer>(2 * sizeof(glm::mat4), 0);

    CubemapSpecification cbSpec;
    cbSpec.Right  = "../../../assets/textures/skyboxes/space/blue/right.png";
    cbSpec.Left   = "../../../assets/textures/skyboxes/space/blue/left.png";
    cbSpec.Top    = "../../../assets/textures/skyboxes/space/blue/top.png";
    cbSpec.Bottom = "../../../assets/textures/skyboxes/space/blue/bottom.png";
    cbSpec.Front  = "../../../assets/textures/skyboxes/space/blue/front.png";
    cbSpec.Back   = "../../../assets/textures/skyboxes/space/blue/back.png";

    m_Data.Skybox = CreateRef<Cubemap>(cbSpec);

    m_Data.Skybox->Bind();
}

void Renderer::BeginScene(const PerspectiveCamera& camera)
{
    m_Data.MatricesUB->SetData(0, sizeof(glm::mat4), glm::value_ptr(camera.GetProjectionMatrix()));
    m_Data.MatricesUB->SetData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.GetViewMatrix()));

    m_Data.ShaderLibrary.Get("skybox")->UploadMat4("u_CameraRotation", camera.GetCameraRotationMatrix());
}

void Renderer::EndScene()
{
}

void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
{
    shader->Bind();

    vertexArray->Bind();
    glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Submit(const Ref<Shader>& shader, const Ref<Model>& model, const glm::mat4& transform)
{
    uint8_t diffuseMapIndex = 0;
    uint8_t specularMapIndex = 0;

    shader->Bind();
    shader->UploadMat4("u_Model", transform);

    auto& textures = model->GetTextures();
    for (const auto& mesh : model->GetMeshes())
    {
        uint32_t i = 0;
        for (const auto& textureKey : mesh->GetTextureKeys())
        {
            const auto& [texture, type] = textures.at(textureKey);
            if (type == TextureType::Diffuse)
            {
                texture->Bind(i);
                shader->UploadInt("u_Material.diffuse[" + std::to_string(diffuseMapIndex++) + "]", i++);
            }
            else if (type == TextureType::Specular)
            {
                texture->Bind(i);
                shader->UploadInt("u_Material.specular[" + std::to_string(specularMapIndex++) + "]", i++);
            }
        }

        shader->UploadFloat("u_DiffuseMapsUsed", ++diffuseMapIndex);
        shader->UploadFloat("u_SpecularMapsUsed", ++specularMapIndex);

        diffuseMapIndex = 0;
        specularMapIndex = 0;

        mesh->GetVertexArray()->Bind();
        glDrawRangeElements(GL_POINTS, 0, mesh->GetPointsIndicesCount() - 1, mesh->GetPointsIndicesCount(), GL_UNSIGNED_INT, nullptr);
        glDrawRangeElements(GL_LINES, mesh->GetPointsIndicesCount(), mesh->GetLinesIndicesCount() - 1, mesh->GetLinesIndicesCount(), GL_UNSIGNED_INT, nullptr);
        glDrawRangeElements(GL_TRIANGLES, mesh->GetLinesIndicesCount(), mesh->GetTrianglesIndicesCount() - 1, mesh->GetTrianglesIndicesCount(), GL_UNSIGNED_INT, nullptr);
    }
}