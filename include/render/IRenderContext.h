#ifndef THEPROJECT2_LIBS_THEENGINE2_INCLUDE_RENDER_IRENDERCONTEXT_H_
#define THEPROJECT2_LIBS_THEENGINE2_INCLUDE_RENDER_IRENDERCONTEXT_H_

#include "BaseMaterial.h"
namespace render {
class ICamera;
class IRenderContext
{
public:
    virtual ~IRenderContext()
    {
    }
    virtual core::SharedPtr<ICamera> GetCurrentCamera() const         = 0;
    virtual void SetCurrentCamera(core::SharedPtr<ICamera> camera)    = 0;
    virtual void SetCurrentMaterial(material::BaseMaterial* material) = 0;
    virtual material::BaseMaterial* GetCurrentMaterial() const        = 0;
    virtual void SetDepthTest(bool enabled) = 0;
    virtual bool IsDepthTestEnabled() const = 0;
    virtual void SetWindowSize(core::pod::Vec2<uint32_t> windowSize) = 0;
};
} // namespace render

#endif // THEPROJECT2_LIBS_THEENGINE2_INCLUDE_RENDER_IRENDERCONTEXT_H_
