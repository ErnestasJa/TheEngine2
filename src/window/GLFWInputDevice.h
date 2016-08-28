#ifndef GLFWINPUTDEVICE_H
#define GLFWINPUTDEVICE_H

#include "input/IInputDevice.h"

class GLFWwindow;
class GLFWInputDevice : public input::IInputDevice
{
public:
    static core::SharedPtr<input::IInputDevice> Create(GLFWwindow* window);

public:
    GLFWInputDevice(GLFWwindow* window);
    virtual ~GLFWInputDevice();
    virtual void PollEvents(float deltaTime);
    virtual void SetInputHandler(const core::SharedPtr<input::InputHandler>& handler);

private:
    virtual void BindEventHandlers();

private:
    GLFWwindow* m_window;
    core::SharedPtr<input::InputHandler> m_handler;
};

#endif