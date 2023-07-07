#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float near, float far)
    : m_FOV(fov), m_AspectRatio(aspectRatio), m_Near(near), m_Far(far)
{
    m_ProjectionMatrix = glm::perspective(fov, aspectRatio, near, far);
    m_ViewMatrix = glm::translate(glm::mat4(1.0f), -m_Position);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void PerspectiveCamera::SetCameraRotation(float offsetX, float offsetY)
{
    const glm::quat quatPitch = glm::angleAxis(glm::radians(offsetY), glm::vec3(1, 0, 0));
    const glm::quat quatYaw = glm::angleAxis(glm::radians(offsetX), glm::vec3(0, 1, 0));

    // Note: the order of multiplication does matter
    m_Orientation = m_Orientation * quatYaw;
    m_Orientation = quatPitch * m_Orientation;

    const glm::quat invOrientation = glm::conjugate(m_Orientation);
    m_Target = glm::normalize(invOrientation * glm::vec3(0, 0, -1));
    m_Up = glm::normalize(invOrientation * glm::vec3(0, 1, 0));
    m_Right = glm::normalize(invOrientation * glm::vec3(1, 0, 0));

    RecalculateMatrices();
}

void PerspectiveCamera::SetProjectionParameters(float fov, float aspectRatio, float near, float far)
{
    m_ProjectionMatrix = glm::perspective(fov, aspectRatio, near, far);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void PerspectiveCamera::SetAspectRatio(float aspectRatio)
{
    m_ProjectionMatrix = glm::perspective(m_FOV, aspectRatio, m_Near, m_Far);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void PerspectiveCamera::RecalculateMatrices()
{
    m_ViewMatrix = glm::mat4_cast(m_Orientation) *
                   glm::translate(glm::mat4(1.0f), -m_Position);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}
