#include "Renderer.h"

#include <glad/glad.h>

Renderer::RendererData Renderer::m_Data;

void Renderer::SetClearColor(glm::vec4 color)
{
    glClearColor(color.x, color.y, color.z, color.w);
}

void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::SetViewport(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
}

void Renderer::BeginScene(const std::unique_ptr<Camera>& camera)
{
    m_Data.ViewProjectionMatrix = camera->GetViewProjectionMatrix();
}

void Renderer::EndScene()
{
}

void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray)
{
    shader->UploadUniformMat4("u_ViewProjectionMatrix", m_Data.ViewProjectionMatrix);
    shader->Bind();
    vertexArray->Bind();
    glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
}