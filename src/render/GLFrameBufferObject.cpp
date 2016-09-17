#include "GLFrameBufferObject.h"
#include "render/ITexture.h"

namespace render {
uint32_t GLFrameBufferObject::AttachmentCount = 8;

/// Free functions
inline uint32_t GetFBOTargetFromEnum(render::FrameBufferTarget target)
{
    switch (target) {
    case render::FrameBufferTarget::Read:
        return GL_READ_FRAMEBUFFER;
    case render::FrameBufferTarget::Write:
        return GL_DRAW_FRAMEBUFFER;
    case render::FrameBufferTarget::ReadWrite:
        return GL_FRAMEBUFFER;
    default:
        throw target;
    }
}

core::SharedPtr<IFrameBufferObject> CreateFrameBufferObject(
    const FrameBufferObjectDescriptor& descriptor)
{
    gpu_fbo_handle handle;
    handle.target = GetFBOTargetFromEnum(descriptor.target);
    glGenFramebuffers(1, &handle.id);

    if (gl::IsHandleValid(handle)) {
        return core::MakeShared<GLFrameBufferObject>(handle);
    }

    return nullptr;
}

void BindObject(GLFrameBufferObject* object, FrameBufferTarget target)
{
    if (!object) {
        glBindFramebuffer(target, handle.id);
    }
    else {
        auto handle = object->GetHandle();
        glBindFramebuffer(target, handle.id);

        uint32_t textureAttachments[GLFrameBufferObject::AttachmentCount];
        uint32_t enabledTextureCounter = 0;
        for (uint32_t i = 0; i < GLFrameBufferObject::AttachmentCount; i++) {
            if (object->m_colorTextures[i]) {
                textures[enabledTextureCounter] = i + GL_COLOR_ATTACHMENT0;
                enabledTextureCounter++;
            }

            glDrawBuffers(enabledTextureCounter, textures);
        }
    }
}

bool GetStatus(GLFrameBufferObject* object)
{
    auto handle = object->GetHandle();
    return glCheckFramebufferStatus(handle.target) == GL_FRAMEBUFFER_COMPLETE;
}


// implementation
GLFrameBufferObject::GLFrameBufferObject(const gl::gpu_fbo_handle& handle)
    : m_handle(handle)
{
}

GLFrameBufferObject::~GLFrameBufferObject()
{
    gl::FreeHandle(m_handle);
}

void GLFrameBufferObject::Attach(core::SharedPtr<ITexture> attachment,
                                 FrameBufferAttachmentTarget attachmentTarget,
                                 uint32_t attachmentPoint = 0)
{
    BindObject(this);
    auto glTexure = static_cast<render::GLTexture*>(attachment.get());
    if (attachmentTarget == FrameBufferAttachmentTarget::Color) {
        m_colorTextures[attachmentPoint] = attachment;
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentPoint + GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, glTexure->Id, level);
    }
    else if (attachmentTarget == FrameBufferAttachmentTarget::Depth) {
        m_depthTexture = attachment;
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, glTexure->Id,
                               level);
    }
    BindObject(nullptr, attachmentTarget);
}
}