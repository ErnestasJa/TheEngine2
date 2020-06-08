#ifndef IRENDERCONTEXT_H
#define IRENDERCONTEXT_H
namespace render {
struct SWindowDefinition;
class IWindow;
class IRenderer;
class ICamera;
} // namespace render
namespace engine {

class IEngineContext
{
  public:
  virtual ~IEngineContext()
  {
  }

  virtual render::IWindow* GetWindow()     = 0;
  virtual render::IRenderer* GetRenderer() = 0;
};

core::UniquePtr<IEngineContext> CreateContext(const render::SWindowDefinition& def);
} // namespace engine

#endif