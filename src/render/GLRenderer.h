#ifndef GLRENDERER_H
#define GLRENDERER_H

#include "render/IRenderer.h"

namespace render
{
class GLRenderer : public IRenderer
{
public:
    GLRenderer();
    virtual ~GLRenderer();
    virtual core::SharedPtr<IProgram> CreateProgram(
        const core::String& vertSource = "",
        const core::String& fragSource = "",
        const core::String& geomSource = "");
    virtual core::SharedPtr<IGpuBufferArrayObject> CreateBufferArrayObject(
        const core::Vector<BufferDescriptor>& descriptors);
    virtual void SetClearColor(const Vec3i& color);
    virtual void Clear();

private:
};
}

#endif