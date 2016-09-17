#ifndef CFRAMEBUFFEROBJECT_HH
#define CFRAMEBUFFEROBJECT_H

namespace render {
enum class FrameBufferTarget
{
    Read = 0,
    Write,
    ReadWrite
};

struct FrameBufferObjectDescriptor
{
    FrameBufferTarget target;
};

enum class FrameBufferAttachmentTarget
{
    Color,
    Depth,
};

// struct FrameBufferAttachment
// {
//     FrameBufferAttachmentPoint attachmentPoint;
//     uint32_t mip_map_level;
//     core::SharedPtr<ITexture> texture;

//     FrameBufferAttachment() : mip_map_level(0),
//     attachmentPoint(FrameBufferAttachmentPoint::Color)
//     {
//     }
// }
}

#endif