//
// Created by ernes on 10/6/2019.
//

#include "resource_management/ImageLoader.h"
#include "filesystem/IFileSystem.h"
#include "render/ITexture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
namespace res {

ImageLoader::ImageLoader(io::IFileSystem* fs, render::IRenderer* renderer)
    : m_fileSystem(fs)
    , m_renderer(renderer)
{
}
core::SharedPtr<render::ITexture> ImageLoader::LoadImage(const io::Path& path)
{
    auto file = m_fileSystem->OpenRead(path);

    if (!file) {
        elog::LogInfo(core::string::format("Failed to open {}\n", path.AsString().c_str()));
        return nullptr;
    }

    core::TByteArray testByteArray;
    auto bytesRead = file->Read(testByteArray);

    elog::LogInfo(core::string::format("Image size bytes: {}\n", bytesRead));
    Image img;
    img.data = core::UniquePtr<uint8_t>(
        stbi_load_from_memory((stbi_uc*)testByteArray.data(), bytesRead, &img.size.w,
                              &img.size.h, &img.channels, STBI_rgb));

    elog::LogInfo(core::string::format("Image size x: {}\n", img.size.x));
    elog::LogInfo(core::string::format("Image size y: {}\n", img.size.y));

    render::TextureDescriptor desc;
    desc.filterMode = render::TextureFilterMode::TRILINEAR;
    auto texture    = m_renderer->CreateTexture(desc);
    texture->UploadData(render::TextureDataDescriptor{
        (void*)img.data.get(), render::TextureDataFormat::RGB, img.size });

    return texture;
}

}