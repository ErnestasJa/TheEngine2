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

core::UniquePtr<ITexture> GLTexture::CreateTexture(const TextureDescriptor& descriptor)
{
    auto GetType = [&]() {
        switch (descriptor.type) {
        case render::TextureType::T2D:
            return GL_TEXTURE_2D;
        case render::TextureType::T3D:
            return GL_TEXTURE_3D;
        case render::TextureType::CUBEMAP:
            return GL_TEXTURE_CUBE_MAP;
        case render::TextureType::T2DArray:
            return GL_TEXTURE_2D_ARRAY;
        default:
            return GL_TEXTURE_2D;
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
        switch (descriptor.DataFormat) {
        case render::TextureDataFormat::DEPTH32F:
            return GL_FLOAT;
        default:
            return GL_UNSIGNED_BYTE;
        }
    };


    gl::gpu_texture_handle handle;
    handle.type       = GetType();
    handle.data_type  = GetDataType();
    handle.format     = gl::GetBaseTextureFormat(descriptor.DataFormat);
    handle.sizedFormat     = gl::GetSizedTextureFormat(descriptor.DataFormat);
    handle.wrap_s     = GetWrapMode(descriptor.wrapModeS);
    handle.wrap_t     = GetWrapMode(descriptor.wrapModeT);
    handle.filter_min = GetMinFilterMode(descriptor.filterMode);
    handle.filter_mag = GetMagFilterMode(descriptor.filterMode);
    handle.width = descriptor.width;
    handle.height = descriptor.height;
    handle.layerCount = descriptor.layerCount;



    glGenTextures(1, &handle.id);
    glBindTexture(handle.type, handle.id);

    if(handle.type == GL_TEXTURE_2D_ARRAY)
    {
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, handle.sizedFormat, handle.width, handle.height, handle.layerCount);
    }

    glTexParameteri(handle.type, GL_TEXTURE_WRAP_S, handle.wrap_s);
    glTexParameteri(handle.type, GL_TEXTURE_WRAP_T, handle.wrap_t);
    glTexParameteri(handle.type, GL_TEXTURE_MIN_FILTER, handle.filter_min);
    glTexParameteri(handle.type, GL_TEXTURE_MAG_FILTER, handle.filter_mag);
    glTexParameteri(handle.type, GL_TEXTURE_COMPARE_MODE, GL_NONE);
    glBindTexture(handle.type, 0);

    if (gl::IsHandleValid(handle)) {
        return core::MakeUnique<GLTexture>(handle);
    }

    return nullptr;
}

void GLTexture::BindObject(GLTexture* object, uint32_t attachmentIndex)
{
    glActiveTexture(GL_TEXTURE0 + attachmentIndex);

    ///todo: sort out unbind logic when texture type is not GL_TEXTURE_2D
    if (!object) {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else {
        auto handle = object->GetHandle();
        glBindTexture(handle.type, handle.id);
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
    GLTexture::BindObject(this, 0);


    if(m_handle.type == GL_TEXTURE_2D_ARRAY) {
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, m_handle.width, m_handle.height,
            m_handle.layerCount, m_handle.format, m_handle.data_type, descriptor.data);
    }
    else {
        glTexImage2D(m_handle.type, 0, m_handle.sizedFormat, descriptor.size.x, descriptor.size.y, 0,
                     m_handle.format, m_handle.data_type, descriptor.data);

        if (m_handle.filter_min == GL_LINEAR_MIPMAP_LINEAR) {
            glGenerateMipmap(m_handle.type);
        }
    }

    GLTexture::BindObject(nullptr, 0);
}
}
