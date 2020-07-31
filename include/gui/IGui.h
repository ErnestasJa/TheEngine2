#ifndef GUI_INC_H
#define GUI_INC_H

#include "engine/IEngineContext.h"
#include "imgui.h"

namespace gui {
class IGui
{
  public:
  virtual ~IGui()            = default;
  virtual void BeginRender() = 0;
  virtual void EndRender()   = 0;
};

core::UniquePtr<IGui> CreateGui(engine::IEngineContext* context);
} // namespace gui

#endif
