#ifndef GLRENDERER_H
#define GLRENDERER_H

#include "render/IRenderer.h"

namespace render {
class GLRendererDebugMessageMonitor;
class GLFrameBufferObject;
class GLRenderer : public IRenderer
{
public:
    virtual ~GLRenderer();

    GLRenderer(core::UniquePtr<GLRendererDebugMessageMonitor>&& debugMessageMonitor);
    virtual IRendererDebugMessageMonitor* GetDebugMessageMonitor();

    virtual core::SharedPtr<IGpuProgram> CreateProgram(const core::String& vertSource = "",
                                                       const core::String& fragSource = "",
                                                       const core::String& geomSource = "");

    virtual core::SharedPtr<IGpuBufferArrayObject> CreateBufferArrayObject(
        const core::Vector<BufferDescriptor>& descriptors);

    virtual core::SharedPtr<ITexture> CreateTexture(const TextureDescriptor& descriptor);
    virtual core::SharedPtr<IFrameBufferObject> CreateFrameBufferObject(
        const FrameBufferObjectDescriptor& descriptor);
    virtual core::SharedPtr<IRenderBufferObject> CreateRenderBufferObject(
        const RenderBufferObjectDescriptor& descriptor);

    virtual void SetActiveTextures(const core::Vector<core::SharedPtr<ITexture>>& textures);
    virtual void SetActiveFrameBuffer(core::SharedPtr<IFrameBufferObject> fbo,
                                      FrameBufferTarget target);

    virtual void SetClearColor(const Vec3i& color);
    virtual void Clear();

private:
    core::SharedPtr<GLFrameBufferObject> m_activeFrameBufferObject;
    core::UniquePtr<GLRendererDebugMessageMonitor> m_debugMessageMonitor;
};

core::UniquePtr<IRenderer> CreateRenderer(
    core::UniquePtr<GLRendererDebugMessageMonitor>&& debugMessageMonitor);
}

#endif