#include "GLRenderer.h"
#include "GLProgram.h"
#include "OpenGLRender.hpp"
#include "render/OpenGL.h"

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
    uint32_t vertObj = 0, fragObj = 0, geomObj = 0;

    if (vertSource.size())
        vertObj =
            gl::CreateShaderFromString(GL_VERTEX_SHADER, vertSource.c_str());

    if (fragSource.size())
        fragObj =
            gl::CreateShaderFromString(GL_FRAGMENT_SHADER, fragSource.c_str());

    if (geomSource.size())
        geomObj =
            gl::CreateShaderFromString(GL_GEOMETRY_SHADER, geomSource.c_str());

    uint32_t programObj = gl::CreateProgramPipeline(vertObj, fragObj, geomObj);

    return programObj ? core::MakeShared<GLProgram>(programObj) : nullptr;
}
}