#ifndef GLRENDERCONTEXT_H
#define GLRENDERCONTEXT_H

#include "engine/IEngineContext.h"
namespace render {
class DefaultWindowModule;
}
namespace engine {
class EngineContext : public IEngineContext
{
  public:
  EngineContext(std::unique_ptr<render::DefaultWindowModule>&& windowModule,
                std::unique_ptr<render::IWindow>&& window,
                std::unique_ptr<render::IRenderer>&& renderer);
  virtual ~EngineContext();
  virtual render::IWindow* GetWindow();
  virtual render::IRenderer* GetRenderer();

  private:
  std::unique_ptr<render::DefaultWindowModule> m_windowModule;
  std::unique_ptr<render::IWindow> m_window;
  std::unique_ptr<render::IRenderer> m_renderer;
};
} // namespace engine

#endif