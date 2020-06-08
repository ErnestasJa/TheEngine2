#ifndef IWINDOW_H
#define IWINDOW_H

#include "input/IInputDevice.h"

// TODO: this should be window namespace

namespace render {
enum class CursorMode
{
  Normal,
  Hidden,
  HiddenCapture
};

class IWindow
{
  public:
  virtual ~IWindow()
  {
  }

  virtual core::pod::Vec2<int32_t> GetDimensions()                       = 0;
  virtual void SetDimensions(const core::pod::Vec2<int32_t>& dimensions) = 0;
  virtual core::pod::Vec2<int32_t> GetPosition()                         = 0;
  virtual void SetPosition(const core::pod::Vec2<int32_t>& position)     = 0;
  virtual bool PollEvents()                                              = 0;
  virtual void SwapBuffers()                                             = 0;
  virtual bool ShouldClose()                                             = 0;
  virtual void Close()                                                   = 0;
  virtual input::IInputDevice* GetInputDevice()                          = 0;
  virtual void SetCursorMode(CursorMode cursorMode)                      = 0;
};
} // namespace render

#endif