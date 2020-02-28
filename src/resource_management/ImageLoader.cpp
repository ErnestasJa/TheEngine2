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

inline render::TextureDataFormat GetImageDataFormat(const Image & img){
  return img.channels == 3 ? render::TextureDataFormat::RGB : render::TextureDataFormat::RGBA;
}

core::UniquePtr<render::ITexture> ImageLoader::LoadTexture(const io::Path& path)
{
    auto img = LoadImage(path);

    if(!img.data){
        return nullptr;
    }

    render::TextureDescriptor desc;
    desc.filterMode = render::TextureFilterMode::NEAREST;
    desc.DataFormat = GetImageDataFormat(img);
    auto texture    = m_renderer->CreateTexture(desc);
    texture->UploadData(render::TextureDataDescriptor{
        (void*)img.data.get(), desc.DataFormat, img.size });

    return texture;
}

core::UniquePtr<uint8_t> PrepareImageAtlasData(uint8_t* originalData, int originalWidth, int originalHeight, int numChannels, int subImageSize){
    auto dataPtr = core::UniquePtr<uint8_t>(new uint8_t[originalWidth*originalHeight*numChannels]);
    uint8_t* data = dataPtr.get();

    if(originalWidth % subImageSize != 0 || originalHeight % subImageSize != 0){
        elog::LogInfo(core::string::format("Could not create 2d array texture image,"
                                           " one of the dimensions is invalid. "
                                           "Image size: {}x{}, Requested sub-image size: {}x{}"
                                           , originalWidth, originalHeight, subImageSize, subImageSize));
        return nullptr;
    }

    int columns = originalWidth / subImageSize;
    int rows = originalHeight / subImageSize;
    int totalSubImages = columns * rows;
    int rowOffset = originalWidth * numChannels;

    auto getImageStart = [&](int x, int y) -> int {
        return (y * subImageSize *rowOffset) + (x * numChannels * subImageSize);
    };



    for(int32_t row = 0; row < rows; row++) {
        for (int32_t col = 0; col < columns; col++) {
            int start = getImageStart(col, row);

            for (int y = 0; y < subImageSize; y++) {
                for (int x = 0; x < subImageSize; x++) {
                    for (int c = 0; c < numChannels; c++) {
                         *data = originalData[start + x * numChannels + c];
                         data++;
                    }
                }
                start += rowOffset;
            }
        }
    }

    return dataPtr;
}

core::UniquePtr<render::ITexture> ImageLoader::LoadAtlasAs2DTexture(const io::Path& path,
                                                                    uint32_t subImageSize)
{
    auto img = LoadImage(path);

    if(!img.data){
        return nullptr;
    }

    auto dataForUpload = PrepareImageAtlasData(img.data.get(), img.size.x, img.size.y, img.channels, subImageSize);

    if(!dataForUpload) {
        return nullptr;
    }

    render::TextureDescriptor desc;
    desc.filterMode = render::TextureFilterMode::NEAREST;
    desc.width = subImageSize;
    desc.height = subImageSize;
    desc.layerCount = (img.size.x / subImageSize) * (img.size.y / subImageSize);
    desc.type = render::TextureType::T2DArray;
    desc.DataFormat = GetImageDataFormat(img);

    auto texture    = m_renderer->CreateTexture(desc);
    texture->UploadData(render::TextureDataDescriptor{
        (void*)dataForUpload.get(), desc.DataFormat, img.size });

    return texture;
}

Image ImageLoader::LoadImage(const io::Path& path)
{
    auto file = m_fileSystem->OpenRead(path);

    if (!file) {
        elog::LogInfo(core::string::format("Failed to open {}\n", path.AsString().c_str()));
        return Image();
    }

    core::TByteArray testByteArray;
    auto bytesRead = file->Read(testByteArray);

    elog::LogInfo(core::string::format("Image size bytes: {}\n", bytesRead));
    Image img;
    img.data = core::UniquePtr<uint8_t>(
        stbi_load_from_memory((stbi_uc*)testByteArray.data(), bytesRead, &img.size.w,
                              &img.size.h, &img.channels, 0));

    elog::LogInfo(core::string::format("Image size x: {}\n", img.size.x));
    elog::LogInfo(core::string::format("Image size y: {}\n", img.size.y));

    return img;
}

}