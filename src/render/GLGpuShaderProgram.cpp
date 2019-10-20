#include "GLGpuShaderProgram.h"
#include "GLGpuShaderProgramUniform.h"
#include "OpenGL.hpp"

namespace render {
GLGpuShaderProgram::GLGpuShaderProgram(const gl::gpu_shader_handle& handle) : m_handle(handle)
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

            if(core::string::EndsWith(handle.name,"[0]")){
                handle.name = core::String(handle.name, 0, handle.name.length() - 3);
            }

            m_uniforms.push_back(core::Move(core::MakeUnique<GLGpuShaderProgramUniform>(handle)));
            elog::LogInfo("Loaded uniform: " + handle.name);
        }
    };

    query(m_handle.id);
}

const core::Vector<core::UniquePtr<IGpuProgramUniform>>& GLGpuShaderProgram::GetUniforms()
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
