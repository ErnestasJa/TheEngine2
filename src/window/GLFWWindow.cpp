#include "GLFWWindow.h"
#include "GLFW/glfw3.h"
#include "GLFWInputDevice.h"

namespace render
{
GLFWWindow::GLFWWindow() : m_window(nullptr)
{
}

GLFWWindow::~GLFWWindow()
{
    if (m_window) glfwDestroyWindow(m_window);
}

void SetWindowHints(const SWindowDefinition& wDef)
{
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, wDef.ContextMajorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, wDef.ContextMinorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, wDef.ForwardCompatible);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, wDef.DebugContext);
    glfwWindowHint(GLFW_RED_BITS, wDef.ColorFramebufferBits.r);
    glfwWindowHint(GLFW_GREEN_BITS, wDef.ColorFramebufferBits.g);
    glfwWindowHint(GLFW_BLUE_BITS, wDef.ColorFramebufferBits.b);
    glfwWindowHint(GLFW_ALPHA_BITS, wDef.ColorFramebufferBits.a);
    glfwWindowHint(GLFW_DEPTH_BITS, wDef.DepthBits);
    glfwWindowHint(GLFW_RESIZABLE, wDef.Resizeable);
}

bool GLFWWindow::Init(const SWindowDefinition& wDef)
{
    SetWindowHints(wDef);

    m_window = glfwCreateWindow(wDef.Dimensions.x, wDef.Dimensions.y,
                                wDef.Title.c_str(), NULL, NULL);

    if (m_window == nullptr) {
        return false;
    }

    m_inputDevice = GLFWInputDevice::Create(m_window);

    if (m_inputDevice == nullptr) {
        return false;
    }

    return true;
}

core::pod::Vec2<int32_t> GLFWWindow::GetDimensions()
{
    core::pod::Vec2<int32_t> dim;
    glfwGetWindowSize(m_window, &dim.x, &dim.y);
    return dim;
}

void GLFWWindow::SetDimensions(const core::pod::Vec2<int32_t>& dimensions)
{
    glfwSetWindowSize(m_window, dimensions.x, dimensions.y);
}

core::pod::Vec2<int32_t> GLFWWindow::GetPosition()
{
    core::pod::Vec2<int32_t> pos;
    glfwGetWindowPos(m_window, &pos.x, &pos.y);
    return pos;
}

void GLFWWindow::SetPosition(const core::pod::Vec2<int32_t>& position)
{
    glfwSetWindowPos(m_window, position.x, position.y);
}

bool GLFWWindow::PollEvents()
{
    m_inputDevice->PollEvents(0);
    glfwPollEvents();
    return true;
}

void GLFWWindow::SwapBuffers()
{
    if (m_window) glfwSwapBuffers(m_window);
}

bool GLFWWindow::ShouldClose()
{
    return m_window == nullptr || glfwWindowShouldClose(m_window);
}

core::WeakPtr<input::IInputDevice> GLFWWindow::GetInputDevice()
{
    return m_inputDevice;
}
}