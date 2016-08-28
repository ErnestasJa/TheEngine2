#ifndef GLGPUBUFFEROBJECT_H
#define GLGPUBUFFEROBJECT_H

#include "OpenGL.hpp"
#include "render/IGpuBufferObject.h"

namespace render {
class GLGpuBufferObject : public IGpuBufferObject
{
public:
    GLGpuBufferObject(const gl::gpu_buffer_object_handle& handle);
    virtual ~GLGpuBufferObject();

    virtual void Bind();
    virtual void UpdateBuffer(uint32_t count, void* data);

public:
    void Enable();
    const gl::gpu_buffer_object_handle& GetHandle();

private:
    gl::gpu_buffer_object_handle m_handle;
};
}

#endif