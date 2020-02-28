#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include "OpenGL.hpp"
#include "render/ITexture.h"

namespace render {
namespace gl {
struct gpu_texture_handle
{
    uint32_t id;
    uint32_t type;
    uint32_t data_type;
    uint32_t format;
    uint32_t wrap_s;
    uint32_t wrap_t;
    uint32_t filter_min;
    uint32_t filter_mag;
    uint32_t layerCount;
    uint32_t width;
    uint32_t height;
    uint32_t sizedFormat;
};
}

class GLTexture : public ITexture, public gl::gpu_object<gl::gpu_texture_handle>
{
public:
    static core::UniquePtr<ITexture> CreateTexture(const TextureDescriptor& descriptor);
    static void BindObject(GLTexture* object, uint32_t attachmentIndex);

public:
    GLTexture(const gl::gpu_texture_handle& handle);
    virtual ~GLTexture();
    virtual void UploadData(const TextureDataDescriptor& descriptor);
private:
};
}

#endif