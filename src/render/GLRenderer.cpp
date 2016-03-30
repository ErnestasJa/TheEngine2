#include "GLRenderer.h"
#include "GLProgram.h"
#include "GLGpuBufferObject.h"
#include "GLGpuBufferArrayObject.h"
#include "OpenGL.hpp"

namespace render
{
core::SharedPtr<IRenderer> CreateRenderer()
{
    return core::MakeShared<GLRenderer>();
}

GLRenderer::GLRenderer()
{
}
GLRenderer::~GLRenderer()
{
}

core::SharedPtr<IProgram> GLRenderer::CreateProgram(
    const core::String& vertSource, const core::String& fragSource,
    const core::String& geomSource)
{
    auto handle = gl::CreatePipelineFromShaderStrings(
        vertSource.c_str(), fragSource.c_str(), geomSource.c_str());

    if (gl::IsHandleValid(handle))
        return core::MakeShared<GLProgram>(handle);
    else
        return nullptr;
}

void ProcessHandle(const BufferDescriptor& desc,
                   gl::gpu_buffer_object_handle& handle)
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

    handle.buffer_size = desc.size;
    handle.buffer_type = desc.type == BufferObjectType::index
                             ? GL_ELEMENT_ARRAY_BUFFER
                             : GL_ARRAY_BUFFER;
    handle.component_size = GetByteCount();
    handle.component_type = GetDataType();
    handle.component_count = desc.component_count;
    handle.data = desc.data;
}

core::SharedPtr<IGpuBufferArrayObject> GLRenderer::CreateBufferArrayObject(
    const core::Vector<BufferDescriptor>& descriptors)
{
    auto handles = gl::CreateGpuStorages(descriptors.size());

    if (core::Each(handles, [](const gl::gpu_buffer_object_handle& h) {
            return gl::IsHandleValid(h);
        })) {
        uint32_t layout_index = 0;
        for (int i = 0; i < handles.size(); i++) {
            ProcessHandle(descriptors[i], handles[i]);
            if (descriptors[i].type == BufferObjectType::vertex) {
                handles[i].index = layout_index;
                layout_index++;
            } else {
                handles[i].index = 0;
            }
        }

        auto handle = gl::CreateVertexArrayObject();
        if (gl::IsHandleValid(handle) == false) return nullptr;

        core::Vector<core::UniquePtr<GLGpuBufferObject>> buffers;

        for (auto h : handles) {
            buffers.push_back(core::MakeUnique<GLGpuBufferObject>(h));
        }

        auto vao = core::MakeShared<GLGpuBufferArrayObject>(handle,
                                                            std::move(buffers));

        vao->EnableBuffers();

        return vao;
    }

    return nullptr;
}

void GLRenderer::SetClearColor(const Vec3i& color)
{
    gl::SetClearColor(color);
}

void GLRenderer::Clear()
{
    gl::Clear();
}
}