#include "render/AnimatedMesh.h"
#include "render/IGpuBufferArrayObject.h"
#include "render/IGpuBufferObject.h"
#include "utils/Math.h"

namespace render {
AnimatedMesh::AnimatedMesh()
{
}

AnimatedMesh::AnimatedMesh(core::UniquePtr<IGpuBufferArrayObject> vao)
    : BaseMesh(core::Move(vao))
{
}

void dump_buffer(core::String name, const core::Vector<glm::vec4>& buffer)
{
  core::String formattedBuffer;

  formattedBuffer += "Buffer " + name + ": ";

  int x = 0;
  for (auto& v : buffer) {

    formattedBuffer += core::string::format("v[{}] = [{:03.2f},{:03.2f},{:03.2f},{:03.2f}]\n", x,
                                            v.x, v.y, v.z, v.w);
    x++;
  }
  formattedBuffer += "\n\n";
  elog::LogInfo(formattedBuffer);
}

void AnimatedMesh::Upload()
{
  m_vao->GetBufferObject(0)->UpdateBuffer(IndexBuffer.size(), IndexBuffer.data());
  m_vao->GetBufferObject(1)->UpdateBuffer(UVBuffer.size(), UVBuffer.data());
  m_vao->GetBufferObject(2)->UpdateBuffer(VertexBuffer.size(), VertexBuffer.data());
  m_vao->GetBufferObject(3)->UpdateBuffer(NormalBuffer.size(), NormalBuffer.data());
  m_vao->GetBufferObject(4)->UpdateBuffer(BlendIndexBuffer.size(), BlendIndexBuffer.data());
  m_vao->GetBufferObject(5)->UpdateBuffer(BlendWeightBuffer.size(), BlendWeightBuffer.data());

  // dump_buffer("BlendIndexBuffer", BlendIndexBuffer);
  // dump_buffer("BlendWeightBuffer", BlendWeightBuffer);
}


void AnimatedMesh::Render()
{
  BaseMesh::Render();
}

void AnimatedMesh::Clear()
{
  IndexBuffer.clear();
  UVBuffer.clear();
  VertexBuffer.clear();
  NormalBuffer.clear();
  BlendIndexBuffer.clear();
  BlendWeightBuffer.clear();
  Upload();
}

void AnimatedMesh::SetArmature(const render::anim::Armature& armature)
{
  m_armature = armature;
}


} // namespace render
