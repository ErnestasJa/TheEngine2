#ifndef GLRENDERERDEBUGMESSAGEMONITOR_H
#define GLRENDERERDEBUGMESSAGEMONITOR_H

#include "render/IRendererDebugMessageMonitor.h"

namespace render {
class GLRendererDebugMessageMonitor : public IRendererDebugMessageMonitor
{
  public:
  static bool IsDebugOutputSupported();

  public:
  GLRendererDebugMessageMonitor();
  virtual ~GLRendererDebugMessageMonitor();
  bool AddMessage(const core::SharedPtr<IRendererDebugMessage>& message);
  void ClearMessages();
  void SetDebugging(bool enabled);
  bool isDebuggingEnabled();
  const core::Vector<core::SharedPtr<IRendererDebugMessage>>& GetMessages() const;

  private:
  core::Vector<core::SharedPtr<IRendererDebugMessage>> m_debugMessages;
};

core::UniquePtr<GLRendererDebugMessageMonitor> CreateRendererDebugMessageMonitor();
} // namespace render

#endif