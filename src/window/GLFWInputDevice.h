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
    virtual input::InputHandlerHandle AddInputHandler(input::InputHandler* handler);
    virtual void RemoveInputHandler(input::InputHandler* handler);
    const core::Vector<input::InputHandler*>& GetInputHandlers();

private:
    virtual void BindEventHandlers();

private:
    GLFWwindow* m_window;
    core::Vector<input::InputHandler*> m_handlers;
    core::Vector<input::InputHandler*> m_handlersToRemove;
    core::Vector<input::InputHandler*> m_handlersToAdd;
};

#endif
