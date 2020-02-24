#ifndef GLFRAMEBUFFEROBJECT_H
#define GLFRAMEBUFFEROBJECT_H

#include "OpenGL.hpp"
#include "render/IFrameBufferObject.h"

namespace render {
namespace gl {
struct gpu_fbo_handle
{
    uint32_t id;
    uint32_t target;
};
}

class GLFrameBufferObject : public IFrameBufferObject, public gl::gpu_object<gl::gpu_fbo_handle>
{
public:
    static const uint32_t AttachmentCount = 8;
    friend void BindObject(GLFrameBufferObject* object, FrameBufferTarget target);
    static void BindObject(GLFrameBufferObject* object, FrameBufferTarget target);
    static core::UniquePtr<IFrameBufferObject> CreateFrameBufferObject(
        const FrameBufferObjectDescriptor& descriptor);

public:
    GLFrameBufferObject(const gl::gpu_fbo_handle& handle);
    virtual ~GLFrameBufferObject();

    virtual void Attach(
        core::SharedPtr<ITexture> attachment,
        FrameBufferAttachmentTarget attachmentTarget = FrameBufferAttachmentTarget::Color,
        uint32_t attachmentPoint                     = 0);

    virtual void Attach(
        core::SharedPtr<IRenderBufferObject> attachment,
        FrameBufferAttachmentTarget attachmentTarget = FrameBufferAttachmentTarget::Color);


    virtual bool GetStatus();


private:
    core::SharedPtr<ITexture> m_depthTexture;
    core::SharedPtr<ITexture> m_colorTextures[GLFrameBufferObject::AttachmentCount];
    core::SharedPtr<IRenderBufferObject> m_renderBufferAttachment;
};
}

#endif