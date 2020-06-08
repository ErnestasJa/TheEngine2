#include "render/debug/DebugRenderer.h"
#include <render/BaseMaterial.h>
#include <render/BaseMesh.h>
#include <render/IGpuBufferArrayObject.h>
#include <render/IGpuBufferObject.h>
#include <render/IRenderContext.h>
#include <render/IRenderer.h>
#include <resource_management/ResourceManagementInc.h>

namespace render {

template <class TPrimitiveType> inline int32_t PrimiteCountPerObject()
{
  if constexpr (std::is_same<TPrimitiveType, DebugLineObject>::value) {
    return 2;
  }

  return 3;
}

DebugMesh::DebugMesh(core::UniquePtr<render::IGpuBufferArrayObject> vao, int32_t bufferSize)
    : m_vao(core::Move(vao))
    , m_bufferSize(bufferSize)
{
}

void DebugMesh::Init()
{
  m_vao->GetBufferObject(0)->UpdateBuffer(m_bufferSize, nullptr);
  m_vao->GetBufferObject(1)->UpdateBuffer(m_bufferSize, nullptr);
  m_vao->GetBufferObject(2)->UpdateBuffer(m_bufferSize, nullptr);

  IndexBuffer.resize(m_bufferSize);
  VertexBuffer.resize(m_bufferSize);
  ColorBuffer.resize(m_bufferSize);

  for (int i = 0; i < IndexBuffer.size(); i++) {
    IndexBuffer[i] = i;
  }

  m_vao->GetBufferObject(0)->UpdateBuffer(m_bufferSize, nullptr);
}

void DebugMesh::PartialUpdate(int32_t start, int32_t count)
{
  m_vao->GetBufferObject(1)->UpdateBufferSubData(start, count, VertexBuffer.data() + start);
  m_vao->GetBufferObject(2)->UpdateBufferSubData(start, count, ColorBuffer.data() + start);
}

void DebugMesh::Render(int32_t count)
{
  m_vao->Render(count);
}

void UpdateBuffer(DebugMesh* mesh, const DebugLineObject& obj)
{
  int32_t pos                 = obj.Index * 2;
  mesh->ColorBuffer[pos]      = obj.Color;
  mesh->ColorBuffer[pos + 1]  = obj.Color;
  mesh->VertexBuffer[pos]     = obj.Start;
  mesh->VertexBuffer[pos + 1] = obj.End;
  mesh->PartialUpdate(pos, PrimiteCountPerObject<DebugLineObject>());
}

DebugRenderer::DebugRenderer(int32_t maxObjects, IRenderer* renderer,
                             res::ResourceManager* resource_manager)
    : m_debuggerAbsTimeElapsed(0)
    , m_maxObjects(maxObjects)
    , m_renderer(renderer)
    , m_resource_manager(resource_manager)
    , m_totalLineObjects(0)
{
  auto vao = m_renderer->CreateBufferArrayObject({
      render::BufferDescriptor{ 1, render::BufferObjectType::index,
                                render::BufferComponentDataType::uint32, 0,
                                render::BufferUsageHint::StaticDraw },

      render::BufferDescriptor{ 3, render::BufferObjectType::vertex,
                                render::BufferComponentDataType::float32, 0,
                                render::BufferUsageHint::StaticDraw },

      render::BufferDescriptor{ 3, render::BufferObjectType::vertex,
                                render::BufferComponentDataType::float32, 1,
                                render::BufferUsageHint::StaticDraw },
  });

  m_lines = core::MakeUnique<DebugMesh>(core::Move(vao), m_maxObjects);
  m_lines->Init();
  m_lineObjects.resize(m_maxObjects);

  m_material             = m_resource_manager->LoadMaterial("resources/shaders/debug_renderer");
  m_material->RenderMode = material::MeshRenderMode::Lines;
}

void DebugRenderer::Update(float deltaMs)
{
  m_debuggerAbsTimeElapsed += deltaMs;
  int32_t lastActiveObjIndex = m_totalLineObjects - 1;

  if (lastActiveObjIndex < 0) {
    return;
  }

  core::Vector<int> m_expiredObjects;

  for (int i = 0; i < m_lineObjects.size(); i++) {
    auto& obj = m_lineObjects[i];

    if (obj.Alive && ((obj.TimeToLive + obj.AbsTimeAdded) <= m_debuggerAbsTimeElapsed)) {
      obj.Alive = false;
      m_totalLineObjects--;
      m_expiredObjects.push_back(i);
      elog::LogInfo(core::string::format("Line expired. Total lines: {}", m_totalLineObjects));
    }
  }

  if (m_expiredObjects.size() == 0) {
    return;
  }

  int currentExpiredToBeFilled = 0;
  int firstExpiredIndex        = m_expiredObjects[currentExpiredToBeFilled];

  while (true) {
    if (firstExpiredIndex >= lastActiveObjIndex) {
      break;
    }

    if (lastActiveObjIndex > 0 && m_lineObjects[lastActiveObjIndex].Alive) {
      auto& expired = m_lineObjects[firstExpiredIndex];
      expired       = m_lineObjects[lastActiveObjIndex];
      expired.Index = firstExpiredIndex;
      UpdateBuffer(m_lines.get(), expired);

      currentExpiredToBeFilled++;
      firstExpiredIndex = m_expiredObjects[currentExpiredToBeFilled];
    }
    lastActiveObjIndex--;
  }

  m_lines->PartialUpdate(0, PrimiteCountPerObject<DebugLineObject>() * m_totalLineObjects);
}

void DebugRenderer::Render()
{
  m_renderer->GetRenderContext()->SetCurrentMaterial(m_material.get());
  m_lines->Render(m_totalLineObjects * PrimiteCountPerObject<DebugLineObject>());
}

void DebugRenderer::AddLine(glm::vec3 start, glm::vec3 end, float lifeTimeSeconds)
{
  auto expiredLineIt = std::find_if(std::begin(m_lineObjects), std::end(m_lineObjects),
                                    [](const DebugLineObject& obj) { return obj.Alive == false; });
  int32_t index;

  if (expiredLineIt != std::end(m_lineObjects)) {
    index = std::distance(std::begin(m_lineObjects), expiredLineIt);
  }
  else {
    elog::LogInfo("Failed to add debug line. Too many active debug line objects already.");
    return;
  }

  DebugLineObject obj;
  obj.Alive            = true;
  obj.Index            = index;
  obj.Start            = start;
  obj.End              = end;
  obj.Color            = { 1.f, 0, 0 };
  obj.AbsTimeAdded     = m_debuggerAbsTimeElapsed;
  obj.TimeToLive       = lifeTimeSeconds * 1000.0f;
  m_lineObjects[index] = obj;

  UpdateBuffer(m_lines.get(), obj);
  m_totalLineObjects++;

  elog::LogInfo(core::string::format("Line added. Total lines: {}", m_totalLineObjects));
}
} // namespace render