#include "GLGpuBufferArrayObject.h"
#include "GLGpuBufferObject.h"

namespace render
{
GLGpuBufferArrayObject::GLGpuBufferArrayObject(
    const gl::gpu_vertex_array_object_handle& handle,
    core::Vector<core::UniquePtr<IGpuBufferObject>>&& bufferObjects)
    : m_handle(handle)
{
    m_gpuBufferObjects = core::Move(bufferObjects);
}

GLGpuBufferArrayObject::~GLGpuBufferArrayObject()
{
    gl::FreeHandle(m_handle);
}

void GLGpuBufferArrayObject::Bind()
{
    gl::BindHandle(m_handle);
}

const core::Vector<core::UniquePtr<IGpuBufferObject>>&
GLGpuBufferArrayObject::GetBuffers()
{
    return m_gpuBufferObjects;
}

IGpuBufferObject* GLGpuBufferArrayObject::GetBufferObject(uint32_t index)
{
    return m_gpuBufferObjects[index].get();
}

uint32_t GLGpuBufferArrayObject::GetBufferObjectCount()
{
    return m_gpuBufferObjects.size();
}

IGpuBufferObject* GLGpuBufferArrayObject::GetIndexBuffer()
{
    for (const auto& buffer : m_gpuBufferObjects) {
        if (static_cast<GLGpuBufferObject*>(buffer.get())
                ->GetHandle()
                .buffer_type == GL_ELEMENT_ARRAY_BUFFER)
            return buffer.get();
    }
    return nullptr;
}

void GLGpuBufferArrayObject::Render(uint32_t count)
{
    gl::BindHandle(m_handle);
    GetIndexBuffer()->Bind();
    gl::Render(static_cast<GLGpuBufferObject*>(GetIndexBuffer())->GetHandle(),
               count);
}

void GLGpuBufferArrayObject::EnableBuffers()
{
    gl::BindHandle(m_handle);
    for (const auto& buf : m_gpuBufferObjects) {
        static_cast<GLGpuBufferObject*>(buf.get())->Enable();
    }
}
}