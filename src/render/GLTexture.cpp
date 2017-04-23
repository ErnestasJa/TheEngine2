#include "GLTexture.h"

namespace render {
/// free functions
inline void SetUnpackAlignment(render::TextureDataFormat format)
{
    switch (format) {
    case render::TextureDataFormat::RGB:
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        break;

    case render::TextureDataFormat::RGBA:
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        break;

    default:
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }
}

core::SharedPtr<ITexture> GLTexture::CreateTexture(const TextureDescriptor& descriptor)
{
    auto GetType = [&]() {
        switch (descriptor.type) {
        case render::TextureType::T2D:
            return GL_TEXTURE_2D;
        case render::TextureType::T3D:
            return GL_TEXTURE_3D;
        case render::TextureType::CUBEMAP:
            return GL_TEXTURE_CUBE_MAP;
        default:
            return GL_TEXTURE_2D;
        }
    };

    auto GetFormat = [&]() {
        switch (descriptor.internalFormat) {
        case render::TextureInternalDataFormat::RGB:
            return GL_RGB;
        case render::TextureInternalDataFormat::RGBA:
            return GL_RGBA;
        case render::TextureInternalDataFormat::DEPTH32F:
            return GL_DEPTH_COMPONENT32F;
        default:
            return GL_RGBA;
        }
    };

    auto GetWrapMode = [](render::TextureWrapMode mode) {
        switch (mode) {
        case render::TextureWrapMode::REPEAT:
            return GL_REPEAT;
        default:
            return GL_REPEAT;
        }
    };

    auto GetMinFilterMode = [](render::TextureFilterMode mode) {
        switch (mode) {
        case render::TextureFilterMode::NEAREST:
            return GL_NEAREST;
        case render::TextureFilterMode::BILINEAR:
            return GL_LINEAR;
        case render::TextureFilterMode::TRILINEAR:
            return GL_LINEAR_MIPMAP_LINEAR;
        default:
            return GL_LINEAR;
        }
    };

    auto GetMagFilterMode = [](render::TextureFilterMode mode) {
        switch (mode) {
        case render::TextureFilterMode::NEAREST:
            return GL_NEAREST;
        case render::TextureFilterMode::BILINEAR:
            return GL_LINEAR;
        case render::TextureFilterMode::TRILINEAR:
            return GL_LINEAR;
        default:
            return GL_LINEAR;
        }
    };

    auto GetDataType = [&]() {
        switch (descriptor.internalFormat) {
        case render::TextureInternalDataFormat::DEPTH32F:
            return GL_FLOAT;
        default:
            return GL_UNSIGNED_BYTE;
        }
    };


    gl::gpu_texture_handle handle;
    handle.type       = GetType();
    handle.data_type  = GetDataType();
    handle.format     = GetFormat();
    handle.wrap_s     = GetWrapMode(descriptor.wrapModeS);
    handle.wrap_t     = GetWrapMode(descriptor.wrapModeT);
    handle.filter_min = GetMinFilterMode(descriptor.filterMode);
    handle.filter_mag = GetMagFilterMode(descriptor.filterMode);

    glGenTextures(1, &handle.id);
    glBindTexture(GL_TEXTURE_2D, handle.id);
    glTexParameteri(handle.type, GL_TEXTURE_WRAP_S, handle.wrap_s);
    glTexParameteri(handle.type, GL_TEXTURE_WRAP_T, handle.wrap_t);
    glTexParameteri(handle.type, GL_TEXTURE_MIN_FILTER, handle.filter_min);
    glTexParameteri(handle.type, GL_TEXTURE_MAG_FILTER, handle.filter_mag);
    glTexParameteri(handle.type, GL_TEXTURE_COMPARE_MODE, GL_NONE);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (gl::IsHandleValid(handle)) {
        return core::MakeShared<GLTexture>(handle);
    }

    return nullptr;
}

void GLTexture::BindObject(GLTexture* object, uint32_t attachmentIndex)
{
    glActiveTexture(GL_TEXTURE0 + attachmentIndex);

    if (!object) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else {
        auto handle = object->GetHandle();
        glBindTexture(GL_TEXTURE_2D, handle.id);
    }
}

// implementation
GLTexture::GLTexture(const gl::gpu_texture_handle& handle)
    : gpu_object(handle)
{
}

GLTexture::~GLTexture()
{
    glDeleteTextures(1, &m_handle.id);
}

void GLTexture::UploadData(const TextureDataDescriptor& descriptor)
{
    auto GetFormat = [&]() {
        switch (descriptor.format) {
        case render::TextureDataFormat::RGB:
            return GL_RGB;
        case render::TextureDataFormat::RGBA:
            return GL_RGBA;
        case render::TextureDataFormat::DEPTH32F:
            return GL_DEPTH_COMPONENT;
        default:
            return GL_RGBA;
        }
    };

    GLTexture::BindObject(this, 0);

    uint32_t imageFormat = GetFormat();
    glTexImage2D(m_handle.type, 0, m_handle.format, descriptor.size.x, descriptor.size.y, 0,
                 imageFormat, m_handle.data_type, descriptor.data);

    if (m_handle.filter_min == GL_LINEAR_MIPMAP_LINEAR) {
        glGenerateMipmap(m_handle.type);
    }

    GLTexture::BindObject(nullptr, 0);
}
}
