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
    }

    virtual bool Initialize()
    {
        if (glfwInit() == false) return false;
        return true;
    }

    virtual bool Finalize()
    {
        glfwTerminate();
        return true;
    }

    virtual core::SharedPtr<IWindow> CreateWindow(
        const SWindowDefinition& windowDefinition)
    {
        auto window = std::make_shared<GLFWWindow>();

        if (window->Init(windowDefinition)) return window;

        return nullptr;
    }
};

core::SharedPtr<IWindowModule> CreateDefaultWindowModule()
{
    return std::make_shared<DefaultWindowModule>();
}
}