#ifndef GLGPUSHADERPROGRAMUNIFORM_H
#define GLGPUSHADERPROGRAMUNIFORM_H

#include "render/IGpuProgramUniform.h"
#include "OpenGL.hpp"

namespace render
{
class GLGpuShaderProgramUniform : public IGpuProgramUniform
{
public:
    GLGpuShaderProgramUniform(const gl::gpu_shader_uniform_handle& handle);
    virtual ~GLGpuShaderProgramUniform();

    virtual const core::String& GetName();
    virtual const void Set(float value);
    virtual const void Set(const core::pod::Vec2<float>& value);
    virtual const void Set(const core::pod::Vec3<float>& value);
    virtual const void Set(const core::pod::Vec4<float>& value);

private:
    gl::gpu_shader_uniform_handle m_handle;
};
}

#endif