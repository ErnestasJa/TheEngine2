#ifndef ENGINE_BASEMESH_H
#define ENGINE_BASEMESH_H

#include "glm/glm.hpp"
#include <render/BufferDescriptor.h>

namespace render {
struct BufferDescriptor;
class IGpuBufferArrayObject;
class IRenderer;

class BaseMesh
{
  public:
  core::Vector<uint32_t> IndexBuffer;
  core::Vector<glm::vec2> UVBuffer;
  core::Vector<glm::vec3> VertexBuffer;
  core::Vector<glm::vec3> NormalBuffer;
  core::Vector<glm::vec3> ColorBuffer;

  BaseMesh();
  BaseMesh(core::UniquePtr<IGpuBufferArrayObject> vao);

  virtual ~BaseMesh();

  virtual void InitEmpty();
  virtual void Upload();
  virtual void UploadSubData(int32_t start, int32_t count);

  virtual void Render();

  IGpuBufferArrayObject* GetGpuBufferObject()
  {
    return m_vao.get();
  }

  void SetUseColorBuffer(bool use)
  {
    m_useColorBuffer = use;
  }

  bool ShouldUseColorBuffer()
  {
    return m_useColorBuffer;
  }

  protected:
  core::UniquePtr<render::IGpuBufferArrayObject> m_vao;
  bool m_useColorBuffer;
};

} // namespace render

#endif // ENGINE_BASEMESH_H
