
#include <engine/IEngineContext.h>

#include "../render/GLExtensionLoader.h"
#include "../render/GLRenderer.h"
#include "../render/GLRendererDebugMessageMonitor.h"
#include "../window/DefaultWindowModule.h"
#include "../window/GLFWWindow.h"
#include "EngineContext.h"
#include "window/WindowInc.h"

namespace engine {
core::UniquePtr<IEngineContext> CreateContext(const render::SWindowDefinition& def)
{
  auto windowModule = render::CreateDefaultWindowModule();
  if (!windowModule)
    return nullptr;

  auto window = windowModule->CreateWindow(def);
  if (!window)
    return nullptr;

  auto extensionLoader = render::CreateExtensionLoader();
  if (!extensionLoader)
    return nullptr;
  if (!extensionLoader->LoadExtensions())
    return nullptr;

  auto wnd = static_cast<render::GLFWWindow*>(window.get());
  wnd->UpdateContext();

  auto debugMonitor = render::CreateRendererDebugMessageMonitor();

  auto renderer = CreateRenderer(std::move(debugMonitor));
  if (!renderer)
    return nullptr;

  if(def.DebugContext){
    renderer->GetDebugMessageMonitor()->SetDebugging(true);
  }


  renderer->WindowResized({ (uint32_t)def.Dimensions.x, (uint32_t)def.Dimensions.y });

  wnd->SetWindowResizeCallback(
      [&renderer](core::pod::Vec2<uint32_t> size) { renderer->WindowResized(size); });

  return core::MakeUnique<EngineContext>(std::move(windowModule), std::move(window),
                                         std::move(renderer));
}

EngineContext::EngineContext(std::unique_ptr<render::DefaultWindowModule>&& windowModule,
                             std::unique_ptr<render::IWindow>&& window,
                             std::unique_ptr<render::IRenderer>&& renderer)
    : m_windowModule(std::move(windowModule))
    , m_window(std::move(window))
    , m_renderer(std::move(renderer))
{
}

EngineContext::~EngineContext()
{
}

render::IWindow* EngineContext::GetWindow()
{
  return m_window.get();
}

render::IRenderer* EngineContext::GetRenderer()
{
  return m_renderer.get();
}
} // namespace engine