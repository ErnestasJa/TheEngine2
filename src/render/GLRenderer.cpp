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

core::SharedPtr<IGpuBufferArrayObject> GLRenderer::CreateBufferArrayObject(
    const core::Vector<BufferDescriptor>& descriptors)
{
    // setup buffers
    auto handles = gl::CreateGpuStorages(descriptors.size());
    auto IsValid = [](const gl::gpu_buffer_object_handle& h) {
        return gl::IsHandleValid(h);
    };

    if (core::Any(handles, IsValid) == false) return nullptr;

    core::Vector<core::UniquePtr<GLGpuBufferObject>> buffers;
    for (uint32_t i = 0; i < handles.size(); i++) {
        gl::ProcessHandle(descriptors[i], handles[i]);
        buffers.push_back(core::MakeUnique<GLGpuBufferObject>(handles[i]));
    }

    // create the vao
    auto handle = gl::CreateVertexArrayObject();
    if (gl::IsHandleValid(handle) == false) return nullptr;

    auto vao =
        core::MakeShared<GLGpuBufferArrayObject>(handle, std::move(buffers));

    vao->EnableBuffers();

    return vao;
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