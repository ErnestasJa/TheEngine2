#ifndef GLGPUBUFFERARRAYOBJECT_H
#define GLGPUBUFFERARRAYOBJECT_H

#include "render/IGpuBufferArrayObject.h"
#include "render/IGpuBufferObject.h"
#include "OpenGL.hpp"

namespace render
{
class GLGpuBufferObject;
class GLGpuBufferArrayObject : public IGpuBufferArrayObject
{
public:
    GLGpuBufferArrayObject(
        const gl::gpu_vertex_array_object_handle& handle,
        core::Vector<core::UniquePtr<IGpuBufferObject>>&& bufferObjects);
    virtual ~GLGpuBufferArrayObject();

    virtual void Bind();
    virtual const core::Vector<core::UniquePtr<IGpuBufferObject>>& GetBuffers();
    virtual IGpuBufferObject* GetBufferObject(uint32_t index);
    virtual uint32_t GetBufferObjectCount();
    virtual void Render(uint32_t count);

public:
    void EnableBuffers();
    IGpuBufferObject* GetIndexBuffer();

private:
    gl::gpu_vertex_array_object_handle m_handle;
    core::Vector<std::unique_ptr<IGpuBufferObject>> m_gpuBufferObjects;
};
}

#endif