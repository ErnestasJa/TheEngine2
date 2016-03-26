#include "GLRenderer.h"
#include "GLProgram.h"
#include "OpenGL.hpp"

namespace render
{
core::SharedPtr<IRenderer> CreateRenderer()
{
    return core::MakeShared<GLRenderer>();
}

core::SharedPtr<IProgram> GLRenderer::CreateProgram(
    const core::String& vertSource, const core::String& fragSource,
    const core::String& geomSource)
{
    auto handle = gl::CreatePipelineFromShaderStrings(
        vertSource.c_str(), fragSource.c_str(), geomSource.c_str());

    if (gl::IsHandleValid(handle))
        return core::MakeShared<GLProgram>(handle);
    else
        return nullptr;
}
void GLRenderer::SetClearColor(const Vec3i& color)
{
    gl::SetClearColor(color);
}
void GLRenderer::Clear()
{
    gl::Clear();
}
}