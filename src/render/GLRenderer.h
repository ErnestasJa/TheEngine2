#ifndef GLRENDERER_H
#define GLRENDERER_H

#include "render/IRenderer.h"

namespace render
{
class GLRenderer : public IRenderer
{
public:
    virtual core::SharedPtr<IProgram> CreateProgram(
        const core::String& vertSource = "",
        const core::String& fragSource = "",
        const core::String& geomSource = "");

    virtual void SetClearColor(const Vec3i& color);
    virtual void Clear();

private:
};
}

#endif