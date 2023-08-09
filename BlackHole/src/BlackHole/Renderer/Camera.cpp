#include "bhpch.h"
#include "BlackHole/Renderer/Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

PerspectiveCamera::PerspectiveCamera(float fovDegrees, float aspectRatio, float near, float far)
    : m_FOV(fovDegrees)
    , m_AspectRatio(aspectRatio)
    , m_Near(near)
    , m_Far(far)
{
    UpdateProjection();
    UpdateView();
}

void PerspectiveCamera::SetRotation(float yaw, float pitch)
{
    const glm::quat quatYaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));
    glm::quat quatPitch = glm::angleAxis(pitch, glm::vec3(1, 0, 0));

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

    UpdateView();
}

void PerspectiveCamera::SetPosition(const glm::vec3& pos)
{
    m_Position = pos;

    UpdateView();
}

void PerspectiveCamera::SetPerspectiveProjection(float fovDegrees, float aspectRatio, float near, float far)
{
    m_FOV = fovDegrees;
    m_AspectRatio = aspectRatio;
    m_Near = near;
    m_Far = far;
    
    UpdateProjection();
}

void PerspectiveCamera::SetAspectRatio(float aspectRatio)
{
    m_AspectRatio = aspectRatio;

    UpdateProjection();
}

void PerspectiveCamera::SetFOV(float fovDegrees)
{
    m_FOV = fovDegrees;

    UpdateProjection();
}

void PerspectiveCamera::UpdateProjection()
{
    m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_Near, m_Far);
}

void PerspectiveCamera::UpdateView()
{
    m_ViewMatrix = glm::mat4_cast(m_Orientation) * glm::translate(glm::mat4(1.0f), -m_Position);
}