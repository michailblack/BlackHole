#pragma once
#include "BlackHole/Events/MouseEvent.h"

class PerspectiveCameraController
{
public:
    PerspectiveCameraController(const PerspectiveCamera& camera)
        : m_Camera(camera), m_CameraPosition(camera.GetPosition()), m_FOV(camera.GetFOV())
    {
    }
    ~PerspectiveCameraController() = default;
    void OnUpdate(Timestep ts);
    void OnEvent(Event& e);
    const PerspectiveCamera& GetCamera() const { return m_Camera; }

private:
    bool OnMouseMovedEvent(MouseMovedEvent& e);
    bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
    bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
    bool OnWindowResizeEvent(WindowResizeEvent& e);
    bool OnMouseScrolled(MouseScrolledEvent& e);
private:
    PerspectiveCamera m_Camera;

    glm::vec3 m_CameraPosition;

    float m_CameraTranslateSpeed = 5.0f;
    float m_CameraRotateSpeed = 3.0f;

    float m_Yaw;
    float m_Pitch;

    float m_LastMouseX;
    float m_LastMouseY;

    bool m_CameraWasRotated = false;
    bool m_FirstMouseMoveEvent = true;
    // In degrees
    float m_FOV;
};