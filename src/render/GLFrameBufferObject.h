#ifndef GLFRAMEBUFFEROBJECT_H
#define GLFRAMEBUFFEROBJECT_H

#include "IFrameBufferObject.h"
#include "OpenGL.hpp"

namespace render {

struct gpu_fbo_handle : gpu_handle
{
    uint32_t target;
};

class GLFrameBufferObject : IFrameBufferObject, gl::gpu_object<gpu_fbo_handle>
{
public:
    static const uint32_t AttachmentCount;

public:
    GLFrameBufferObject(const gl::gpu_fbo_handle& handle);
    ~GLFrameBufferObject();

    void Attach(core::SharedPtr<ITexture> attachment,
                FrameBufferAttachmentTarget attachmentTarget = FrameBufferAttachmentTarget::Color,
                uint32_t attachmentPoint                     = 0);

    friend void BindObject(GLFrameBufferObject*, FrameBufferTarget);

private:
    core::SharedPtr<ITexture> m_depthTexture;
    core::SharedPtr<ITexture> m_colorTextures[GLFrameBufferObject::AttachmentCount];
};

void BindObject(GLFrameBufferObject* object, FrameBufferTarget target);
bool GetStatus(GLFrameBufferObject* object);
}


#endif