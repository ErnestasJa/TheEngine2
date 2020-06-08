#include "GuiImpl.h"
#include "../render/GLBindingInc.h"
#include "../window/GLFWWindow.h"
#include "GLFW/glfw3.h"
#include "ImGuiEventHandler.h"
#include "imgui.h"
#include "imgui_impl.h"

namespace gui {

core::UniquePtr<IGui> CreateGui(core::SharedPtr<engine::IEngineContext> context)
{
  return core::MakeUnique<GuiImpl>(context);
}

GuiImpl::GuiImpl(core::SharedPtr<engine::IEngineContext> context)
{
  m_context      = context;
  auto appWindow = ((render::GLFWWindow*)m_context->GetWindow());
  auto window    = appWindow->GetUnderlyingWindow();
  m_inputHandler = CreateImGuiInputHandler();
  ImGui_ImplGlfw_InitForOpenGL(window, true);

  m_inputHandlerHandle = appWindow->GetInputDevice()->AddInputHandler(m_inputHandler.get());
}

GuiImpl::~GuiImpl()
{
  ImGui_ImplGlfw_Shutdown();
}

void GuiImpl::BeginRender()
{
  ImGui_ImplGlfw_NewFrame();
}

void GuiImpl::EndRender()
{
  auto window = ((render::GLFWWindow*)m_context->GetWindow())->GetUnderlyingWindow();

  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  ImGui::Render();
}
} // namespace gui
