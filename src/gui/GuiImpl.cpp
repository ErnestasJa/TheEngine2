#include "GuiImpl.h"
#include "../render/GLBindingInc.h"
#include "../window/GLFWWindow.h"
#include "GLFW/glfw3.h"
#include "ImGuiEventHandler.h"
#include "imgui.h"
#include "imgui_impl.h"
#include "imgui_gl3_impl.h"

namespace gui {

core::UniquePtr<IGui> CreateGui(engine::IEngineContext* context)
{
  return core::MakeUnique<GuiImpl>(context);
}

GuiImpl::GuiImpl(engine::IEngineContext* context)
{
  m_context      = context;
  auto appWindow = ((render::GLFWWindow*)m_context->GetWindow());
  auto window    = appWindow->GetUnderlyingWindow();

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();

  ImGui::StyleColorsDark();
  m_inputHandler = CreateImGuiInputHandler();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  m_inputHandlerHandle = appWindow->GetInputDevice()->AddInputHandler(m_inputHandler.get());
}

GuiImpl::~GuiImpl()
{
  ImGui_ImplGlfw_Shutdown();
}

void GuiImpl::BeginRender()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void GuiImpl::EndRender()
{
  auto window = ((render::GLFWWindow*)m_context->GetWindow())->GetUnderlyingWindow();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
}
} // namespace gui
