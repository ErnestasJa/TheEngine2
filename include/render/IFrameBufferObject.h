#ifndef IFRAMEBUFFEROBJECT_H
#define IFRAMEBUFFEROBJECT_H

#include "CFrameBufferObject.h"

namespace render {

class ITexture;
class IRenderBufferObject;
class IFrameBufferObject
{
public:
    virtual ~IFrameBufferObject() = default;

    virtual void Attach(
        core::SharedPtr<ITexture> attachment,
        FrameBufferAttachmentTarget attachmentTarget = FrameBufferAttachmentTarget::Color,
        uint32_t attachmentPoint = 0) = 0;

    virtual void Attach(
        core::SharedPtr<IRenderBufferObject> attachment,
        FrameBufferAttachmentTarget attachmentTarget = FrameBufferAttachmentTarget::Color) = 0;

    virtual bool GetStatus() = 0;
};
}

#endif