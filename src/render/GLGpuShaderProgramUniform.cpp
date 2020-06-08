#include "GLGpuShaderProgramUniform.h"
#include "OpenGL.hpp"

namespace render {
GLGpuShaderProgramUniform::GLGpuShaderProgramUniform(const gl::gpu_shader_uniform_handle& handle)
    : m_handle(handle)
{
}

GLGpuShaderProgramUniform::~GLGpuShaderProgramUniform()
{
}

const core::String& GLGpuShaderProgramUniform::GetName()
{
  return m_handle.name;
}

const void GLGpuShaderProgramUniform::Set(int value)
{
  gl::SetUniform(m_handle, value);
}

const void GLGpuShaderProgramUniform::Set(float value)
{
  gl::SetUniform(m_handle, value);
}
const void GLGpuShaderProgramUniform::Set(const int size, float* vec)
{
  gl::SetUniform(m_handle, size, vec);
}

const void GLGpuShaderProgramUniform::Set(const core::pod::Vec2<float>& value)
{
  gl::SetUniform(m_handle, value);
}

const void GLGpuShaderProgramUniform::Set(const core::pod::Vec3<float>& value)
{
  gl::SetUniform(m_handle, value);
}

const void GLGpuShaderProgramUniform::Set(const core::pod::Vec4<float>& value)
{
  gl::SetUniform(m_handle, value);
}

const void GLGpuShaderProgramUniform::SetMat4(float* value)
{
  gl::SetUniformMat4(m_handle, value);
}

const void GLGpuShaderProgramUniform::SetMat3(float* value)
{
  gl::SetUniformMat3(m_handle, value);
}

const void GLGpuShaderProgramUniform::SetMat3x4(float* value, int count)
{
  gl::SetUniformMat3x4(m_handle, value, count);
}

const void GLGpuShaderProgramUniform::SetMat4(const float* value, int count, bool transpose)
{
  gl::SetUniformMat4(m_handle, value, count, transpose);
}
} // namespace render
