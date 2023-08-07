#include "Renderer.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "BlackHole/OpenGL/Cubemap.h"
#include "BlackHole/OpenGL/Shader.h"
#include "BlackHole/OpenGL/VertexArray.h"

struct RendererData
{
    Ref<UniformBuffer> MatricesUniformBuffer;

    Ref<Shader> ModelShader;

    Ref<Shader> SkyboxShader;
    Ref<VertexArray> SkyboxVertexArray;
    Ref<Cubemap> SkyboxCubemap;

    Renderer::Statistics Stats;
} static s_Data;

void Renderer::Init()
{
    s_Data.MatricesUniformBuffer = CreateRef<UniformBuffer>(2 * sizeof(glm::mat4), 0);

    s_Data.ModelShader = CreateRef<Shader>(Filesystem::GetShadersPath() / "model.glsl");
    s_Data.ModelShader->UploadInt("u_Material.Diffuse", 0);
    s_Data.ModelShader->UploadInt("u_Material.Specular", 0);

    CubemapSpecification cbSpec;
    cbSpec.Right  = Filesystem::GetTexturesPath() / "skyboxes/space/blue/right.png";
    cbSpec.Left   = Filesystem::GetTexturesPath() / "skyboxes/space/blue/left.png";
    cbSpec.Top    = Filesystem::GetTexturesPath() / "skyboxes/space/blue/top.png";
    cbSpec.Bottom = Filesystem::GetTexturesPath() / "skyboxes/space/blue/bottom.png";
    cbSpec.Front  = Filesystem::GetTexturesPath() / "skyboxes/space/blue/front.png";
    cbSpec.Back   = Filesystem::GetTexturesPath() / "skyboxes/space/blue/back.png";

    s_Data.SkyboxShader = CreateRef<Shader>(Filesystem::GetShadersPath() / "skybox.glsl");
    s_Data.SkyboxCubemap = CreateRef<Cubemap>(cbSpec);
    s_Data.SkyboxCubemap->Bind();

    s_Data.SkyboxVertexArray = CreateRef<VertexArray>();
    {
        float vertices[] = {
            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
        };

        uint32_t indices[] = {
            4, 5, 6,
            6, 7, 4,

            5, 1, 2,
            2, 6, 5,

            0, 4, 7,
            7, 3, 0,

            1, 0, 3,
            3, 2, 1,

            7, 6, 2,
            2, 3, 7,

            0, 1, 5,
            5, 4, 0
        };

        const auto& skyboxVertexBuffer = CreateRef<VertexBuffer>(sizeof(vertices), vertices);
        skyboxVertexBuffer->SetLayout({
            { ShaderDataType::Float3, "a_Pos" }
        });
        const auto& skyboxIndexBuffer = CreateRef<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));

        s_Data.SkyboxVertexArray->AddVertexBuffer(skyboxVertexBuffer);
        s_Data.SkyboxVertexArray->SetIndexBuffer(skyboxIndexBuffer);
    }
}

void Renderer::Shutdown()
{
}

void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    glViewport(static_cast<int32_t>(x), static_cast<int32_t>(y), static_cast<int32_t>(width), static_cast<int32_t>(height));
}

void Renderer::BeginScene(const PerspectiveCamera& camera)
{
    auto* const matricesUniformBufferRange = static_cast<glm::mat4*>(s_Data.MatricesUniformBuffer->Map(0, 2 * sizeof(glm::mat4)));
    *matricesUniformBufferRange = camera.GetProjectionMatrix();
    *(matricesUniformBufferRange + 1) = camera.GetViewMatrix();
    s_Data.MatricesUniformBuffer->Unmap();
}

void Renderer::EndScene()
{
}

void Renderer::Submit(const Ref<Model>& model, const glm::mat4& transform)
{
    model->GetDiffuseMapArray()->Bind();

    for (const auto& mesh : model->GetMeshes())
    {
        const auto& vertexArray = mesh->GetVertexArray();
        const uint32_t pointIndicesCount = mesh->GetPointIndicesCount();
        const uint32_t lineIndicesCount = mesh->GetLineIndicesCount();
        const uint32_t triangleIndicesCount = mesh->GetTriangleIndicesCount();

        s_Data.ModelShader->UploadMat4("u_Model", transform);
        s_Data.ModelShader->UploadUint("u_Material.DiffuseLayer", mesh->GetDiffuseTextureLayer());

        s_Data.ModelShader->Bind();
        vertexArray->Bind();
        if (pointIndicesCount)
        {
            glDrawRangeElements(GL_POINTS,
                0,
                pointIndicesCount - 1,
                static_cast<int32_t>(pointIndicesCount),
                GL_UNSIGNED_INT,
                nullptr
            );
            ++s_Data.Stats.DrawCalls;
            s_Data.Stats.PointsCount += pointIndicesCount;
        }
        if (lineIndicesCount)
        {
            glDrawRangeElements(GL_LINES,
                pointIndicesCount,
                pointIndicesCount + lineIndicesCount - 1,
                static_cast<int32_t>(lineIndicesCount),
                GL_UNSIGNED_INT,
                nullptr
            );
            ++s_Data.Stats.DrawCalls;
            s_Data.Stats.LinesCount += lineIndicesCount / 2;
        }
        if (triangleIndicesCount)
        {
            glDrawRangeElements(GL_TRIANGLES,
                lineIndicesCount,
                lineIndicesCount + triangleIndicesCount - 1,
                static_cast<int32_t>(triangleIndicesCount),
                GL_UNSIGNED_INT,
                nullptr
            );
            ++s_Data.Stats.DrawCalls;
            s_Data.Stats.TriangleCount += triangleIndicesCount / 3;
        }
    }
}

void Renderer::DrawSkybox()
{
    s_Data.SkyboxShader->Bind();
    s_Data.SkyboxVertexArray->Bind();
    glDrawElements(GL_TRIANGLES, static_cast<int32_t>(s_Data.SkyboxVertexArray->GetIndexBuffer()->GetCount()), GL_UNSIGNED_INT, nullptr);
}

void Renderer::ResetStats()
{
    memset(&s_Data.Stats, 0, sizeof(Statistics));
}

Renderer::Statistics Renderer::GetStats()
{
    return s_Data.Stats;
}