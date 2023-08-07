#pragma once
#include "BlackHole/Core/Layer.h"

#include "BlackHole/OpenGL/Framebuffer.h"

#include "BlackHole/Renderer/CameraController.h"
#include "BlackHole/Renderer/Model.h"

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
    PerspectiveCameraController m_CameraController;
    Ref<Framebuffer> m_FramebufferMSAA;
    Ref<Framebuffer> m_Framebuffer;
    Ref<Model> m_Model;
    glm::vec3 m_ModelTranslation = glm::vec3(0.0f);
    glm::vec3 m_ModelRotation = glm::vec3(0.0f);
    glm::vec3 m_ModelScale = glm::vec3(1.0f);

    float m_FPS;

    bool m_ViewportFocused = false;
    bool m_ViewportHovered = false;
    glm::vec2 m_ViewportSize;
};
