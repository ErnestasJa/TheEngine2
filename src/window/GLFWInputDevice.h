#ifndef GLFWINPUTDEVICE_H
#define GLFWINPUTDEVICE_H

#include "input/IInputDevice.h"

struct GLFWwindow;
class GLFWInputDevice : public input::IInputDevice
{
public:
    static core::SharedPtr<input::IInputDevice> Create(GLFWwindow* window);

public:
    GLFWInputDevice(GLFWwindow* window);
    virtual ~GLFWInputDevice();
    virtual void PollEvents(float deltaTime);
    virtual void AddInputHandler(const core::SharedPtr<input::InputHandler>& handler);
    virtual void RemoveInputHandler(const core::SharedPtr<input::InputHandler>& handler);
    const core::Vector<core::SharedPtr<input::InputHandler>>& GetInputHandlers();

private:
    virtual void BindEventHandlers();

private:
    GLFWwindow* m_window;
    core::Vector<core::SharedPtr<input::InputHandler>> m_handlers;
    core::Vector<core::SharedPtr<input::InputHandler>> m_handlersToRemove;
    core::Vector<core::SharedPtr<input::InputHandler>> m_handlersToAdd;
};

#endif
