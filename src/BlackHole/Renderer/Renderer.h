#pragma once
#include "BlackHole/Renderer/Camera.h"
#include "BlackHole/Renderer/Model.h"

class Renderer
{
public:
    static void Init();
    static void Shutdown();

    static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

    static void BeginScene(const PerspectiveCamera& camera);
    static void EndScene();

    static void Submit(const Ref<Model>& model, const glm::mat4& transform = glm::mat4(1.0f));

    static void DrawSkybox();

    // Stats
    struct Statistics
    {
        uint32_t DrawCalls = 0;
        uint32_t PointsCount = 0;
        uint32_t LinesCount = 0;
        uint32_t TriangleCount = 0;

        uint32_t GetTotalVertexCount() const { return TriangleCount * 3 + LinesCount * 2 + PointsCount; }
        uint32_t GetTotalIndexCount() const { return TriangleCount * 3 + LinesCount * 2 + PointsCount; }
    };
	static void ResetStats();
	static Statistics GetStats();
};