#ifndef IRENDERCONTEXT_H
#define IRENDERCONTEXT_H

namespace render {
struct SWindowDefinition;
class IWindow;
class IRenderer;
class IRenderContext
{
public:
    virtual ~IRenderContext()
    {
    }

    virtual const std::unique_ptr<IWindow>& GetWindow()     = 0;
    virtual const std::unique_ptr<IRenderer>& GetRenderer() = 0;
};

std::shared_ptr<IRenderContext> CreateContext(const SWindowDefinition& def);
}

#endif