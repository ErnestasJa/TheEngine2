#ifndef THEPROJECT2_IMAGE_H
#define THEPROJECT2_IMAGE_H

namespace render {

enum class ImageFormat
{
  RGB  = 1,
  RGBA = 2,
};

class Image
{
  public:
  Image() = delete;
  Image(const core::pod::Vec2<uint32_t>& size, ImageFormat imageFormat = ImageFormat::RGB)
  {
    m_size           = size;
    m_imageFormat    = imageFormat;
    m_componentCount = imageFormat == ImageFormat::RGBA ? 4 : 3;
    m_data       = core::UniquePtr<uint8_t[]>(new uint8_t[m_size.x * m_size.y * m_componentCount]);
    m_bufferSize = m_size.x * m_size.y * m_componentCount;
  }

  void WritePixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b)
  {
    auto pixelStart = (y * m_size.x * m_componentCount) + (x * m_componentCount);

    if (pixelStart >= m_bufferSize) {
      // todo: implement assert
      elog::LogWarning(core::string::format(
          "Failed to write pixel at [{}, {}] which was outside image bounds [{}, {}]", x, y,
          m_size.x, m_size.y));
      return;
    }

    uint8_t* data = m_data.get();

    data[pixelStart]     = r;
    data[pixelStart + 1] = g;
    data[pixelStart + 2] = b;
  }

  uint8_t* GetData() const
  {
    return m_data.get();
  }

  core::pod::Vec2<uint32_t> GetSize() const
  {
    return m_size;
  }

  private:
  core::pod::Vec2<uint32_t> m_size;
  core::UniquePtr<uint8_t[]> m_data;
  ImageFormat m_imageFormat;
  uint32_t m_bufferSize;
  uint32_t m_componentCount;
};
} // namespace render

#endif // THEPROJECT2_IMAGE_H
