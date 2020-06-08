#include "render/BaseMesh.h"
#include "render/IGpuBufferArrayObject.h"
#include "render/IGpuBufferObject.h"
#include "render/IRenderer.h"

namespace render {

BaseMesh::BaseMesh()
    : m_useColorBuffer(true)
{
}

BaseMesh::BaseMesh(core::UniquePtr<IGpuBufferArrayObject> vao)
    : m_useColorBuffer(true)
    , m_vao(core::Move(vao))
{
}

BaseMesh::~BaseMesh()
{
}

void BaseMesh::Upload()
{
  m_vao->GetBufferObject(0)->UpdateBuffer(IndexBuffer.size(), IndexBuffer.data());
  m_vao->GetBufferObject(1)->UpdateBuffer(UVBuffer.size(), UVBuffer.data());
  m_vao->GetBufferObject(2)->UpdateBuffer(VertexBuffer.size(), VertexBuffer.data());
  m_vao->GetBufferObject(3)->UpdateBuffer(NormalBuffer.size(), NormalBuffer.data());
  m_vao->GetBufferObject(4)->UpdateBuffer(ColorBuffer.size(), ColorBuffer.data());
}

void BaseMesh::InitEmpty()
{
  m_vao->GetBufferObject(0)->UpdateBuffer(IndexBuffer.size(), nullptr);
  m_vao->GetBufferObject(1)->UpdateBuffer(UVBuffer.size(), nullptr);
  m_vao->GetBufferObject(2)->UpdateBuffer(VertexBuffer.size(), nullptr);
  m_vao->GetBufferObject(3)->UpdateBuffer(NormalBuffer.size(), nullptr);
  m_vao->GetBufferObject(4)->UpdateBuffer(ColorBuffer.size(), nullptr);
}

void BaseMesh::UploadSubData(int32_t start, int32_t count)
{
  m_vao->GetBufferObject(0)->UpdateBufferSubData(start, count, IndexBuffer.data());
  m_vao->GetBufferObject(1)->UpdateBufferSubData(start, count, UVBuffer.data());
  m_vao->GetBufferObject(2)->UpdateBufferSubData(start, count, VertexBuffer.data());
  m_vao->GetBufferObject(3)->UpdateBufferSubData(start, count, NormalBuffer.data());
  m_vao->GetBufferObject(4)->UpdateBufferSubData(start, count, ColorBuffer.data());
}

void BaseMesh::Render()
{
  m_vao->Render(IndexBuffer.size());
}

} // namespace render
