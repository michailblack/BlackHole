#pragma once

#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class PerspectiveCamera
{
public:
    PerspectiveCamera(float fovDegrees, float aspectRatio, float near, float far);
    ~PerspectiveCamera() = default;

    void SetRotation(float offsetX, float offsetY);
    void SetPosition(const glm::vec3& pos) { m_Position = pos; RecalculateMatrices(); }

    void SetPerspectiveProjection(float fovDegrees, float aspectRatio, float near, float far);
    void SetAspectRatio(float aspectRatio);
    void SetFOV(float fov);

    float GetFOV() const { return m_FOV; }

    glm::vec3 GetTargetDirection() const { return m_Target; }
    glm::vec3 GetRightDirection() const { return m_Right; }
    glm::vec3 GetUpDirection() const { return m_Up; }
    glm::vec3 GetPosition() const { return m_Position; }

    glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
    glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
    glm::mat4 GetCameraRotationMatrix() const { return glm::mat4_cast(m_Orientation); }
private:
    void RecalculateMatrices();
private:
    // In degrees
    float m_FOV;
    float m_AspectRatio;
    float m_Near;
    float m_Far;

    glm::vec3 m_Position = { 0.0f, 0.0f,  3.0f };
    glm::vec3 m_Target   = { 0.0f, 0.0f, -1.0f };
    glm::vec3 m_Up       = { 0.0f, 1.0f,  0.0f };
    glm::vec3 m_Right    = { 1.0f, 0.0f,  0.0f };

    glm::quat m_Orientation = { 1.0f, 0.0f, 0.0f, 0.0f };
    glm::quat m_QuatPitch   = { 1.0f, 0.0f, 0.0f, 0.0f };;

    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewProjectionMatrix;
};