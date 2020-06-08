#include "input/InputHandlerHandle.h"
#include "input/IInputDevice.h"

namespace input {
InputHandlerHandle::InputHandlerHandle(input::IInputDevice* device, InputHandler* handler)
    : m_device(device)
    , m_handler(handler)
{
  m_handleCount =
      core::SharedPtr<int>(0, [&device, &handler](auto p) { device->RemoveInputHandler(handler); });
}
} // namespace input
