#include "GLGpuBufferArrayObject.h"
#include "GLGpuBufferObject.h"

namespace render
{
GLGpuBufferArrayObject::GLGpuBufferArrayObject(
    const gl::gpu_vertex_array_object_handle& handle,
    core::Vector<core::UniquePtr<GLGpuBufferObject>>&& bufferObjects)
    : m_handle(handle)
{
    m_gpuBufferObjects = std::move(bufferObjects);
}

GLGpuBufferArrayObject::~GLGpuBufferArrayObject()
{
    gl::FreeHandle(m_handle);
}

void GLGpuBufferArrayObject::Bind()
{
    gl::BindHandle(m_handle);
}

IGpuBufferObject* GLGpuBufferArrayObject::GetBufferObject(uint32_t index)
{
    return m_gpuBufferObjects[index].get();
}

uint32_t GLGpuBufferArrayObject::GetBufferObjectCount()
{
    return m_gpuBufferObjects.size();
}

GLGpuBufferObject* GLGpuBufferArrayObject::GetIndexBuffer()
{
    for (const auto& buffer : m_gpuBufferObjects) {
        if (buffer->GetHandle().buffer_type == GL_ELEMENT_ARRAY_BUFFER)
            return buffer.get();
    }
    return nullptr;
}

void GLGpuBufferArrayObject::Render()
{
    gl::BindHandle(m_handle);
    GetIndexBuffer()->Bind();
    gl::Render(GetIndexBuffer()->GetHandle());
}

void GLGpuBufferArrayObject::EnableBuffers()
{
    gl::BindHandle(m_handle);
    for (const auto& buf : m_gpuBufferObjects) {
        buf->Enable();
    }
}
}