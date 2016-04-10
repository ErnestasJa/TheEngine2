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

    auto query = [this](uint32_t id) {
        uint32_t count = gl::GetUniformCount(id);

        for (uint32_t i = 0; i < count; i++) {
            auto handle = gl::GetUniform(id, i);

            m_uniforms.push_back(core::Move(
                core::MakeUnique<GLGpuShaderProgramUniform>(handle)));
        }
    };

    if (m_handle.vertex_program_id) query(m_handle.vertex_program_id);
    if (m_handle.fragment_program_id) query(m_handle.fragment_program_id);
    if (m_handle.geometry_program_id) query(m_handle.vertex_program_id);
}

const core::Vector<core::UniquePtr<IGpuProgramUniform>>&
GLGpuShaderProgram::GetUniforms()
{
    return m_uniforms;
}

IGpuProgramUniform* GLGpuShaderProgram::GetUniform(const core::String& name)
{
    auto it = core::alg::find_if(m_uniforms, [&name](const auto& uniform) {
        return uniform->GetName() == name;
    });
    return it != m_uniforms.end() ? (*it).get() : nullptr;
}
}
