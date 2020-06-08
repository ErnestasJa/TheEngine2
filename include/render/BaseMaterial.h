#ifndef ENGINE_BASEMATERIAL_H
#define ENGINE_BASEMATERIAL_H

#include <glm/fwd.hpp>
#include <utility>

namespace render {
class IGpuProgram;
class IGpuProgramUniform;
class ITexture;
} // namespace render

namespace material {

enum class MeshRenderMode
{
  Triangles = 1,
  Lines
};

class BaseMaterial
{
  public:
  bool UseDepthTest;
  MeshRenderMode RenderMode;

  public:
  BaseMaterial(render::IGpuProgram* shader);
  virtual void Use();

  void SetI(const core::String& name, int i);
  void SetVec3(const core::String& name, glm::vec3 uniformValue);
  void SetMat3(const core::String& name, glm::mat3 uniformValue);
  void SetMat3x4(const core::String& name, glm::mat3x4* uniformValue, int count);
  void SetMat4(const core::String& name, glm::mat4 uniformValue);
  void SetMat4(const core::String& name, const glm::mat4* uniformValue, int count,
               bool transpose = false);
  void SetTexture(int slot, render::ITexture* texture);
  const core::Array<render::ITexture*, 8>& GetTextures();

  virtual core::SharedPtr<BaseMaterial> Instance();


  protected:
  core::Array<render::ITexture*, 8> m_textures;
  render::IGpuProgram* m_shader;
  bool m_textureListNeedsRebuild;
};
} // namespace material


#endif // ENGINE_BASEMATERIAL_H
