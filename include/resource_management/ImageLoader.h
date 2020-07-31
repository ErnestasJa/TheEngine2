//
// Created by ernes on 10/6/2019.
//

#ifndef THEPROJECT2_LIBS_THEENGINE2_INCLUDE_RESOURCE_MANAGEMENT_IMAGELOADER_H_
#define THEPROJECT2_LIBS_THEENGINE2_INCLUDE_RESOURCE_MANAGEMENT_IMAGELOADER_H_

#include <render/IRenderer.h>
namespace res {
class ImageLoader
{
  private:
  public:
  ImageLoader(io::IFileSystem* fs, render::IRenderer* renderer);

  core::UniquePtr<render::ITexture> LoadTexture(const io::Path& path);
  core::UniquePtr<render::ITexture> LoadAtlasAs2DTexture(const io::Path& path,
                                                         uint32_t subImageSize);

  private:
  io::IFileSystem* m_fileSystem;
  render::IRenderer* m_renderer;
};
} // namespace res

#endif // THEPROJECT2_LIBS_THEENGINE2_INCLUDE_RESOURCE_MANAGEMENT_IMAGELOADER_H_
