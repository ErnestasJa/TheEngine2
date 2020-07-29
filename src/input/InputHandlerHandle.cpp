#include "input/InputHandlerHandle.h"
#include "input/IInputDevice.h"

namespace input {
InputHandlerHandle::InputHandlerHandle(input::IInputDevice* device, InputHandler* handler)
    : m_device(device)
    , m_handler(handler)
{
}

void InputHandlerHandle::Disconnect() {
    if(m_device && m_handler)
    m_device->RemoveInputHandler(m_handler);
}
} // namespace input
