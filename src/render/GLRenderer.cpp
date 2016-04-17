#include "GLRenderer.h"
#include "GLGpuShaderProgram.h"
#include "GLGpuBufferObject.h"
#include "GLGpuBufferArrayObject.h"
#include "GLRendererDebugMessageMonitor.h"
#include "render/CTexture.h"
#include "GLTexture.h"
#include "OpenGL.hpp"

namespace render
{
core::UniquePtr<IRenderer> CreateRenderer(
    core::UniquePtr<GLRendererDebugMessageMonitor>&& debugMessageMonitor)
{
    return core::MakeUnique<GLRenderer>(core::Move(debugMessageMonitor));
}

GLRenderer::GLRenderer(
    core::UniquePtr<GLRendererDebugMessageMonitor>&& debugMessageMonitor)
    : m_debugMessageMonitor(core::Move(debugMessageMonitor))
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
}
GLRenderer::~GLRenderer()
{
}

IRendererDebugMessageMonitor* GLRenderer::GetDebugMessageMonitor()
{
    return m_debugMessageMonitor.get();
}

core::SharedPtr<IGpuProgram> GLRenderer::CreateProgram(
    const core::String& vertSource, const core::String& fragSource,
    const core::String& geomSource)
{
    auto handle = gl::CreatePipelineFromShaderStrings(
        vertSource.c_str(), fragSource.c_str(), geomSource.c_str());

    if (gl::IsHandleValid(handle) && gl::IsProgramPipelineLinked(handle))
        return core::MakeShared<GLGpuShaderProgram>(handle);
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

    core::Vector<core::UniquePtr<IGpuBufferObject>> buffers;
    for (uint32_t i = 0; i < handles.size(); i++) {
        gl::ProcessHandle(descriptors[i], handles[i]);
        buffers.push_back(core::MakeUnique<GLGpuBufferObject>(handles[i]));
    }

    // create the vao
    auto handle = gl::CreateVertexArrayObject();
    if (gl::IsHandleValid(handle) == false) return nullptr;

    auto vao =
        core::MakeShared<GLGpuBufferArrayObject>(handle, core::Move(buffers));

    vao->EnableBuffers();

    return vao;
}

core::SharedPtr<ITexture> GLRenderer::CreateTexture(
    const TextureDescriptor& descriptor)
{
    auto handle = gl::CreateTexture(descriptor);

    if (gl::IsHandleValid(handle)) {
        auto texture = core::MakeShared<GLTexture>(handle);
        return texture;
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