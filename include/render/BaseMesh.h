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

    BaseMesh(core::SharedPtr<IGpuBufferArrayObject> vao);

    virtual void Upload();

    virtual void Render();

    IGpuBufferArrayObject* GetGpuBufferObject(){
        return m_vao.get();
    }

    void SetUseColorBuffer(bool use){
        m_useColorBuffer = use;
    }

    bool ShouldUseColorBuffer(){
        return m_useColorBuffer;
    }

protected:
    core::SharedPtr<render::IGpuBufferArrayObject> m_vao;
    bool m_useColorBuffer;
};

}

#endif // ENGINE_BASEMESH_H
