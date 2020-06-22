#include "render/debug/DebugRenderer.h"
#include <render/BaseMaterial.h>
#include <render/BaseMesh.h>
#include <render/ICamera.h>
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
  else if constexpr (std::is_same<TPrimitiveType, DebugAABVObject>::value) {
    return 8 * 3;
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
  // m_vao->GetBufferObject(0)->UpdateBuffer(m_bufferSize, nullptr);
  m_vao->GetBufferObject(1)->UpdateBuffer(m_bufferSize, nullptr);
  m_vao->GetBufferObject(2)->UpdateBuffer(m_bufferSize, nullptr);

  IndexBuffer.resize(m_bufferSize);
  VertexBuffer.resize(m_bufferSize);
  ColorBuffer.resize(m_bufferSize);

  for (int i = 0; i < IndexBuffer.size(); i++) {
    IndexBuffer[i] = i;
  }

  m_vao->GetBufferObject(0)->UpdateBuffer(m_bufferSize, IndexBuffer.data());
}

void DebugMesh::PartialUpdate(int32_t start, int32_t count)
{
  m_vao->GetBufferObject(1)->UpdateBufferSubData(start, count, &VertexBuffer[start]);
  m_vao->GetBufferObject(2)->UpdateBufferSubData(start, count, &ColorBuffer[start]);
}

void DebugMesh::Render(int32_t count)
{
  m_vao->RenderLines(count);
}

void UpdateBuffer(DebugMesh* mesh, const DebugAABVObject& obj)
{
  int32_t pos                 = obj.Index * PrimiteCountPerObject<DebugAABVObject>();

  for(int i = pos; i < pos + PrimiteCountPerObject<DebugAABVObject>(); i++){
    mesh->ColorBuffer[i]      = obj.Color;
  }

  //bottom
  mesh->VertexBuffer[pos]     = obj.Start;
  mesh->VertexBuffer[pos + 1] = {obj.End.x, obj.Start.y, obj.Start.z};

  mesh->VertexBuffer[pos + 2] = {obj.End.x, obj.Start.y, obj.Start.z};
  mesh->VertexBuffer[pos + 3] = {obj.End.x, obj.Start.y, obj.End.z};

  mesh->VertexBuffer[pos + 4] = {obj.End.x, obj.Start.y, obj.End.z};
  mesh->VertexBuffer[pos + 5] = {obj.Start.x, obj.Start.y, obj.End.z};

  mesh->VertexBuffer[pos + 6]     = obj.Start;
  mesh->VertexBuffer[pos + 7] = {obj.Start.x, obj.Start.y, obj.End.z};

  //sides
  mesh->VertexBuffer[pos + 8]     = obj.Start;
  mesh->VertexBuffer[pos + 9] = {obj.Start.x, obj.End.y, obj.Start.z};

  mesh->VertexBuffer[pos + 10] = {obj.Start.x, obj.Start.y, obj.End.z};
  mesh->VertexBuffer[pos + 11] = {obj.Start.x, obj.End.y, obj.End.z};

  mesh->VertexBuffer[pos + 12] = {obj.End.x, obj.Start.y, obj.Start.z};
  mesh->VertexBuffer[pos + 13] = {obj.End.x, obj.End.y, obj.Start.z};

  mesh->VertexBuffer[pos + 14] = {obj.End.x, obj.Start.y, obj.End.z};
  mesh->VertexBuffer[pos + 15] = obj.End;

  //top
  mesh->VertexBuffer[pos + 16] = {obj.Start.x, obj.End.y, obj.Start.z};
  mesh->VertexBuffer[pos + 17] = {obj.End.x, obj.End.y, obj.Start.z};

  mesh->VertexBuffer[pos + 18] = {obj.End.x, obj.End.y, obj.Start.z};
  mesh->VertexBuffer[pos + 19] = obj.End;

  mesh->VertexBuffer[pos + 20] = obj.End;
  mesh->VertexBuffer[pos + 21] = {obj.Start.x, obj.End.y, obj.End.z};

  mesh->VertexBuffer[pos + 22] = {obj.Start.x, obj.End.y, obj.End.z};
  mesh->VertexBuffer[pos + 23] = {obj.Start.x, obj.End.y, obj.Start.z};

  mesh->PartialUpdate(pos, PrimiteCountPerObject<DebugAABVObject>());
}

void UpdateBuffer(DebugMesh* mesh, const DebugLineObject & obj)
{
  int32_t pos                 = obj.Index * 2;
  mesh->ColorBuffer[pos]      = obj.Color;
  mesh->ColorBuffer[pos + 1]  = obj.Color;
  mesh->VertexBuffer[pos]     = obj.Start;
  mesh->VertexBuffer[pos + 1] = obj.End;
  mesh->PartialUpdate(pos, PrimiteCountPerObject<DebugLineObject>());
}

template <class T>
core::UniquePtr<DebugMesh> CreateDebugMesh(IRenderer * renderer, uint32_t maxObjects)
{
  auto vao = renderer->CreateBufferArrayObject({
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

  auto debugMesh = core::MakeUnique<DebugMesh>(core::Move(vao),
                              maxObjects * PrimiteCountPerObject<T>());

  debugMesh->Init();
  return debugMesh;
}

DebugRenderer::DebugRenderer(int32_t maxObjects, IRenderer* renderer,
                             res::ResourceManager* resource_manager)
    : m_debuggerAbsTimeElapsed(0)
    , m_maxObjects(maxObjects)
    , m_renderer(renderer)
    , m_resource_manager(resource_manager)
    , m_totalActiveLineObjects(0)
    , m_totalActiveAABVObjects(0)
{
  m_lines = CreateDebugMesh<DebugLineObject>(m_renderer, m_maxObjects);
  m_aabvs = CreateDebugMesh<DebugAABVObject>(m_renderer, m_maxObjects);

  m_lineObjects.resize(m_maxObjects);
  m_aabvObjects.resize(m_maxObjects);

  m_material               = m_resource_manager->LoadMaterial("resources/shaders/debug_renderer");
  m_material->RenderMode   = material::MeshRenderMode::Lines;
  m_material->UseDepthTest = false;
}

template <class TObjectType>
const char* GetObjectName(){
  if constexpr (std::is_same<TObjectType, DebugLineObject>::value) {
    return "Line";
  }
  else if constexpr (std::is_same<TObjectType, DebugAABVObject>::value) {
    return "AABV";
  }
  return "";
}

template <class TObjectType>
void UpdateObjects(float currentTime, int32_t& totalActiveObjects, core::Vector<TObjectType> & objects, DebugMesh* mesh){
  int32_t lastActiveObjIndex = totalActiveObjects - 1;

  if (lastActiveObjIndex < 0) {
    return;
  }

  core::Vector<int> m_expiredObjects;

  for (int i = 0; i < objects.size(); i++) {
    auto& obj = objects[i];

    if (obj.Alive && ((obj.TimeToLive + obj.AbsTimeAdded) <= currentTime)) {
      obj.Alive = false;
      totalActiveObjects--;
      m_expiredObjects.push_back(i);
      //elog::LogInfo(core::string::format("{} expired. Total lines: {}", GetObjectName<TObjectType>(), totalActiveObjects));
    }
  }

  if (m_expiredObjects.size() == 0) {
    return;
  }

  int currentExpiredToBeFilled = 0;
  int expiredIndex             = m_expiredObjects[currentExpiredToBeFilled];
  int lastExpiredObj           = m_expiredObjects.size() - 1;

  while (true) {
    if (lastActiveObjIndex > 0) {
      auto& lastActive = objects[lastActiveObjIndex];

      if (lastActive.Alive) {
        auto& expired    = objects[expiredIndex];
        expired          = objects[lastActiveObjIndex];
        expired.Index    = expiredIndex;
        lastActive.Alive = false;

        UpdateBuffer(mesh, expired);

        currentExpiredToBeFilled++;

        if (currentExpiredToBeFilled > lastExpiredObj) {
          break;
        }

        expiredIndex = m_expiredObjects[currentExpiredToBeFilled];
      }
    }

    lastActiveObjIndex--;

    if (expiredIndex >= lastActiveObjIndex) {
      break;
    }
  }

  mesh->PartialUpdate(0, PrimiteCountPerObject<DebugLineObject>() * totalActiveObjects);
}

void DebugRenderer::Update(float deltaMs)
{
  m_debuggerAbsTimeElapsed += deltaMs;
  UpdateObjects(m_debuggerAbsTimeElapsed, m_totalActiveAABVObjects, m_aabvObjects, m_aabvs.get());
  UpdateObjects(m_debuggerAbsTimeElapsed, m_totalActiveLineObjects, m_lineObjects, m_lines.get());
}

void DebugRenderer::Render()
{
  if(!m_totalActiveAABVObjects && !m_totalActiveLineObjects){
    return;
  }

  auto camera = m_renderer->GetRenderContext()->GetCurrentCamera();
  m_renderer->GetRenderContext()->SetCurrentMaterial(m_material.get());
  m_material->SetMat4("MVP", camera->GetProjection() * camera->GetView() * glm::mat4(1));

  if(m_totalActiveLineObjects) {
    m_lines->Render(m_totalActiveLineObjects * PrimiteCountPerObject<DebugLineObject>());
  }
  if(m_totalActiveAABVObjects){
    m_aabvs->Render(m_totalActiveAABVObjects * PrimiteCountPerObject<DebugAABVObject>());
  }
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
    //elog::LogInfo("Failed to add debug line. Too many active debug line objects already.");
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
  m_totalActiveLineObjects++;

  //elog::LogInfo(core::string::format("Line added. Total lines: {}", m_totalActiveLineObjects));
}

void DebugRenderer::AddAABV(glm::vec3 start, glm::vec3 end, float lifeTimeSeconds)
{
  auto expiredIt = std::find_if(std::begin(m_aabvObjects), std::end(m_aabvObjects),
                                    [](const DebugAABVObject& obj) { return obj.Alive == false; });
  int32_t index;

  if (expiredIt != std::end(m_aabvObjects)) {
    index = std::distance(std::begin(m_aabvObjects), expiredIt);
  }
  else {
    //elog::LogInfo("Failed to add debug line. Too many active debug line objects already.");
    return;
  }

  DebugAABVObject obj;
  obj.Alive            = true;
  obj.Index            = index;
  obj.Start            = start;
  obj.End              = end;
  obj.Color            = { 0, 1, 0 };
  obj.AbsTimeAdded     = m_debuggerAbsTimeElapsed;
  obj.TimeToLive       = lifeTimeSeconds * 1000.0f;
  m_aabvObjects[index] = obj;

  UpdateBuffer(m_aabvs.get(), obj);
  m_totalActiveAABVObjects++;

  //elog::LogInfo(core::string::format("AABV added. Total AABVs: {}", m_totalActiveAABVObjects));
}

} // namespace render