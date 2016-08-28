#ifndef IINPUT_DEVICE_H
#define IINPUT_DEVICE_H

namespace input {
class InputHandler;
class IInputDevice
{
public:
    virtual ~IInputDevice(){};
    virtual void PollEvents(float deltaTime)                                   = 0;
    virtual void SetInputHandler(const core::SharedPtr<InputHandler>& handler) = 0;
};
}

#endif