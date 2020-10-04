#ifndef THEPROJECT2_STUBDEBUGMESSAGEMONITOR_H
#define THEPROJECT2_STUBDEBUGMESSAGEMONITOR_H

#include "render/IRendererDebugMessageMonitor.h"

namespace render {
class StubDebugMessageMonitor: public IRendererDebugMessageMonitor {
  public:
  StubDebugMessageMonitor(){}
  virtual ~StubDebugMessageMonitor(){}

  virtual void ClearMessages() {}
  virtual void SetDebugging(bool enabled){}

  virtual bool isDebuggingEnabled() { return false; }
  virtual const core::Vector<core::SharedPtr<IRendererDebugMessage>>& GetMessages() const {  return m_messages; }

  private:
      core::Vector<core::SharedPtr<IRendererDebugMessage>> m_messages;
};
}

#endif // THEPROJECT2_STUBDEBUGMESSAGEMONITOR_H
