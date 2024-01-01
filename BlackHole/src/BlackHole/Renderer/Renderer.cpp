#include "bhpch.h"
#include "BlackHole/Renderer/Renderer.h"

#include "Platform/OpenGL/Buffer.h"
#include "Platform/OpenGL/Cubemap.h"
#include "Platform/OpenGL/Shader.h"
#include "Platform/OpenGL/VertexArray.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

struct RendererData
{
    Ref<UniformBuffer> MatricesUniformBuffer;

    ::ShaderLibrary ShaderLibrary;

    //Ref<VertexArray> SkyboxVAO;
    //Ref<Cubemap> SkyboxCubemap;
    Ref<VertexArray> ScreenSquadVAO;

    Renderer::Statistics Stats;
} static s_Data;

void Renderer::Init()
{
    s_Data.MatricesUniformBuffer = CreateRef<UniformBuffer>(2 * sizeof(glm::mat4), 0);

    ShaderSpecification modelShaderSpec;
    modelShaderSpec.VertexPath = Filesystem::GetShadersPath() / "model.vs.glsl";
    modelShaderSpec.FragmentPath = Filesystem::GetShadersPath() / "model.fs.glsl";

    s_Data.ShaderLibrary.Load("Model", modelShaderSpec);
    s_Data.ShaderLibrary.Get("Model")->UploadInt("u_Material.Diffuse", 0);
    //s_Data.ShaderLibrary.Get("Model")->UploadInt("u_Material.Specular", 1);
    s_Data.ShaderLibrary.Get("Model")->UploadInt("u_Material.Normal", 2);
    //s_Data.ShaderLibrary.Get("Model")->UploadInt("u_Material.Displacement", 3);

    CubemapSpecification cbSpec;
    cbSpec.Right  = Filesystem::GetTexturesPath() / "skyboxes/mountains/right.jpg";
    cbSpec.Left   = Filesystem::GetTexturesPath() / "skyboxes/mountains/left.jpg";
    cbSpec.Top    = Filesystem::GetTexturesPath() / "skyboxes/mountains/top.jpg";
    cbSpec.Bottom = Filesystem::GetTexturesPath() / "skyboxes/mountains/bottom.jpg";
    cbSpec.Front  = Filesystem::GetTexturesPath() / "skyboxes/mountains/front.jpg";
    cbSpec.Back   = Filesystem::GetTexturesPath() / "skyboxes/mountains/back.jpg";

    /*s_Data.ShaderLibrary.Load(Filesystem::GetShadersPath() / "skybox.glsl");
    s_Data.SkyboxCubemap = CreateRef<Cubemap>(cbSpec);
    s_Data.SkyboxCubemap->Bind();

    s_Data.SkyboxVAO = CreateRef<VertexArray>();
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
            { ShaderDataType::Float3, "a_Position" }
        });
        const auto& skyboxIndexBuffer = CreateRef<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));

        s_Data.SkyboxVAO->AddVertexBuffer(skyboxVertexBuffer);
        s_Data.SkyboxVAO->SetIndexBuffer(skyboxIndexBuffer);
    }*/

    s_Data.ScreenSquadVAO = CreateRef<VertexArray>();
	{
	    float vertices[] = {
			-1.0f, -1.0f,	0.0f, 0.0f,
			 1.0f, -1.0f,	1.0f, 0.0f,
			 1.0f,  1.0f,	1.0f, 1.0f,

			 1.0f,  1.0f,	1.0f, 1.0f,
	        -1.0f,  1.0f,	0.0f, 1.0f,
	        -1.0f, -1.0f,	0.0f, 0.0f,
	    };

	    auto vbo = CreateRef<VertexBuffer>(sizeof(vertices) * sizeof(float), vertices);
		vbo->SetLayout({
			{ ShaderDataType::Float2, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		});
		s_Data.ScreenSquadVAO->AddVertexBuffer(vbo);
	}
}

void Renderer::Shutdown()
{
}

void Renderer::SetClearColor(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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

void Renderer::Submit(const Ref<Model>& model, const Ref<Shader>& shader, const glm::mat4& transform)
{
    if (model->GetDiffuseMapArray().get())
        model->GetDiffuseMapArray()->Bind();

    if (model->GetNormalMapArray().get())
        model->GetNormalMapArray()->Bind(1);

    shader->Bind();
    shader->UploadMat4("u_Model", transform);

    for (const auto& mesh : model->GetMeshes())
    {
        const auto& vertexArray = mesh->GetVertexArray();
        const uint32_t pointIndicesCount = mesh->GetPointIndicesCount();
        const uint32_t lineIndicesCount = mesh->GetLineIndicesCount();
        const uint32_t triangleIndicesCount = mesh->GetTriangleIndicesCount();

        shader->UploadUint("u_Material.DiffuseLayer", mesh->GetDiffuseTextureLayer());
        shader->UploadUint("u_Material.NormalLayer", mesh->GetNormalLayer());

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

void Renderer::RenderScreenQuad(const Ref<Shader>& shader, const std::initializer_list<uint32_t>& textureIDs)
{
    uint32_t i = 0;
    for (const uint32_t textureID : textureIDs)
        glBindTextureUnit(i++, textureID);

    shader->Bind();
    s_Data.ScreenSquadVAO->Bind();
    glDrawArrays(GL_TRIANGLES, 0, 12);
}

ShaderLibrary& Renderer::GetShaderLibrary()
{
    return s_Data.ShaderLibrary;
}

void Renderer::ResetStats()
{
    memset(&s_Data.Stats, 0, sizeof(Statistics));
}

Renderer::Statistics Renderer::GetStats()
{
    return s_Data.Stats;
}