#include "CameraController.h"

#include "GLFW/glfw3.h"

void PerspectiveCameraController::OnUpdate(Timestep ts)
{
    float cameraTranslateSpeed = m_CameraTranslateSpeed;
    if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
    {
        cameraTranslateSpeed /= 4.0f;
    }

    if (Input::IsKeyPressed(GLFW_KEY_W))
    {
        m_CameraPosition += m_Camera.GetTargetDirection() * glm::vec3(cameraTranslateSpeed * ts);
        m_Camera.SetPosition(m_CameraPosition);
    }

    if (Input::IsKeyPressed(GLFW_KEY_S))
    {
        m_CameraPosition -= m_Camera.GetTargetDirection() * glm::vec3(cameraTranslateSpeed * ts);
        m_Camera.SetPosition(m_CameraPosition);
    }

    if (Input::IsKeyPressed(GLFW_KEY_A))
    {
        m_CameraPosition -= m_Camera.GetRightDirection() * glm::vec3(cameraTranslateSpeed * ts);
        m_Camera.SetPosition(m_CameraPosition);
    }

    if (Input::IsKeyPressed(GLFW_KEY_D))
    {
        m_CameraPosition += m_Camera.GetRightDirection() * glm::vec3(cameraTranslateSpeed * ts);
        m_Camera.SetPosition(m_CameraPosition);
    }

    if (Input::IsKeyPressed(GLFW_KEY_SPACE))
    {
        m_CameraPosition += m_Camera.GetUpDirection() * glm::vec3(cameraTranslateSpeed * ts);
        m_Camera.SetPosition(m_CameraPosition);
    }

    if (Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
    {
        m_CameraPosition -= m_Camera.GetUpDirection() * glm::vec3(cameraTranslateSpeed * ts);
        m_Camera.SetPosition(m_CameraPosition);
    }

    if (m_CameraWasRotated)
    {
        m_Camera.SetRotation(m_Yaw * ts, m_Pitch * ts);
        m_CameraWasRotated = false;
    }
}

void PerspectiveCameraController::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseButtonPressedEvent>(BH_BIND_EVENT_FN(OnMouseButtonPressedEvent));
    dispatcher.Dispatch<MouseMovedEvent>(BH_BIND_EVENT_FN(OnMouseMovedEvent));
    dispatcher.Dispatch<MouseButtonReleasedEvent>(BH_BIND_EVENT_FN(OnMouseButtonReleasedEvent));
    dispatcher.Dispatch<MouseScrolledEvent>(BH_BIND_EVENT_FN(OnMouseScrolled));
    dispatcher.Dispatch<WindowResizeEvent>(BH_BIND_EVENT_FN(OnWindowResizeEvent));
}

bool PerspectiveCameraController::OnMouseMovedEvent(MouseMovedEvent& e)
{
    if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
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

        m_Yaw   = offsetX * m_CameraRotateSpeed;
        m_Pitch = offsetY * m_CameraRotateSpeed;

        m_CameraWasRotated = true;
        return true;
    }
    return false;
}

bool PerspectiveCameraController::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
{
    const auto& window = Application::Get().GetWindow();
    if (e.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT)
    {
        glfwSetInputMode(window.GetWindowGLFW(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        return true;
    }
    return false;
}

bool PerspectiveCameraController::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
{
    const auto& window = Application::Get().GetWindow();
    if (e.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT)
    {
        glfwSetInputMode(window.GetWindowGLFW(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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

bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& e)
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