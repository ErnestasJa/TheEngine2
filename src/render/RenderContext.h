#ifndef THEPROJECT2_LIBS_THEENGINE2_SRC_RENDER_RENDERCONTEXT_H_
#define THEPROJECT2_LIBS_THEENGINE2_SRC_RENDER_RENDERCONTEXT_H_
#include "render/IRenderContext.h"

namespace render {
class IRenderer;
class RenderContext : public IRenderContext
{
public:
    RenderContext(IRenderer* renderer);
    virtual ~RenderContext()
    {
    }
    core::SharedPtr<ICamera> GetCurrentCamera() const final;
    void SetCurrentCamera(core::SharedPtr<ICamera> camera) final;

    void SetCurrentMaterial(material::BaseMaterial* material) final;
    material::BaseMaterial* GetCurrentMaterial() const final;

    void SetDepthTest(bool enabled) final;
    bool IsDepthTestEnabled() const final;

private:
    material::BaseMaterial* m_currentMaterial;
    IRenderer* m_renderer;
    core::SharedPtr<ICamera> m_camera;
    bool m_depthTest;
};
} // namespace render


#endif // THEPROJECT2_LIBS_THEENGINE2_SRC_RENDER_RENDERCONTEXT_H_
