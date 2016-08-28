#ifndef IRENDERERDEBUGMESSAGE_H
#define IRENDERERDEBUGMESSAGE_H

namespace render {
class IRendererDebugMessage
{
public:
    virtual ~IRendererDebugMessage()
    {
    }

    virtual core::String AsString() const = 0;
};
}

#endif