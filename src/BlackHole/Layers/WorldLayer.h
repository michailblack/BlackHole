#pragma once

#include "BlackHole/Core/Layer.h"
#include "BlackHole/Renderer/CameraController.h"

class WorldLayer : public Layer
{
public:
    WorldLayer();
    ~WorldLayer() override = default;

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(Timestep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Event& e) override;
private:
    PerspectiveCameraController m_CameraController;

    Ref<VertexArray>  m_PointsVAO;
    Ref<VertexBuffer> m_PointsVBO;
    Ref<IndexBuffer>  m_PointsIBO;

    Ref<VertexArray>  m_CubeVAO;
    Ref<VertexBuffer> m_CubeVBO;
    Ref<IndexBuffer>  m_CubeIBO;

    Ref<VertexArray>  m_ScreenSquadVAO;
    Ref<VertexBuffer> m_ScreenSquadVBO;
    Ref<IndexBuffer>  m_ScreenSquadIBO;

    Ref<Model> m_Model;
    Ref<Model> m_Rock;
};