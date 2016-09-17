#ifndef IFRAMEBUFFEROBJECT_H
#define IFRAMEBUFFEROBJECT_H

#include "CFrameBufferObject.h"

namespace render {

class ITexture;
class IFrameBufferObject
{
public:
    virtual ~IFrameBufferObject() = default;

    virtual void Attach(
        core::SharedPtr<ITexture> attachment,
        FrameBufferAttachmentTarget attachmentTarget = FrameBufferAttachmentTarget::Color,
        uint32_t attachmentPoint = 0) = 0;
};

core::SharedPtr<IFrameBufferObject> CreateFrameBufferObject(
    const FrameBufferObjectDescriptor& descriptor);
}

#endif