#ifndef IRENDERCONTEXT_H
#define IRENDERCONTEXT_H
namespace render {
struct SWindowDefinition;
class IWindow;
class IRenderer;
class ICamera;
}
namespace engine {

class IEngineContext
{
public:
    virtual ~IEngineContext()
    {
    }

    virtual render::IWindow* GetWindow()     = 0;
    virtual render::IRenderer * GetRenderer() = 0;
};

std::shared_ptr<IEngineContext> CreateContext(const render::SWindowDefinition& def);
}

#endif