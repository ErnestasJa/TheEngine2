#include "window/WindowInc.h"
#include "render/RenderInc.h"
#include "CommonUtil.h"

core::SharedPtr<render::IWindow> CreateWindow(
    const core::SharedPtr<render::IWindowModule> &wmodule);

const char *vertSource =
    R"(#version 330

void main(void)
{
    gl_Position = vec4(1,1,1,1);
}
)";

const char *fragSource =
    R"(#version 330
out vec4 FragColor;

void main()
{
    FragColor = vec4(1,1,1,1);
}
)";

int main(int argc, char const *argv[])
{
    auto engineLogStream = core::MakeShared<EngineCoutPipe>();
    log::AddLogStream(engineLogStream);

    auto wmodule = render::CreateDefaultWindowModule();
    wmodule->Initialize();

    auto window = CreateWindow(wmodule);

    if (!window) {
        std::cout << "Failed to create window" << std::endl;
        return -1;
    }

    LoadExtensions();
    auto debugMonitor = GetDebugMessageMonitor();
    auto renderer = render::CreateRenderer();
    auto program = renderer->CreateProgram(vertSource, fragSource);

    while (window->ShouldClose() == false) {
        LogDebugMessagesAndFlush(debugMonitor);

        window->SwapBuffers();
        window->PollEvents();
    }

    wmodule->Finalize();

    return 0;
}

core::SharedPtr<render::IWindow> CreateWindow(
    const core::SharedPtr<render::IWindowModule> &wmodule)
{
    render::SWindowDefinition wDef;
    wDef.Dimensions = {1280, 720};
    wDef.Title = "Window example application";
    wDef.DebugContext = true;

    return wmodule->CreateWindow(wDef);
}
