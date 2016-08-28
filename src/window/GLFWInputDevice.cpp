#include "GLFWInputDevice.h"
#include "GLFW/glfw3.h"
#include "GLFWInputKeyMap.h"
#include "input/InputHandler.h"

namespace {
struct SWindowUserData
{
    core::WeakPtr<input::InputHandler> inputHandler;

    static void Bind(GLFWwindow* window, core::WeakPtr<input::InputHandler> inputHandler)
    {
        glfwSetWindowUserPointer(window, new SWindowUserData{inputHandler});
    };

    static void Unbind(GLFWwindow* window)
    {
        auto data = Get(window);
        if (data)
            delete data;
        glfwSetWindowUserPointer(window, nullptr);
    };

    static SWindowUserData* Get(GLFWwindow* window)
    {
        return (SWindowUserData*)glfwGetWindowUserPointer(window);
    }

    static core::SharedPtr<input::InputHandler> GetInputHandler(GLFWwindow* window)
    {
        auto ud = Get(window);
        if (!ud)
            return nullptr;
        return ud->inputHandler.lock();
    }
};
}

core::SharedPtr<input::IInputDevice> GLFWInputDevice::Create(GLFWwindow* window)
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
}

void GLFWInputDevice::SetInputHandler(const core::SharedPtr<input::InputHandler>& handler)
{
    m_handler = handler;
    SWindowUserData::Unbind(m_window);
    SWindowUserData::Bind(m_window, m_handler);
}

void GLFWInputDevice::BindEventHandlers()
{
    auto keyHandler = [](GLFWwindow* window, int glfwKey, int scancode, int action,
                         int mods) -> void {
        auto handler = SWindowUserData::GetInputHandler(window);
        if (!handler)
            return;

        const input::Key& key = input::MapKey(glfwKey);
        switch (action) {
        case GLFW_PRESS:
            handler->OnKeyDown(key, false);
            break;
        case GLFW_RELEASE:
            handler->OnKeyUp(key, false);
            break;
        case GLFW_REPEAT:
            handler->OnKeyDown(key, true);
            break;
        }
    };

    auto mouseHandler = [](GLFWwindow* window, double xpos, double ypos) -> void {
        auto handler = SWindowUserData::GetInputHandler(window);
        if (!handler)
            return;

        handler->OnMouseMove(xpos, ypos);
    };

    glfwSetKeyCallback(m_window, keyHandler);
    glfwSetCursorPosCallback(m_window, mouseHandler);
}
