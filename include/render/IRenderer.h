#ifndef IRENDERER_H
#define IRENDERER_H

#include "CFrameBufferObject.h"
#include "CRenderBufferObject.h"
#include <glm/fwd.hpp>

namespace material {
class BaseMaterial;
}

namespace render {
// Note: remove these
using Vec2i = core::pod::Vec2<int32_t>;
using Vec2f = core::pod::Vec2<float>;
using Vec3i = core::pod::Vec3<int32_t>;
using Vec3f = core::pod::Vec3<float>;
using Vec4i = core::pod::Vec4<int32_t>;
using Vec4f = core::pod::Vec4<float>;

class IGpuProgram;
class IGpuBufferObject;
class IGpuBufferArrayObject;
class IRendererDebugMessageMonitor;
class ITexture;
class IFrameBufferObject;
class IRenderBufferObject;
class IRenderContext;

struct BufferDescriptor;
struct TextureDescriptor;
struct FrameBufferObjectDescriptor;
class ITexture;
class BaseMesh;
class AnimatedMesh;

class IRenderer
{
  public:
  virtual ~IRenderer()
  {
  }
  virtual IRendererDebugMessageMonitor* GetDebugMessageMonitor()                          = 0;
  virtual core::UniquePtr<IGpuProgram> CreateProgram(const core::String& vertSource = "",
                                                     const core::String& fragSource = "",
                                                     const core::String& geomSource = "") = 0;
  virtual core::UniquePtr<IGpuBufferArrayObject> CreateBufferArrayObject(
      const core::Vector<BufferDescriptor>& descriptors)                               = 0;
  virtual core::UniquePtr<ITexture> CreateTexture(const TextureDescriptor& descriptor) = 0;
  virtual core::SharedPtr<IFrameBufferObject> CreateFrameBufferObject(
      const FrameBufferObjectDescriptor& descriptor) = 0;
  virtual core::SharedPtr<IRenderBufferObject> CreateRenderBufferObject(
      const RenderBufferObjectDescriptor& descriptor)                       = 0;
  virtual void SetActiveTextures(const core::Array<ITexture*, 8>& textures) = 0;
  virtual void SetActiveFrameBuffer(core::SharedPtr<IFrameBufferObject> fbo,
                                    FrameBufferTarget target)               = 0;
  virtual void SetClearColor(const Vec3i& color)                            = 0;
  virtual void Clear()                                                      = 0;

  virtual core::UniquePtr<BaseMesh> CreateBaseMesh()         = 0;
  virtual core::UniquePtr<AnimatedMesh> CreateAnimatedMesh() = 0;

  virtual void BeginFrame() = 0;
  virtual void EndFrame()   = 0;

  virtual IRenderContext* GetRenderContext() const           = 0;
  virtual void RenderMesh(BaseMesh* mesh, material::BaseMaterial* material,
                          const glm::vec3 position)          = 0;
  virtual void RenderMesh(BaseMesh* mesh, material::BaseMaterial* material,
                          const glm::mat4 transform)         = 0;
  virtual void RenderMesh(AnimatedMesh* mesh, material::BaseMaterial* material,
                          const glm::mat4 transform)         = 0;
  virtual void WindowResized(core::pod::Vec2<uint32_t> size) = 0;
};
} // namespace render

#endif
