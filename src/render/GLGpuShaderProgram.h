#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include "render/IGpuProgram.h"
#include "render/IGpuProgramUniform.h"
#include "OpenGL.hpp"

namespace render
{
class GLGpuShaderProgram : public IGpuProgram
{
public:
    GLGpuShaderProgram(const gl::gpu_shader_handle& handle);
    virtual ~GLGpuShaderProgram();

    virtual void Bind();
    virtual const core::Vector<core::UniquePtr<IGpuProgramUniform>>&
    GetUniforms();
    virtual IGpuProgramUniform* GetUniform(const core::String& name);

private:
    void InitUniforms();

private:
    gl::gpu_shader_handle m_handle;
    core::Vector<core::UniquePtr<IGpuProgramUniform>> m_uniforms;
};
}

#endif