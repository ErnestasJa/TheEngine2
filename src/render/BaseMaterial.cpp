//
// Created by ernes on 9/22/2019.
//

#include "render/BaseMaterial.h"
#include "render/IGpuProgram.h"
#include "render/IGpuProgramUniform.h"
#include <glm/glm/detail/type_mat3x3.hpp>
#include <glm/glm/detail/type_mat4x4.hpp>
#include <glm/glm/detail/type_vec3.hpp>

namespace material {
BaseMaterial::BaseMaterial(render::IGpuProgram* shader)
    : m_shader(shader)
    , RenderMode(MeshRenderMode::Triangles)
    , UseDepthTest(true)
    , m_textures({ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr })
    , m_textureListNeedsRebuild(false)
{
}

void BaseMaterial::Use()
{
  m_shader->Bind();

  static const core::String textureSamplerNames[8] = {
    "textureSampler0", "textureSampler1", "textureSampler2", "textureSampler3",
    "textureSampler4", "textureSampler5", "textureSampler6", "textureSampler7",
  };

  if (m_textureListNeedsRebuild) {
    m_textureListNeedsRebuild = false;
    int textureSlot           = 0;
    for (int i = 0; i < m_textures.size(); i++) {
      if (m_textures[i]) {
        SetI(textureSamplerNames[i], textureSlot);
        textureSlot++;
      }
    }
  }
}

void BaseMaterial::SetI(const core::String& name, int i)
{
  auto uniform = m_shader->GetUniform(name);

  if (uniform) {
    uniform->Set(i);
  }
  else {
    elog::LogError("Failed to set uniform: " + name);
  }
}

void BaseMaterial::SetF(const core::String& name, float i)
{
  auto uniform = m_shader->GetUniform(name);

  if (uniform) {
    uniform->Set(i);
  }
  else {
    elog::LogError("Failed to set uniform: " + name);
  }
}

void BaseMaterial::SetVec3(const core::String& name, glm::vec3 uniformValue)
{
  auto uniform = m_shader->GetUniform(name);

  if (uniform) {
    uniform->Set(3, &uniformValue.x);
  }
  else {
    elog::LogError("Failed to set uniform: " + name);
  }
}
void BaseMaterial::SetMat3(const core::String& name, glm::mat3 uniformValue)
{
  auto uniform = m_shader->GetUniform(name);

  if (uniform) {
    uniform->SetMat3(&uniformValue[0].x);
  }
  else {
    elog::LogError("Failed to set uniform: " + name);
  }
}

void BaseMaterial::SetMat3x4(const core::String& name, glm::mat3x4* uniformValue, int count)
{
  auto uniform = m_shader->GetUniform(name);

  if (uniform) {
    uniform->SetMat3x4(&uniformValue[0][0].x, count);
  }
  else {
    elog::LogError("Failed to set uniform: " + name);
  }
}

void BaseMaterial::SetMat4(const core::String& name, const glm::mat4* uniformValue, int count,
                           bool transpose)
{
  auto uniform = m_shader->GetUniform(name);

  if (uniform) {
    uniform->SetMat4(&uniformValue[0][0].x, count, transpose);
  }
  else {
    elog::LogError("Failed to set uniform: " + name);
  }
}

void BaseMaterial::SetMat4(const core::String& name, glm::mat4 uniformValue)
{
  auto uniform = m_shader->GetUniform(name);

  if (uniform) {
    uniform->SetMat4(&uniformValue[0].x);
  }
  else {
    elog::LogError("Failed to set uniform: " + name);
  }
}

void BaseMaterial::SetTexture(int slot, render::ITexture* texture)
{
  m_textures[slot]          = texture;
  m_textureListNeedsRebuild = true;
}
const core::Array<render::ITexture*, 8>& BaseMaterial::GetTextures()
{
  return m_textures;
}
core::SharedPtr<BaseMaterial> BaseMaterial::Instance()
{
  auto mat                       = new BaseMaterial(m_shader);
  mat->m_textureListNeedsRebuild = mat->m_textureListNeedsRebuild;
  mat->RenderMode                = RenderMode;
  mat->UseDepthTest              = UseDepthTest;

  for (int i = 0; i < m_textures.size(); i++) {
    mat->m_textures[i] = m_textures[i];
  }

  return core::SharedPtr<BaseMaterial>(mat);
}

} // namespace material