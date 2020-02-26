#ifndef GLRENDERER_H
#define GLRENDERER_H

#include "render/CFrameBufferObject.h"
#include "render/CRenderBufferObject.h"
#include "render/IRenderer.h"

namespace render {
class GLRendererDebugMessageMonitor;
class GLFrameBufferObject;
class IRenderContext;
class GLRenderer : public IRenderer
{
public:
    ~GLRenderer();

    GLRenderer(core::UniquePtr<GLRendererDebugMessageMonitor>&& debugMessageMonitor);
    IRendererDebugMessageMonitor* GetDebugMessageMonitor() final;

    core::SharedPtr<IGpuProgram> CreateProgram(const core::String& vertSource = "",
                                               const core::String& fragSource = "",
                                               const core::String& geomSource = "") final;

    core::SharedPtr<IGpuBufferArrayObject> CreateBufferArrayObject(
        const core::Vector<BufferDescriptor>& descriptors) final;

    core::UniquePtr<ITexture> CreateTexture(const TextureDescriptor& descriptor) final;
    core::SharedPtr<IFrameBufferObject> CreateFrameBufferObject(
        const FrameBufferObjectDescriptor& descriptor) final;
    core::SharedPtr<IRenderBufferObject> CreateRenderBufferObject(
        const RenderBufferObjectDescriptor& descriptor) final;

    void SetActiveTextures(const core::Array<ITexture*,8>& textures) final;
    void SetActiveFrameBuffer(core::SharedPtr<IFrameBufferObject> fbo,
                              FrameBufferTarget target) final;

    void SetClearColor(const Vec3i& color) final;
    void Clear() final;

    core::UniquePtr<BaseMesh> CreateBaseMesh() final;
    core::UniquePtr<AnimatedMesh> CreateAnimatedMesh() final;
    IRenderContext* GetRenderContext() const final;

    void BeginFrame() final;
    void EndFrame() final;
    void RenderMesh(BaseMesh * mesh, material::BaseMaterial * material, const glm::vec3 position) final;
    void RenderMesh(BaseMesh * mesh, material::BaseMaterial * material, const glm::mat4 transform) final;
    void RenderMesh(AnimatedMesh * mesh, material::BaseMaterial * material, const glm::mat4 transform) final;

public:
    void WindowResized(core::pod::Vec2<uint32_t> size);

private:
    core::UniquePtr<IRenderContext> m_renderContext;
    core::SharedPtr<GLFrameBufferObject> m_activeFrameBufferObject;
    core::UniquePtr<GLRendererDebugMessageMonitor> m_debugMessageMonitor;
};

core::UniquePtr<IRenderer> CreateRenderer(
    core::UniquePtr<GLRendererDebugMessageMonitor>&& debugMessageMonitor);
} // namespace render

#endif