#ifndef GLFWINPUTDEVICE_H
#define GLFWINPUTDEVICE_H

#include "input/IInputDevice.h"

class GLFWwindow;
class GLFWInputDevice : public core::IInputDevice
{
public:
    static core::SharedPtr<core::IInputDevice> Create(GLFWwindow* window);

public:
    GLFWInputDevice(GLFWwindow* window);
    virtual ~GLFWInputDevice();
    virtual void PollEvents(float deltaTime);
    virtual void SetInputHandler(
        const core::SharedPtr<core::InputHandler>& handler);

private:
    virtual void BindEventHandlers();

private:
    GLFWwindow* m_window;
    core::SharedPtr<core::InputHandler> m_handler;
};

#endif