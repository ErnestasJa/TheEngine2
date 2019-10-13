#ifndef GUI_IMPL_H
#define GUI_IMPL_H
#include "gui/IGui.h"

namespace gui {
class GuiImpl : public IGui
{
public:
    GuiImpl(core::SharedPtr<engine::IEngineContext> context);
    virtual ~GuiImpl();
    void BeginRender();
    void EndRender();

private:
    core::SharedPtr<engine::IEngineContext> m_context;
};
}

#endif
