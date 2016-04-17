#include "GLTexture.h"

namespace render
{
GLTexture::GLTexture(const gl::gpu_texture_handle &handle) : m_handle(handle)
{
}

GLTexture::~GLTexture()
{
    gl::FreeHandle(m_handle);
}

void GLTexture::UploadData(const TextureDataDescriptor &descriptor)
{
    gl::UploadTextureData(m_handle, descriptor);
}

void GLTexture::Bind()
{
    gl::BindHandle(m_handle);
}
}