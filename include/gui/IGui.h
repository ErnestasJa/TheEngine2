#ifndef GUI_INC_H
#define GUI_INC_H

#include "imgui.h"
#include "render/IRenderContext.h"

namespace gui {
class IGui
{
public:
    virtual void BeginRender() = 0;
    virtual void EndRender()   = 0;
};

core::SharedPtr<IGui> CreateGui(core::SharedPtr<render::IRenderContext> context);
}

#endif
