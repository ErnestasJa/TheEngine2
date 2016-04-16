#ifndef GLRENDERER_H
#define GLRENDERER_H

#include "render/IRenderer.h"

namespace render
{
class GLRendererDebugMessageMonitor;
class GLRenderer : public IRenderer
{
public:
    GLRenderer(
        core::UniquePtr<GLRendererDebugMessageMonitor>&& debugMessageMonitor);
    virtual ~GLRenderer();
    virtual IRendererDebugMessageMonitor* GetDebugMessageMonitor();
    virtual core::SharedPtr<IGpuProgram> CreateProgram(
        const core::String& vertSource = "",
        const core::String& fragSource = "",
        const core::String& geomSource = "");
    virtual core::SharedPtr<IGpuBufferArrayObject> CreateBufferArrayObject(
        const core::Vector<BufferDescriptor>& descriptors);
    virtual void SetClearColor(const Vec3i& color);
    virtual void Clear();

private:
    core::UniquePtr<GLRendererDebugMessageMonitor> m_debugMessageMonitor;
};

core::UniquePtr<IRenderer> CreateRenderer(
    core::UniquePtr<GLRendererDebugMessageMonitor>&& debugMessageMonitor);
}

#endif