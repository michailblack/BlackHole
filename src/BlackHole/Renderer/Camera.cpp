#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

PerspectiveCamera::PerspectiveCamera(float fovDegrees, float aspectRatio, float near, float far)
    : m_FOV(fovDegrees)
    , m_AspectRatio(aspectRatio)
    , m_Near(near)
    , m_Far(far)
{
    m_ProjectionMatrix = glm::perspective(glm::radians(fovDegrees), aspectRatio, near, far);
    m_ViewMatrix = glm::translate(glm::mat4(1.0f), -m_Position);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void PerspectiveCamera::SetRotation(float offsetX, float offsetY)
{
    glm::quat quatPitch = glm::angleAxis(glm::radians(offsetY), glm::vec3(1, 0, 0));
    const glm::quat quatYaw = glm::angleAxis(glm::radians(offsetX), glm::vec3(0, 1, 0));

    m_QuatPitch = m_QuatPitch * quatPitch;

    const float overallPitch = glm::acos(m_QuatPitch.w) * 2.0f;
    if (overallPitch > glm::radians(89.0f) || overallPitch < glm::radians(-89.0f))
    {
        m_QuatPitch = m_QuatPitch * glm::conjugate(quatPitch);
        quatPitch = glm::angleAxis(0.0f, glm::vec3(1, 0, 0));
    }

    // Note: the order of multiplication does matter
    m_Orientation = m_Orientation * quatYaw;
    m_Orientation = quatPitch * m_Orientation;

    const glm::quat invOrientation = glm::conjugate(m_Orientation);
    m_Target = glm::normalize(invOrientation * glm::vec3(0, 0, -1));
    m_Up = glm::normalize(invOrientation * glm::vec3(0, 1, 0));
    m_Right = glm::normalize(invOrientation * glm::vec3(1, 0, 0));

    RecalculateMatrices();
}

void PerspectiveCamera::SetPerspectiveProjection(float fovDegrees, float aspectRatio, float near, float far)
{
    m_ProjectionMatrix = glm::perspective(glm::radians(fovDegrees), aspectRatio, near, far);
    m_FOV = fovDegrees;
    m_AspectRatio = aspectRatio;
    m_Near = near;
    m_Far = far;
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void PerspectiveCamera::SetAspectRatio(float aspectRatio)
{
    m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), aspectRatio, m_Near, m_Far);
    m_AspectRatio = aspectRatio;
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void PerspectiveCamera::SetFOV(float fov)
{
    m_ProjectionMatrix = glm::perspective(glm::radians(fov), m_AspectRatio, m_Near, m_Far);
    m_FOV = fov;
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void PerspectiveCamera::RecalculateMatrices()
{
    m_ViewMatrix = glm::mat4_cast(m_Orientation) *
                   glm::translate(glm::mat4(1.0f), -m_Position);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}