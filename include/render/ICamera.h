#ifndef THEPROJECT2_LIBS_THEENGINE2_INCLUDE_RENDER_ICAMERA_H_
#define THEPROJECT2_LIBS_THEENGINE2_INCLUDE_RENDER_ICAMERA_H_

#include <glm/fwd.hpp>
namespace render {

class ICamera
{
public:
    virtual ~ICamera(){};
    virtual void SetAspectRatio(float aspectRatio) = 0;
    virtual glm::vec3 GetPosition() const = 0;
    virtual glm::vec3 GetRotation() const = 0;
    virtual glm::vec3 GetLocalZ() const = 0;
    virtual glm::vec3 GetLocalX() const = 0;
    virtual glm::mat4 GetView() const = 0;
    virtual glm::mat4 GetProjection() const = 0;
    virtual void SetPosition(glm::vec3 position) = 0;
    virtual void SetRotation(glm::vec3 rotation) = 0;
    virtual void SetFOV(float fov) = 0;
    virtual float GetFOV()  = 0;
};

}

#endif // THEPROJECT2_LIBS_THEENGINE2_INCLUDE_RENDER_ICAMERA_H_
