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
    struct Image
    {
        int32_t channels;
        core::pod::Vec2<int32_t> size;
        core::UniquePtr<uint8_t> data;
    };

public:
    ImageLoader(io::IFileSystem* fs, render::IRenderer* renderer);

    core::SharedPtr<render::ITexture> LoadImage(const io::Path& path);

private:
    io::IFileSystem* m_fileSystem;
    render::IRenderer* m_renderer;
};
}

#endif // THEPROJECT2_LIBS_THEENGINE2_INCLUDE_RESOURCE_MANAGEMENT_IMAGELOADER_H_
