#include "GuiImpl.h"
#include "../render/GLBindingInc.h"
#include "../window/GLFWWindow.h"
#include "GLFW/glfw3.h"
#include "ImGuiEventHandler.h"
#include "imgui.h"
#include "imgui_impl.h"

namespace gui {

core::SharedPtr<IGui> CreateGui(core::SharedPtr<engine::IEngineContext> context)
{
    return std::make_shared<GuiImpl>(context);
}

GuiImpl::GuiImpl(core::SharedPtr<engine::IEngineContext> context)
{
    m_context      = context;
    auto appWindow = ((render::GLFWWindow*)m_context->GetWindow());
    auto window    = appWindow->GetUnderlyingWindow();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    appWindow->GetInputDevice().lock()->AddInputHandler(CreateImGuiInputHandler());
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
} /* gui */
