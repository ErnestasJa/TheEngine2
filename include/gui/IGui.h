#ifndef GUI_INC_H
#define GUI_INC_H

#include "engine/IEngineContext.h"
#include "imgui.h"

namespace gui {
class IGui
{
  public:
  virtual void BeginRender() = 0;
  virtual void EndRender()   = 0;
};

core::UniquePtr<IGui> CreateGui(core::SharedPtr<engine::IEngineContext> context);
} // namespace gui

#endif
