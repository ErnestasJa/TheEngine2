#ifndef GLRENDERCONTEXT_H
#define GLRENDERCONTEXT_H

#include "render/IRenderContext.h"

namespace render
{
class DefaultWindowModule;
class GLRenderContext : public IRenderContext
{
public:
    GLRenderContext(std::unique_ptr<DefaultWindowModule>&& windowModule,
                    std::unique_ptr<IWindow>&& window,
                    std::unique_ptr<IRenderer>&& renderer);
    virtual ~GLRenderContext();
    virtual const std::unique_ptr<IWindow>& GetWindow();
    virtual const std::unique_ptr<IRenderer>& GetRenderer();

private:
    std::unique_ptr<DefaultWindowModule> m_windowModule;
    std::unique_ptr<IWindow> m_window;
    std::unique_ptr<IRenderer> m_renderer;
};
}

#endif