#pragma once

class Renderer
{
public:
    static void SetClearColor(glm::vec4 color);
    static void Clear();

    static void SetViewport(int x, int y, int width, int height);

    static void BeginScene(const std::unique_ptr<Camera>& camera);
    static void EndScene();

    static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray);
private:
    struct RendererData
    {
        glm::mat4 ViewProjectionMatrix;
    } static m_Data;
};