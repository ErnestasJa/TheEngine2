#ifndef PERSPECTIVE_CAMERA_CPP
#define PERSPECTIVE_CAMERA_CPP

#include "ICamera.h"
#include "glm/ext.hpp"
#include "glm/glm.hpp"

namespace render {

class PerspectiveCamera : public ICamera
{
  public:
  PerspectiveCamera(float aspectRatio, float FOV = 45.0f, float nearPlane = 0.1,
                    float farPlane = 2048.0f)
      : m_aspectRatio(aspectRatio)
      , m_fov(FOV)
      , m_far(farPlane)
      , m_near(nearPlane)
      , m_position(0, 0, 0)
      , m_rotation(0, 0, 0)
  {
    SetRotation(glm::vec3());
    RecalcProjection();
  }

  void RecalcProjection()
  {
    m_projectionMatrix = glm::perspective(m_fov, m_aspectRatio, m_near, m_far);
  }

  void SetAspectRatio(float aspectRatio) override
  {
    m_aspectRatio = aspectRatio;
    RecalcProjection();
  }

  void SetFOV(float fov) override
  {
    m_fov = fov;
    RecalcProjection();
  }

  float GetFOV() override
  {
    return m_fov;
  }

  void SetRotation(glm::vec3 rotation) override
  {
    m_rotation = rotation;
    Update();
  }

  void SetPosition(glm::vec3 position) override
  {
    m_position = position;
    Update();
  }

  void MoveForward(float amount)
  {
    m_position += m_direction * amount;
    Update();
  }

  void MoveStrafe(float amount)
  {
    m_position += m_right * amount;
    Update();
  }

  glm::vec3 GetRotation() const override
  {
    return m_rotation;
  }

  glm::vec3 GetDirection() const override
  {
    return m_direction;
  }

  glm::vec3 GetPosition() const override
  {
    return m_position;
  }

  glm::vec3 GetLocalZ() const override
  {
    return m_direction;
  }

  glm::vec3 GetLocalX() const override
  {
    return m_right;
  }

  glm::mat4 GetView() const override
  {
    return m_viewMatrix;
  }

  glm::mat4 GetProjection() const override
  {
    return m_projectionMatrix;
  }

  private:
  void Update()
  {
    float horizontalAngle = m_rotation.x, verticalAngle = m_rotation.y;

    m_direction = glm::vec3(cos(verticalAngle) * sin(horizontalAngle), //
                            sin(verticalAngle),                        //
                            cos(verticalAngle) * cos(horizontalAngle));

    m_right = glm::vec3(sin(horizontalAngle - 3.14f / 2.0f), //
                        0,                                   //
                        cos(horizontalAngle - 3.14f / 2.0f));

    m_up = glm::cross(m_right, m_direction);

    m_viewMatrix = glm::lookAt(m_position,               //
                               m_position + m_direction, //
                               m_up);
  }

  private:
  glm::vec3 m_position;
  glm::vec3 m_rotation;
  glm::vec3 m_direction;
  glm::vec3 m_right;
  glm::vec3 m_up;
  glm::mat4 m_viewMatrix;
  glm::mat4 m_projectionMatrix;

  float m_aspectRatio;
  float m_fov;
  float m_far;
  float m_near;
};
} // namespace render

#endif