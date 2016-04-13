#include "window/IWindowModule.h"
#include "GLFWWindow.h"
#include "GLFW/glfw3.h"

namespace render
{
class DefaultWindowModule : public IWindowModule
{
public:
    virtual ~DefaultWindowModule()
    {
        glfwTerminate();
    }

    bool Initialize()
    {
        if (glfwInit() == false) return false;

        return true;
    }

    virtual core::SharedPtr<IWindow> CreateWindow(
        const SWindowDefinition& windowDefinition)
    {
        auto window = core::MakeShared<GLFWWindow>();

        if (window->Init(windowDefinition)) return window;

        return nullptr;
    }

private:
};

core::SharedPtr<IWindowModule> CreateDefaultWindowModule()
{
    auto module = std::make_shared<DefaultWindowModule>();

    if (module->Initialize()) {
        return module;
    }

    return nullptr;
}
}