#include "RenderContext.h"
#include "GLTexture.h"
#include "OpenGL.hpp"
#include "render/ICamera.h"

namespace render {
RenderContext::RenderContext(render::IRenderer* renderer)
    : m_renderer(renderer)
    , m_currentMaterial(nullptr)
    , m_depthTest(true)
{
}

core::SharedPtr<ICamera> RenderContext::GetCurrentCamera() const
{
  return m_camera;
}

void RenderContext::SetCurrentCamera(core::SharedPtr<ICamera> camera)
{
  m_camera = camera;
  elog::LogInfo(
      core::string::format("Setting aspect ratio {} / {}", m_windowSize.w, m_windowSize.h));
  m_camera->SetAspectRatio(float(m_windowSize.w) / float(m_windowSize.h));
}

void SetActiveTextures(const core::Array<ITexture*, 8>& textures)
{
  int slot = 0;
  for (int32_t i = 0; i < textures.size(); i++) {
    if (textures[i]) {
      auto texture = static_cast<GLTexture*>(textures[i]);
      GLTexture::BindObject(texture, slot);
      slot++;
    }
  }
}

void RenderContext::SetCurrentMaterial(material::BaseMaterial* material)
{
  m_currentMaterial = material;
  m_currentMaterial->Use();
  SetDepthTest(m_currentMaterial->UseDepthTest);
  SetActiveTextures(m_currentMaterial->GetTextures());
}


material::BaseMaterial* RenderContext::GetCurrentMaterial() const
{
  return m_currentMaterial;
}

void RenderContext::SetDepthTest(bool enabled)
{
  if (m_depthTest == enabled)
    return;

  gl::SetDepthTest(enabled);
  m_depthTest = enabled;
}

bool RenderContext::IsDepthTestEnabled() const
{
  return m_depthTest;
}
void RenderContext::SetWindowSize(core::pod::Vec2<uint32_t> windowSize)
{
  m_windowSize = windowSize;
}

} // namespace render