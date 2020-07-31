#ifndef ITEXTURE_H
#define ITEXTURE_H

#include "CTexture.h"

namespace render {
class ITexture
{
  public:
  virtual ~ITexture()
  {
  }

  virtual void UploadData(const TextureDataDescriptor& descriptor) = 0;
  virtual uint32_t GetId() const = 0;
};
} // namespace render

#endif