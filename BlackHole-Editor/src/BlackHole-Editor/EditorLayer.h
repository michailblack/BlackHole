#pragma once
#include "BlackHole.h"

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
    Ref<Framebuffer> m_ViewportFBO;
    Ref<Framebuffer> m_GBufferFBO;

    Ref<UniformBuffer> m_PointLights;

    PerspectiveCameraController m_CameraController;

    Ref<Model> m_Model;

    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;
    glm::vec2 m_ViewportSize;

    float m_FrameTime;
};
