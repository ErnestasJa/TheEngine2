#include "GLProgram.h"
#include "OpenGL.hpp"

namespace render
{
GLProgram::GLProgram(const gl::gpu_shader_handle& handle) : m_handle(handle)
{
}

GLProgram::~GLProgram()
{
    gl::FreeHandle(m_handle);
}

void GLProgram::Bind()
{
    gl::BindHandle(m_handle);
}
}
