#include "GuiImpl.h"
#include "../render/GLBindingInc.h"
#include "../window/GLFWWindow.h"
#include "GLFW/glfw3.h"
#include "ImGuiEventHandler.h"
#include "imgui.h"
#include "imgui_impl.h"

namespace gui {

core::SharedPtr<IGui> CreateGui(core::SharedPtr<render::IRenderContext> context)
{
    return std::make_shared<GuiImpl>(context);
}

GuiImpl::GuiImpl(core::SharedPtr<render::IRenderContext> context)
{
    m_context      = context;
    auto appWindow = ((render::GLFWWindow*)m_context->GetWindow().get());
    auto window    = appWindow->GetUnderlyingWindow();

    ImGui_ImplGlfwGL3_Init(window);
    appWindow->GetInputDevice().lock()->AddInputHandler(CreateImGuiInputHandler());
}

GuiImpl::~GuiImpl()
{
    ImGui_ImplGlfwGL3_Shutdown();
}

void GuiImpl::BeginRender()
{
    ImGui_ImplGlfwGL3_NewFrame();
}

void GuiImpl::EndRender()
{
    auto window = ((render::GLFWWindow*)m_context->GetWindow().get())->GetUnderlyingWindow();

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui::Render();
}
} /* gui */
