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
    static void Submit(const Ref<Shader>& shader, const Ref<Model>& model);
private:
    struct ScreenData
    {
        glm::mat4 ProjectionMatrix;
        glm::mat4 ViewMatrix;
    } static m_Data;
};