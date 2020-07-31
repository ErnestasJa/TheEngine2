// clang-format off
#include "GLFWWindow.h"
#include "GLFWInputDevice.h"
#include "log/LogInc.h"
#include "../render/GLBindingInc.h"
#include "GLFW/glfw3.h"
// clang-format on

namespace render {
GLFWWindow::GLFWWindow()
    : m_window(nullptr)
    , m_close(false)
{
}

GLFWWindow::~GLFWWindow()
{
  m_inputDevice = nullptr;

  if (m_window)
    glfwDestroyWindow(m_window);
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

CursorMode GlfwToEngineCursorMode(uint32_t glfwCursorMode){
  switch (glfwCursorMode) {
  case GLFW_CURSOR_HIDDEN:
    return CursorMode::Hidden;
  case GLFW_CURSOR_DISABLED:
    return CursorMode::HiddenCapture;
  default:
    return CursorMode::Normal;
  }
}

uint32_t EngineToGlfwCursorMode(CursorMode cursorMode){
  switch (cursorMode) {
  case CursorMode::Hidden:
    return GLFW_CURSOR_HIDDEN;
  case CursorMode::HiddenCapture:
    return GLFW_CURSOR_DISABLED;
  default:
    return GLFW_CURSOR_NORMAL;
  }
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
  elog::Log(elog::LogSource::Engine, elog::LogSeverity::Info,
            core::string::format("\n\n"
                                 "==========================\n"
                                 "Context version: {}.{}\n"
                                 "Forward compatible: {}\n"
                                 "Debug enabled: {}\n"
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

  m_window = glfwCreateWindow(wDef.Dimensions.x, wDef.Dimensions.y, wDef.Title.c_str(),
                              wDef.Fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

  glfwMakeContextCurrent(m_window);
  glfwSwapInterval(0);

  if (m_window == nullptr) {
    return false;
  }

  glfwSetWindowUserPointer(m_window, this);

  m_inputDevice = GLFWInputDevice::Create(m_window);

  if (m_inputDevice == nullptr) {
    return false;
  }

  glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
    GLFWWindow* currentWindow = (GLFWWindow*)glfwGetWindowUserPointer(window);
    currentWindow->m_onWindowResize(core::pod::Vec2<uint32_t>{ (uint32_t)width, (uint32_t)height });
  });

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
  if (m_window)
    glfwSwapBuffers(m_window);
}

bool GLFWWindow::ShouldClose()
{
  return m_window == nullptr || glfwWindowShouldClose(m_window) || m_close;
}

void GLFWWindow::Close()
{
  m_close = true;
}

input::IInputDevice* GLFWWindow::GetInputDevice()
{
  return m_inputDevice.get();
}

const SWindowDefinition& GLFWWindow::GetWindowDefinition()
{
  return m_windowDefinition;
}

void GLFWWindow::SetCursorMode(CursorMode cursorMode)
{
  uint32_t glfwCursorMode = EngineToGlfwCursorMode(cursorMode);
  glfwSetInputMode(m_window, GLFW_CURSOR, glfwCursorMode);
}

GLFWwindow* GLFWWindow::GetUnderlyingWindow()
{
  return m_window;
}

CursorMode GLFWWindow::GetCursorMode()
{
  auto glfwCursorMode = glfwGetInputMode(m_window, GLFW_CURSOR);
  return GlfwToEngineCursorMode(glfwCursorMode);
}
} // namespace render
