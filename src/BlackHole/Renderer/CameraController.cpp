#include "CameraController.h"

#include "GLFW/glfw3.h"

void PerspectiveCameraController::OnUpdate(Timestep ts)
{
    /// TODO
    /// -Fix mouse position reaction on borders artifact
    if (Input::IsKeyPressed(GLFW_KEY_W))
    {
        m_CameraPosition += m_Camera->GetTargetDirection() * glm::vec3(m_CameraTranslateSpeed * ts);
        m_Camera->SetCameraPosition(m_CameraPosition);
    }

    if (Input::IsKeyPressed(GLFW_KEY_S))
    {
        m_CameraPosition -= m_Camera->GetTargetDirection() * glm::vec3(m_CameraTranslateSpeed * ts);
        m_Camera->SetCameraPosition(m_CameraPosition);
    }

    if (Input::IsKeyPressed(GLFW_KEY_A))
    {
        m_CameraPosition -= m_Camera->GetRightDirection() * glm::vec3(m_CameraTranslateSpeed * ts);
        m_Camera->SetCameraPosition(m_CameraPosition);
    }

    if (Input::IsKeyPressed(GLFW_KEY_D))
    {
        m_CameraPosition += m_Camera->GetRightDirection() * glm::vec3(m_CameraTranslateSpeed * ts);
        m_Camera->SetCameraPosition(m_CameraPosition);
    }

    if (Input::IsKeyPressed(GLFW_KEY_SPACE))
    {
        m_CameraPosition += m_Camera->GetUpDirection() * glm::vec3(m_CameraTranslateSpeed * ts);
        m_Camera->SetCameraPosition(m_CameraPosition);
    }

    if (Input::IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
    {
        m_CameraPosition -= m_Camera->GetUpDirection() * glm::vec3(m_CameraTranslateSpeed * ts);
        m_Camera->SetCameraPosition(m_CameraPosition);
    }

    if (m_CameraWasRotated)
    {
        static_cast<PerspectiveCamera*>(m_Camera)->SetCameraRotation(m_OffsetX * m_CameraRotateSpeed * ts, m_OffsetY * m_CameraRotateSpeed * ts);
        m_CameraWasRotated = false;
    }
}

void PerspectiveCameraController::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseButtonPressedEvent>(BH_BIND_EVENT_FN(OnMouseButtonPressedEvent));
    dispatcher.Dispatch<MouseMovedEvent>(BH_BIND_EVENT_FN(OnMouseMovedEvent));
    dispatcher.Dispatch<MouseButtonReleasedEvent>(BH_BIND_EVENT_FN(OnMouseButtonReleasedEvent));
    dispatcher.Dispatch<WindowResizeEvent>(BH_BIND_EVENT_FN(OnWindowResizeEvent));
}

bool PerspectiveCameraController::OnMouseMovedEvent(MouseMovedEvent& e)
{
    const auto& window = Application::Get().GetWindow();
    if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
    {
        const glm::vec2 centerPos = { static_cast<float>(window.GetWidth()) / 2.0f, static_cast<float>(window.GetHeight()) / 2.0f };
        const float offsetX = e.GetX() - centerPos.x;
        const float offsetY = e.GetY() - centerPos.y;

        m_OverallOffsetY += glm::radians(offsetY * m_CameraRotateSpeed);
        if (m_OverallOffsetY >= 89.0f || m_OverallOffsetY <= -89.0f)
        {
            m_OverallOffsetY -= glm::radians(offsetY * m_CameraRotateSpeed);
            m_OffsetY = 0.0f;
        }
        else
        {
            m_OffsetY = offsetY;
        }
        m_OffsetX = offsetX;

        m_CameraWasRotated = true;

        glfwSetCursorPos(window.GetWindowGLFW(), centerPos.x, centerPos.y);
    }
    return true;
}

bool PerspectiveCameraController::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
{
    const auto& window = Application::Get().GetWindow();
    if (e.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT)
    {
        glfwSetCursorPos(window.GetWindowGLFW(), static_cast<double>(window.GetWidth()) / 2.0, static_cast<double>(window.GetHeight()) / 2.0);
        glfwSetInputMode(window.GetWindowGLFW(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    return true;
}

bool PerspectiveCameraController::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
{
    const auto& window = Application::Get().GetWindow();
    if (e.GetMouseButton() == GLFW_MOUSE_BUTTON_LEFT)
    {
        glfwSetInputMode(window.GetWindowGLFW(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    return true;
}

bool PerspectiveCameraController::OnWindowResizeEvent(WindowResizeEvent& e)
{
    static_cast<PerspectiveCamera*>(m_Camera)->SetAspectRatio(static_cast<float>(e.GetWidth()) / static_cast<float>(e.GetHeight()));
    return true;
}