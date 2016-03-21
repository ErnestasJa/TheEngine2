#include "GLRendererDebugMessageMonitor.h"
#include "GLRendererDebugMessage.h"
#include "render/OpenGL.h"
#include <iostream>

namespace render
{
static core::WeakPtr<IRendererDebugMessageMonitor> Instance;

core::SharedPtr<IRendererDebugMessageMonitor> GetRendererDebugMessageMonitor()
{
    if (GLRendererDebugMessageMonitor::IsDebugOutputSupported()) {
        auto instance = Instance.lock();
        if (!instance) {
            Instance = instance =
                core::MakeShared<GLRendererDebugMessageMonitor>();
        }
        return instance;
    }

    return nullptr;
}

bool GLRendererDebugMessageMonitor::IsDebugOutputSupported()
{
    int32_t value = 0;
    glGetIntegerv(GL_CONTEXT_FLAGS, &value);
    return value & GL_CONTEXT_FLAG_DEBUG_BIT;
}

void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id,
                            GLenum severity, GLsizei length,
                            const GLchar *message, const void *userParam)
{
    auto monitor = (GLRendererDebugMessageMonitor *)userParam;
    auto msg = core::MakeShared<GLRendererDebugMessage>(core::String(message));
    monitor->AddMessage(msg);
}

GLRendererDebugMessageMonitor::GLRendererDebugMessageMonitor()
{
}

GLRendererDebugMessageMonitor::~GLRendererDebugMessageMonitor()
{
    SetDebugging(false);
}

bool GLRendererDebugMessageMonitor::AddMessage(
    const core::SharedPtr<IRendererDebugMessage> &message)
{
    m_debugMessages.push_back(message);
}

void GLRendererDebugMessageMonitor::ClearMessages()
{
    m_debugMessages.clear();
}

void GLRendererDebugMessageMonitor::SetDebugging(bool enabled)
{
    if (enabled) {
        glDebugMessageCallback(debugCallback, this);
        glEnable(GL_DEBUG_OUTPUT);
        uint32_t unusedIds = 0;
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0,
                              &unusedIds, true);
    } else {
        glDisable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(nullptr, nullptr);
    }
}

bool GLRendererDebugMessageMonitor::isDebuggingEnabled()
{
    return glIsEnabled(GL_DEBUG_OUTPUT);
}

const core::Vector<core::SharedPtr<IRendererDebugMessage>>
    &GLRendererDebugMessageMonitor::GetMessages() const
{
    return m_debugMessages;
}
}