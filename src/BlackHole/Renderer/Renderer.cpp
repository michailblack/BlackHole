#include "Renderer.h"

#include <glad/glad.h>

extern Ref<Shader> g_OutlineShader;

Renderer::RendererData Renderer::m_Data;

void Renderer::SetClearColor(glm::vec4 color)
{
    glClearColor(color.x, color.y, color.z, color.w);
}

void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::SetViewport(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
}

void Renderer::BeginScene(const std::unique_ptr<Camera>& camera)
{
    m_Data.ProjectionMatrix = camera->GetProjectionMatrix();
    m_Data.ViewMatrix = camera->GetViewMatrix();
}

void Renderer::EndScene()
{
}

void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray)
{
    shader->UploadMat4("u_Projection", m_Data.ProjectionMatrix);
    shader->UploadMat4("u_View"      , m_Data.ViewMatrix);
    shader->UploadMat4("u_Model"     , glm::mat4(1.0f));

    shader->Bind();
    vertexArray->Bind();
    glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Submit(const Ref<Shader>& shader, const Ref<Shader>& outlineShader, const Ref<Model>& model)
{
    uint8_t diffuseMapIndex = 0;
    uint8_t specularMapIndex = 0;

    shader->Bind();
    shader->UploadMat4("u_Projection", m_Data.ProjectionMatrix);
    shader->UploadMat4("u_View"      , m_Data.ViewMatrix);
    shader->UploadMat4("u_Model"     , glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));

    shader->UploadFloat3("dirLight.direction", glm::vec3(1.0f, -1.0f, 1.0f));
    shader->UploadFloat3("dirLight.ambient"  , glm::vec3(0.2f));
    shader->UploadFloat3("dirLight.diffuse"  , glm::vec3(0.8f));
    shader->UploadFloat3("dirLight.specular" , glm::vec3(0.5f));

    shader->UploadFloat("material.shininess", 0.2f);

    auto& textures = model->GetTextures();
    for (const auto& mesh : model->GetMeshes())
    {
        size_t i = 0;
        for (const auto& textureKey : mesh->GetTextureKeys())
        {
            const auto& [texture, type] = textures.at(textureKey);
            if (type == TextureType::Diffuse)
            {
                texture->Bind(i);
                shader->UploadInt("material.diffuse[" + std::to_string(diffuseMapIndex++) + "]", i);
            }
            else if (type == TextureType::Specular)
            {
                texture->Bind(i);
                shader->UploadInt("material.specular[" + std::to_string(specularMapIndex++) + "]", i);
            }
            ++i;
        }

        shader->UploadInt("u_DiffuseMapsUsed" , std::min(++diffuseMapIndex , static_cast<uint8_t>(8)));
        shader->UploadInt("u_SpecularMapsUsed", std::min(++specularMapIndex, static_cast<uint8_t>(8)));

        diffuseMapIndex = 0;
        specularMapIndex = 0;

        mesh->GetVertexArray()->Bind();
        glDrawRangeElements(GL_POINTS, 0, mesh->GetPointsIndicesCount() - 1, mesh->GetPointsIndicesCount(), GL_UNSIGNED_INT, nullptr);
        glDrawRangeElements(GL_LINES, mesh->GetPointsIndicesCount(), mesh->GetLinesIndicesCount() - 1, mesh->GetLinesIndicesCount(), GL_UNSIGNED_INT, nullptr);
        glDrawRangeElements(GL_TRIANGLES, mesh->GetLinesIndicesCount(), mesh->GetTrianglesIndicesCount() - 1, mesh->GetTrianglesIndicesCount(), GL_UNSIGNED_INT, nullptr);
    }

    glDisable(GL_DEPTH_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);

    outlineShader->Bind();
    outlineShader->UploadMat4("u_Projection", m_Data.ProjectionMatrix);
    outlineShader->UploadMat4("u_View"      , m_Data.ViewMatrix);
    outlineShader->UploadMat4("u_Model"     , glm::scale(glm::mat4(1.0f), glm::vec3(1.01f)));
    for (const auto& mesh : model->GetMeshes())
    {
        mesh->GetVertexArray()->Bind();
        glDrawRangeElements(GL_POINTS, 0, mesh->GetPointsIndicesCount() - 1, mesh->GetPointsIndicesCount(), GL_UNSIGNED_INT, nullptr);
        glDrawRangeElements(GL_LINES, mesh->GetPointsIndicesCount(), mesh->GetLinesIndicesCount() - 1, mesh->GetLinesIndicesCount(), GL_UNSIGNED_INT, nullptr);
        glDrawRangeElements(GL_TRIANGLES, mesh->GetLinesIndicesCount(), mesh->GetTrianglesIndicesCount() - 1, mesh->GetTrianglesIndicesCount(), GL_UNSIGNED_INT, nullptr);
    }
}