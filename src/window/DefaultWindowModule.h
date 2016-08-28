#include "GLFW/glfw3.h"
#include "GLFWWindow.h"

namespace render {
class DefaultWindowModule
{
public:
    virtual ~DefaultWindowModule()
    {
        glfwTerminate();
    }

    bool Initialize()
    {
        if (glfwInit() == false)
            return false;

        return true;
    }

    virtual core::UniquePtr<IWindow> CreateWindow(const SWindowDefinition& windowDefinition)
    {
        auto window = core::MakeUnique<GLFWWindow>();

        if (window->Init(windowDefinition))
            return window;

        return nullptr;
    }

private:
};

core::UniquePtr<DefaultWindowModule> CreateDefaultWindowModule()
{
    auto module = std::make_unique<DefaultWindowModule>();

    if (module->Initialize()) {
        return module;
    }

    return nullptr;
}
}