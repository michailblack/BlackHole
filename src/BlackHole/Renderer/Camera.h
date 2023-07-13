#pragma once

#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Camera
{
    friend class CameraController;
public:
    Camera()
        : m_ViewMatrix(1.0f)
        , m_Position(0.0f, 0.0f, 3.0f)
        , m_Target(0, 0, -1)
        , m_Up(0, 1, 0)
        , m_Right(1, 0, 0)
        , m_Orientation(1, 0, 0, 0)
    {
    }

    virtual ~Camera() = default;

    void SetCameraPosition(glm::vec3 position) { m_Position = position; RecalculateMatrices(); }

    glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
    glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
    glm::mat4 GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

    glm::vec3 GetCameraPosition() const { return m_Position; }

    glm::vec3 GetTargetDirection() const { return m_Target; }
    glm::vec3 GetUpDirection() const { return m_Up; }
    glm::vec3 GetRightDirection() const { return m_Right; }

protected:
    virtual void RecalculateMatrices() = 0;
protected:
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewProjectionMatrix;

    glm::vec3 m_Position;
    glm::vec3 m_Target;
    glm::vec3 m_Up;
    glm::vec3 m_Right;

    glm::quat m_Orientation;
};

class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(float fovDegrees, float aspectRatio, float near, float far);
    ~PerspectiveCamera() final = default;

    void SetCameraRotation(float offsetX, float offsetY);

    void SetPerspectiveParameters(float fovDegrees, float aspectRatio, float near, float far);
    void SetAspectRatio(float aspectRatio);
    void SetFOV(float fov);
    float GetFOV() const { return m_FOV; }

private:
    void RecalculateMatrices() final;
private:
    // In degrees
    float m_FOV;
    float m_AspectRatio;
    float m_Near;
    float m_Far;

    glm::quat m_QuatPitch;
};