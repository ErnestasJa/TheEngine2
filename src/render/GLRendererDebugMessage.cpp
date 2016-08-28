#include "GLRendererDebugMessage.h"

namespace render {
GLRendererDebugMessage::GLRendererDebugMessage(const core::String& message) : m_message(message)
{
}

GLRendererDebugMessage::~GLRendererDebugMessage()
{
}

core::String GLRendererDebugMessage::AsString() const
{
    return m_message;
}
}