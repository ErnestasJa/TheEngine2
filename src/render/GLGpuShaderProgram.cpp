#include "GLGpuShaderProgram.h"
#include "GLGpuShaderProgramUniform.h"
#include "OpenGL.hpp"

namespace render
{
GLGpuShaderProgram::GLGpuShaderProgram(const gl::gpu_shader_handle& handle)
    : m_handle(handle)
{
    InitUniforms();
}

GLGpuShaderProgram::~GLGpuShaderProgram()
{
    gl::FreeHandle(m_handle);
}

void GLGpuShaderProgram::Bind()
{
    gl::BindHandle(m_handle);
}

void GLGpuShaderProgram::InitUniforms()
{
    Bind();
    uint32_t count = gl::GetUniformCount(m_handle);

    for (uint32_t i = 0; i < count; i++) {
        auto handle = gl::GetUniform(m_handle, i);

        m_uniforms.push_back(
            core::Move(core::MakeUnique<GLGpuShaderProgramUniform>(handle)));
    }
}

const core::Vector<core::UniquePtr<IGpuProgramUniform>>&
GLGpuShaderProgram::GetUniforms()
{
    return m_uniforms;
}
}
