#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

void Camera::SetCameraRotation(float offsetX, float offsetY)
{
    glm::quat quatX;
    quatX.x = glm::sin(glm::radians(offsetY) / 2.0f);
    quatX.y = 0.0f;
    quatX.z = 0.0f;
    quatX.w = glm::cos(glm::radians(offsetY) / 2.0f);

    glm::quat quatY;
    quatY.x = 0.0f;
    quatY.y = glm::sin(glm::radians(offsetX) / 2.0f);
    quatY.z = 0.0f;
    quatY.w = glm::cos(glm::radians(offsetX) / 2.0f);

    m_QuatX *= quatX;
    m_QuatY *= quatY;

    glm::normalize(m_QuatX);
    glm::normalize(m_QuatY);

    /*m_Direction = m_QuatY * m_QuatX * m_Direction;
    glm::normalize(m_Direction);
    m_Up = m_QuatY * m_QuatX * m_Up;
    glm::normalize(m_Up);
    m_Right = m_QuatY * m_QuatX * m_Right;
    glm::normalize(m_Right);*/

    RecalculateMatrices();
}

PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float near, float far)
{
    m_ProjectionMatrix = glm::perspective(fov, aspectRatio, near, far);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void PerspectiveCamera::RecalculateMatrices()
{
    m_ViewMatrix = glm::toMat4(m_QuatY * m_QuatX);

    m_ViewMatrix = glm::translate(glm::inverse(m_ViewMatrix), -m_Position);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}