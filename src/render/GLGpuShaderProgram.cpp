#include "GLGpuShaderProgram.h"
#include "OpenGL.hpp"

namespace render
{
GLGpuShaderProgram::GLGpuShaderProgram(const gl::gpu_shader_handle& handle)
    : m_handle(handle)
{
}

GLGpuShaderProgram::~GLGpuShaderProgram()
{
    gl::FreeHandle(m_handle);
}

void GLGpuShaderProgram::Bind()
{
    gl::BindHandle(m_handle);
}
}
