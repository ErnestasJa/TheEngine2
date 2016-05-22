#ifndef MATERIALS_H
#define MATERIALS_H

#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "render/RenderInc.h"
#include "log/LogInc.h"
#include "Shaders.h"
#include "Util.h"

namespace material
{
static struct {
    glm::mat4 View, Projection;
} SharedUniforms;

class BaseMaterial
{
private:
    BaseMaterial(const core::SharedPtr<render::IGpuProgram> &shader)
        : m_shader(shader)
    {
#define LOOKUP_UNIFORM(uniform_name) \
    uniform_name = m_shader->GetUniform(#uniform_name)

        LOOKUP_UNIFORM(MVP);
        LOOKUP_UNIFORM(V);
        LOOKUP_UNIFORM(M);
        LOOKUP_UNIFORM(MV3x3);
        LOOKUP_UNIFORM(LightPosition_worldspace);
    }

public:
    void Use(const glm::mat4 &model)
    {
        glm::mat4 m = model;
        glm::mat4 v = SharedUniforms.View;
        glm::mat4 p = SharedUniforms.Projection;
        glm::mat4 mvp = p * v * m;
        glm::mat3 mv3x3(v * m);

        m_shader->Bind();
        MVP->SetMat4(glm::value_ptr(mvp));
        V->SetMat4(glm::value_ptr(v));
        M->SetMat4(glm::value_ptr(m));
        MV3x3->SetMat3(glm::value_ptr(mv3x3));
        LightPosition_worldspace->Set(core::pod::Vec3<float>{0.0f, 5.f, 0.f});
    }

    static core::SharedPtr<BaseMaterial> CreateMaterial(
        render::IRenderer *renderer);

private:
    core::SharedPtr<render::IGpuProgram> m_shader;
    render::IGpuProgramUniform *MVP;
    render::IGpuProgramUniform *V;
    render::IGpuProgramUniform *M;
    render::IGpuProgramUniform *MV3x3;
    render::IGpuProgramUniform *LightPosition_worldspace;
};

core::SharedPtr<BaseMaterial> BaseMaterial::CreateMaterial(
    render::IRenderer *renderer)
{
    auto program = renderer->CreateProgram(shader::NormalMapShader.Vertex,
                                           shader::NormalMapShader.Fragment);

    if (!program) {
        return nullptr;
    }

    sutil::LogShaderUniforms(program);

    return core::SharedPtr<BaseMaterial>(new BaseMaterial(program));
}
}

#endif