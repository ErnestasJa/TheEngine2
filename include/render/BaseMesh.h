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
    core::Vector<glm::vec2> UVBuffer;
    core::Vector<glm::vec3> VertexBuffer;
    core::Vector<glm::vec3> NormalBuffer;
    core::Vector<glm::vec3> ColorBuffer;

    core::Vector<uint32_t> IndexBuffer;

    BaseMesh();

    BaseMesh(core::SharedPtr<IGpuBufferArrayObject> vao);

    void Upload();

    void Render();

    IGpuBufferArrayObject* GetGpuBufferObject(){
        return m_vao.get();
    }
protected:

    core::SharedPtr<IGpuBufferArrayObject> m_vao;
};

}

#endif // ENGINE_BASEMESH_H
