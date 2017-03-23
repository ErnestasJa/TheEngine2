#pragma once
#include "OpenGL.hpp"
#include "render/IRenderBufferObject.h"

namespace render {
namespace gl {
struct gpu_render_buffer_object_handle
{
    uint32_t id;
    uint32_t format;
    uint32_t width;
    uint32_t height;
};
}

class GLRenderBufferObject : public IRenderBufferObject,
                             public gl::gpu_object<gl::gpu_render_buffer_object_handle>
{
public:
    static core::SharedPtr<IRenderBufferObject> Create(
        const RenderBufferObjectDescriptor& descriptor);
    static void BindObject(GLRenderBufferObject* object);

public:
    GLRenderBufferObject(const gl::gpu_render_buffer_object_handle& handle);
    virtual ~GLRenderBufferObject();
};
}