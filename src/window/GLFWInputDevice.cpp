#include "GLFWInputDevice.h"
#include "GLFW/glfw3.h"
#include "input/InputHandler.h"

namespace
{
struct SWindowUserData {
    core::WeakPtr<core::InputHandler> inputHandler;

    static void Bind(GLFWwindow* window,
                     core::WeakPtr<core::InputHandler> inputHandler)
    {
        glfwSetWindowUserPointer(window, new SWindowUserData{inputHandler});
    };

    static void Unbind(GLFWwindow* window)
    {
        auto data = Get(window);
        if (data) delete data;
        glfwSetWindowUserPointer(window, nullptr);
    };

    static SWindowUserData* Get(GLFWwindow* window)
    {
        return (SWindowUserData*)glfwGetWindowUserPointer(window);
    }

    static core::SharedPtr<core::InputHandler> GetInputHandler(
        GLFWwindow* window)
    {
        auto ud = Get(window);
        if (!ud) return nullptr;
        return ud->inputHandler.lock();
    }
};
}

core::SharedPtr<core::IInputDevice> GLFWInputDevice::Create(GLFWwindow* window)
{
    auto ptr = core::MakeShared<GLFWInputDevice>(window);
    ptr->BindEventHandlers();
    return ptr;
}

GLFWInputDevice::GLFWInputDevice(GLFWwindow* window) : m_window(window)
{
}

GLFWInputDevice::~GLFWInputDevice()
{
    SWindowUserData::Unbind(m_window);
}

void GLFWInputDevice::PollEvents(float deltaTime)
{
    // we might be doing double event polling
    // maybe skip it here or check if they were polled for window.
    glfwPollEvents();
}

void GLFWInputDevice::SetInputHandler(
    const core::SharedPtr<core::InputHandler>& handler)
{
    m_handler = handler;
    SWindowUserData::Unbind(m_window);
    SWindowUserData::Bind(m_window, m_handler);
}

void GLFWInputDevice::BindEventHandlers()
{
    auto keyHandler = [](GLFWwindow* window, int key, int scancode, int action,
                         int mods) -> void {
        auto handler = SWindowUserData::GetInputHandler(window);
        if (!handler) return;

        switch (action) {
            case GLFW_PRESS:
                handler->OnKeyDown(key, scancode, false);
                break;
            case GLFW_RELEASE:
                handler->OnKeyUp(key, scancode, false);
                break;
            case GLFW_REPEAT:
                handler->OnKeyDown(key, scancode, true);
                break;
        }
    };

    auto mouseHandler = [](GLFWwindow* window, double xpos,
                           double ypos) -> void {
        auto handler = SWindowUserData::GetInputHandler(window);
        if (!handler) return;

        handler->OnMouseMove(xpos, ypos);
    };

    glfwSetKeyCallback(m_window, keyHandler);
    glfwSetCursorPosCallback(m_window, mouseHandler);
}
