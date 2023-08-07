#include "CameraController.h"

#include <GLFW/glfw3.h>

#include "BlackHole/Core/Application.h"
#include "BlackHole/Core/Input.h"

PerspectiveCameraController::PerspectiveCameraController(const PerspectiveCamera& camera)
    : m_Camera(camera), m_CameraPosition(camera.GetPosition()), m_FOV(camera.GetFOV())
{
}

void PerspectiveCameraController::OnUpdate(Timestep ts)
{
    if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
    {
        if (Input::IsKeyPressed(GLFW_KEY_W))
        {
            m_CameraPosition += m_Camera.GetTargetDirection() * glm::vec3(m_CameraTranslateSpeed * ts);
        }
        if (Input::IsKeyPressed(GLFW_KEY_S))
        {
            m_CameraPosition -= m_Camera.GetTargetDirection() * glm::vec3(m_CameraTranslateSpeed * ts);
        }

        if (Input::IsKeyPressed(GLFW_KEY_A))
        {
            m_CameraPosition -= m_Camera.GetRightDirection() * glm::vec3(m_CameraTranslateSpeed * ts);
        }
        if (Input::IsKeyPressed(GLFW_KEY_D))
        {
            m_CameraPosition += m_Camera.GetRightDirection() * glm::vec3(m_CameraTranslateSpeed * ts);
        }

        if (Input::IsKeyPressed(GLFW_KEY_E))
        {
            m_CameraPosition += m_Camera.GetUpDirection() * glm::vec3(m_CameraTranslateSpeed * ts);
        }
        if (Input::IsKeyPressed(GLFW_KEY_Q))
        {
            m_CameraPosition -= m_Camera.GetUpDirection() * glm::vec3(m_CameraTranslateSpeed * ts);
        }

        m_Camera.SetPosition(m_CameraPosition);

        if (m_CameraWasRotated)
        {
            m_Camera.SetRotation(m_Yaw * ts, m_Pitch * ts);
            m_CameraWasRotated = false;
        }
    }
}

void PerspectiveCameraController::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseButtonPressedEvent>(BH_BIND_EVENT_FN(OnMouseButtonPressedEvent));
    dispatcher.Dispatch<MouseMovedEvent>(BH_BIND_EVENT_FN(OnMouseMovedEvent));
    dispatcher.Dispatch<MouseButtonReleasedEvent>(BH_BIND_EVENT_FN(OnMouseButtonReleasedEvent));
    dispatcher.Dispatch<MouseScrolledEvent>(BH_BIND_EVENT_FN(OnMouseScrolledEvent));
    dispatcher.Dispatch<WindowResizeEvent>(BH_BIND_EVENT_FN(OnWindowResizeEvent));
}

void PerspectiveCameraController::OnResize(uint32_t width, uint32_t height)
{
    m_Camera.SetAspectRatio(static_cast<float>(width) / static_cast<float>(height));
}

bool PerspectiveCameraController::OnMouseMovedEvent(MouseMovedEvent& e)
{
    if (m_FirstMouseMoveEvent)
    {
        m_LastMouseX = e.GetX();
        m_LastMouseY = e.GetY();
        m_FirstMouseMoveEvent = false;
    }
    const float offsetX = e.GetX() - m_LastMouseX;
    const float offsetY = e.GetY() - m_LastMouseY;
    m_LastMouseX = e.GetX();
    m_LastMouseY = e.GetY();

    m_Yaw   = offsetX * m_CameraRotateSensitivity;
    m_Pitch = offsetY * m_CameraRotateSensitivity;

    m_CameraWasRotated = true;
    return true;
}

bool PerspectiveCameraController::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
{
    const auto& window = Application::Get().GetWindow();
    if (e.GetMouseButton() == GLFW_MOUSE_BUTTON_RIGHT)
    {
        glfwSetInputMode(window.GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(window.GetNativeWindow(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        return true;
    }
    return false;
}

bool PerspectiveCameraController::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
{
    const auto& window = Application::Get().GetWindow();
    if (e.GetMouseButton() == GLFW_MOUSE_BUTTON_RIGHT)
    {
        glfwSetInputMode(window.GetNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        m_FirstMouseMoveEvent = true;
        return true;
    }
    return false;
}

bool PerspectiveCameraController::OnWindowResizeEvent(WindowResizeEvent& e)
{
    m_Camera.SetAspectRatio(static_cast<float>(e.GetWidth()) / static_cast<float>(e.GetHeight()));
    return true;
}

bool PerspectiveCameraController::OnMouseScrolledEvent(MouseScrolledEvent& e)
{
    m_FOV -= e.GetYOffset();

    if (m_FOV < 1.0f)
    {
        m_FOV = 1.0f;
    }

    if (m_FOV > 45.0f)
    {
        m_FOV = 45.0f;
    }

    m_Camera.SetFOV(m_FOV);

    return true;
}