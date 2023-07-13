#pragma once
#include "BlackHole/Events/MouseEvent.h"

class CameraController
{
public:
    CameraController(Camera* camera)
        : m_Camera(camera)
        , m_CameraPosition(camera->GetCameraPosition())
    {
    }
    virtual ~CameraController() = default;

    virtual void OnUpdate(Timestep ts) = 0;
    virtual void OnEvent(Event& e) = 0;
protected:
    Camera* m_Camera;

    glm::vec3 m_CameraPosition;
    float m_CameraTranslateSpeed = 5.0f;
    float m_CameraRotateSpeed = 3.0f;
    float m_Yaw;
    float m_Pitch;
    float m_LastMouseX;
    float m_LastMouseY;
    bool m_CameraWasRotated = false;
    bool m_FirstMouseMoveEvent = true;
};

class PerspectiveCameraController : public CameraController
{
public:
    PerspectiveCameraController(PerspectiveCamera* camera)
        : CameraController(camera), m_FOV(camera->GetFOV())
    {
    }
    ~PerspectiveCameraController() override = default;
    void OnUpdate(Timestep ts) override;
    void OnEvent(Event& e) override;
private:
    bool OnMouseMovedEvent(MouseMovedEvent& e);
    bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
    bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
    bool OnWindowResizeEvent(WindowResizeEvent& e);
    bool OnMouseScrolled(MouseScrolledEvent& e);
private:
    // In degrees
    float m_FOV;
};