#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/ext.hpp"

class Camera
{
public:
    Camera(float aspectRatio, float FOV = 45.0f, float nearPlane = 0.1,
           float farPlane = 2048.0f)
        : m_aspectRatio(aspectRatio),
          m_fov(FOV),
          m_far(farPlane),
          m_near(nearPlane)
    {
        SetRotation(glm::vec3());
        m_projectionMatrix =
            glm::perspective(m_fov, m_aspectRatio, m_near, m_far);
    }

    void SetRotation(const glm::vec3& rotation)
    {
        m_rotation = rotation;
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

    const glm::vec3& GetRotation()
    {
        return m_rotation;
    }

    const glm::mat4& GetView()
    {
        return m_viewMatrix;
    }

    const glm::mat4& GetProjection()
    {
        return m_projectionMatrix;
    }

private:
    void Update()
    {
        float horizontalAngle = m_rotation.x, verticalAngle = m_rotation.y;

        m_direction = glm::vec3(cos(verticalAngle) * sin(horizontalAngle),  //
                                sin(verticalAngle),                         //
                                cos(verticalAngle) * cos(horizontalAngle));

        m_right = glm::vec3(sin(horizontalAngle - 3.14f / 2.0f),  //
                            0,                                    //
                            cos(horizontalAngle - 3.14f / 2.0f));

        m_up = glm::cross(m_right, m_direction);

        m_viewMatrix = glm::lookAt(m_position,                //
                                   m_position + m_direction,  //
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

#endif