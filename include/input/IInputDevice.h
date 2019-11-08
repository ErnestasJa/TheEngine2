#ifndef IINPUT_DEVICE_H
#define IINPUT_DEVICE_H
#include "InputHandlerHandle.h"

namespace input {
class InputHandler;

class IInputDevice
{
public:
    virtual ~IInputDevice(){};
    virtual void PollEvents(float deltaTime)                                      = 0;
    virtual InputHandlerHandle AddInputHandler(InputHandler* handler)    = 0;
    virtual void RemoveInputHandler(InputHandler* handler) = 0;
};
}

#endif
