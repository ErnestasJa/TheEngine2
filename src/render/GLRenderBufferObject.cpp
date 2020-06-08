#include "GLRenderBufferObject.h"
#include "OpenGL.hpp"

namespace render {

void GLRenderBufferObject::BindObject(GLRenderBufferObject* object)
{
  glBindRenderbuffer(GL_RENDERBUFFER, object ? object->GetHandle().id : 0);
}

core::SharedPtr<IRenderBufferObject> GLRenderBufferObject::Create(
    const RenderBufferObjectDescriptor& descriptor)
{
  auto GetFormat = [&]() {
    switch (descriptor.format) {
    case render::RenderBufferObjectInternalDataFormat::RGB:
      return GL_RGB;
    case render::RenderBufferObjectInternalDataFormat::RGBA:
      return GL_RGBA;
    case render::RenderBufferObjectInternalDataFormat::DEPTH16:
      return GL_DEPTH_COMPONENT16;
    default:
      return GL_RGBA;
    }
  };

  gl::gpu_render_buffer_object_handle handle;
  handle.format = GetFormat();
  handle.height = descriptor.height;
  handle.width  = descriptor.width;

  glGenRenderbuffers(1, &handle.id);
  glBindRenderbuffer(GL_RENDERBUFFER, handle.id);
  glRenderbufferStorage(GL_RENDERBUFFER, handle.format, handle.width, handle.height);

  return core::MakeShared<GLRenderBufferObject>(handle);
}

GLRenderBufferObject::GLRenderBufferObject(const gl::gpu_render_buffer_object_handle& handle)
    : gl::gpu_object<gl::gpu_render_buffer_object_handle>(handle)
{
}

GLRenderBufferObject::~GLRenderBufferObject()
{
}
} // namespace render