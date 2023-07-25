#pragma once

#include "BlackHole/OpenGL/Cubemap.h"
#include "BlackHole/OpenGL/UniformBuffer.h"
#include "BlackHole/OpenGL/VertexArray.h"
#include "BlackHole/Renderer/Camera.h"
#include "BlackHole/Renderer/Model.h"

class Renderer
{
public:
    static void SetClearColor(glm::vec4 color) { glClearColor(color.x, color.y, color.z, color.w); }
    static void Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); }
    static void SetViewport(int x, int y, int width, int height) { glViewport(x, y, width, height); }

    static void Init();

    static void BeginScene(const PerspectiveCamera& camera);
    static void EndScene();

    static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform);
    static void Submit(const Ref<Shader>& shader, const Ref<Model>& model, const glm::mat4& transform);

    static ShaderLibrary& GetShaderLibrary() { return m_Data.ShaderLibrary; }
private:
    struct RendererData
    {
        ShaderLibrary ShaderLibrary;
        Ref<UniformBuffer> MatricesUB;

        Ref<Cubemap> Skybox;
    } static m_Data;
};