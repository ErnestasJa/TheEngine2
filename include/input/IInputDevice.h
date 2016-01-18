#ifndef IINPUT_DEVICE_H
#define IINPUT_DEVICE_H

namespace core
{
class IInputDevice
{
public:
    virtual ~IInputDevice(){};
    virtual void PollEvents(float deltaTime) = 0;
    virtual void SetInputHandler(const SharedPtr<InputHandler>& handler) = 0;
};
}

#endif