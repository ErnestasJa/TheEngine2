#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include "OpenGL.hpp"
#include "render/ITexture.h"

namespace render {
namespace gl {
struct gpu_texture_handle : gpu_handle
{
    uint32_t type;
    uint32_t format;
    uint32_t wrap_s;
    uint32_t wrap_t;
    uint32_t filter_min;
    uint32_t filter_mag;
};
}


class GLTexture : public ITexture, gl::gpu_object<gl::gpu_texture_handle>
{
public:
    GLTexture(const gl::gpu_texture_handle& handle);
    virtual ~GLTexture();
    virtual void UploadData(const TextureDataDescriptor& descriptor);
};

core::SharedPtr<ITexture> CreateTexture(const TextureDescriptor& descriptor);
void BindObject(GLTexture* object, uint32_t attachmentIndex);
}

#endif