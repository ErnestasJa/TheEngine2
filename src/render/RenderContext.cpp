#include "RenderContext.h"
#include "OpenGL.hpp"
#include "render/ICamera.h"

namespace render {
RenderContext::RenderContext(render::IRenderer* renderer)
: m_renderer(renderer), m_currentMaterial(nullptr), m_depthTest(true)
{

}

core::SharedPtr<ICamera> RenderContext::GetCurrentCamera() const {
    return m_camera;
}

void RenderContext::SetCurrentCamera(core::SharedPtr<ICamera> camera) {
    m_camera = camera;
}

void RenderContext::SetCurrentMaterial(material::BaseMaterial* material)
{
    m_currentMaterial = material;
}

material::BaseMaterial* RenderContext::GetCurrentMaterial() const{
    return m_currentMaterial;
}

void RenderContext::SetDepthTest(bool enabled)
{
    if(m_depthTest == enabled)
        return;

    gl::SetDepthTest(enabled);
    m_depthTest = enabled;
}

bool RenderContext::IsDepthTestEnabled() const
{
    return m_depthTest;
}

}