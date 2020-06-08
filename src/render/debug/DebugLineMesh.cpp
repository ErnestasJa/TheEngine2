#include "render/debug/DebugLineMesh.h"
#include "render/BaseMaterial.h"
#include "render/BaseMesh.h"
#include <glm/detail/type_vec3.hpp>

namespace render::debug {
DebugLineMesh::DebugLineMesh(core::SharedPtr<BaseMesh> mesh,
                             core::SharedPtr<material::BaseMaterial> material)
    : m_mesh(mesh)
    , m_material(material)
    , m_isUploaded(false)
{
  m_material->UseDepthTest = false;
  m_material->RenderMode   = material::MeshRenderMode::Lines;
}

void DebugLineMesh::AddLine(glm::vec3 start, glm::vec3 end, glm::tvec3<uint8_t> color)
{
  auto index = m_mesh->VertexBuffer.size();
  m_mesh->IndexBuffer.push_back(index);
  m_mesh->IndexBuffer.push_back(index + 1);

  m_mesh->VertexBuffer.push_back(start);
  m_mesh->VertexBuffer.push_back(end);

  auto normalizedColor = glm::vec3(color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
  m_mesh->ColorBuffer.push_back(normalizedColor);
  m_mesh->ColorBuffer.push_back(normalizedColor);


  m_isUploaded = false;
}

void DebugLineMesh::Clear()
{
  m_mesh->VertexBuffer.clear();
  m_mesh->IndexBuffer.clear();
  m_mesh->ColorBuffer.clear();
  m_isUploaded = false;
}

void DebugLineMesh::Upload()
{
  if (!m_isUploaded) {
    m_mesh->Upload();
  }
}

void DebugLineMesh::AddGrid(int count, int spacing, glm::tvec3<uint8_t> color)
{
  count = count + (count % 2);

  float end   = (count * spacing / 2);
  float start = end * -1;
  for (int i = 0; i < count; i++) {
    float offset = start + (i * spacing);
    AddLine(glm::vec3(start, 0, offset), glm::vec3(end, 0, offset), color);
    AddLine(glm::vec3(offset, 0, start), glm::vec3(offset, 0, end), color);
  }
}

} // namespace render::debug