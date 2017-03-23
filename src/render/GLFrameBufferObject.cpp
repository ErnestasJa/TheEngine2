#include "GLFrameBufferObject.h"
#include "GLRenderBufferObject.h"
#include "GLTexture.h"
#include "log/LogInc.h"

namespace render {

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

core::SharedPtr<IFrameBufferObject> GLFrameBufferObject::CreateFrameBufferObject(
    const FrameBufferObjectDescriptor& descriptor)
{
    gl::gpu_fbo_handle handle;
    handle.target = GetFBOTargetFromEnum(descriptor.target);
    glGenFramebuffers(1, &handle.id);

    if (gl::IsHandleValid(handle)) {
        return core::MakeShared<GLFrameBufferObject>(handle);
    }

    return nullptr;
}

void EnableTextures(const core::SharedPtr<ITexture> textures[8])
{
    uint32_t textureAttachments[GLFrameBufferObject::AttachmentCount];
    uint32_t enabledTextureCounter = 0;
    for (uint32_t i = 0; i < GLFrameBufferObject::AttachmentCount; i++) {
        if (textures[i]) {
            textureAttachments[enabledTextureCounter] = i + GL_COLOR_ATTACHMENT0;
            enabledTextureCounter++;
        }
    }
    glDrawBuffers(enabledTextureCounter, textureAttachments);
}

void GLFrameBufferObject::BindObject(GLFrameBufferObject* object, FrameBufferTarget target)
{
    uint32_t fboTarget = GetFBOTargetFromEnum(target);

    if (!object) {
        glBindFramebuffer(fboTarget, 0);
        elog::LogInfo("Unbinding FBO");
    }
    else {
        elog::LogInfo("Binding FBO");
        object->m_handle.target = fboTarget;
        glBindFramebuffer(fboTarget, object->m_handle.id);
        EnableTextures(object->m_colorTextures);
    }
}


// implementation
GLFrameBufferObject::GLFrameBufferObject(const gl::gpu_fbo_handle& handle)
    : gpu_object(handle)
{
}

GLFrameBufferObject::~GLFrameBufferObject()
{
    glDeleteFramebuffers(1, &m_handle.id);
}

void GLFrameBufferObject::Attach(core::SharedPtr<ITexture> attachment,
                                 FrameBufferAttachmentTarget attachmentTarget,
                                 uint32_t attachmentPoint)
{
    glBindFramebuffer(m_handle.target, m_handle.id);

    auto glTexure = static_cast<render::GLTexture*>(attachment.get());

    if (attachmentTarget == FrameBufferAttachmentTarget::Color) {
        if (attachmentPoint == 0) {
            m_renderBufferAttachment = nullptr;
        }

        m_colorTextures[attachmentPoint] = attachment;
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentPoint + GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, glTexure->GetHandle().id, 0);
    }
    else if (attachmentTarget == FrameBufferAttachmentTarget::Depth) {
        m_renderBufferAttachment = nullptr;
        m_depthTexture           = attachment;
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                               glTexure->GetHandle().id, 0);
    }

    glBindFramebuffer(m_handle.target, 0);
}


void GLFrameBufferObject::Attach(core::SharedPtr<IRenderBufferObject> attachment,
                                 FrameBufferAttachmentTarget attachmentTarget)
{
    glBindFramebuffer(m_handle.target, m_handle.id);

    auto rbo = static_cast<render::GLRenderBufferObject*>(attachment.get());

    if (attachmentTarget == FrameBufferAttachmentTarget::Color) {
        m_colorTextures[0]       = nullptr;
        m_renderBufferAttachment = attachment;

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER,
                               rbo->GetHandle().id, 0);
    }
    else if (attachmentTarget == FrameBufferAttachmentTarget::Depth) {
        m_depthTexture           = nullptr;
        m_renderBufferAttachment = attachment;
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,
                               rbo->GetHandle().id, 0);
    }

    glBindFramebuffer(m_handle.target, 0);
}


bool GLFrameBufferObject::GetStatus()
{
    return glCheckFramebufferStatus(m_handle.target) == GL_FRAMEBUFFER_COMPLETE;
}
}