#pragma once
#include "BlackHole.h"

struct PointLight
{
    glm::vec3 Position;
    glm::vec3 Color;
    float Linear;
    float Quadratic;
    float Intensity;
};

class EditorLayer : public Layer
{
public:
    EditorLayer();
    ~EditorLayer() override = default;

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(Timestep ts) override;
    void OnImGuiRender() override;
    void OnEvent(Event& e) override;
private:
    Ref<Framebuffer> m_FinalResultFBO;
    Ref<Framebuffer> m_GBufferFBO;
    Ref<Framebuffer> m_PingPongPostProcessFBO;

    PerspectiveCameraController m_CameraController;

    Ref<Model> m_FloorModel;
    Ref<Model> m_CarModel;
    Ref<Model> m_PointLightModel;

    std::vector<PointLight> m_PointLightsInfo;

    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;
    glm::vec2 m_ViewportSize;

    float m_FrameTime;
};
