#include "GLFWInputDevice.h"
#include "GLFW/glfw3.h"
#include "GLFWInputKeyMap.h"
#include "input/InputHandler.h"

core::SharedPtr<input::IInputDevice> GLFWInputDevice::Create(GLFWwindow* window)
{
    auto ptr = core::MakeShared<GLFWInputDevice>(window);
    ptr->BindEventHandlers();
    return ptr;
}

GLFWInputDevice::GLFWInputDevice(GLFWwindow* window)
    : m_window(window)
{
    glfwSetWindowUserPointer(window, this);
}

GLFWInputDevice::~GLFWInputDevice()
{
    glfwSetWindowUserPointer(m_window, nullptr);
}

void GLFWInputDevice::PollEvents(float deltaTime)
{
    // we might be doing double event polling
    // maybe skip it here or check if they were polled for window.
}

void GLFWInputDevice::AddInputHandler(const core::SharedPtr<input::InputHandler>& handler)
{
    m_handlers.push_back(handler);
}

void GLFWInputDevice::RemoveInputHandler(const core::SharedPtr<input::InputHandler>& handler)
{
    for (int i = 0; i < m_handlers.size(); i++) {
        if (m_handlers[i].get() == handler.get()) {
            m_handlers.erase(std::begin(m_handlers) + i);
            break;
        }
    }
}

const core::Vector<core::SharedPtr<input::InputHandler>>& GLFWInputDevice::GetInputHandlers()
{
    return m_handlers;
}


void GLFWInputDevice::BindEventHandlers()
{
    glfwSetKeyCallback(
        m_window, [](GLFWwindow* window, int glfwKey, int scancode, int action, int mods) -> void {
            auto thisInputDevice = (GLFWInputDevice*)glfwGetWindowUserPointer(window);

            for (auto& handler : thisInputDevice->GetInputHandlers()) {
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
            }
        });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) -> void {
        auto thisInputDevice = (GLFWInputDevice*)glfwGetWindowUserPointer(window);

        for (auto& handler : thisInputDevice->GetInputHandlers()) {
            handler->OnMouseMove(xpos, ypos);
        }
    });

    glfwSetMouseButtonCallback(
        m_window, [](GLFWwindow* window, int button, int action, int mods) -> void {
            auto thisInputDevice = (GLFWInputDevice*)glfwGetWindowUserPointer(window);

            for (auto& handler : thisInputDevice->GetInputHandlers()) {


                const input::MouseButton& mouseButton = input::MapMouseButton(button);

                switch (action) {
                case GLFW_PRESS:
                    handler->OnMouseDown(mouseButton);
                    break;
                case GLFW_RELEASE:
                    handler->OnMouseUp(mouseButton);
                    break;
                }
            }

        });
}
