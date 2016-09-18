#ifndef MATERIALS_H
#define MATERIALS_H

#include "Shaders.h"
#include "Util.h"
#include "glm/ext.hpp"
#include "glm/glm.hpp"
#include "log/LogInc.h"
#include "render/RenderInc.h"

namespace material {
static struct
{
    glm::mat4 View, Projection;
} SharedUniforms;

class BaseMaterial
{
public:
    virtual void Use(const glm::mat4& model)
    {
        m_shader->Bind();
    }

protected:
    BaseMaterial(const core::SharedPtr<render::IGpuProgram>& shader)
        : m_shader(shader)
    {
    }

    void LookupUniform(render::IGpuProgramUniform*& uniform, const core::String& name)
    {
        uniform = m_shader->GetUniform(name);

        if (!uniform) {
            elog::LogWarning("Uniform not found: " + name);
        }
    }

protected:
    core::SharedPtr<render::IGpuProgram> m_shader;
};

class PhongMaterial : public BaseMaterial
{
public:
    PhongMaterial(const core::SharedPtr<render::IGpuProgram>& shader)
        : BaseMaterial(shader)
    {
        LookupUniform(MVP, "MVP");
        LookupUniform(M, "M");
        LookupUniform(V, "V");
        LookupUniform(MV3x3, "MV3x3");
        LookupUniform(LightPosition_worldspace, "LightPosition_worldspace");
    }

    virtual void Use(const glm::mat4& model) override
    {
        BaseMaterial::Use(model);

        glm::mat4 m   = model;
        glm::mat4 v   = SharedUniforms.View;
        glm::mat4 p   = SharedUniforms.Projection;
        glm::mat4 mvp = p * v * m;
        glm::mat3 mv3x3(v * m);

        if (MVP)
            MVP->SetMat4(glm::value_ptr(mvp));
        if (V)
            V->SetMat4(glm::value_ptr(v));
        if (M)
            M->SetMat4(glm::value_ptr(m));
        if (MV3x3)
            MV3x3->SetMat3(glm::value_ptr(mv3x3));
        if (LightPosition_worldspace)
            LightPosition_worldspace->Set(core::pod::Vec3<float>{ 0.0f, 5.f, 0.f });
    }

protected:
    render::IGpuProgramUniform* MVP;
    render::IGpuProgramUniform* V;
    render::IGpuProgramUniform* M;
    render::IGpuProgramUniform* MV3x3;
    render::IGpuProgramUniform* LightPosition_worldspace;
};

class CellMaterial : public BaseMaterial
{
public:
    CellMaterial(const core::SharedPtr<render::IGpuProgram>& shader)
        : BaseMaterial(shader)
    {
        LookupUniform(MVP, "MVP");
        LookupUniform(MV3x3, "MV3x3");
        LookupUniform(ldir, "ldir");
    }

    virtual void Use(const glm::mat4& model) override
    {
        BaseMaterial::Use(model);

        glm::mat4 m   = model;
        glm::mat4 v   = SharedUniforms.View;
        glm::mat4 p   = SharedUniforms.Projection;
        glm::mat4 mvp = p * v * m;
        glm::mat3 mv3x3(v * m);

        MVP->SetMat4(glm::value_ptr(mvp));
        MV3x3->SetMat3(glm::value_ptr(mv3x3));
        ldir->Set(core::pod::Vec3<float>{ 0.66f, 0.66f, 0.f });
    }

protected:
    render::IGpuProgramUniform* MVP;
    render::IGpuProgramUniform* MV3x3;
    render::IGpuProgramUniform* ldir;
};

static core::SharedPtr<render::IGpuProgram> CreateMaterialShader(render::IRenderer* renderer,
                                                                 io::IFileSystem* fs,
                                                                 const core::String& shaderName)
{
    auto programSource = shader::LoadShaderSource(fs, shaderName);
    auto program       = renderer->CreateProgram(programSource.Vertex, programSource.Fragment,
                                           programSource.Geometry);

    if (!program) {
        return nullptr;
    }

    sutil::LogShaderUniforms(program);

    return program;
}
}

#endif