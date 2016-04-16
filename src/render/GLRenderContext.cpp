#include "GLRenderContext.h"
#include "GLRenderer.h"
#include "GLExtensionLoader.h"
#include "window/WindowInc.h"
#include "../window/GLFWWindow.h"
#include "../window/DefaultWindowModule.h"

namespace render
{
std::shared_ptr<IRenderContext> CreateContext(const SWindowDefinition& def)
{
    auto windowModule = CreateDefaultWindowModule();
    if (!windowModule) return nullptr;

    auto window = windowModule->CreateWindow(def);
    if (!window) return nullptr;

    auto extensionLoader = CreateExtensionLoader();
    if (!extensionLoader) return nullptr;
    if (!extensionLoader->LoadExtensions()) return nullptr;

    static_cast<GLFWWindow*>(window.get())->UpdateContext();

    auto renderer = CreateRenderer();
    if (!renderer) return nullptr;

    return std::make_shared<GLRenderContext>(
        std::move(windowModule), std::move(window), std::move(renderer));
}

GLRenderContext::GLRenderContext(
    std::unique_ptr<DefaultWindowModule>&& windowModule,
    std::unique_ptr<IWindow>&& window, std::unique_ptr<IRenderer>&& renderer)
    : m_windowModule(std::move(windowModule)),
      m_window(std::move(window)),
      m_renderer(std::move(renderer))
{
}

GLRenderContext::~GLRenderContext()
{
}
const std::unique_ptr<IWindow>& GLRenderContext::GetWindow()
{
    return m_window;
}
const std::unique_ptr<IRenderer>& GLRenderContext::GetRenderer()
{
    return m_renderer;
}
}