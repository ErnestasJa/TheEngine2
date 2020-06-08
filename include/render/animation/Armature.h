#ifndef THEPROJECT2_LIBS_THEENGINE2_SRC_RENDER_ANIMATEDMESH_CPP_ARMATURE_H_
#define THEPROJECT2_LIBS_THEENGINE2_SRC_RENDER_ANIMATEDMESH_CPP_ARMATURE_H_
#include "Bone.h"

namespace render::anim {
class Armature
{
  public:
  Armature()
  {
  }

  Armature(glm::mat4 globalInverseTransform, core::Vector<Bone> bones)
      : m_GlobalInverseTransform(globalInverseTransform)
      , m_bones(bones)
  {
  }

  const core::Vector<Bone>& GetBones() const
  {
    return m_bones;
  }

  const glm::mat4& GetGlobalInverseTransform() const
  {
    return m_GlobalInverseTransform;
  }

  protected:
  core::Vector<Bone> m_bones;
  glm::mat4 m_GlobalInverseTransform;
};
} // namespace render::anim
#endif // THEPROJECT2_LIBS_THEENGINE2_SRC_RENDER_ANIMATEDMESH_CPP_ARMATURE_H_
