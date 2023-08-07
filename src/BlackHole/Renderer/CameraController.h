#pragma once
#include "Camera.h"

#include "BlackHole/Core/Timestep.h"

#include "BlackHole/Events/ApplicationEvent.h"
#include "BlackHole/Events/MouseEvent.h"

class PerspectiveCameraController
{
public:
    PerspectiveCameraController(const PerspectiveCamera& camera);
    ~PerspectiveCameraController() = default;

    void OnUpdate(Timestep ts);
    void OnEvent(Event& e);

    void OnResize(uint32_t width, uint32_t height);

    const PerspectiveCamera& GetCamera() const { return m_Camera; }
private:
    bool OnMouseMovedEvent(MouseMovedEvent& e);
    bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
    bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
    bool OnWindowResizeEvent(WindowResizeEvent& e);
    bool OnMouseScrolledEvent(MouseScrolledEvent& e);
private:
    float m_CameraTranslateSpeed = 5.0f;
    float m_CameraRotateSensitivity = 0.1f;

    float m_Yaw = 0.0f;
    float m_Pitch = 0.0f;

    float m_LastMouseX = 0.0f;
    float m_LastMouseY = 0.0f;

    bool m_CameraWasRotated = false;
    bool m_FirstMouseMoveEvent = true;

    PerspectiveCamera m_Camera;
    glm::vec3 m_CameraPosition;
    float m_FOV;
};