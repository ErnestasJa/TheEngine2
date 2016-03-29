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
    UpdateHandle(desc);
    Bind();
    gl::UpdateBufferObject(m_handle);
}

void GLGpuBufferObject::UpdateHandle(const BufferDescriptor& desc)
{
    auto GetByteCount = [&]() {
        switch (desc.component_type) {
            case BufferComponentDataType::uint8:
            case BufferComponentDataType::int8:
                return 1;
            case BufferComponentDataType::uint16:
            case BufferComponentDataType::int16:
                return 2;
            case BufferComponentDataType::uint32:
            case BufferComponentDataType::int32:
            case BufferComponentDataType::float32:
                return 4;
            default:
                return 0;
        }
    };

    auto GetDataType = [&]() {
        switch (desc.component_type) {
            case BufferComponentDataType::int8:
                return GL_BYTE;
            case BufferComponentDataType::int16:
                return GL_SHORT;
            case BufferComponentDataType::int32:
                return GL_INT;
            case BufferComponentDataType::uint8:
                return GL_UNSIGNED_BYTE;
            case BufferComponentDataType::uint16:
                return GL_UNSIGNED_SHORT;
            case BufferComponentDataType::uint32:
                return GL_UNSIGNED_INT;
            case BufferComponentDataType::float32:
                return GL_FLOAT;
            default:
                return 0;
        }
    };

    m_handle.type = desc.type == BufferObjectType::index
                        ? GL_ELEMENT_ARRAY_BUFFER
                        : GL_ARRAY_BUFFER;
    m_handle.bytes = GetByteCount();
    m_handle.data_type = GetDataType();
    m_handle.data = desc.data;
    m_handle.count = desc.size;
}
}