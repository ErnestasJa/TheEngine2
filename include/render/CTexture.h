#ifndef CTEXTURE_H
#define CTEXTURE_H

namespace render {
enum class TextureFilterMode
{
    NEAREST,
    BILINEAR,
    TRILINEAR
};
enum class TextureDataFormat
{
    RGB,
    RGBA,
    DEPTH32F
};
enum class TextureWrapMode
{
    REPEAT
};
enum class TextureType
{
    T2D,
    T3D,
    CUBEMAP,
    T2DArray
};

struct TextureDescriptor
{
    TextureType type;
    TextureDataFormat DataFormat;
    TextureFilterMode filterMode;
    TextureWrapMode wrapModeS;
    TextureWrapMode wrapModeT;
    uint32_t width;
    uint32_t height;
    uint32_t layerCount;

    TextureDescriptor()
        : type(TextureType::T2D)
        , DataFormat(TextureDataFormat::RGBA)
        , filterMode(TextureFilterMode::BILINEAR)
        , wrapModeS(TextureWrapMode::REPEAT)
        , wrapModeT(TextureWrapMode::REPEAT)
        , layerCount(1)
        , width(0)
        , height(0)
    {
    }
};

struct TextureDataDescriptor
{
    void* data;
    TextureDataFormat format;
    core::pod::Vec2<int32_t> size;
};

}

#endif