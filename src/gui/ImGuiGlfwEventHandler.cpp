#ifndef IMGUI_GLFW_EVENT_HANDLER_IMPL_
#define IMGUI_GLFW_EVENT_HANDLER_IMPL_

#include "input/InputInc.h"

class ImGuiEventHandler : public input::InputHandler
{
  public:
  static auto Create()
  {
    return core::MakeShared<ImGuiEventHandler>();
  }

  public:
  virtual ~ImGuiEventHandler()
  {
  }

  virtual bool OnMouseDown(const input::MouseButton& key)
  {
    /*if (key == input::MouseButtons::Left)
        g_MousePressed[0] = true;
    if (key == input::MouseButtons::Right)
        g_MousePressed[2] = true;*/
    return false;
  }

  virtual bool OnMouseWheel(const float delta)
  {
    /*g_MouseWheel += delta;*/
    return false;
  }
};


core::SharedPtr<input::InputHandler> CreateImGuiInputHandler()
{
  return ImGuiEventHandler::Create();
}
#endif