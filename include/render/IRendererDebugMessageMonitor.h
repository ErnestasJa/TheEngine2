#ifndef IRENDERERDEBUGMESSAGEMONITOR_H
#define IRENDERERDEBUGMESSAGEMONITOR_H

#include "IRendererDebugMessage.h"

namespace render
{
class IRendererDebugMessageMonitor
{
public:
    virtual ~IRendererDebugMessageMonitor()
    {
    }

    virtual void ClearMessages() = 0;
    virtual void SetDebugging(bool enabled) = 0;
    virtual bool isDebuggingEnabled() = 0;
    virtual const core::Vector<core::SharedPtr<IRendererDebugMessage>>&
    GetMessages() const = 0;
};
}

#endif