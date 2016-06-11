#ifndef RESOURCE_LOADER_H
#define RESOURCE_LOADER_H

#include "filesystem/IFileSystem.h"
#include "render/RenderInc.h"
#include "third_party/stb_image.h"

struct Image {
    int32_t channels;
    core::pod::Vec2<int32_t> size;
    core::UniquePtr<uint8_t> data;
};

class ImageLoader
{
public:
    ImageLoader(core::SharedPtr<io::IFileSystem> fs,
                render::IRenderer* renderer)
        : m_fileSystem(fs), m_renderer(renderer)
    {
    }

    core::SharedPtr<render::ITexture> LoadImage(const io::Path& path)
    {
        auto file = m_fileSystem->OpenRead(path);

        if (!file) {
            printf("Failed to open %s\n", path.AsString().c_str());
            return nullptr;
        }

        core::TByteArray testByteArray;
        auto bytesRead = file->Read(testByteArray);

        printf("Image size bytes: %u\n", bytesRead);
        Image img;
        img.data = core::UniquePtr<uint8_t>(stbi_load_from_memory(
            (stbi_uc*)testByteArray.data(), bytesRead, &img.size.w, &img.size.h,
            &img.channels, STBI_rgb));

        printf("Image size x: %i\n", img.size.x);
        printf("Image size y: %i\n", img.size.y);

        auto texture = m_renderer->CreateTexture(render::TextureDescriptor());
        texture->UploadData(render::TextureDataDescriptor{
            (void*)img.data.get(), render::TextureDataFormat::RGB, img.size});

        return texture;
    }

private:
    core::SharedPtr<io::IFileSystem> m_fileSystem;
    render::IRenderer* m_renderer;
};

#endif