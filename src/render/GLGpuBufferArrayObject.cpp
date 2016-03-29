#include "GLGpuBufferArrayObject.h"
#include "GLGpuBufferObject.h"

namespace render
{
GLGpuBufferArrayObject::GLGpuBufferArrayObject(
    const gl::gpu_vertex_array_object_handle& handle)
    : m_handle(handle)
{
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

bool GLGpuBufferArrayObject::CreateBufferObjects(BufferDescriptor* descriptors,
                                                 uint32_t count)
{
    auto handles = core::UniquePtr<gl::gpu_buffer_object_handle[]>(
        new gl::gpu_buffer_object_handle[count]);

    gl::CreateGpuStorage(handles.get(), count);

    for (uint32_t i = 0; i < count; i++) {
        if (gl::IsHandleValid(handles.get()[i]) == false) {
            return false;
        }
    }

    for (uint32_t i = 0; i < count; i++) {
        m_gpuBufferObjects.push_back(
            core::MakeUnique<GLGpuBufferObject>(handles.get()[i]));
    }

    return true;
}
}