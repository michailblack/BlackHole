#pragma once
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

class PerspectiveCamera
{
public:
    PerspectiveCamera(float fovDegrees, float aspectRatio, float near, float far);
    ~PerspectiveCamera() = default;

    void SetRotation(float yaw, float pitch);
    void SetPosition(const glm::vec3& pos);

    void SetPerspectiveProjection(float fovDegrees, float aspectRatio, float near, float far);
    void SetFOV(float fovDegrees);
    void SetAspectRatio(float aspectRatio);

    float GetFOV() const { return m_FOV; }

    glm::vec3 GetPosition() const { return m_Position; }

    glm::vec3 GetTargetDirection() const { return m_Target; }
    glm::vec3 GetRightDirection() const { return m_Right; }
    glm::vec3 GetUpDirection() const { return m_Up; }

    glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
    glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
private:
    void UpdateProjection();
    void UpdateView();
private:
    glm::vec3 m_Position = { 0.0f, 0.0f,  3.0f };
    glm::vec3 m_Target   = { 0.0f, 0.0f, -1.0f };
    glm::vec3 m_Up       = { 0.0f, 1.0f,  0.0f };
    glm::vec3 m_Right    = { 1.0f, 0.0f,  0.0f };

    glm::quat m_Orientation = { 1.0f, 0.0f, 0.0f, 0.0f };
    glm::quat m_QuatPitch   = { 1.0f, 0.0f, 0.0f, 0.0f };

    float m_FOV;
    float m_AspectRatio;
    float m_Near;
    float m_Far;

    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
};