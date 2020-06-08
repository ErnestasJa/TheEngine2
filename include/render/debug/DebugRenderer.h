#ifndef THEPROJECT2_LIBS_THEENGINE2_INCLUDE_RENDER_DEBUG_DEBUGRENDERER_H_
#define THEPROJECT2_LIBS_THEENGINE2_INCLUDE_RENDER_DEBUG_DEBUGRENDERER_H_

#include <render/RenderFwd.h>
#include <resource_management/ResourceManagementFwd.h>
namespace render {
struct DebugLineObject
{
  int32_t Index;
  glm::vec3 Start;
  glm::vec3 End;
  glm::vec3 Color;
  float AbsTimeAdded;
  float TimeToLive;
  bool Alive;

  DebugLineObject()
      : Alive(false)
  {
  }
};

class DebugMesh
{
  public:
  DebugMesh(core::UniquePtr<render::IGpuBufferArrayObject> vao, int32_t bufferSize);
  void Init();
  void PartialUpdate(int32_t start, int32_t count);
  void Render(int32_t count);

  public:
  core::Vector<uint32_t> IndexBuffer;
  core::Vector<glm::vec3> VertexBuffer;
  core::Vector<glm::vec3> ColorBuffer;

  private:
  int32_t m_bufferSize;
  core::UniquePtr<render::IGpuBufferArrayObject> m_vao;
};

class DebugRenderer
{
  public:
  DebugRenderer(int32_t maxObjects, IRenderer* renderer, res::ResourceManager* resource_manager);
  void Update(float deltaMs);
  void Render();
  void AddLine(glm::vec3 start, glm::vec3 end, float lifeTimeSeconds);

  private:
  core::Vector<DebugLineObject> m_lineObjects;
  core::UniquePtr<DebugMesh> m_lines;
  core::SharedPtr<material::BaseMaterial> m_material;

  IRenderer* m_renderer;
  res::ResourceManager* m_resource_manager;
  float m_debuggerAbsTimeElapsed;
  int32_t m_maxObjects;
  int32_t m_totalLineObjects;
};
} // namespace render

#endif // THEPROJECT2_LIBS_THEENGINE2_INCLUDE_RENDER_DEBUG_DEBUGRENDERER_H_
