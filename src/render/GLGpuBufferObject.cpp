#include "GLGpuBufferObject.h"

namespace render
{
GLGpuBufferObject::GLGpuBufferObject(const gl::gpu_buffer_object_handle& handle)
    : m_handle(handle)
{
}

GLGpuBufferObject::~GLGpuBufferObject()
{
    gl::FreeHandle(m_handle);
}

void GLGpuBufferObject::Bind()
{
    gl::BindHandle(m_handle);
}

void GLGpuBufferObject::UpdateBuffer(const BufferDescriptor& desc)
{
    gl::BindHandle(m_handle);
    gl::UpdateBufferObject(m_handle);
}

void GLGpuBufferObject::Enable()
{
    gl::BindHandle(m_handle);
    gl::EnableVertexArrayBuffer(m_handle);
}

const gl::gpu_buffer_object_handle& GLGpuBufferObject::GetHandle()
{
    return m_handle;
}
}