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
        , m_Position(0.0f, 0.0f, 0.0f)
        , m_Direction(0.0f, 0.0f, -1.0f)
        , m_Right(1.0f, 0.0f, 0.0f)
        , m_Up(0.0f, 1.0f, 0.0f)
        , m_QuatX(0.0f, 0.0f, 0.0f, 1.0f)
        , m_QuatY(0.0f, 0.0f, 0.0f, 1.0f)
    {
    }

    virtual ~Camera() = default;

    void SetCameraPosition(glm::vec3 position) { m_Position = position; RecalculateMatrices(); }
    void SetCameraRotation(float offsetX, float offsetY);

    glm::mat4 GetViewMatrix() const { return m_ViewMatrix; }
    glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }
    glm::mat4 GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

    glm::vec3 GetCameraPosition() const { return m_Position; }
    glm::quat GetCameraQuatX() const { return m_QuatX; }
    glm::quat GetCameraQuatY() const { return m_QuatY; }

    glm::vec3 GetDirection() const { return m_Direction; }
    glm::vec3 GetUpDirection() const { return m_Up; }
    glm::vec3 GetRightDirection() const { return m_Right; }

protected:
    virtual void RecalculateMatrices() = 0;
protected:
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewProjectionMatrix;

    glm::vec3 m_Position;
    glm::vec3 m_Direction;
    glm::vec3 m_Right;
    glm::vec3 m_Up;

    glm::quat m_QuatX;
    glm::quat m_QuatY;
};

class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(float fov, float aspectRatio, float near, float far);
    ~PerspectiveCamera() final = default;

private:
    void RecalculateMatrices() final;
};