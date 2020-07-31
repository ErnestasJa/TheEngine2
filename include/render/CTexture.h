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

  TextureDescriptor(uint32_t texWidth = 0, uint32_t texHeight = 0, TextureDataFormat dataFormat = TextureDataFormat::RGBA)
      : type(TextureType::T2D)
      , DataFormat(dataFormat)
      , filterMode(TextureFilterMode::BILINEAR)
      , wrapModeS(TextureWrapMode::REPEAT)
      , wrapModeT(TextureWrapMode::REPEAT)
      , layerCount(1)
      , width(texWidth)
      , height(texHeight)
  {
  }
};

struct TextureDataDescriptor
{
  TextureDataDescriptor() = default;
  TextureDataDescriptor(void* texData, core::pod::Vec2<uint32_t> texSize,
                        TextureDataFormat texFormat = TextureDataFormat::RGB)
  {
    data   = texData;
    size.x   = texSize.x;
    size.y   = texSize.y;
    format = texFormat;
  }
  void* data;
  TextureDataFormat format;
  core::pod::Vec2<int32_t> size;
};

} // namespace render

#endif