#ifndef GLRENDERERRROR_H
#define GLRENDERERRROR_H

#include "render/IRendererDebugMessage.h"

namespace render {
class GLRendererDebugMessage : public IRendererDebugMessage
{
  public:
  GLRendererDebugMessage(const core::String& message);
  virtual ~GLRendererDebugMessage();

  virtual core::String AsString() const;

  private:
  core::String m_message;
};
} // namespace render

#endif