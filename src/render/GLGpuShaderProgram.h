#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include "render/IGpuProgram.h"
#include "OpenGL.hpp"

namespace render
{
class GLGpuShaderProgram : public IGpuProgram
{
public:
    GLGpuShaderProgram(const gl::gpu_shader_handle& handle);
    virtual ~GLGpuShaderProgram();

    virtual void Bind();

private:
    gl::gpu_shader_handle m_handle;
};
}

#endif