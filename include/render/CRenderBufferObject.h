#ifndef CRENDERBUFFEROBJECT_H
#define CRENDERBUFFEROBJECT_H

namespace render {

enum class RenderBufferObjectInternalDataFormat
{
  RGB,
  RGBA,
  DEPTH16
};

struct RenderBufferObjectDescriptor
{
  RenderBufferObjectInternalDataFormat format;
  uint32_t width, height;

  RenderBufferObjectDescriptor()
  {
  }

  RenderBufferObjectDescriptor(core::pod::Vec2<int32_t> size,
                               RenderBufferObjectInternalDataFormat rbo_format)
      : format(rbo_format)
      , width(size.w)
      , height(size.h)
  {
  }

  RenderBufferObjectDescriptor(uint32_t w, uint32_t h,
                               RenderBufferObjectInternalDataFormat rbo_format)
      : format(rbo_format)
      , width(w)
      , height(h)
  {
  }
};
} // namespace render


#endif // !CRENDERBUFFEROBJECT_H