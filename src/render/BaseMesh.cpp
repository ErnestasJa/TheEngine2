#include "render/IRenderer.h"
#include "render/BaseMesh.h"
#include "render/IGpuBufferArrayObject.h"
#include "render/IGpuBufferObject.h"

namespace render {

BaseMesh::BaseMesh(): m_useColorBuffer(true)
{
}

BaseMesh::BaseMesh(core::SharedPtr<IGpuBufferArrayObject> vao): m_useColorBuffer(true)
{
    m_vao = vao;
}

void BaseMesh::Upload()
{
    m_vao->GetBufferObject(0)->UpdateBuffer(IndexBuffer.size(), IndexBuffer.data());
    m_vao->GetBufferObject(1)->UpdateBuffer(UVBuffer.size(), UVBuffer.data());
    m_vao->GetBufferObject(2)->UpdateBuffer(VertexBuffer.size(), VertexBuffer.data());
    m_vao->GetBufferObject(3)->UpdateBuffer(NormalBuffer.size(), NormalBuffer.data());
    m_vao->GetBufferObject(4)->UpdateBuffer(ColorBuffer.size(), ColorBuffer.data());
}

void BaseMesh::Render()
{
    m_vao->Render(IndexBuffer.size());
}

}
