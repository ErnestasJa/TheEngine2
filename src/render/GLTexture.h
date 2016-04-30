#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include "render/ITexture.h"
#include "OpenGL.hpp"

namespace render
{
class GLTexture : public ITexture
{
public:
    GLTexture(const gl::gpu_texture_handle &handle);
    virtual ~GLTexture();
    virtual void UploadData(const TextureDataDescriptor &descriptor);
    virtual void Bind() const;

private:
    gl::gpu_texture_handle m_handle;
};
}

#endif