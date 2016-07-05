#include "GLFWWindow.h"
#include "GLXW/glxw.h"
#include "GLFW/glfw3.h"
#include "GLFWInputDevice.h"
#include "log/LogInc.h"

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
    // glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, wDef.ContextMajorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, wDef.ContextMinorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, wDef.ForwardCompatible);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, wDef.DebugContext);
    glfwWindowHint(GLFW_RED_BITS, wDef.ColorFramebufferBits.r);
    glfwWindowHint(GLFW_GREEN_BITS, wDef.ColorFramebufferBits.g);
    glfwWindowHint(GLFW_BLUE_BITS, wDef.ColorFramebufferBits.b);
    glfwWindowHint(GLFW_ALPHA_BITS, wDef.ColorFramebufferBits.a);
    glfwWindowHint(GLFW_DEPTH_BITS, wDef.DepthBits);
    glfwWindowHint(GLFW_RESIZABLE, wDef.Resizeable);
}

void ReadBackContext(SWindowDefinition& def)
{
    // int32_t flags = 0;
    // glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    // def.ForwardCompatible = flags & GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT;
    // def.DebugContext = flags & GL_CONTEXT_FLAG_DEBUG_BIT;

    // glGetIntegerv(GL_MAJOR_VERSION, &def.ContextMajorVersion);
    // glGetIntegerv(GL_MINOR_VERSION, &def.ContextMinorVersion);
}

void LogContext(const SWindowDefinition& def)
{
    elog::Log(
        elog::LogSource::Engine, elog::LogSeverity::Info,
        core::string::CFormat("\n\n"
                              "==========================\n"
                              "Context version: %u.%u\n"
                              "Forward compatible: %s\n"
                              "Debug enabled: %s\n"
                              "==========================\n\n",
                              def.ContextMajorVersion, def.ContextMinorVersion,
                              def.ForwardCompatible ? "yes" : "no",
                              def.DebugContext ? "yes" : "no"));
}

void GLFWWindow::UpdateContext()
{
    ReadBackContext(m_windowDefinition);
    LogContext(m_windowDefinition);
}

bool GLFWWindow::Init(const SWindowDefinition& wDef)
{
    m_windowDefinition = wDef;
    SetWindowHints(m_windowDefinition);

    m_window = glfwCreateWindow(wDef.Dimensions.x, wDef.Dimensions.y,
                                wDef.Title.c_str(), NULL, NULL);

    glfwMakeContextCurrent(m_window);

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

const SWindowDefinition& GLFWWindow::GetWindowDefinition()
{
    return m_windowDefinition;
}

void GLFWWindow::SetCursorMode(CursorMode cursorMode)
{
    uint32_t glfwCursorMode = GLFW_CURSOR_NORMAL;

    switch (cursorMode) {
        case CursorMode::Normal:
            glfwCursorMode = GLFW_CURSOR_NORMAL;
            break;
        case CursorMode::Hidden:
            glfwCursorMode = GLFW_CURSOR_HIDDEN;
            break;

        case CursorMode::HiddenCapture:
            glfwCursorMode = GLFW_CURSOR_DISABLED;
            break;
    }

    glfwSetInputMode(m_window, GLFW_CURSOR, glfwCursorMode);
}
}