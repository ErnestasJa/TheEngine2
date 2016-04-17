#ifndef CTEXTURE_H
#define CTEXTURE_H

namespace render
{
enum class TextureFilterMode { NEAREST, BILINEAR, TRILINEAR };
enum class TextureDataFormat { RGB, RGBA };
enum class TextureInternalDataFormat { RGB, RGBA };
enum class TextureWrapMode { REPEAT };
enum class TextureType { T2D, T3D, CUBEMAP };

struct TextureDescriptor {
    TextureType type;
    TextureInternalDataFormat internalFormat;
    TextureFilterMode filterMode;
    TextureWrapMode wrapModeS;
    TextureWrapMode wrapModeT;

    TextureDescriptor()
        : type(TextureType::T2D),
          internalFormat(TextureInternalDataFormat::RGBA),
          filterMode(TextureFilterMode::BILINEAR),
          wrapModeS(TextureWrapMode::REPEAT),
          wrapModeT(TextureWrapMode::REPEAT)
    {
    }
};

struct TextureDataDescriptor {
    void *data;
    TextureDataFormat format;
    core::pod::Vec2<int32_t> size;
};
}

#endif