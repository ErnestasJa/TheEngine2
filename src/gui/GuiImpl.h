#ifndef GUI_IMPL_H
#define GUI_IMPL_H
#include "gui/IGui.h"

namespace gui {
class GuiImpl : public IGui
{
public:
    GuiImpl(core::SharedPtr<render::IRenderContext> context);
    virtual ~GuiImpl();
    void BeginRender();
    void EndRender();

private:
    core::SharedPtr<render::IRenderContext> m_context;
};
}

#endif
