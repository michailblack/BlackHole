#include "CameraController.h"

#include "GLFW/glfw3.h"

void PerspectiveCameraController::OnUpdate(Timestep ts)
{
    /// TODO
    /// -Fix camera translation after its rotation
    /// -Fix mouse position reaction on borders artifact
    if (Input::IsKeyPressed(GLFW_KEY_W))
    {
        m_CameraPosition += m_Camera->GetDirection() * glm::vec3(m_CameraTranslateSpeed * ts);
        m_Camera->SetCameraPosition(m_CameraPosition);
    }

    if (Input::IsKeyPressed(GLFW_KEY_S))
    {
        m_CameraPosition -= m_Camera->GetDirection() * glm::vec3(m_CameraTranslateSpeed * ts);
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
        m_Camera->SetCameraRotation(m_OffsetX * m_CameraRotateSpeed * ts, m_OffsetY * m_CameraRotateSpeed * ts);
        m_CameraWasRotated = false;
    }
}

void PerspectiveCameraController::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<MouseButtonPressedEvent>(BH_BIND_EVENT_FN(OnMouseButtonPressedEvent));
    dispatcher.Dispatch<MouseMovedEvent>(BH_BIND_EVENT_FN(OnMouseMovedEvent));
    dispatcher.Dispatch<MouseButtonReleasedEvent>(BH_BIND_EVENT_FN(OnMouseButtonReleasedEvent));
}

bool PerspectiveCameraController::OnMouseMovedEvent(MouseMovedEvent& e)
{
    const auto& window = Application::Get().GetWindow();
    if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
    {
        glfwSetInputMode(window.GetWindowGLFW(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPos(window.GetWindowGLFW(), static_cast<double>(window.GetWidth()) / 2.0, static_cast<double>(window.GetHeight()) / 2.0);
        const float offsetX = e.GetX() - static_cast<float>(window.GetWidth()) / 2.0f;
        const float offsetY = e.GetY() - static_cast<float>(window.GetHeight()) / 2.0f;
        m_OffsetX = offsetX;
        m_OffsetY = -offsetY;

        BH_LOG_DEBUG("{}, {}", offsetX, offsetY);

        m_CameraWasRotated = true;
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
        glfwSetCursorPos(window.GetWindowGLFW(), static_cast<double>(window.GetWidth()) / 2.0, static_cast<double>(window.GetHeight()) / 2.0);
        glfwSetInputMode(window.GetWindowGLFW(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    return true;
}